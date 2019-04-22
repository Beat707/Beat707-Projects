/*

  Beat707 XXL LE LCD (no SD Card support)
  See the LCD_SD_Card sketch for details
 
  Created by Guilherme (William) Kalfelz
  @ Beat707 (c) 2014 - http://www.Beat707.com

*/

// ======================================================================================= //
#define MIDI_CHANNEL1 9
#define MIDI_CHANNEL2 9
#define MIDI_CHANNEL3 9
#define MIDI_CHANNEL4 9
#define MIDI_CHANNEL5 9
#define MIDI_CHANNEL6 9
#define MIDI_CHANNEL7 9
#define MIDI_CHANNEL8 9

// ======================================================================================= //
#include "Config.h"
#include <avr/eeprom.h>
#include <EEPROM.h>

// ======================================================================================= //
uint8_t buttonCounter=0;
uint8_t ledCounter = 0;
uint16_t stepLeds = 0, transportLeds = 0;
uint8_t holdingLeft = 0, holdingRight = 0, autoStepPos = 1;
uint8_t currentTrack=0, editStepsPos=0, currentPattern=0, nextPattern=0;
uint8_t mirrorStepsEdit = 1; // On by Default //
uint8_t extraLEDs = 1; // 0 = Off, 1 = Blink on Every Note Out //
int extLEDblink = 0;
uint8_t doExtLEDBlink = 0;
uint8_t showNavigationKeys = 0;
int8_t patternFade = 0;
uint8_t patternFadeCounter = 0;
uint8_t ledsReady = 0;

// Sequencer Clock  //
#define PPQ 96 // Min BPM = 50 //
uint8_t seqClockCounterDivider, seqClockCounter, prevseqClockCounter, seqClockCounter2, sync24PPQ = 0;
int transportBlinkCounter = 0;
uint8_t transportBlinkClear = 0;

// Boolean Variables //
uint8_t seqClockRunning, seqClockProcessDoubleSteps, stepsPos;

// ======================================================================================= //
uint16_t dmSteps[PATTERNS][DRUMTRACKS*8]; // each bit is a step on/off //
uint16_t dmStepsBuffer[DRUMTRACKS*8];
uint16_t ACCENT1[8] = {0x1111, 0x1111, 0x1111, 0x1111, 0x1111, 0x1111, 0x1111, 0x1111 };
uint16_t ACCENT2[8] = {0x5555, 0x5555, 0x5555, 0x5555, 0x5555, 0x5555, 0x5555, 0x5555 };
uint8_t seqClockBPM; // 25 to 255 BPM //
uint8_t numberOfSteps;
uint8_t seqClockShuffle;
uint8_t seqClockDirection;
uint8_t dmNotes[DRUMTRACKS]; // Those two options are set on the code instead //
uint8_t destChannel[DRUMTRACKS];
uint8_t dmBlink[DRUMTRACKS];
uint16_t noteTrackLEDs = 0;
uint8_t tracksOnOff = 0;

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
  memset(dmStepsBuffer,0,sizeof(dmStepsBuffer));
  memset(dmSteps,0,sizeof(dmSteps));
  memset(destChannel,0,sizeof(destChannel));
  memset(dmBlink,0,sizeof(dmBlink));
  memset(dmNotes,36,sizeof(dmNotes));    
  numberOfSteps = 16; 
  //
  // GM Note Numbers
  #if USE_GM_NOTES
    dmNotes[0] = 36; // Bass
    dmNotes[1] = 38; // Snare
    dmNotes[2] = 42; // Closed Hat
    dmNotes[3] = 46; // Open Hat
  #else
    dmNotes[0] = 60;
    dmNotes[1] = 61;
    dmNotes[2] = 62;
    dmNotes[3] = 63;
  #endif
  //
  seqClockBPM = 120;
  //
  // Set Which Tracks should blink when there's a new note //
  // By default we set the first two tracks below //
  dmBlink[0] = 1;
  dmBlink[1] = 1;
  dmBlink[2] = 0;
  dmBlink[3] = 0;

  //
  // Default MIDI Channels //
  destChannel[0] = MIDI_CHANNEL1;
  destChannel[1] = MIDI_CHANNEL2;
  destChannel[2] = MIDI_CHANNEL3;
  destChannel[3] = MIDI_CHANNEL4;
  destChannel[4] = MIDI_CHANNEL5;
  destChannel[5] = MIDI_CHANNEL6;
  destChannel[6] = MIDI_CHANNEL7;
  destChannel[7] = MIDI_CHANNEL8;
  //
  for (uint8_t x=0; x<PATTERNS; x++) { initPattern(x); }
  //
  // Clear Up MIDI Input Buffer //
  while (MSerial.available() > 0) { MSerial.read(); }
  //
  blinkExtraLEDs();
}
