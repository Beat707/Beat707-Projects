/*

  Beat707 XXL LE LCD (MIDI Only)
  
  - 4 Drum Tracks
  - 16 x 4 (64) Patterns - each has 16 steps and 4 variations named ABCD, and also extra double steps for a total of 128 steps per pattern.
  - 1 Note (or MIDI CC) Track with 16 steps, special options per step (Gate, Flam, Slide, Note Off) and 4 transpose slots x 16 steps. So you can have 4 variations to go along the 4 ABCD patterns above.
  - Song mode with 99 positions, each with pattern #, repeat #, ABCD Pattern Seletion, Track Mute and special options (Velocity Fade In and Fade Out).
  - Song Performance mode with 16 loop sections that you can trigger to play once or loop (with repeat # option). Plus a Stack option that allows you to select those sections and they stack up, playing in order.
  - 2 hardcoded Accent Track  

  Created by Guilherme (William) Kalfelz
  @ Beat707 (c) 2014 - http://www.Beat707.com
  
  IMPORTANT - Nothing is saved into the EEPROM memory until you select Save All Data from the options menu or use the following shortcut: hold Left and click the 15th Step Button (at the bottom row, from left to right, the 3rd button).
  The options menu can be open by holding Left and clicking the last bottom right button.

  Interface Settings:
    
    Left Hold + Click Step (the 4x4 buttons grid, starts counting from top-left)
    
        01) Drum Track T1
        02) Drum Track T2
        03) Drum Track T3
        04) Drum Track T4        
        05) Note Track TN (for Synth/Bass lines)
        06) ON = Enable/Disable Tracks
        07) PT = Pattern Selector
        ...
        09) Steps A Mirror Off
        10) Steps B Mirror Off
        11) Steps C Mirror Off
        12) Steps D Mirror Off
        13) Auto Steps Mirror On
        14) Auto Steps Mirror Off
        15) Save Data
        16) Options Menu (see table below)
        
    Right Click = Play/Stop.        
    Left Click = Enter/Exits Edit Mode.        
    Left Hold + Right Click = First page of the interface.
    Right Hold + Click Step =  Pattern Selector (16 patterns).
    
    Any Step Hold = Double Step On/Off (blinks).
    Mirror Mode = It will edit all Steps at once. When you select Steps A, B, C, D it will disable the Mirror Mode.
    Edit Mode: when in edit mode, you will use the step-pads as up/down/left/right and center keys. The center key have different usage depending on what you are editing.
    The second page of the interface is for editing the TN (Track Notes for Synth/Bass Lines). The two last buttons at the bottom are shortcuts for next/previous position.
    The third page is to edit the Note Track Transpose variation, for the ABCD steps. As you only have 16 steps for the Note Track but it will use this transpose variation to create the 16 x 4 = ABCD steps just like the Drum Tracks has.

    Options Menu - Left Hold + very last button (step 16) on the bottom-right corner.
    
    - Pattern Mode
      #1 - To Song Mode
      #2 - To Song Performance Mode
      #3 - Copy Pattern
      #4 - Paste Pattern
      #5 - Copy To Next Pat
      #6 - Init Pattern
      #7 - Init Current Track
      #8 - Init All Data (this can only be found in pattern mode)
      #9 - Save All Data
      
    - Song Mode
      #1 - To Pattern Mode
      #2 - To Song Performance Mode
      #3 - Insert Position
      #4 - Remove Position
      #5 - Init Song Data
      #6 - Save All Data
      
    - Song Performance Mode
      #1 - To Song Mode    
      #2 - To Pattern Mode
      #3 - Save All Data
*/

// ======================================================================================= //
#define MIDI_CHANNEL_NOTE_TRACK 1

// ======================================================================================= //
//#include <SPI.h>
//#include <SD.h>

//#include <tinyFAT.h>
//#include <avr/pgmspace.h>

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
//#include <avr/eeprom.h>
//#include <EEPROM.h>
#include "W_Messages.h" // And Configurations //

// ======================================================================================= //
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
uint8_t nSeqUnits = 0;
uint8_t nSNDUnits = 0;
uint8_t xPos = 0; // Big Font LCD Position //
const int chipSelect = 8;


