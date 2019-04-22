/*

  Created by Beat707 (c) 2011 - http://www.Beat707.com
  
  Pinage Configuration File
  
*/

#ifndef EXTRASETTINGS_h
#define EXTRASETTINGS_h

  // ===================================================================================================================================================================== //
  #define MAXSONGPOS 99         // By changing any of those 2 settings you will need to re-do the Storage code sizes definitions for patterns, setup and song storage
  #define MAXSPATTERNS 90       // Check W_Storage to see the size of each pattern, so you know how many patterns you can store on an EEPROM chip
  #define MAXSONGSFILE 21       // Used by the Flash Storage, to determinate how many songs the Flash memory can hold (see W_Storage)
  #define MIDIECHO 0            // Copies all Midi Input to the Midi Output (check EXTRA_MIDI_IN_HACKS below, as when its on, by default it will split the keyboard into 3 zones: Drums/S1/S2)
    #define MIDIECHO_BYTRACK 0  // If set in conjunction with MIDIECHO, notes will be translated to the current selected track channel

  // ===================================================================================================================================================================== // 
  // List of possible Hacks and Mods - Note: ANALOG_INPUT_A0, GATE_OUTS and ENCODER_INPUT can be used only one at a time (as they use the same pins)
  // Most functions are set in the W_Hacks Tab and used in the W_Loop and W_Midi Tabs
  #define ANALOG_INPUT_A0 0     // Reads the analog input A0 (D14 on the Beat707 SV2 Headers) for multiple options (also uses ANALOG_PATT_MAX and ANALOG_MDLY below)
    #define ANALOG_INPUT_BT 0   // When Enabled in conjunction with ANALOG_INPUT_A0, it will only work if a button is attached and pressed on Digital Pin 2 (S2 on Beat707 SV3 Headers) Pull-Up Resistors internally used, so all you need to do is attach a regular button to the port and connect the button to ground.
    #define ANALOG_PATT_MAX 16  // If Analog Input is enabled and is in Pattern mode, this will define the max number of patterns to select. (0 to MAX)
    #define ANALOG_MDLY 100     // If Analog Input is enabled, the delay for when a new mode is selected (in ms) or, if ANALOG_INPUT_BT is enabled, the time to pause when the button is pressed 
  #define GATE_OUTS 0           // When enabled adds 3 Gate Outputs on pins A0, D2 and D3. (check the Board Details PDF file for headers information - should be SV2 and SV3) Gates are fixed for Tracks 1, 2 and 3. (MIDI is disabled on those tracks)
    #define GATE_OUTS_TIME 15   // Time of the Gate Trig (from High to Low)
  #define ENCODER_INPUT 0       // When set, it will setup and read an endless encoder on pins D2 and D3. (see Header SV3) The encoder will act as an Up and Down button, therefore, working on anywhere in the interface where Up and Down strokes can be used to tweak values. Currently speed is not been detected, but this could change in the future
    #define ENCODER_SPEED 0     // When set, it will detect the speed of the encoder movement and apply changes faster or slower
  #define EXTRA_8_BUTTONS 0     // Will use the extra 8 buttons input header to read 8 inputs (no need for pull-up/down resistors, the hardware already has it) and call user-code that can be written on the W_Hacks tab. By default, J1 is for sequencer play / stop.
  #define MIDI_INPUT_REC 1      // Adds extra code for when Record is pressed in Pattern Mode - Input MIDI Notes will be added to the current playing steps. (Omni MIDI Channel)
  #define MIDI_INPUT_ST 0       // Midi Note Input to Tracks S1/S2 - this allows you to manipulate the 2x synth tracks directly from a Midi Keyboard (Omni MIDI Channel)
    #define MIDI_INPUT_AUTO 0   // Auto-Step - When activated and a new note is hit, the current editing step will move to the next one
    #define MIDI_INPUT_AUTO_N 0 // Used by the Auto-Step - number of steps to move when a new note is hit
    #define MIDI_INPUT_AUTO_V 0 // When set, a low-velocity note will set an empty note (velocity < 40)
    #define MIDI_INPUT_AU_LW 24 // When set, a lower-octave note will set an empty note (note < MIDI_INPUT_AU_LW)
  #define EXTRA_MIDI_IN_HACKS 0 // When set, will call midiInputHacks() in the W_Hacks Tab for any new Midi Input Data. (see below)
    #define EXTRA_MIDI_IN_H_1 0 // It includes the following code: Program Change to Pattern Selection, Modulation Wheel to BPM Tempo, CC #2 to Number of Steps, Drums/S1/S2-Tracks KeyZone Split and Pitch Wheel (Bend) to Sequence Stop/Play
    #define EXTRA_MIDI_IN_H_2 0 // It includes the following code: Note Number to Pattern Selection (starting on note 24, channel 16) Play on Note-On and Stop on Note 23, All-Notes-Off and MIDI Clock Stop (it doesn't respond to MIDI Clock Start)
      #define XTH2_AUTO_STOP 0  // When set in conjunction with EXTRA_MIDI_IN_H_2, playback will stop at the end of the pattern. This is handy when using with a sequencer, so you can insert notes to select patterns, and there's no risk of double-notes when a pattern is about to re-start and a new one is selected.
  #define BEAT707_BIG_TIME 0    // This is a special hack that requires a special hardware as seen on the following link: https://github.com/Beat707/Beat707-BigTime
    #define BIGTIME_ADDR 0x26   // Beat707 BigTime I2C Address in Hex format (should match TWI_SLAVE_ADDR from the BigTime code (found inside the Library.h file)
    
  // ===================================================================================================================================================================== //
  #define EXTENDED_DRUM_NAMES 1 // Add more GM Drum Note Names to the Track Drum Note Selectors
  #define STORAGE_FORCE_INIT 0  // Force an Initiation of all EEPROM memory during startup 
  #define MANAGER_DUMP_RECV 0   // Adds extra code to be used with the Beat707 Manager program in order to Dump and Receive complete machine data
  #define SHOWFREEMEM 0         // Outputs free RAM to LCD
  #define MSerial Serial        // Used for MIDI Input/Output
  #define SPI_CLOCK_SPEED 0x05  // SPI Clock Speed - usually 0x00 (DIV4), but some Arduino boards may only work at 0x05 (DIV8)
  #define INIT_EMPTY_SONG 1     // Determinates if an Empty Song should be saved during Initiation of the EEPROM + Flash
  #define DISABLE_MIDI 0        // Debug Only
  #define DISABLE_STORAGE_CHK 0 // Debug Only
  #define CHECK_CPU_USAGE 0     // Debug Only - Checks CPU usage (uses special macros: CHECK_CPU_START and CHECK_CPU_END)
  #define LEDS_PWM_A1 50        // The LEDs dim light is controlled by a PWM code, that counts from 0 to LEDS_PWM_A1, while LEDS_PWM_A2 is the near LEDS_PWM_A1 value. So, anything before LEDS_PWM_A1 is the lowest level, going above LEDS_PWM_A2 is the medium level and going over LEDS_PWM_A1 is the highest level.
  #define LEDS_PWM_A2 40        // The Medium level of the LEDs PWM dim lights.
  #define MIDI_NO_OUT_BUFFER 0  // When set MIDI notes won't be buffered, they will go out to the MIDI Serial Output right away, but this may flicker the LEDs a bit
  #define DISABLE_SHFT_SHCUTS 0 // When set this will disable some Quick and Double Shift ShortCuts: Double Steps Edit and Mirror Pattern On/Off
  #define INI_PATT_FULL_ACNT 1  // During Pattern Initialization both Accents tracks will be set to either full-value or the values set below (depending if you use INI_PATT_USER_V or not)
    #define INI_PATT_USER_V 1   // If set, instead of setting everything to 1, it will use the values set below
    #define INI_PATT_FULL_AC_1A "1010101010101010"
    #define INI_PATT_FULL_AC_1B "1010101010101011"
    #define INI_PATT_FULL_AC_2A "1000100010001000"
    #define INI_PATT_FULL_AC_2B "1000100010001011"
  #if ANALOG_INPUT_A0
    #define LAST_PATT_ZONE 16   // The code of the last Pattern Zone. If you add a new option/zone, you will need to update this value.
  #else
    #define LAST_PATT_ZONE 15
  #endif
  
  // ===================================================================================================================================================================== //  
  // Buttons and LEDs using C165 and C595 Shifters //
  // An Important Note - since the latest version the following pins can't be changed anymore: LATCHOUT and SWITCH_SSn
  #define LATCHOUT 8
  #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    #define FLASH_SSn 55
    #define SWITCH_SSn 56
    #define MIDI_ENn 57
  #else
    #define FLASH_SSn 15
    #define SWITCH_SSn 16
    #define MIDI_ENn 17
  #endif

  #define BTN_LEFT  B00000001
  #define BTN_RIGHT B00000010
  #define BTN_PLAY  B00000100
  #define BTN_STOP  B00001000
  #define BTN_REC   B00010000
  #define BTN_SHIFT B00100000
  #define BTN_UP    B01000000
  #define BTN_DOWN  B10000000

  #define BTN_EXT1 B00000001
  #define BTN_EXT2 B00000010
  #define BTN_EXT3 B00000100
  #define BTN_EXT4 B00001000
  #define BTN_EXT5 B00010000
  #define BTN_EXT6 B00100000
  #define BTN_EXT7 B01000000
  #define BTN_EXT8 B10000000

  #define SONG_VERSION 2 // As used by the Storage to check the song format version //
 
#endif
