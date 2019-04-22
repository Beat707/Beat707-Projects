/*

  Beat707 XXL LE (Limited Edition)

  Created by Guilherme (William) Kalfelz
  @ Beat707 (c) 2014 - http://www.Beat707.com

  Interface Settings:
  
    Left Hold + Select = Drum/Accent Track Selection (for the first 10 buttons).
    Right Click = Play/Stop.
    Right Hold + Select =  Edit Steps A, B, C, D and the last two is Auto-Steps Mirror On and Mirror Off. (for the first 6 buttons).
                           The two bottom rows selects the pattern (for the first 6 buttons).
                           The last button will open the BPM Tempo Selector and the one before that the Number of Steps Selector.
    Any Step Hold = Double Step On/Off (blinks).
    
    Mirror Mode = It will edit all Steps at once. When you select Steps A, B, C, D it will disable the Mirror Mode.
    
    Hold Left and them Click Right to Init a Pattern.
    Hold Right and them Click Left to Copy Current Pattern into Next Pattern and Switch to the Next Pattern right away.
    
    Song Data is saved when you Stop the sequencer and something was changed before it, otherwise nothing is saved, to avoid writing too much to the EEPROM.
    
*/

// ======================================================================================= //
#include "Config.h"
#include <avr/eeprom.h>
#include <EEPROM.h>

// ======================================================================================= //
enum { kInterface_Zone_None, kInterface_Zone_BPM, kInterface_Zone_Steps };
uint8_t buttonCounter=0;
int tClock = 0;
uint8_t ledCounter = 0;
uint16_t stepLeds = 0, transportLeds = 0;
uint8_t holdingLeft = 0, holdingRight = 0, autoStepPos = 1;
uint8_t currentTrack=0, editStepsPos=0, currentPattern=0, nextPattern=0;
uint8_t mirrorStepsEdit = 1; // On by Default //
uint8_t extraLEDs = 1; // 0 = Off, 1 = Blink on Every Note Out //
int extLEDblink = 0;
uint8_t doExtLEDBlink = 0, interfaceZone = kInterface_Zone_None;

// Sequencer Clock  //
#define PPQ 96 // Min BPM = 50 //
uint8_t seqClockCounterDivider, seqClockCounter, prevseqClockCounter, seqClockCounter2, sync24PPQ = 0;
uint8_t seqClockShuffleData[2][3], seqClockShuffleCounter;
int transportBlinkCounter = 0;
uint8_t transportBlinkClear = 0;
int holdUpDown = 0;
int holdUpDownCounter = 0;

// Boolean Variables //
uint8_t seqClockRunning, seqClockProcess, seqClockProcessDoubleSteps, stepsPos, somethingChanged = 0;

// ======================================================================================= //
struct last_settings_t
{
  char header[7];
  uint8_t versionNumber;
  uint16_t dmSteps[PATTERNS][(DRUMTRACKS+2)*8]; // [tracks*steps] - 64x2 Steps - last 2 tracks are Accents - each bit is a step on/off //
  uint8_t seqClockBPM; // 25 to 255 BPM //
  uint8_t numberOfSteps;
  uint8_t seqClockShuffle;
  uint8_t seqClockDirection;
} last_settings; // For 8 drum trakcs + 2 accent tracks and 6 patterns = 972 bytes - ATmega328 EEPROM has 1024 bytes of space //
//
uint8_t dmNotes[DRUMTRACKS]; // Those two options are set on the code instead //
uint8_t destChannel[DRUMTRACKS];
uint8_t dmBlink[DRUMTRACKS];

// ======================================================================================= //
void setup()
{  
  // Enables all connections using the 74HC125 IC
  pinMode(A5, OUTPUT);
  Serial.begin(31250); // MIDI Input/Output //
  //
  // Setup the LED Pins as Output //
  for (uint8_t x=0; x<12; x++) { pinMode(2 + x, OUTPUT); }
  for (uint8_t x=0; x<5; x++) { pinMode(A0 + x, OUTPUT); }
  // 
  clearDisplay();
  //
  // Setup the Default Settings //
  memset(last_settings.dmSteps,0,sizeof(last_settings.dmSteps));
  memset(destChannel,0,sizeof(destChannel)); // 0 = none //
  memset(dmBlink,0,sizeof(dmBlink));
  memset(dmNotes,36,sizeof(dmNotes));    
  last_settings.numberOfSteps = 16;
  //
  // GM Note Numbers
  #if USE_GM_NOTES
    dmNotes[0] = 36; // Bass
    dmNotes[1] = 37; // Stick
    dmNotes[2] = 38; // Snare
    dmNotes[3] = 39; // Clap 
    dmNotes[4] = 40; // Snare2
    dmNotes[5] = 42; // Closed Hat
    dmNotes[6] = 46; // Open Hat
    dmNotes[7] = 54; // Tambourine
  #else
    dmNotes[0] = 60;
    dmNotes[1] = 61;
    dmNotes[2] = 62;
    dmNotes[3] = 63;
    dmNotes[4] = 64;
    dmNotes[5] = 65;
    dmNotes[6] = 66;
    dmNotes[7] = 67;
  #endif
  //
  last_settings.seqClockBPM = 120;
  last_settings.seqClockShuffle = seqClockShuffleCounter = 0;
  //
  // Set Which Tracks should blink when there's a new note //
  // By default we set the first two tracks below //
  dmBlink[0] = 1;
  dmBlink[1] = 1;
  dmBlink[2] = 0;
  dmBlink[3] = 0;
  dmBlink[4] = 0;
  dmBlink[5] = 0;
  dmBlink[6] = 0;
  dmBlink[7] = 0;  
  //
  // Default MIDI Channels //
  destChannel[0] = 9; // 9 = 10 Drums
  destChannel[1] = 9;
  destChannel[2] = 9;
  destChannel[3] = 9;
  destChannel[4] = 9;
  destChannel[5] = 9;
  destChannel[6] = 9;
  destChannel[7] = 9;
  //
  seqClockShuffleData[0][0] = 12;
  seqClockShuffleData[1][0] = 6;
  //
  for (uint8_t x=0; x<PATTERNS; x++) { initPattern(x); }
  //
  // Check EEPROM Data //
  eeprom_read_block((void*)&last_settings, (void*)0, sizeof(char)*7);
  if (INIT_EEPROM == 1 || last_settings.header[0] != 'B' || last_settings.header[1] != '7' || last_settings.header[2] != '0' || last_settings.header[3] != '7' || last_settings.header[4] != 'X' || last_settings.header[5] != 'L' || last_settings.header[6] != 'E')
  {
    last_settings.header[0] = 'B'; last_settings.header[1] = '7'; last_settings.header[2] = '0'; last_settings.header[3] = '7'; last_settings.header[4] = 'X'; last_settings.header[5] = 'L'; last_settings.header[6] = 'E';
    last_settings.versionNumber = 1;
    eeprom_write_block((const void*)&last_settings, (void*)0, sizeof(last_settings));
  }
  else
  {
    eeprom_read_block((void*)&last_settings, (void*)0, sizeof(last_settings));
  }
  
  // Clear Up MIDI Input Buffer //
  while (MSerial.available() > 0) { MSerial.read(); }
  //
  blinkExtraLEDs();
}
