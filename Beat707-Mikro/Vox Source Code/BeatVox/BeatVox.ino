/*

  Created by Beat707 (c) 2011 - http://www.Beat707.com
  
  The code uses the MIDI Shield: http://www.ruggedcircuits.com/html/flexible_midi_shield.html

*/

#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "PCM_Sound.h"

// ======================================================================================= //
#define TEST_LOOP_PATTERN 0  // Debug Only - Adds a small loop pattern to test the code
#define USE_EXTERNAL_CTRLS 0 // When enabled it will use the BUTTON_PIN for REVERSE and ANALOG_PIN to change PITCH of all voices
#define BUTTON_PIN 8         // General-purpose digital input pin (active low) for REVERSE Sound
#define ANALOG_PIN 0         // General-purpose potentiometer input for PITCH changes
#define MIDIchannel 9        // Regular MIDI Channel - 1 (0 to 15)
#define MAXVOICES 6
//
unsigned int sampleLen[MAXVOICES];
unsigned char sampleNote[MAXVOICES];
int samplePos[MAXVOICES][2];  // tune/fine
char rate[MAXVOICES][2];      // tune/fine
unsigned char noteVelMatrix[6][242];  // This is used to add Velocity without doing any heavy processing divisions (but also uses most of the RAM)
unsigned char* samplePCM[MAXVOICES];
unsigned int mixer[2] = {363,363};
unsigned char voice = 0;
unsigned char notevel[MAXVOICES];
byte incomingByte;
byte channel = 0;
byte note;
byte noteOn = 0;
byte state = 0;
byte sound = 0;
boolean flipFlop = false;
boolean isReverse = false;

// ======================================================================================= //
void setup()
{   
    memset(sampleLen,0,sizeof(sampleLen));
    memset(samplePos,0,sizeof(samplePos));
    memset(samplePCM,0,sizeof(samplePCM));
    memset(rate,1,sizeof(rate));
    memset(notevel,3,sizeof(notevel));
    memset(sampleNote,0,sizeof(sampleNote));
    
    // Create the Velocity Matrix Values //
    float val = 0.0f;
    float valr = 1.0f;
    for (int x=0; x<6; x++)
    {
      if (x == 1)        valr = 0.80f;
        else if (x == 2) valr = 0.60f;
        else if (x == 3) valr = 0.35f;
        else if (x == 4) valr = 0.20f;
        else if (x == 5) valr = 0.08f;

      for (int v=0; v<242; v++)
      {
        if (x == 0) noteVelMatrix[x][v] = v;
        else
        {
          val = (((2.0f/242.0f)*float(v))-1.0f)*valr;
          noteVelMatrix[x][v] = (unsigned int)((val+1.0f)*(242.0f/2.0f));
        }
      }
    }
      
    pinMode(9,OUTPUT);
    pinMode(10,OUTPUT);
    //    
    pinMode(BUTTON_PIN,INPUT);
    digitalWrite(BUTTON_PIN,HIGH);
    pinMode(ANALOG_PIN,INPUT);
    //
    // Timer1 is configured in Mode 14 (WGM1_3:0=1110), Fast PWM mode with ICR1 as TOP, no prescale.
    // The OC1A/OC1B outputs are configured to clear on compare match and set at BOTTOM.
    // The TOP value (ICR1) is set to 725 to generate a sample rate of 16E6/(725+1) = 22039 Hz.
    TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM11);
    TCCR1B = _BV(WGM12) | _BV(WGM13) | _BV(CS10);
    ICR1 = (726-1);
    TIMSK1 = _BV(TOIE1); // Enable timer overflow interrupt, once every 1/22039Hz = 45.375us.
    OCR1A = OCR1B = 363; // (242/2)*3
    sei(); // enable global interrupts
     
    // MIDI Startup //
    Serial.begin(31250);
}

// ======================================================================================= //
boolean pingVoice = true;
unsigned char getFreeVoice(void)
{
  if (pingVoice)
  {
    pingVoice = false;
    if (samplePCM[0] == 0) return 0;
    if (samplePCM[1] == 0) return 1;
    if (samplePCM[2] == 0) return 2;
    if (samplePCM[3] == 0) return 3;
    if (samplePCM[4] == 0) return 4;
    if (samplePCM[5] == 0) return 5;
  }
  else
  {
    pingVoice = true;
    if (samplePCM[5] == 0) return 5;
    if (samplePCM[4] == 0) return 4;
    if (samplePCM[3] == 0) return 3;
    if (samplePCM[2] == 0) return 2;
    if (samplePCM[1] == 0) return 1;
    if (samplePCM[0] == 0) return 0;
  }
  
  return 5;
}

