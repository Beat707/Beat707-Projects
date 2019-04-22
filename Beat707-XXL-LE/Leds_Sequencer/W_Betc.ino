/*

  Created by Guilherme (William) Kalfelz
  @ Beat707 (c) 2014 - http://www.Beat707.com
 
  Extra Functions and Classes used by this project
  
*/


// ======================================================================================= //
void volatile nop(void) { asm __volatile__ ("nop"); }

// ======================================================================================= //
#define dbSteps (((DRUMTRACKS+2)*2)*editStepsPos)

// ======================================================================================= //
void clearDisplay()
{
  stepLeds = transportLeds = 0;
}

// ======================================================================================= //
void timerStart()
{
  TCCR1A = TCCR1B = 0;
  bitWrite(TCCR1B, CS11, 1);
  bitWrite(TCCR1B, WGM12, 1);
  timerSetFrequency();
  bitWrite(TIMSK1, OCIE1A, 1);
}

void timerSetFrequency()
{
  // Calculates the Frequency for the Timer, used by the PPQ clock (Pulses Per Quarter Note) //
  // This uses the 16-bit Timer1, unused by the Arduino, unless you use the analogWrite or Tone functions //
  #define frequency (((last_settings.seqClockBPM)*(PPQ))/60)
  OCR1A = (F_CPU/ 8) / frequency - 1;
}

void timerStop(void)
{
  bitWrite(TIMSK1, OCIE1A, 0);
  TCCR1A = TCCR1B = OCR1A = 0;
}

// ======================================================================================= //

void waitSerialNextByte()
{
  while (Serial.available() == 0) 
  { 
    clockTick();
  }
}

// ======================================================================================= //
void updateBPM(int mode)
{
  if (mode > 0 && (last_settings.seqClockBPM+mode) <= 255) last_settings.seqClockBPM += mode;
  else if (mode == 0) last_settings.seqClockBPM = 120;
  else if (mode < 0 && (last_settings.seqClockBPM+mode) >= 25) last_settings.seqClockBPM += mode;
  somethingChanged = 1;
  seqClockNewTime();
  //
  #if HAS_SLAVE_UNITS
    // Broadcast Change Message Here !!! //
  #endif
}

// ======================================================================================= //
void updateSteps(int mode)
{
  if (mode > 0 && (last_settings.numberOfSteps+mode) <= 16) last_settings.numberOfSteps += mode;
  else if (mode == 0) last_settings.numberOfSteps = 16;
  else if (mode < 0 && (last_settings.numberOfSteps+mode) >= 0) last_settings.numberOfSteps += mode;
  somethingChanged = 1;
  //
  #if HAS_SLAVE_UNITS
    // Broadcast Change Message Here !!! //
  #endif
}

// ======================================================================================= //
void checkUpDown()
{
  if (holdUpDown != 0 && holdingRight == 1 && interfaceZone != kInterface_Zone_None)
  {
    holdUpDownCounter++;
    if (holdUpDownCounter > UP_DOWN_REPEATE_RATE)
    {
      holdUpDownCounter = 0;
      //
      if (interfaceZone == kInterface_Zone_BPM) updateBPM(holdUpDown);
      else if (interfaceZone == kInterface_Zone_Steps) updateSteps(holdUpDown);
    }
  }
}

// ======================================================================================= //
void initPattern(uint8_t pattern)
{
  memset(last_settings.dmSteps[pattern],0,sizeof(last_settings.dmSteps[pattern]));
  //
  last_settings.dmSteps[pattern][DRUMTRACKS] = 0x1111;
  last_settings.dmSteps[pattern][DRUMTRACKS+((DRUMTRACKS+2)*2)] = 0x1111;
  last_settings.dmSteps[pattern][DRUMTRACKS+((DRUMTRACKS+2)*4)] = 0x1111;
  last_settings.dmSteps[pattern][DRUMTRACKS+((DRUMTRACKS+2)*6)] = 0x1111;
  last_settings.dmSteps[pattern][DRUMTRACKS+1] = 0x5555;
  last_settings.dmSteps[pattern][DRUMTRACKS+1+((DRUMTRACKS+2)*2)] = 0x5555;
  last_settings.dmSteps[pattern][DRUMTRACKS+1+((DRUMTRACKS+2)*4)] = 0x5555;
  last_settings.dmSteps[pattern][DRUMTRACKS+1+((DRUMTRACKS+2)*6)] = 0x5555;  
}

// ======================================================================================= //
void copyToNextPattern()
{
  if ((currentPattern+1) < PATTERNS)
  {
    for (char xdtm=0; xdtm<((DRUMTRACKS+2)*8); xdtm++)
    {
      last_settings.dmSteps[currentPattern+1][xdtm] = last_settings.dmSteps[currentPattern][xdtm];
    }
    nextPattern = currentPattern+1;
    currentPattern = nextPattern;
  }
}
