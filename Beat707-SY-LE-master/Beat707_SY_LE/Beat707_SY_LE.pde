/*

  Created by Beat707 (c) 2011 - http://www.Beat707.com

  August 18 2011 - Version 1.0.0
  
  Requires the MIDI Shield: 
    http://www.ruggedcircuits.com/html/flexible_midi_shield.html
    
  And 8 x 10k Resistors on each output, starting on digital output 2 ... 9 - connect all resistors to Left+Right of a Headphone or Line-Output, and Arduino-Ground to Output-Ground.

*/

#define MULT 65535
#define MULT2 (65535/2)
#define MTYPE unsigned int
#define MAX_V 8
#define SAMPLE_RATE 48048.048f
#define TIMER_ICR 333
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#  define MIDI_EN 57     // PF3 = 57
#else
#  define MIDI_EN 17
#endif
MTYPE oscPos[MAX_V];
MTYPE oscRate[MAX_V];
MTYPE oscRateOriginal[MAX_V];;
MTYPE oscPWM = MULT2;
MTYPE freqTable[128];
uint8_t maxVoices = MAX_V;
uint8_t outPins = 0;
char pitchBend = 64;
uint8_t voiceNote[MAX_V];
uint8_t voiceOrder[MAX_V];
uint8_t voiceOrderCounter = 0;
uint8_t voiceActive[MAX_V];
uint8_t deviation = 1;
uint8_t deviationMode = 1;
uint8_t velocityMode = 0;
byte incomingByte;
byte note;
byte noteOn = 0;
byte state = 0;

// ======================================================================================= //
void setup() 
{ 
  memset(oscPos,0,sizeof(oscPos));
  memset(oscRate,0,sizeof(oscRate));
  memset(oscRateOriginal,0,sizeof(oscRateOriginal));
  memset(voiceNote,0,sizeof(voiceNote));
  memset(voiceOrder,0,sizeof(voiceOrder));
  memset(voiceActive,0,sizeof(voiceActive));
  
  double k = 1.059463094359; // Frequency (Hz) Table
  double a = 6.875;
  a *= k; a *= k; a *= k;
  for (int i = 0; i < 128; i++) 
  { 
    freqTable[i] = (MTYPE) (MULT * (1.0f/SAMPLE_RATE) * a);
    a *= k;
  }
   
  TCCR1A = _BV(WGM11);
  TCCR1B = _BV(WGM12) | _BV(WGM13) | _BV(CS10);
  ICR1 = TIMER_ICR-1;
    
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  
  TIMSK1 = _BV(TOIE1);
  sei();
      
  Serial.begin(31250);
  pinMode(MIDI_EN, OUTPUT); digitalWrite(MIDI_EN, HIGH);  
}

// ======================================================================================= //
uint8_t isDecay = 1;
uint8_t pwmModRate = 1;
uint8_t pwmModRateCounter = 0;
uint8_t pwmModAdder = 1;
#define calcOSC(a) oscPos[a] += oscRate[a]; if (voiceActive[a] == 1 && oscPos[a] < oscPWM) bitSet(outPins,a);

// ======================================================================================= //
ISR(TIMER1_OVF_vect)
{ 
  PORTD = outPins; // Pins 0 to 7 //
  PORTB = outPins; // Pins 8 to 13 //
  outPins = 0;
   
  calcOSC(0);
  calcOSC(1);
  calcOSC(2);
  calcOSC(3);
  calcOSC(4);
  calcOSC(5);
  calcOSC(6);
  calcOSC(7);
  
  if (pwmModAdder > 0)
  {
    pwmModRateCounter++;
    if (pwmModRateCounter > pwmModRate)
    {
      pwmModRateCounter = 0;
      
      if (isDecay == 1) 
      {
        oscPWM -= pwmModAdder; if (oscPWM < (MULT2/100)) isDecay = 0;
      }
      else
      {
        oscPWM += pwmModAdder; if (oscPWM >= MULT2) isDecay = 1;
      }
    }
  }
}

