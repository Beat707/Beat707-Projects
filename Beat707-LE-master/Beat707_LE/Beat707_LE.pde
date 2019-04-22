/*

  Created by Beat707 (c) 2011 - http://www.Beat707.com

  Main File for Variable Declaration and Setup - June 19 2011 - Version 1.0.0
  
  --------
  
  Headers x Arduino Pins
  C595:    Clock=A0 / Data=A1 / Latch=A2 / Clear=A3 / Enable=A4    
  Buttons: Coln0=D0 / Coln1=D1 / Coln2=D2 / Coln3=D3 / Butn0=D4 / Butn1=D5 / Butn2=D6 / Butn3=D7
  Header:  CS=D10(PWM) / MOSI=D11(PWM) / MISO=D12 / SCK=D13 (used by the MIDI Ouput)
  
  --------

  Interface Settings:
    1 = BPM Time
    2 = Note-Number
    3 = Channel
    4 = Number of Steps
    5 = Clock Shuffle
    6 = Clock Direction
    
*/

#include "Config.h"
#include "Library.h"
#include <avr/eeprom.h>
#include <EEPROM.h>

uint8_t row=0, frame_num=0, buttonCounter=0;
uint8_t redf[16], bluef[16], greenf[16], buttonClick[16];
uint16_t buttons=0, buttonsHold=0, lineByte=0;
uint8_t row_offset=0, LED=0, buttonRow=0, buttonHoldCounter=1;
uint8_t specialMode=0, specialModeZone=0; // 1 = special features for each button: bpm, ...
uint8_t currentTrack=0, currentPattern=0, nextPattern=0,editDoubleSteps=0, editStepsPos=0;
uint8_t tempColor[8];
uint16_t newPatternBlinkCounter=0;

// Midi Clock  //
#define PPQ 96 // Min BPM = 50 //
uint8_t midiClockCounterDivider, midiClockCounter, prevMidiClockCounter, midiClockCounter2, sync24PPQ = 0;
uint8_t midiClockShuffleData[2][3], midiClockShuffleCounter;
uint8_t bufferMIDI[DRUMTRACKS*2*3]; // Number of DrumTracks x 2 (noteon/off) x 3 bytes
uint8_t bufferMIDIpos=0, bufferMIDIsize = 0;
uint8_t transportBlinkCounter = 0;
uint8_t transportBlinkClear = 0;

// Boolean Variables //
uint8_t midiClockRunning, midiClockProcess, midiClockProcessDoubleSteps, stepsPos, somethingChanged, sendAllNoteOff, sendSpecialMidiData = 0;

NewSoftSerial xSerial(12,13); // (Receive/Transmit)

struct song_settings_t
{
  char header[6];
  uint8_t versionNumber;
  uint16_t dmSteps[NPATTERNS][(DRUMTRACKS+2)*4]; // [tracks*steps] - 32x2 Steps - last 2 tracks are Accents - each bit is a step on/off = 768 bytes
  uint8_t dmNotes[DRUMTRACKS];
  uint8_t dmChannel[DRUMTRACKS];
  uint8_t midiClockBPM; // 25 to 255 BPM //
  uint8_t timeScale;
  uint8_t numberOfSteps;
  uint8_t midiClockShuffle;
  uint8_t mirrorPatternEdit;
  uint8_t midiClockDirection;
} song_settings; // total size = 801 bytes

// ======================================================================================= //
void setup()
{
  clearDisplay();
  memset(buttonClick,0,sizeof(buttonClick));
  memset(song_settings.dmSteps,0,sizeof(song_settings.dmSteps));
  memset(song_settings.dmChannel,9,sizeof(song_settings.dmChannel));  
  CUR_TRACK_COLOR[currentTrack] = CUR_TRACK_COLOR_STRENG;
  memset(bufferMIDI,0,sizeof(bufferMIDI));
  bufferMIDIpos = bufferMIDIsize = 0;
  song_settings.numberOfSteps = 16;
  
  // GM Note Numbers //
  song_settings.dmNotes[0] = 36; // Bass
  song_settings.dmNotes[1] = 37; // Stick
  song_settings.dmNotes[2] = 38; // Snare
  song_settings.dmNotes[3] = 39; // Clap 
  song_settings.dmNotes[4] = 40; // Snare2
  song_settings.dmNotes[5] = 42; // Closed Hat
  song_settings.dmNotes[6] = 46; // Open Hat
  song_settings.dmNotes[7] = 54; // Tambourine
  song_settings.dmNotes[8] = 69; // Casaba
  song_settings.dmNotes[9] = 49; // Crash
    
  pinMode(A0, OUTPUT);  pinMode(A1, OUTPUT);  pinMode(A2, OUTPUT);  pinMode(A3, OUTPUT);  pinMode(A4, OUTPUT);
  pinMode(0, OUTPUT);  pinMode(1, OUTPUT);  pinMode(2, OUTPUT);  pinMode(3, OUTPUT);
  pinMode(4, INPUT);  digitalWrite(4, HIGH);  pinMode(5, INPUT);  digitalWrite(5, HIGH);  
  pinMode(6, INPUT);  digitalWrite(6, HIGH);  pinMode(7, INPUT);  digitalWrite(7, HIGH);
  
  digitalWrite(SR_CLEAR, LOW);  nop();  digitalWrite(SR_CLEAR, HIGH);
  digitalWrite(SR_EN,LOW);
    
  pinMode(13,OUTPUT);
  pinMode(12,INPUT);
  MSerial.begin(31250);
  
  song_settings.midiClockBPM = 120;
  song_settings.mirrorPatternEdit = song_settings.midiClockShuffle = midiClockShuffleCounter = 0;
  song_settings.timeScale = 2;
  
  midiClockShuffleData[0][0] = 12;
  midiClockShuffleData[1][0] = 6;
  
  // Check EEPROM Data //
  eeprom_read_block((void*)&song_settings, (void*)0, sizeof(char)*6);
  if (INIT_EEPROM == 1 || song_settings.header[0] != 'B' || song_settings.header[1] != '7' || song_settings.header[2] != '0' || song_settings.header[3] != '7' || song_settings.header[4] != 'L' || song_settings.header[5] != 'E')
  {
    song_settings.header[0] = 'B'; song_settings.header[1] = '7'; song_settings.header[2] = '0'; song_settings.header[3] = '7'; song_settings.header[4] = 'L'; song_settings.header[5] = 'E';
    song_settings.versionNumber = 1;
    somethingChanged = 1;
  }
  else
  {
    eeprom_read_block((void*)&song_settings, (void*)0, sizeof(song_settings));
  }
  
  // Timer 2 for the LEDs + Buttons //
  TCCR2A = TCCR2B = 0;
  bitWrite(TCCR2A, WGM21, 1);
  bitWrite(TCCR2B, CS20, 1);
  TCCR2B = 0b111; // 0b011=32_scalar / 0b111=1024_scalar / 0b001=no_scalar;
  OCR2A = 0x4;
  bitWrite(TIMSK2, OCIE2A, 1);  
}
