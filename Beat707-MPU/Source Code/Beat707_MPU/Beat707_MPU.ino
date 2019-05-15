/*
 * 
 * Beat707 MPU - MIDI Processing Unit
 * Created by William Kalfelz @ Beat707 (c) 2019 - http://www.Beat707.com
 * 
 * Pins are Hardwired: Pin 2 is LED Activity and Pin 3 is Button Input (using pull-up resistors). Connect Button to Ground. ATtiny2313 Used.
 * 
 *    To setup, hold the button until the LED stays on, release the button. 
 *    The next key setup the MODE, from the following list. It won't exit until you select a key below, once you do, the LED will go off.
 *      #60 - Middle C = No Mode, all keys will just echo from input to output.
 *      #61 - Key Switching. Now you need to press 4 keys: Low Key Zone, High Key Zone, Key Root 1 and Root 2.
 *      #62 - Low Key to MIDI Channel change. Now you need to press 2 keys: Low Key Zone, High Key Zone. Low Key is MIDI Channel 1 and so on forward.
 *      #63 - Ignore MIDI Clock's Start and Stop Messages.
 *      #64 - Sequencer Clock. Keys you need to press: Key Zone Low, High, Sequencer Start, Stop, Speed Up and Down. Speed goes from 50 to 200, with a 10 increment. Default is 120.
 * 
 * Version 1.0.0 - 11/04/2017 - Initial Version
 * Version 1.0.2 - 11/06/2017 - Cleaned up the code and added new modes
 * Version 1.0.4 - 11/07/2017 - Cleaned up the code again. Added Sequencer Clock Mode
 * 
 */

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "Serial_Lite.h"
#include <avr/eeprom.h>
#define HEADER1 82
#define HEADER2 54
#define LED_ACTIVITY_TIME 8

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
byte led_On[2] = {0, 0};
byte buttonCheck[2] = {0, 0};
byte learnNewSettings = 0;
bool errorLearning = false;
byte learnNewSettingsSize = 2;
byte globalVars[8];
char forceMIDIChannel = -1;
byte lowKeyZone[2] = {0, 0};

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
enum
{
  kMode = 0, // The First Setting
  //
  // Modes -------------- //
  kMode_None = 0, kMode_KeySwitching, kMode_LowKeysToChannel, kModeIgnoreStartStop, kMode_Sequencer, kMode_Last,
  //
  // Key Switching ------------ //
  kKeyS_Glob_Active = 0,
  kKeyS_Set_KeyLow = 1, kKeyS_Set_KeyHigh, kKeyS_Set_KeyRoot1, kKeyS_Set_KeyRoot2, kKeyS_Set_Last,
  //
  // Low Keys to MIDI Channel ------------ //
  kKeySM_Set_KeyLow = 1, kKeySM_Set_KeyHigh, kKeySM_Set_Last,
  //
  // Sequencer Clock ------------ //
  kSequencer_Glob_BPM = 0,
  kSequencer_KeyLow = 1, kSequencer_KeyHigh, kSequencer_Start, kSequencer_Stop, kSequencer_SpeedDown, kSequencer_SpeedUp, kSequencer_Last,
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
struct SettingsStruct
{
  SettingsStruct::SettingsStruct() { init(); };
  void init()
  {
    header[0] = HEADER1;
    header[1] = HEADER2;
    memset(settings, 0, sizeof(settings));
  };
  byte header[2];
  byte settings[8];
};
SettingsStruct allSettings;

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void initGlobalVars()
{
  memset(globalVars, 0, sizeof(globalVars));
  forceMIDIChannel = -1;
  lowKeyZone[0] = lowKeyZone[1] = 0;
  //
  switch (allSettings.settings[kMode])
  {
    case kMode_KeySwitching: 
      globalVars[kKeyS_Glob_Active] = 1;
      lowKeyZone[0] = allSettings.settings[kKeyS_Set_KeyLow];
      lowKeyZone[1] = allSettings.settings[kKeyS_Set_KeyHigh];
    break;
    //
    case kMode_LowKeysToChannel:
      lowKeyZone[0] = allSettings.settings[kKeySM_Set_KeyLow];
      lowKeyZone[1] = allSettings.settings[kKeySM_Set_KeyHigh];
    break;
    //
    case kMode_Sequencer:
      globalVars[kSequencer_Glob_BPM] = 7;
      lowKeyZone[0] = allSettings.settings[kSequencer_KeyLow];
      lowKeyZone[1] = allSettings.settings[kSequencer_KeyHigh];
    break;
  }  
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void setup() 
{
  // Here we use direct port manipulation to save up on flash space //
  DDRA |= (1<<PA1);  // Pin 2 Output
  DDRA &= ~(1<<PA0); // Pin 3 Configure as an input
  PORTA |= (1<<PA0); // Pin 3 Activate pull-ups 
  //
  LED_Activity_On(200);
  readEEPROM();
  XSerial.begin();
  //
  // Check EEPROM //
  if (allSettings.header[0] != HEADER1 || allSettings.header[1] != HEADER2) 
  {
    allSettings.init();
    saveEEPROM();
  }
  initGlobalVars();  
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void LED_Activity_On(byte xtime)
{
  if (led_On[0] == 0) LED_STATE(HIGH);
  led_On[0] = 250;
  led_On[1] = xtime;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
inline void LED_STATE(byte state)
{
  if (state == HIGH) PORTA |= (1<<PA1); else PORTA &= ~(1<<PA1);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
byte SerialNextByte()
{
  while (!XSerial.hasSomething()) { ; }
  return XSerial.readByte();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void midiChannelCheck(byte &value)
{
  if (forceMIDIChannel != -1)
  {
    value &= 0xF0;
    value += forceMIDIChannel;
  }
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool isInsideLowKeyZone(byte note)
{
  return (lowKeyZone[1] > 0 && note >= lowKeyZone[0] && note <= lowKeyZone[1]);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void setupNewMode()
{
  saveEEPROM();
  initGlobalVars();
  learnNewSettings = 0;
  LED_STATE(HIGH);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void saveEEPROM() 
{ 
  eeprom_write_block(&allSettings, (const void*)0, sizeof(allSettings));
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
inline void readEEPROM() 
{ 
  eeprom_read_block(&allSettings, (const void*)0, sizeof(allSettings));
}
