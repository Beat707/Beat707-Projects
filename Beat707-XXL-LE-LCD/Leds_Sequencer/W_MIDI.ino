/*

  Created by Guilherme (William) Kalfelz
  @ Beat707 (c) 2014 - http://www.Beat707.com
 
  seq Output, Timer and related functions
  
*/

// ======================================================================================= //
inline void clockTick()
{
  doExtLEDBlink = 0;
  //  
  if (seqClockDirection == 1) seqClockCounter2 = 15-seqClockCounter;
    else if (seqClockDirection == 2) seqClockCounter2 = random(0, 15);
      else seqClockCounter2 = seqClockCounter;        
  //
  uint8_t dBB = ((DRUMTRACKS*seqClockProcessDoubleSteps)+((DRUMTRACKS*2)*stepsPos));
  uint8_t dBBac = seqClockProcessDoubleSteps+(2*stepsPos);
  uint8_t velocity = 87+(bitRead(ACCENT1[dBBac],seqClockCounter2)*20)+(bitRead(ACCENT2[dBBac],seqClockCounter2)*20);
  //
  if (patternFade == 1)
  {
    patternFadeCounter++;
    velocity = patternFadeCounter;
    if (patternFadeCounter >= 127) patternFade = 0;
  }
  else if (patternFade == -1)
  {
    patternFadeCounter--;    
    velocity = patternFadeCounter;
    if (patternFadeCounter == 0) patternFade = 0;
  }
  //
  for (char xdtm=0; xdtm<DRUMTRACKS; xdtm++)
  {
    if (bitRead(dmSteps[currentPattern][xdtm+dBB],seqClockCounter2) && destChannel[xdtm] > 0 && bitRead(tracksOnOff, xdtm))
    {
      sendseqNoteOff(dmNotes[xdtm], destChannel[xdtm]);
      sendseqNoteOn(dmNotes[xdtm],velocity, destChannel[xdtm]);
      if (dmBlink[xdtm] == 1) doExtLEDBlink = 1;
    }
  }
  //
  if (seqClockProcessDoubleSteps == 0) seqClockProcessDoubleSteps = 1; else seqClockProcessDoubleSteps = 0;
  if (doExtLEDBlink == 1) blinkExtraLEDs();
}

// ======================================================================================= //
void seqClockStart(uint8_t restart)
{
  seqClockRunning = 1;
  prevseqClockCounter = 15;
  /*if (holdingLeft == 0 && holdingRight == 0)
  {
    transportLeds = 38505; // X Pattern //
    transportBlinkClear = 1;  
  }*/
  stepsPos = 0;
  if (autoStepPos == 1) editStepsPos = 0;
  if (restart)
  {
    seqClockProcessDoubleSteps = sync24PPQ = 0;
    seqClockCounter = seqClockCounterDivider = 0;
  }
}

// ======================================================================================= //
void seqClockStop()
{
  seqClockRunning = stepsPos = patternFade = 0;
  if (autoStepPos == 1) editStepsPos = 0;
  transportLeds = 0;
  //
  // All Notes Off //
  for (char xd=0; xd<(DRUMTRACKS); xd++)
  {
    if (destChannel[xd] > 0)
    {
      sendseqNoteOff(dmNotes[xd], destChannel[xd]);
      //
      MSerial.write(0xB0+destChannel[xd]); // All Notes Off //
      MSerial.write(0x7B);
      MSerial.write(0);
    }
  }
}

// ======================================================================================= //
void sendseqNoteOn(char note, char velocity, char channel)
{
  MSerial.write(0x90+channel);
  MSerial.write(note);
  MSerial.write(velocity);
}

// ======================================================================================= //
void sendseqNoteOff(char note, char channel)
{
  MSerial.write(0x80+channel);
  MSerial.write(note);
  MSerial.write(0);  // Velocity = 0
}
