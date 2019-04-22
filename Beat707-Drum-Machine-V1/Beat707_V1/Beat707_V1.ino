/*

  Created by Beat707 (c) 2011 - http://www.Beat707.com

  Main File for Variable Declaration and Setup - October 07 2017 - Version 1.5.0

*/

#include "Config.h"  // Where all PIN settings are stored
#include "Library.h"

// Multi Button //
uint8_t prevbutton, multiButton, tempButton = 99;
uint8_t lastMillisCounter = 0;
unsigned long lastMillis, lastMillisRecButtons, lastMillisStepPosBlinker, lastMillisLateLCDupdate, lastMillisShiftClick = 0;

// Midi Clock  //
#define PPQ 96 // Min BPM = 50 //
uint8_t midiClockBPM; // 25 to 255 BPM //
uint8_t midiClockType; // 0=Internal Only (no Sync), 1=SyncToExternal (Slave), 2=SendSync (Master)
uint8_t midiClockCounterDivider, midiClockCounter, midiClockCounter2, currentTrack, currentPattern, 
        nextPattern, shiftMode, midiClockDirection, noteStepPositionBlinker, sync24PPQ = 0;
uint8_t midiInputB[3] = {0,0,0}; // Used by the MIDI Input code to read 3 Bytes
uint8_t channel = 0; // Used by the MIDI Input code, the last MIDI channel been Input
uint8_t state = 0; // Used by the MIDI Input code (to check 3 bytes of MIDI information)
uint8_t incomingByte = 0; // Used by the MIDI Input Code
uint8_t timeScale;
uint8_t autoSteps; // Used to rotate from 16 to 32 extra steps automaticaly
uint8_t midiClockShuffleData[2][3], midiClockShuffle, midiClockShuffleCounter;
uint8_t numberOfSteps = 16;

// Boolean Variables //
uint8_t doLCDupdate, nextPatternReady, patternBufferN, midiClockRunning, editStepsPos, holdingStepButton,
        shiftClick, holdingShift, holdingShiftUsed, patternChanged, stickyShift, mirrorPatternEdit,
        midiClockProcess, setupChanged, recordEnabled, recordShowCurPos, holdingButton, midiUSBmode, rawMIDImode,
        editDoubleSteps, midiClockProcessDoubleSteps, soloCheckerTemp, stepsPos, enableABpattern, enableButtonsAndLEDs, 
        showOnlyOnce, lateAutoSave, songChanged, songNextPosition, doPatternLCDupdate = 0;

// Patterns //
#define DRUMTRACKS 14
uint8_t dmNotes[DRUMTRACKS];
uint8_t dmChannel[DRUMTRACKS+2];
unsigned int dmSteps[2][(DRUMTRACKS+2)*4]; // [currentPattern/nextPattern][tracks*steps] - 32x2 Steps - last 2 tracks are Accents - each bit is a step on/off
unsigned int dmMutes;
uint8_t dmSynthTrack[2][2][64]; // [track#][pattern_loaded/loading][steps]
uint8_t dmSynthTrackStepPos[2] = {0,0}; // Pattern A|B / Position - as in [0] = 0~1 - [1] = 0~15
uint8_t dmSynthTrackPrevNote[2][2] = { {0,0} , {0,0} };
uint8_t dmSynthTrackLastNoteEdit[2] = {61,61};

// Song //
int curSongPosition = 0;
uint8_t songLoopPos, patternSong, patternSongNext, patternSongRepeat, 
        patternSongRepeatNext, patternSongRepeatCounter, songPattEdit = 0;

// File Mode //
uint8_t fileMode = 0; // 0=Load, 1=Save, 2=Dump MIDI SysE, 3=Erase //
uint8_t fileSelected = 0;
uint8_t sysMIDI_ID = 0; // System Exclusive Data MIDI ID //
char fileSongName[15];
uint8_t fileSongNameEdit = 0;

// Interface //
extern uint8_t LcdCursors[64]; // Cursor Icons as set in the W_Betc file
WLCD lcd;
uint8_t curZone = 0;
uint8_t curMode = 0;  // 0=Pattern - 1=Song - 2=File
uint8_t nextMode = 0; // Used by the interface when selecting a new mode

// Other Variables //
uint8_t wireBufferCounter = 0; // Used with the Wire Library to send 64 bytes of data at once to the EEPROM
int newNote = 0; // Used by the S1/S2 Up/Down editor
char externalMIDIportSelector = 0; // from the external USB to MIDI Converter
uint8_t bufferMIDI[2][(DRUMTRACKS+2)*2*3]; // Number of DrumTracks + S1/S2 Tracks x 2 (noteon/off) x 3 bytes
uint8_t bufferMIDIpos[2] = {0,0};
#if MANAGER_DUMP_RECV
  uint8_t numerOfSongsOnFlashMemory = 0;
#endif

// Hack & Mods //
#if ENCODER_INPUT
  void EncoderChange();
  char globalEncoder[2] = {0,0};
  char enCval1,enCval2,enColdVal1,enColdVal2=1;
  char enCpos,enColdPos,enCturn,enCturnCount=0;
  #if ENCODER_SPEED
    unsigned long encoderMillis = millisNI();
  #endif
