/* 
 *  MIDI Keyboard by WilliamK (c) 2016 - www.William-K.com - www.Beat707.com - www.Wusik.com
 *  
 *  8 x 8 Button Matrix - To avoid ghosting in the notes you must add a diode on each button
 *  
 *  The last Upper Key is the function Key, plus pressing together the below = special function
 *      1,2,3 Octave = down/center/up
 *      6,7,8 Transpose = down/center/up
 *      15 = Fixed Velocity 127 (default)
 *      17 = Random Velocity 1
 *      18 = Random Velocity 2 
 *      Near last upper key = PANIC
 *      The other last 16 keys = Midi Channel # (default is 1)
 */

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------- //
#include "Keypad_3_2.h"
enum { velFixed = 0, velRnd1, velRnd2, midiNoteOn = 0x90, midiNoteOff = 0x80, midiCC = 0xB0 };
char velMode = velFixed;
char midiChannel = 0;
char octave = 0;
char transpose = 0;
char velocity = 127;
const char baseOctave = (2 * 12);
const byte ROWS = 8;
const byte COLS = 8;
char keys[ROWS][COLS] = 
{
  { 0,  1,  2,  3,  4,  5,  6,  7},
  { 8,  9, 10, 11, 12, 13, 14, 15},
  {16, 17, 18, 19, 20, 21, 22, 23},
  {24, 25, 26, 27, 28, 29, 30, 31},
  {32, 33, 34, 35, 36, 37, 38, 39},
  {40, 41, 42, 43, 44, 45, 46, 47},
  {48, 49, 50, 51, 52, 53, 54, 55},
  {56, 57, 58, 59, 60, 61, 62, 63}
};
byte rowPins[ROWS] = {A0, A1, A2, A3, A4, A5, 4, 3};
byte colPins[COLS] = {12, 11, 10, 9, 8, 7, 6, 5};
//
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------- //
void setup() 
{
  Serial.begin(31250);
  MIDIallNotesOff();
  kpd.setHoldTime(8000); // not really using it, ms
  kpd.setDebounceTime(8); // ms
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------- //
void loop() 
{
  if (kpd.getKeys())
  {
    switch (velMode)
    {
      case velFixed: velocity = 127; break;
      case velRnd1: velocity = random(120, 127); break;
      case velRnd2: velocity = random(90, 127); break;
    }
    //      
    for (int i=0; i<LIST_MAX; i++) // Scan the whole key list.
    {
      if ( kpd.key[i].stateChanged) // Only find keys that have changed state.
      {
        switch (kpd.key[i].kstate) // Report active key state
        {
            case PRESSED: sendNote(midiNoteOn, kpd.key[i].kchar); break;
            case RELEASED: sendNote(midiNoteOff, kpd.key[i].kchar); break;
        }
      }
    }
  }
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MIDIallNotesOff()
{
  for (byte x=0; x<16; x++)
  {
    midiMessage(0xB0+x, 0x7B, 0x00);
  }
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------- //
void sendNote(byte type, byte note)
{
  int xnote = note + (octave*12) + transpose + baseOctave;
  if (xnote < 0) xnote = 0;
  if (xnote > 127) xnote = 127;
  char xvelocity = 0;
  if (type == midiNoteOn) xvelocity = velocity;
  midiMessage(type + midiChannel, xnote, xvelocity);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------- //
void midiMessage(byte val1, byte val2, byte val3) 
{
  Serial.write(val1);
  Serial.write(val2);
  Serial.write(val3);
}

