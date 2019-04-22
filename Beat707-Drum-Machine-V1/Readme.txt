Created by Beat707 (c) 2011 - http://www.Beat707.com

--------------------------------------------------------------------------------------------------------

October 04 2017

- Released complete Eagle and Schematics files.

--------------------------------------------------------------------------------------------------------

March 30 2011 - Version 1.0.0

- Complete Design with 3 Modes: Pattern, Song and File
- Midi SysEx Dump/Receive
- Midi Sync Modes: None, Slave, Master
- Drum Tracks and Synth Tracks
- 32 steps per pattern (quick shift toggles)
- 93 Patterns per Song + 99 Song-Pattern Positions on a Song
- 42 Songs Flash Memory Storage + 14 character name per Song

---------------------------------------------------------------------------------

April 17 2011 - Version 1.2.0
  
- This version uses Song Format 2, therefore is not compatible with previous Songs.
- Double Pattern Size: each pattern now holds 64 steps, instead of the 32 previous mode.
- Each pattern now has an A and B set of steps, each has 32 steps for a total of 64 steps.
- The Copy/Paste pattern area is now saved on the EEPROM. So you can copy a pattern from one song and past into another song's pattern.
- Improved the LCD code by reducing the LiquidCrystal library and creating the WLCD library on its place.
- Improved MIDI Clock by using the 16-bit Timer1 (before was 8-bit Timer2)
- BPM Range now goes from 25 to 255 BPM
- Added special Pattern Mirror Mode Editing (slow double click Shift or use the menu navigation)
- Added Midi Clock Shuffle (0 to 6)
- Added option to edit patterns from the Song mode. Just hit Record and it will go directly to the Pattern that is playing, hit Left+Right to go back to Song mode.
- When Playing in Song Mode, now you can go up and down Song Positions. (before you could only edit BPM)
- Added Left+Right key shortcut: cycles all 3 modes: Pattern, Song and File.
- Holding shift and Hiting Record will make the Shift key "stick". Release Shift and hit Record again to "un-stick" the Shift key.
- Quick Shift Click still goes from 1/16 to 1/32 step editing. (or 1/32 to 1/64 in double speed) But now, you don't see a message, just a small icon change next to the pattern number, where now shows A/B. A small square above A/B tells you are in double step editing mode.
- Better code for less Flash space even with the latest additions.
- New USB Mode that uses an extra program to talk to the unit directly without the need of a MIDI connection.

---------------------------------------------------------------------------------

May 14 2011 - Version 1.2.4

- Added Number of Steps (global)
- Added Enable Pattern AB. (when disabled, only Pattern A will play)
- Added ANALOG_INPUT_A0 to Config.h - Analog input option, by using the free Analog A0 Pin. (D14 on the Beat707 SV2 Headers) The current options are: BPM Tempo, Pattern Selector, Track Selector, Note Selector and Number of Steps. There's a 2 second delay for when you select a new input mode or press an optional button attached to Digital Pin 2. (the delay can be tweaked)
- Added GATE_OUTS to Config.h - Gate Output option for the first 3 tracks: T>01, T>02 and T>03. When enabled it will use 3 pins to output a gate voltage trigger on pins A0, D2 and D3. Check the Board Details PDF file for headers information - should be SV2 and SV3) Gates are fixed for Tracks 1, 2 and 3. (MIDI is disabled on those tracks) We also added the option to use GATE_OUTS_VEL_D3 to activate Velocity (PWM) on Track 3 (Digital Pin 3, D3)
- Added EXTRA_8_BUTTONS to the Config.h and also a new tab named W_Hacks - This new option will use the extra 8 buttons input header to read 8 inputs (no need for pull-up/down resistors, the hardware already has it) and call user-code that can be written on the W_I_ExtraBt tab. By default J1 is set to Sequencer Play/Stop.
- Updated S1/S2 Tracks Note Editing - Holding a step button and pressing up/down will change the octave of the selected step-note.
- Added MIDI_INPUT_ST to Config.h - When enabled will drive Midi Note Inputs to Tracks S1/S2 - this allows you to manipulate the 2x synth tracks directly from a Midi Keyboard (Omni MIDI Channel) - Keep in mind that you must have the cursor in the note-editing (zone 3) position.
- Added to the above option: MIDI_INPUT_AUTO - Auto-Step - When activated and a new note is hit, the current editing step will move to the next one (as defined in MIDI_INPUT_AUTO_N) 
- Added to the above option: MIDI_INPUT_AUTO_V - When set, a low-velocity note will set an empty note (velocity < 40)
- Added to the above option: MIDI_INPUT_AU_LW 24 - When set, a lower-octave note will set an empty note (note < MIDI_INPUT_AU_LW)
- Added MIDIECHO_BYTRACK to Config.h - If set in conjunction with MIDIECHO, notes will be translated to the current selected track channel.
- Added MIDI_INPUT_REC to Config.h - Adds extra code for when Record is pressed in Pattern Mode - Input MIDI Notes will be added to the current playing steps. (Omni MIDI Channel)
- Added EXTRA_MIDI_IN_HACKS to Config.h - When set, will call midiInputHacks() in the W_Hacks Tab for any new Midi Input Data - It includes the following code: Program Change to Pattern Selection, Modulation Wheel to BPM Tempo, CC #2 to Number of Steps, Drums/S1/S2-Tracks KeyZone Split and Pitch Wheel (Bend) to Sequence Stop/Play
- Added ENCODER_INPUT to Config.h - When set, it will setup and read an endless encoder on pins D2 and D3. (see Header SV3) The encoder will act as an Up and Down button, therefore, working on anywhere in the interface where Up and Down strokes can be used to tweak values. Currently speed is not been detected, but this could change in the future
- Added ENCODER_SPEED to Config.h - When set, it will detect the speed of the encoder movement and apply changes faster or slower
- New MIDI over USB code that is compatible with the new USB to MIDI Converter applicated found at: http://www.beat707.com/w/downloads/software-and-sources - the previous VST is no longer compatible, we will work on a new version soon. To select USB mode, use the navigation menu to find the option USB MODE and enable it.
- New Sequencer Direction option: FWR = Forward (Normal) - REV = Reverse (Backwards) - RND = Random