//byte initres;

//
// Sequencer Clock  //
#define PPQ 96 // Min BPM = 50 //
//
struct last_settings_t
{
  char header[4];
  uint8_t seq_bpm;
  uint8_t numberOfSteps;
  uint8_t songData[3][99]; // [type] [position]
  uint8_t noteSteps[1][2][16]; // [pattern] [type/message] [value] - type can be note-on, note-off, double-note, all-off, ... - 16 patterns x 16 steps = 512 bytes //
  int8_t patternsTranspose[1][4]; // [pattern] [transpose] -127 ~ +127 //
  uint8_t loopSong;
  uint8_t seqClockShuffle;
  uint8_t bMode;
  uint8_t reserved2[16];
  uint8_t perfData[3][16]; // [0] Start, [1] End, [2] Repeat (0 none, 1 forever, otherwise the number of times to repeat - 1) //
  uint8_t perfDataMode; // 0 normal, 1 stack
} last_settings;
//
uint8_t noteStepsBuffer[2][16]; // [pattern] [type/message] [value] - type can be note-on, note-off, double-note, all-off, ... - 16 patterns x 16 steps = 512 bytes //
int8_t patternsTransposeBuffer[4]; // [pattern] [transpose] -127 ~ +127 //
//
uint8_t seq_mirror = 1;
uint8_t seq_beat = 0;
uint8_t seq_beat_counter = 0;
uint8_t autoStepPos = 1;
uint8_t nextPattern = 0;
uint8_t currentPattern = 0;
uint8_t stepsPos = 0;
uint8_t seqClockRunning = 0;
uint8_t editStepsPos = 0;
uint8_t doLCDupdate = 1;
uint8_t doLCDupdateBeats = 0;
uint8_t currentTrack = 0;
uint8_t editNTrackPos = 0;
uint16_t noteTrackLEDs = 0;
uint8_t upDownForceToTNpos = 0;
uint8_t editingOn = 0;
uint8_t lastTNVals[2] = {1, 60};
uint8_t tracksOnOff = B00011111;
uint8_t tracksOnOffBuffer = B00011111;
uint8_t prevNote = 0;
int tClock = 0;
uint8_t seqClockCounterDivider, seqClockCounter, prevseqClockCounter, sync24PPQ = 0;
uint8_t seqClockShuffleData[2][3], seqClockShuffleCounter;
uint8_t showProcessing = 0;
uint8_t showProcessingMessage = 0;
uint8_t showOptions = 0;
uint8_t showOptionsOption = 0;
uint8_t showOptionsOptionConfirm = 0;
int16_t ignoreKeysCounter = 0;
uint8_t songEditPos = 0;
uint8_t songRepeatCounter = 0;
uint8_t doLateClockStop = 0;
int8_t patternFade = 0;
uint8_t patternFadeCounter = 0;
uint8_t ledsReady = 0;
uint8_t songPerfPos = 0;
uint8_t songPerfPosCounter = 0;
int8_t songPerfPosNext[32];
uint8_t songPerfPosNextUnstack = 0;
//
uint16_t ACCENT1[8] = {0x1111, 0x1111, 0x1111, 0x1111, 0x1111, 0x1111, 0x1111, 0x1111 };
uint16_t ACCENT2[8] = {0x5555, 0x5555, 0x5555, 0x5555, 0x5555, 0x5555, 0x5555, 0x5555 };
//
uint8_t editZone = 0;
uint8_t holdingLeft = 0, holdingRight = 0;
uint8_t requestData = 0;
int holdUpDown = 0;
int holdLeftRight = 0;
int holdUpDownCounter = 0;
//
enum { kZoneBPM, kZoneSteps, kZonePat, kZoneTrack, // Page 1
       kZoneNT_pos, kZoneNT_value, kZoneNT_type, // Page 2
       kZoneNT_transpose1, kZoneNT_transpose2, kZoneNT_transpose3, kZoneNT_transpose4, // Page 3
       //
       kZoneSongPos, kZoneSongPat, kZoneSongRpt, kZoneSongSteps, kZoneSongTrack1, kZoneSongTrack2, kZoneSongTrack3, kZoneSongTrack4, kZoneSongTrack5, kZoneSongSp, // Page 1
       kZoneSongBPM, kZoneSongLoop, // Page 2
       //
       kZoneSPmode, kZoneSPsection, kZoneSPrpt, kZoneSPstart, kZoneSPend
     };