#endif
#if ANALOG_INPUT_A0
  uint8_t prevAnalogA0value = 0;
  uint8_t analogInputMode = 0; // 0=BPM, 1=Pattern#, 2=NumberOfSteps, 3=TrackSelector, 4=Note Selector
  unsigned long analogInputModeNewDelay = 0;
  #if ANALOG_INPUT_BT
    uint8_t prevAnalogButtonCheckState = HIGH;
  #endif
#endif
#if GATE_OUTS
  unsigned long gateOutDelay[3] = {0,0,0};
#endif
#if BEAT707_BIG_TIME
  uint8_t midiClockBeats = 0;
  uint8_t sendBeatToBigTime = 0;
#endif
#if EXTRA_MIDI_IN_H_2
  #if XTH2_AUTO_STOP
    uint8_t doAutoStop = 0;
  #endif
#endif

// ======================================================================================= //
void sysInit()
{
  memset(dmSteps,0,sizeof(dmSteps));
  memset(dmChannel,9,sizeof(dmChannel));  
  dmChannel[DRUMTRACKS] = 0;
  dmChannel[DRUMTRACKS+1] = 1;
  memset(dmSynthTrack,0,sizeof(dmSynthTrack));
  memset(fileSongName,0,sizeof(fileSongName));
  memset(midiClockShuffleData,0,sizeof(midiClockShuffleData));
  memset(bufferMIDI,0,sizeof(bufferMIDI));
  bufferMIDIpos[0] = bufferMIDIpos[1] = 0;
  
  // GM Note Names //
  dmNotes[0] = 36;
  dmNotes[1] = 37;
  dmNotes[2] = 38;
  dmNotes[3] = 39;
  dmNotes[4] = 40;
  dmNotes[5] = 42;
  dmNotes[6] = 44;
  dmNotes[7] = 46;
  dmNotes[8] = 51;
  dmNotes[9] = 49;
  dmNotes[10] = 45;
  dmNotes[11] = 47;
  dmNotes[12] = 48;
  dmNotes[13] = 50;
  
  midiClockBPM = 120;
  midiClockType = dmMutes = mirrorPatternEdit = midiClockShuffle = midiClockShuffleCounter = 0;
  timeScale = 2;
  autoSteps = enableABpattern = 1;
  
  midiClockShuffleData[0][0] = 12;
  midiClockShuffleData[1][0] = 6;
  
  #if INI_PATT_FULL_ACNT
    patternAccentInit();
  #endif
}

// ======================================================================================= //
void setup() 
{  
  pinModeW(MIDI_ENn,OUTPUT);
    
  // Uses the Analog Input for the Random Numbers Seed
  pinModeW(A0, INPUT);
  randomSeed(analogRead(0));
  
  #if ANALOG_INPUT_A0
    pinModeW(A0, INPUT);
    #if ANALOG_INPUT_BT
      pinModeW(2, INPUT);
      digitalWriteW(2, HIGH);
    #endif
  #endif
  #if GATE_OUTS
    pinModeW(A0, OUTPUT);
    pinModeW(2, OUTPUT);
    pinModeW(3, OUTPUT);
    digitalWriteW(A0, LOW);
    digitalWriteW(2, LOW);
    digitalWriteW(3, LOW);
  #endif
  #if ENCODER_INPUT
    pinModeW(2, INPUT);
    pinModeW(3, INPUT);
    digitalWriteW(2, HIGH);
    digitalWriteW(3, HIGH);
    attachInterrupt(0, EncoderChange, CHANGE);
    attachInterrupt(1, EncoderChange, CHANGE);    
  #endif
  
  pinModeW(LATCHOUT, OUTPUT);  digitalWriteW(LATCHOUT, LOW);
  pinModeW(FLASH_SSn, OUTPUT);
  pinModeW(SWITCH_SSn, OUTPUT); digitalWriteW(SWITCH_SSn, HIGH);
  #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    pinModeW(11, INPUT); // Conflicts with SPI MOSI...do not use
    pinModeW(13, INPUT); // Conflicts with SPI CLOCK...do not use
  #endif
   
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_SPEED);
  Wire.begin();
    
  lcd.begin();
  lcd.createChar(LcdCursors);

  flashInit();
  sysInit();

  startLEDsAndButtonsTimer();
  enableButtonsAndLEDs = 1;
  
  storageInit(0);
  loadSetup();
  if (curMode == 0) loadPattern(0); else if (curMode == 1) loadSongPosition();
  patternBufferN = 1;
  recordShowCurPos = 1;
  
  checkMIDIusbMode();
  if (midiUSBmode)
  {
    lcd.setCursor(4,0);
    lcdPrint(107); // USB_MODE
    delayNI(1000);
  }  
  
  if (curMode == 0) updateLCDPattern(); 
    else if (curMode == 1) updateLCDSong();
    else updateLCDFile();
    
  MidiClockStop();
  timerStop();    
    
  #if SHOWFREEMEM
    lcd.clear();
    lcdPrintString("Free Mem: ");
    lcdPrintNumber3Dgts(freeMemory());
  #endif
  
  #if MANAGER_DUMP_RECV
    for (char xs=0; xs<MAXSONGSFILE; xs++)
    {
      fileSelected = xs;
      if (checkSong()) numerOfSongsOnFlashMemory++;
    }
    fileSelected = 0;
  #endif
}