// ======================================================================================= //
void playNote(unsigned char note, unsigned char velocity)
{
    if (note == 36) note = 60;
    if (note == 37) note = 61;
    if (note == 38) note = 62;
    if (note == 39) note = 63;
     
    if (note < 60) return;
    sound = note-60;
    if (sound >= PCMtotal) return;
      
    voice = getFreeVoice();
    samplePCM[voice] = 0; // First stop any sound reference //
    if (isReverse) samplePos[voice][0] = PCMlen[sound]-1; else samplePos[voice][0] = 0;
    samplePos[voice][1] = 0;
    notevel[voice] = 5-min(5,velocity/25);
    sampleLen[voice] = PCMlen[sound];
    samplePCM[voice] = PCMdata[sound];
    sampleNote[voice] = note;
}

// ======================================================================================= //
void stopNote(unsigned char note)
{
  for (char x=0; x<MAXVOICES; x++)
  {
    if (sampleNote[x] == note)
    {
      sampleNote[x] = 0; 
      samplePCM[x] = 0;
    }
  }
}

// ======================================================================================= //
void checkInterface(void)
{
  #if USE_EXTERNAL_CTRLS
    uint16_t analogValue = analogRead(ANALOG_PIN);
    isReverse = !digitalRead(BUTTON_PIN);  
    rate[0][1] = rate[1][1] = rate[2][1] = rate[3][1] = rate[4][1] = rate[5][1] = (((analogValue/120)==0) ? analogValue : 0); //  > 120 counter
    rate[0][0] = rate[1][0] = rate[2][0] = rate[3][0] = rate[4][0] = rate[5][0] = (analogValue/120) * ((isReverse) ? -1 : 1);
  #endif
}

// ======================================================================================= //
void loop()
{
  #if TEST_LOOP_PATTERN
    checkInterface();
    if (flipFlop) playNote(62, 127); else playNote(63, 127);
    delay(40);
    checkInterface();
    if (flipFlop) playNote(62, 60); else playNote(63, 60);
    delay(40);
    checkInterface();
    if (flipFlop) playNote(62, 40); else playNote(63, 40);
    delay(40);
    
    checkInterface();
    if (flipFlop) playNote(62, 20); else playNote(63, 20);
    checkInterface();
    if (flipFlop) playNote(61, 127); else playNote(60, 127);
    delay(60);
    checkInterface();
    if (flipFlop) playNote(61, 40); else playNote(60, 40);
    delay(60);
    checkInterface();
    
    checkInterface();
    if (flipFlop) playNote(66, 127); else playNote(65, 127);
    if (flipFlop) playNote(64, 40); playNote(67, 40); 
    delay(40);
    checkInterface();
    if (flipFlop) playNote(66, 60); else playNote(65, 60);
    if (flipFlop) playNote(64, 20); playNote(67, 20); 
    delay(40);
    checkInterface();
    if (flipFlop) playNote(66, 40); else playNote(65, 40);
    delay(40);
  #endif
  //
  checkInterface();
  //  
  flipFlop = !flipFlop;
  //
  if (Serial.available() > 0) 
  {
    incomingByte = Serial.read();
    switch (state)
    {
      case 0:
        if (incomingByte >= 128)
        {
          if (incomingByte <= 143) // Note Off //
          {
            noteOn = 0;
            state = 1;
            channel = incomingByte-128;
          }
          else if (incomingByte <= 159) // Note On //
          {
            noteOn = 1;
            state = 1;
            channel = incomingByte-144;
          }
        }
        break;
        
       case 1:
         if(incomingByte < 128) // Note Number //
         {
            note = incomingByte;
            state = 2;
         }
         else state = 0;  // reset //
         break;
       
       case 2:
         if(incomingByte < 128) // Velocity //
         {
          if (channel == MIDIchannel)
          {
            if (noteOn == 1 && incomingByte > 0) playNote(note, incomingByte); else stopNote(note);
          }
           noteOn = 0;
         }
         state = 0;  // reset //
         break;
     }
  }
}

// if (samplePos[nv][0] >= sampleLen[nv] || samplePos[nv][0] < 0) samplePCM[nv] = 0;\
// ======================================================================================= //
#define nextSample(nv,nm)\
  if (samplePos[nv][0] >= sampleLen[nv] || samplePos[nv][0] < 0) samplePCM[nv] = 0;\
  if (samplePCM[nv] != 0)\
  {\
    mixer[nm] += noteVelMatrix[notevel[nv]][(unsigned int)pgm_read_byte(&samplePCM[nv][samplePos[nv][0]])];\
    samplePos[nv][0] += rate[nv][0];\
    samplePos[nv][1] += rate[nv][1];\
    if (samplePos[nv][1] > 120) { samplePos[nv][1] = 0; if (isReverse) samplePos[nv][0]--; else samplePos[nv][0]++; }\
  } else mixer[nm] += 121;

// ======================================================================================= //
ISR(TIMER1_OVF_vect)
{   
  // Do this first so the PWM is updated faster //
  #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    OCR4A = mixer[0];
    OCR4B = mixer[1];
  #else
    OCR1A = mixer[0];
    OCR1B = mixer[1];
  #endif

  // Now Calculate the next samples //
  mixer[0] = mixer[1] = 0;
  nextSample(0,0);
  nextSample(1,0);
  nextSample(2,0);
  nextSample(3,1);
  nextSample(4,1);
  nextSample(5,1);
}