enum { kModePat, kModeSong, kModeSongPerf };

// ======================================================================================= //
void setup() 
{
  lcd.begin(16,2);
  setupBigFont();
  lcd.backlight();
  showB707();
  //
  delay(100);
  Serial.begin(31250);
  //
  /*pinMode(10, OUTPUT);
  if (!SD.begin(chipSelect))
  {
    lcd.clear();
    delay(10000);
  }
  else
  {
    File dataFile = SD.open("datalog.txt");
  
    // if the file is available, write to it:
    if (dataFile) {
      while (dataFile.available()) {
        Serial.write(dataFile.read());
      }
      dataFile.close();
    }
    
      File myFile = SD.open("test.txt", FILE_WRITE);
    
      // if the file opened okay, write to it:
      if (myFile) {
        myFile.write(88);
        // close the file:
        myFile.close();
      }
    
  }*/
  
  
  /*initres=file.initFAT();
  char textBuffer[81];
  if (initres!=NO_ERROR)
  {
    byte res=file.openFile("TEXTFILE.TXT", FILEMODE_TEXT_READ);
    if (res==NO_ERROR)
    {
      word result=0;
      while ((result!=EOF) and (result!=FILE_IS_EMPTY))
      {
        result=file.readLn(textBuffer, 80);
      }
      file.closeFile();
    }    
    
    res=file.openFile("BIGFILE.TXT", FILEMODE_TEXT_WRITE);
    if (res==NO_ERROR)
    {
      for (int cc=0; cc<1024; cc++)
      {
        for (int i=0; i<8; i++)
        {
          file.writeLn("!");
        }
      }
      file.closeFile();
    }    
  }  */
  
  //
  initData();
  last_settings.bMode = kModePat;
  //
  seqClockShuffleData[0][0] = 12;
  seqClockShuffleData[1][0] = 6;  
  //
  // Check EEPROM Data //
/*  eeprom_read_block((void*)&last_settings, (void*)0, sizeof(char)*4);
  if (last_settings.header[0] != 'B' || last_settings.header[1] != '7' ||
    last_settings.header[2] != 'V' || last_settings.header[3] != '1')
  {
    delay(2000);
    lcd.clear(); 
    lcdPrint(INIT_SAVE_DATA_);
    lcd.setCursor(0,1);
    lcdPrint(CLICK_TO_CONFIRM);
    //
    waitSerialNextByte();
    while (Serial.available() > 0) { Serial.read(); }
    //
    lcd.setCursor(0,1);
    lcdPrint(PROCESSING);
    //
    initSlaveData();
    initData();
    delay(250);
    last_settings.header[0] = 'B';
    last_settings.header[1] = '7';
    last_settings.header[2] = 'V';
    last_settings.header[3] = '1';
    saveDataToEEPROM();
    while (Serial.available() > 0) { Serial.read(); }
    delay(250);
  }
  //
  eeprom_read_block((void*)&last_settings, (void*)0, sizeof(last_settings));*/
  //  
  if (last_settings.bMode == kModeSong) editZone = kZoneSongPat;
  else if (last_settings.bMode == kModeSongPerf) editZone = kZoneSPsection;
  else if (last_settings.bMode == kModePat) editZone = kZoneBPM;
  //
  delay(2000);
}

// ======================================================================================= //
void initData()
{
  memset(songPerfPosNext, -1, sizeof(songPerfPosNext));
  memset((void*)&last_settings,0,sizeof(last_settings));
  last_settings.seq_bpm = 120;
  last_settings.numberOfSteps = 16;
  initSongData();
}

// ======================================================================================= //
void initSongData()
{
  last_settings.loopSong = 0;
  for (int x=0; x<99; x++)
  {
    last_settings.songData[0][x] = 0;
    last_settings.songData[1][x] = B01100000;
    last_settings.songData[2][x] = B00011111;
  }
}
