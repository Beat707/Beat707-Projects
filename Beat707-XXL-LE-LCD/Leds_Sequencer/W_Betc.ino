/*

  Created by Guilherme (William) Kalfelz
  @ Beat707 (c) 2014 - http://www.Beat707.com
 
  Extra Functions and Classes used by this project
  
*/

// ======================================================================================= //
void volatile nop(void) { asm __volatile__ ("nop"); }

// ======================================================================================= //
#define dbSteps ((DRUMTRACKS*2)*editStepsPos)

// ======================================================================================= //
void clearDisplay()
{
  stepLeds = transportLeds = 0;
}

// ======================================================================================= //
void waitSerialNextByte()
{
  while (Serial.available() == 0) { ; }
}

// ======================================================================================= //
void updateSteps(int mode)
{
  if (mode > 0 && (numberOfSteps+mode) <= 16) numberOfSteps += mode;
  else if (mode == 0) numberOfSteps = 16;
  else if (mode < 0 && (numberOfSteps+mode) >= 0) numberOfSteps += mode;
}

// ======================================================================================= //
void initPattern(uint8_t pattern)
{
  memset(dmSteps[pattern],0,sizeof(dmSteps[pattern]));
}

// ======================================================================================= //
void initCurrentTrack()
{   
    dmSteps[currentPattern][currentTrack] = 
    dmSteps[currentPattern][currentTrack+(DRUMTRACKS*2)] = 
    dmSteps[currentPattern][currentTrack+(DRUMTRACKS*4)] = 
    dmSteps[currentPattern][currentTrack+(DRUMTRACKS*6)] = 
    dmSteps[currentPattern][currentTrack+DRUMTRACKS] = 
    dmSteps[currentPattern][currentTrack+DRUMTRACKS+(DRUMTRACKS*2)] = 
    dmSteps[currentPattern][currentTrack+DRUMTRACKS+(DRUMTRACKS*4)] = 
    dmSteps[currentPattern][currentTrack+DRUMTRACKS+(DRUMTRACKS*6)] = 0;
}

// ======================================================================================= //
void copyPatternToBuffer()
{
    for (char xdtm=0; xdtm<(DRUMTRACKS*8); xdtm++)
    {
      dmStepsBuffer[xdtm] = dmSteps[currentPattern][xdtm];
    }  
}

// ======================================================================================= //
void copyBufferToPattern()
{
    for (char xdtm=0; xdtm<(DRUMTRACKS*8); xdtm++)
    {
      dmSteps[currentPattern][xdtm] = dmStepsBuffer[xdtm];
    }  
}