// ======================================================================================= //
void loop()
{    
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
          }
          else if (incomingByte <= 159) // Note On //
          {
            noteOn = 1;
            state = 1;
          }
          else if (incomingByte <= 191) // CC //
          {
            noteOn = 2; // Midi CC //
            state = 1;
          }
          else if (incomingByte <= 239) // Bend //
          {
            noteOn = 3; // Pitch Bend //
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
           if (noteOn == 1 && incomingByte > 0) 
           {
             if (velocityMode == 1) oscPWM = 1000.0f+(((float)incomingByte/127.0f)*(MULT2-1000));
             setNoteOn(note);  // Note On //
           }
           else if (noteOn == 2) // Midi CC //
           {
             switch (note)
             {
                case 0: deviationMode = incomingByte/40; if (deviationMode > 3) deviationMode = 3; break;
                case 1: if (pwmModAdder == 0) oscPWM = 1000.0f+(((float)incomingByte/127.0f)*(MULT2-1000)); else pwmModRate = (127-incomingByte)/4; break;
                case 2: pwmModAdder = incomingByte/4; if (pwmModAdder == 0) oscPWM = MULT2; break;               
                case 3: velocityMode = (incomingByte > 64); if (velocityMode == 1) pwmModAdder = 0; break;
             }
           }
           else if (noteOn == 3) // Pitch Bend //
           {
              pitchBend = incomingByte;
              for (char w=0; w<MAX_V; w++) { oscRate[w] = oscRateOriginal[w]+((pitchBend-64)/2); }             
           }
           else 
           {
             setNoteOff(note); // Note Off //
           }
           noteOn = 0;
         }
         state = 0;  // reset //
         break;
     }
  }
}

// ======================================================================================= //
void setNoteOn(uint8_t note)
{
  uint8_t voice = 255;
  maxVoices = MAX_V;
  if (deviationMode == 1) maxVoices = MAX_V/2;
  if (deviationMode >= 2) maxVoices = MAX_V/4;
  for (char v=0; v<maxVoices; v++)
  {
    if (voiceNote[v] == 0)
    {
      voice = v;
      break;
    }
  }
  
  // All voices used, steal oldest one //
  if (voice == 255)
  {
    uint8_t order = 255;
    for (char v=0; v<maxVoices; v++)
    {
      if (voiceOrder[v] < order)
      {
        order = voiceOrder[v];
        voice = v;
      }
    }
  }
  
  // Store note //
  voiceOrderCounter++;
  voiceNote[voice] = note;
  voiceOrder[voice] = voiceOrderCounter;
  
  // Set Output //
  oscRate[voice] = oscRateOriginal[voice] = freqTable[note];
  voiceActive[voice] = 1;

  if (deviationMode == 1)
  {
    deviation = 1;
    if (note > 48) deviation = ((note-48)/4)+1;
    oscRate[voice+4] = oscRateOriginal[voice+4] = oscRate[voice] + deviation;
    voiceActive[voice+4] = 1;
  }
  else if (deviationMode == 2)
  {
    deviation = 1;
    if (note > 48) deviation = ((note-48)/4)+1;
    oscRate[voice+2] = oscRateOriginal[voice+2] = oscRate[voice] + deviation;
    voiceActive[voice+2] = 1;
    if ((note+12) > 127) note -= 12;
    oscRate[voice+4] = oscRateOriginal[voice+4] = freqTable[note+12] + 1;
    voiceActive[voice+4] = 1;
  }
  else if (deviationMode == 3)
  {
    deviation = 1;
    if (note > 48) deviation = ((note-48)/4)+1;
    oscRate[voice+2] = oscRateOriginal[voice+2] = oscRate[voice] + deviation;
    voiceActive[voice+2] = 1;
    if ((note+12) > 127) note -= 12;
    oscRate[voice+4] = oscRateOriginal[voice+4] = freqTable[note+12] + 1;
    voiceActive[voice+4] = 1;
    if (((char)note-12) < 0) note += 12;
    oscRate[voice+6] = oscRateOriginal[voice+6] = freqTable[note-12] + 1;
    voiceActive[voice+6] = 1;
  }

  if (pitchBend != 64) { for (char w=0; w<MAX_V; w++) { oscRate[w] = oscRateOriginal[w]+((pitchBend-64)/2); } }
}

// ======================================================================================= //
void setNoteOff(uint8_t note)
{
  uint8_t maxVoices = MAX_V;
  if (deviationMode == 1) maxVoices = MAX_V/2;
  if (deviationMode >= 2) maxVoices = MAX_V/4;
  
  for (char v=0; v<maxVoices; v++)
  {
    if (voiceNote[v] == note)
    {
      voiceActive[v] = 0;
      oscRate[v] = oscRateOriginal[v] = 0;
      voiceNote[v] = 0;

      if (deviationMode == 1)
      {
        voiceActive[v+4] = 0;
        oscRate[v+4] = oscRateOriginal[v+4] = 0;
      }
      else if (deviationMode == 2)
      {
        voiceActive[v+2] = 0;
        oscRate[v+2] = oscRateOriginal[v+2] = 0;
        voiceActive[v+4] = 0;
        oscRate[v+4] = oscRateOriginal[v+4] = 0;
      }
      else if (deviationMode == 3)
      {
        voiceActive[v+2] = 0;
        oscRate[v+2] = oscRateOriginal[v+2] = 0;
        voiceActive[v+6] = 0;
        oscRate[v+6] = oscRateOriginal[v+6] = 0;
        voiceActive[v+4] = 0;
        oscRate[v+4] = oscRateOriginal[v+4] = 0;
      }
      break;
    }
  }  
}
