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
#define SHOWFREEMEM 0        // Debug Only - Prints on the Serial output the current RAM left
#define BUTTON_PIN 8         // General-purpose digital input pin (active low)
#define ANALOG_PIN 0         // General-purpose potentiometer input
#define MIDIchannel 9        // Regular MIDI Channel - 1 (0 to 15)
#define MIDIactivityPin 13
#define MAXVOICES 6
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#  define MIDI_EN 57     // PF3 = 57
#else
#  define MIDI_EN 17
#endif
unsigned long MIDIactivityMillis = millis()+25;
byte MIDIactivity = 0;
unsigned int sampleLen[MAXVOICES];
int samplePos[MAXVOICES][2];  // tune/fine
char rate[MAXVOICES][2];      // tune/fine
unsigned char noteVelMatrix[6][242];  // This is used to add Velocity without doing any heavy processing divisions (but also uses most of the RAM)
unsigned char* samplePCM[MAXVOICES];
unsigned int mixer[2] = {363,363};
unsigned char voice = 0;
unsigned char notevel[MAXVOICES];
byte incomingByte;
byte note;
byte noteOn = 0;
byte state = 0;
byte sound = 0;
boolean flipFlop = false;
boolean isReverse = false;
int freeMemory();

// ======================================================================================= //
void setup()
{   
    memset(sampleLen,0,sizeof(sampleLen));
    memset(samplePos,0,sizeof(samplePos));
    memset(samplePCM,0,sizeof(samplePCM));
    memset(rate,0,sizeof(rate));
    memset(notevel,3,sizeof(notevel));
    
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
      
    #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
      pinMode(6,OUTPUT);
      pinMode(7,OUTPUT);
    #else
      pinMode(9,OUTPUT);
      pinMode(10,OUTPUT);
    #endif
    
    pinMode(BUTTON_PIN,INPUT);
    digitalWrite(BUTTON_PIN,HIGH);
    pinMode(ANALOG_PIN,INPUT);
    pinMode(MIDIactivityPin,OUTPUT);
    digitalWrite(MIDIactivityPin,LOW);
    
    #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
        // Timer4 is configured in Mode 14 (WGM4_3:0=1110), Fast PWM mode with ICR4 as TOP, no prescale.
        // The OC4A/OC4B outputs are configured to clear on compare match and set at BOTTOM.
        // The TOP value (ICR4) is set to 725 to generate a sample rate of 16E6/(725+1) = 22039 Hz.
        TCCR4A = _BV(COM4A1) | _BV(COM4B1) | _BV(WGM41);
        TCCR4B = _BV(WGM42) | _BV(WGM43) | _BV(CS40);
        ICR4 = (726-1);
        TIMSK4 = _BV(TOIE4); // Enable timer overflow interrupt, once every 1/22039Hz = 45.375us.
        OCR4A = OCR4B = 363; // (242/2)*3
    #else
        // Timer1 is configured in Mode 14 (WGM1_3:0=1110), Fast PWM mode with ICR1 as TOP, no prescale.
        // The OC1A/OC1B outputs are configured to clear on compare match and set at BOTTOM.
        // The TOP value (ICR1) is set to 725 to generate a sample rate of 16E6/(725+1) = 22039 Hz.
        TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM11);
        TCCR1B = _BV(WGM12) | _BV(WGM13) | _BV(CS10);
        ICR1 = (726-1);
        TIMSK1 = _BV(TOIE1); // Enable timer overflow interrupt, once every 1/22039Hz = 45.375us.
        OCR1A = OCR1B = 363; // (242/2)*3
    #endif
    
    sei(); // enable global interrupts
    
    #if SHOWFREEMEM
      Serial.begin(38400);  
      Serial.print("Free Mem: ");
      Serial.println(freeMemory());
      Serial.println("");
    #endif
  
    // MIDI Startup //
    Serial.begin(31250);
    pinMode(MIDI_EN, OUTPUT); digitalWrite(MIDI_EN, HIGH);
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
}

// ======================================================================================= //
void checkInterface(void)
{
  uint16_t analogValue = analogRead(ANALOG_PIN);
  isReverse = !digitalRead(BUTTON_PIN);  
  rate[0][1] = rate[1][1] = rate[2][1] = rate[3][1] = rate[4][1] = rate[5][1] = (((analogValue/120)==0) ? analogValue : 0); //  > 120 counter
  rate[0][0] = rate[1][0] = rate[2][0] = rate[3][0] = rate[4][0] = rate[5][0] = (analogValue/120) * ((isReverse) ? -1 : 1);
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
  
  checkInterface();
  
  flipFlop = !flipFlop;
  
  if (MIDIactivity && MIDIactivityMillis < millis())
  {
    MIDIactivity = 0;
    digitalWrite(MIDIactivityPin,LOW);
  }
  
  if (Serial.available() > 0) 
  {  
    incomingByte = Serial.read();
    switch (state)
    {
      case 0:
        if (incomingByte >= 128)
        {
          digitalWrite(MIDIactivityPin,HIGH);
          MIDIactivityMillis = millis()+25;
          MIDIactivity = 1;
          
          if (incomingByte <= 143) // Note Off //
          {
            if ((incomingByte-128) == MIDIchannel) noteOn = 0;
            state = 1;
          }
          else if (incomingByte <= 159) // Note On //
          {
            if ((incomingByte-144) == MIDIchannel) noteOn = 1;
            state = 1;
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
           if (noteOn == 1 && incomingByte > 0) playNote(note, incomingByte);
           noteOn = 0;
         }
         state = 0;  // reset //
         break;
     }
  }
}

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
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
ISR(TIMER4_OVF_vect)
#else
ISR(TIMER1_OVF_vect)
#endif
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

// ======================================================================================= //
// Checks the RAM left - the ATmega328 has only 2K of RAM //
#if SHOWFREEMEM
  extern unsigned int __data_start;
  extern unsigned int __data_end;
  extern unsigned int __bss_start;
  extern unsigned int __bss_end;
  extern unsigned int __heap_start;
  extern void *__brkval;
  
  int freeMemory()
  {
    int free_memory;
  
    if((int)__brkval == 0)
       free_memory = ((int)&free_memory) - ((int)&__bss_end);
    else
      free_memory = ((int)&free_memory) - ((int)__brkval);
  
    return free_memory;
  }
#endif
