/*
 * 
 * Beat707 NXT - Works with 3 x TM1638 boards connected in paralel
 * Created by William Kalfelz @ Beat707 (c) 2018 - http://www.Beat707.com / http://www.Wusik.com / http://www.Kalfelz.com
 * Creative Development by Gert Borovčak
 * 
 * https://github.com/Beat707/Beat707-NXT
 * 
 * Version 1.0.8 - October 26 2018
 * 
 *    Added external MIDI clock code.
 *    Better sequencer timing using extra MIDI buffer.
 *    New option for editing Note Tracks: when editing the steps (not editing a single step) you can hold one (or multiple) steps and use the +/- buttons to raise or lower the note(s) pitch. When you hold a step the BPM display on the left will change to P000 and show if you are adding or subtracting the pitch. While holding a step you can also press the track selector (button #8 from the left) to toggle between selected step(s) or ALL steps.
 *    New option to enable MIDI input to pattern selection MIDI_IN_TO_PATTERN.
 *    Added new option to record MIDI input RECORD_ENABLED - when enabled an extra option will appear in the global menu REC ON or OFF for the selected track. If NO tracks are selected to record, it will record the current selected track, otherwise it will record on multiple selected tracks and not the current selected one (if not selected to record on the global menu).
 *    Added separated button debounce values for when the sequencer is idle or playing (check the TM1638 file).
 *    Added ECHOS_ENABLED option to disable the MIDI Echo Delay FX.
 *    Changed how Drum and Note Tracks output MIDI information, doing a ping-pong from one type of track to another as they are output. EG: Drum1, Note1, Drum2, Note2, Drum3, Note3, ...
 *    Removed the Song Editor to give more room to other better functions for Live usage.
 *    Instead of Song# we now have Pattern Bank#.
 *    Reorganized the global menu to show things that are not global first, which will show all leds on when they are trully global.
 *    When editing notes if you turn it off now it will set velocity=0 and keep the note number, so if you turn a previously off-note it will remember which note is was.
 *    Added Echo/Delay FX to Note Tracks too.
 *    Added TRACK_DEBUG_MIDI, when set the code will store the last 4 bytes of MIDI Input Message and show those values when you hold the track selector button (Bt#8 on the left).
 *    Added SPI code into the folder and optimized a few things for shorter code.
 *    Removed all delay() code with a shorter custom waitMs() code, this also frees another timer.
 *    Replace all pinMode(), digitalRead() and digitalWrite() with direct port manipulation, this frees a lot of programming memory but also hardcodes all pins.
 *    Added another global option, Protect (Memory Protect), when On it will not save patters and any options. Use the TEMPORARY_MESSAGE_TIME value to adjust the message it shows if the memory is protected (max of 255).
 *    Added Pattern Plays, Next Pattern and Chain Plays. You can find those options in the Global Menu, and those values are saved per-pattern. The Chain Plays is separated from the Pattern Plays. So when the chain play is not OFF and INF, it will count how many times this pattern played and instead of going to the Next Pattern value it will go to the actual next pattern instead. See example below.
 *        If you want the following example to play: (P01 P02 P02 P03) (P01 P02 P02 P03) P04 and stay on 4. You set:
 *        Pattern 01 - Next Pattern 02  - Plays 1 - Chain Plays Off
 *        Pattern 02 - Next Pattern 03  - Plays 2 - Chain Plays Off
 *        Pattern 03 - Next Pattern 01  - Plays 1 - Chain Plays 002 (this sets this step as the end of a chain, and Next Step is the start of a chain)
 *        Pattern 04 - Next Pattern Off - Plays 1 - Chain Plays Off
 *    Removed the Solo option in the Mute page.
 *    To stop the sequencer you need to hold the stop button. Pressing stop without holding is ignored.
 *    By default all patterns will repeat until you select another one. You can press play while the sequencer is already playing to check the current repeat mode, press again to change the mode while the message is still been displayed. You have 3 options: Normal, Repeat Chain or Repeat Pattern. In normal mode it will play each pattern and repeat many times depending on the Pattern Plays amount. Once that's finished it will go to the next patter or to the pattern set in Pattern Next.
 *    Small cosmetic changes to the global menu display.
 *    Optimized and organized the code a bit better in a more correct C style.
 *    Several bug fixes.
 * 
 * Version 1.0.0 - Sep 28 2017
 * 

    EEPROM 1024 bytes
    512 patterns, up to 12 flash locale variations = 4 bits = 256 bytes
    768 bytes for pattern storage, working like this.
    
    0000 0000 = pattern 1 = 0 variation / pattern 2 = 0 variation
    0001 0000 = pattern 1 = 0 variation / pattern 2 = 1 variation
    1111 1111 = this block has been fully used, seek next block

    write to each 3 blocks variations in a round-robin style. Requires a read and a write. 
    If the content is 0xFF (11111111) it will seek another of the extra 2 variations, until it finds one that is not 0xFF. Since this is just 3 bytes to read it is very fast.

    
 * 
 */

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define RECORD_ENABLED 1      // When set allows midi notes to be recorded when you hold the play button to enable it
#define ECHOS_ENABLED 1       // When set adds the MIDI Echo Delay FX option to the code and the global menu
#define DISABLE_FLASH 0       // When set it won't use the external flash ic
#define INTRO_ANIMATION 1     // When set will show a random animation them the product name, version and company name

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define INIT_FLASH_MEMORY 0   // Warning, this will erase the entire flash contents
#define INIT_ENTIRE_FLASH 1   // When selected with the above, the entire flash chip will be erased first. This takes around 20 seconds or less to perform before we can write data to the flash chip. If this is not set, the code will erase the flash chip by sector, which takes a bit more time to perform.
#define SHOW_FREE_RAM 0       // Will show how much RAM is left on the display during initiation
#define DEBUG_SERIAL 0        // Sets the serial output to 9600 bauds and sends some debuging information out.
#define DEBUG_MIDI_INPUT 0    // When set the whole unit will operate only to display midi input messages. You will need to set DRUM_TRACKS and NOTE_TRACKS to 1, as this Debug mode uses a lot of memory to store data. Pressing STOP clears the buffer, and pressing +/- shows next/previous event recorded. Pressing the last button on the right toggles between HEX or normal values for the messages.
#define MIDI_OVER_USB 0       // When set will use 38400 bauds for the Serial interface
#define MIDI_IN_TO_PATTERN 1  // When set it will use any MIDI NOTE Input on the selected MIDI Channel (default 1) to select the next playing pattern. Starting from MIDI_IN_START_NOTE
#define MIDI_IN_START_NOTE 24 // MIDI Input to Pattern Selection Starting Note
#define RECORD_ENABLED_ECHO 1 // Enables the midi thru echo on all input notes
#define RECORD_FORCE_VEL 1    // When set velocity will always be the maximum for note track recording
#define EXTERNAL_CLOCK 0      // When set external clock is set by default
#define EXTERNAL_CONTINUE 0   // When set will use it to continue the sequener clock
#define TRACK_DEBUG_MIDI 0    // When set the code will store the last 4 bytes of MIDI Input Message and show those values when you hold the track selector button (Bt#8 on the left).
#define HAS_SYSEX 0

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "Lib_Flash.h"
#include "Functions.h"
#include "Variables.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void setup()
{
  startMIDIinterface();
  initTM1638();
  reset();
  splashScreen();
  //
  #if SHOW_FREE_RAM
    freeMemory();
    sendScreen();
    waitMs(2000);
  #endif 
  flashInit(false);
  calculateSequencer++;
  //
  setupTimerForExternalMIDISync(!configData.midiClockInternal); 
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void loop() 
{
  #if !DEBUG_MIDI_INPUT
    doTickSequencer();
    checkPatternStream();
    readButtons();
    //
    if (editingNoteTranspose != -127 && stepBeenHold() == -2) editingNoteTranspose = -127;
    //
    checkInterface(); 
    //
    if (configData.midiClockInternal) handleMIDIInput();
    if (updateScreen)
    {
      updateScreen = false;
      createScreen(); 
      //
      sendScreen();
    } 
  #else
    debugCheckMIDIInput();
    readButtons();
    debugCheckInterface();
    //
    if (updateScreen)
    {
      updateScreen = false;
      debugCreateScreen();
      //
      sendScreen();
    }
  #endif
}