---------------------------------------------------------------------------------

May 23 2011 - Version 1.3.0

- New LEDs Code - Adds 3 levels of light on each LED, so you can see if the current step velocity is low, medium or high. Internally we used Timer2 to do a fast PWM on all LEDs, and a MIDI Output buffer so any MIDI does not delays the LEDs PWM interrupt; this is done via the bufferMIDI variable. Be sure to read the description of LEDS_PWM_A1 and LEDS_PWM_A2 in the Config.h file, as those two values determinates how the LED PWM code works.
- Added INI_PATT_FULL_ACNT to Config.h - During Pattern Initialization both Accents tracks will be set to either full-value or the values set via the INI_PATT_USER_V option. (be sure to read the description of the INI_PATT_USER_V)
- A few bugs related to the LED Transport and Song Saving/Erasing were fixed.
- Several areas of the code were optimized for lower flash-program usage. The resulting code is actually larger, as we need to add more code to accomodate the LED PWM Timer and Interrupts.
- All extra library files are now included in the Library.h file, no need to update any library-folders anymore.
- Fixed a bug related to S1/S2 Tracks note editing.
- Updated: S1/S2 Tracks note editing - clicking on a step will rotate the following notes: None, Last-Edited-Note, Current-Note-Slide, Note-off.

---------------------------------------------------------------------------------

June 27 2011 - Version 1.4.2

- New Beat707 Manager code, to be used with an extra application made with Processing (this replaces all previous SysEx Dump/Receive)
- Added a new option to Config.h - #define SPI_CLOCK_SPEED - SPI Clock Speed - usually 0x00 (DIV4), but some Arduino boards may only work at 0x05 (DIV8)

---------------------------------------------------------------------------------

June 28 2011 - Version 1.4.4

- Fixed: Note-Numbers display were 1 note above what it should be.
- Fixed: Song-Mode code was saving the first 2 pattern-positions on the wrong EEPROM location.

---------------------------------------------------------------------------------

July 07 2011 - Version 1.4.5

- Better MIDI Sync Slave mode.
- Added new options to Config.h - EXTRA_MIDI_IN_H_1 and EXTRA_MIDI_IN_H_2 - see the Config.h file for details.

---------------------------------------------------------------------------------

July 27 2011 - Version 1.4.6

- Better MIDI Sync code, specially when using the EXTRA_MIDI_IN_H_2 option.
- Beat707 BigTime support: https://github.com/Beat707/Beat707-BigTime - check the Config.h for the new BEAT707_BIG_TIME option.
- Added XTH2_AUTO_STOP to Config.h - When set in conjunction with EXTRA_MIDI_IN_H_2, playback will stop at the end of the pattern. This is handy when using with a sequencer, so you can insert notes to select patterns, and there's no risk of double-notes when a pattern is about to re-start and a new one is selected.

---------------------------------------------------------------------------------

August 24 2011 - Version 1.4.8

- Added to Config.h MIDI_NO_OUT_BUFFER - When set MIDI notes won't be buffered, they will go out to the MIDI Serial Output right away, but this may flicker the LEDs a bit
- Added to Config.h DISABLE_SHFT_SHCUTS - When set this will disable some Quick and Double Shift ShortCuts: Double Steps Edit and Mirror Pattern On/Off
- Fixed a problem when switching MIDI Clock Modes (Slave, Master, Off)
- Fixed some problems with the MIDI Clock Slave Mode
- Clicking the Shift key no longer halts the MIDI Clock while in Slave mode
- Faster Pattern loading when using the EXTRA_MIDI_IN_H_2 option.

---------------------------------------------------------------------------------

October 29 2011 - Version 1.4.9

- Fixed problem with Beat707 Manager (be sure to enable it under the Config.h file if you plan on using it, set #define MANAGER_DUMP_RECV 1 in order to activate it, and switch Beat707 USB MODE to On)
- Added support to the new Arduino 1.0 IDE

---------------------------------------------------------------------------------

October 10 2017 - Version 1.5.0

- Arduino IDE 1.8.3 support (or newer).

---------------------------------------------------------------------------------