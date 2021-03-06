/*
 * 
 * Beat707NXT_1PATTERN (No Flash/External Storage)
 * Created by William Kalfelz @ Beat707 (c) 2019 - http://www.Beat707.com / http://www.Wusik.com / http://www.Kalfelz.com
 * 
 * ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * 
 *  List of Changes
 *  
 *  V0004 - 04/13/2019
 *    * Added Program Change (PC) Offset to the Global Menu, with two values: starting at 0 or starting at 1. This is set per track.
 *    * Added Save to Internal EEPROM (uses the ATmega328 EEPROM) for all data. It compress the pattern steps to fit things out. Here I used a very simple and fast compression code. It checks for zeros in the data, it counts zeros until it finds a non-zero value. It writes a byte message that is the number of zeros up to 127 zeros. The last bit tells if this is zeros or non-zeros. This way I can save space when storing to EEPROM. The reading happens when you power up the device, and writting is done when you stop the sequencer. If the data haven't changed, nothing is saved. It also shows MEM*** where *** is the memory left in the EEPROM. This can be disabled in the EEPROM file: change #define EEPROM_SHOW_MEM_LEFT to 0
 *    
 *  V0002 - 04/12/2019
 *    * Removed some stuff from the interface that is pattern/bank related.
 *    * Added Track Len to the global menu. Goes from 1 to 16. It will follow the Variations ABCD but crop the track length using the LEN value instead. Default is 16, the whole track (all steps).
 *    
 *  V0001 - 04/11/2019
 *    * Fixed the MIDI Input Echo code so it doesn't conflicts with the sequencer interrupt. Before I start sending the note echo I set a variable that tells the sequencer interrupt to perform after I finish up sending the 3 bytes of MIDI information from the note on/off messages.
 *  
 *  V0000 - 04/11/2019
 *    * Improved the midi buffer output, using a faster code so notes are output faster. The code buffers the next tick of the sequencer ahead of time, so when the time comes to output the notes everything is already set in a buffer that is output quickly. The heavy task of "rendering" the next notes are always done after the current buffer is output to the MIDI interface (UART).
 * 
 *  V0000 - 04/10/2019
 *    * Initial Release: removed all flash and spi code. Didn't update the interface, so it will still show the pattern selector but when trying to change a pattern it will do nothing.
 *  
 * ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * 
 */

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define DEBUG_SERIAL 0        // Sets the serial output to 9600 bauds and sends some debuging information out.
#define DEBUG_MIDI_INPUT 0    // When set the whole unit will operate only to display midi input messages. You will need to set DRUM_TRACKS and NOTE_TRACKS to 1, as this Debug mode uses a lot of memory to store data. Pressing STOP clears the buffer, and pressing +/- shows next/previous event recorded. Pressing the last button on the right toggles between HEX or normal values for the messages.
#define MIDI_OVER_USB 0       // When set will use 38400 bauds for the Serial interface
#define MIDI_IN_START_NOTE 24 // MIDI Input to Pattern Selection Starting Note
#define RECORD_ENABLED_ECHO 1 // Enables the midi thru echo on all input notes
#define RECORD_FORCE_VEL 1    // When set velocity will always be the maximum for note track recording
#define EXTERNAL_CLOCK 0      // When set external clock is set by default
#define EXTERNAL_CONTINUE 0   // When set will use it to continue the sequener clock
#define TRACK_DEBUG_MIDI 0    // When set the code will store the last 4 bytes of MIDI Input Message and show those values when you hold the track selector button (Bt#8 on the left).
#define EEPROM_STORAGE 1      // When set it will use the internal EEPROM of the ATmega328 to store some data
//
#define VERSION_NUMBER 4

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "Functions.h"
#include "Variables.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void setup()
{
  DDRC = 0xFF; // For the Pulse-Out (analog pins, A54 Header)
  pulseOut(false);
  //
  startMIDIinterface();
  initTM1638();
  reset();
  EEPROM_Config_Read();
  EEPROM_Pattern_Read();
  //
  #if SHOW_FREE_RAM
    freeMemory();
    sendScreen();
    waitMs(2000);
  #endif 
  calculateSequencer++;
  //
  setupTimerForExternalMIDISync(!configData.midiClockInternal); 
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void loop() 
{
  #if !DEBUG_MIDI_INPUT
    if (showVersion)
    {
      ShowTemporaryMessage(kVersionNumberMessage);
      showVersion = false;
    }
    //
    doTickSequencer();
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
    //
  #else
    //
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
