/*

  Created by Guilherme (William) Kalfelz
  @ Beat707 (c) 2014 - http://www.Beat707.com
 
  seq Output, Timer and related functions
  
*/

// ======================================================================================= //
ISR(TIMER1_COMPA_vect)
{
  tClock++;
}

// ======================================================================================= //
void clockTick()
{
  if (tClock == 0) return;
  tClock--;
  doExtLEDBlink = 0;
  
  if (seqClockProcess || seqClockProcessDoubleSteps)
  {    
    if (last_settings.seqClockDirection == 1) seqClockCounter2 = 15-seqClockCounter;
      else if (last_settings.seqClockDirection == 2) seqClockCounter2 = random(0, 15);
        else seqClockCounter2 = seqClockCounter;        
    
    uint8_t dBB = (((DRUMTRACKS+2)*seqClockProcessDoubleSteps)+(((DRUMTRACKS+2)*2)*stepsPos));
    uint8_t velocity = 87+(bitRead(last_settings.dmSteps[currentPattern][DRUMTRACKS+dBB],seqClockCounter2)*20)+(bitRead(last_settings.dmSteps[currentPattern][DRUMTRACKS+1+dBB],seqClockCounter2)*20);
        
    for (char xdtm=0; xdtm<DRUMTRACKS; xdtm++)
    {
      if (bitRead(last_settings.dmSteps[currentPattern][xdtm+dBB],seqClockCounter2) && destChannel[xdtm] > 0)
      {
        sendseqNoteOff(dmNotes[xdtm], destChannel[xdtm]);
        sendseqNoteOn(dmNotes[xdtm],velocity, destChannel[xdtm]);
        if (dmBlink[xdtm] == 1) doExtLEDBlink = 1;
      }
    }
    
    seqClockProcess = seqClockProcessDoubleSteps = 0;    
  }
  
  // seq Clock //
  seqClockCounterDivider++;
  if (seqClockCounterDivider >= (seqClockShuffleData[0][seqClockShuffleCounter]*2))
  {
    seqClockShuffleCounter++;
    if (seqClockShuffleCounter >= 3) seqClockShuffleCounter = 0;
    
    seqClockProcess = 1;
    seqClockCounterDivider = 0;
    seqClockCounter++;
    //
    if (seqClockCounter >= last_settings.numberOfSteps)
    {
      stepsPos++;
      seqClockCounter = 0;
      
      if (stepsPos >= STEPS_ABCD)
      {
        #if !IS_MASTER_UNIT
          timerStop(); // This is a Slave Unit, so we stop the timer until a new Sequence Trigger message is received //
        #endif
        //
        #if HAS_SLAVE_UNITS
          MSerial.write(0);  // Sequence Trigger all Slaves //
        #endif
        //        
        // Next Pattern Check //
        if (nextPattern != currentPattern) currentPattern = nextPattern;
        //
        stepsPos = 0;
        if (holdingLeft == 0 && holdingRight == 0)
        {
          transportLeds = 38505; // X Pattern //
          transportBlinkClear = 1;
        }
      }
      
      if (autoStepPos == 1) editStepsPos = stepsPos;
    }
  }
  else if (seqClockCounterDivider == (seqClockShuffleData[1][seqClockShuffleCounter]*2)) seqClockProcessDoubleSteps = 1; 
  
  if (doExtLEDBlink == 1) blinkExtraLEDs();
}

// ======================================================================================= //
void seqShuffleUpdate()
{
  seqClockShuffleData[0][1] = 12+last_settings.seqClockShuffle;
  seqClockShuffleData[1][1] = 6+last_settings.seqClockShuffle;
  seqClockShuffleData[0][2] = 12-last_settings.seqClockShuffle;
  seqClockShuffleData[1][2] = 6-last_settings.seqClockShuffle;
}

// ======================================================================================= //
void seqClockStart(uint8_t restart)
{
  seqShuffleUpdate();  
  seqClockRunning = 1;
  prevseqClockCounter = 15;
  if (holdingLeft == 0 && holdingRight == 0)
  {
    transportLeds = 38505; // X Pattern //
    transportBlinkClear = 1;  
  }
  stepsPos = seqClockShuffleCounter = 0;
  if (autoStepPos == 1) editStepsPos = 0;
  if (restart)
  {
    seqClockProcess = 1;
    seqClockProcessDoubleSteps = sync24PPQ = 0;
    seqClockCounter = seqClockCounterDivider = 0;
  }
  timerStart();
}

// ======================================================================================= //
void seqClockStop()
{
  seqClockRunning = stepsPos = 0;
  if (autoStepPos == 1) editStepsPos = 0;
  timerStop();
  nop();
  transportLeds = 0;
  //
  // All Notes Off //
  for (char xd=0; xd<(DRUMTRACKS); xd++)
  {
    if (destChannel[xd] > 0)
    {
      sendseqNoteOff(dmNotes[xd], destChannel[xd]);
      //
      #if IS_LAST_UNIT
        MSerial.write(0xB0+destChannel[xd]); // All Notes Off //
        MSerial.write(0x7B);
        MSerial.write(0);
      #else
        MSerial.write(B01100100);  // Sequencer to SND Generator //
        MSerial.write(B0100000 | (destChannel[xd] & B00011111));  // Reset //
        MSerial.write(0);
        MSerial.write(0);      
      #endif
    }
  }
  //
  if (somethingChanged)
  {
    somethingChanged = 0;
    extLEDblink = EXTRA_LED_BLINK_RATE+1;
    clearDisplay();
    for (uint8_t x=0; x<16; x++) { drawLEDs(); }
    eeprom_write_block((const void*)&last_settings, (void*)0, sizeof(last_settings));
  }  
}

// ======================================================================================= //
void seqClockNewTime()
{
  if (seqClockRunning) timerSetFrequency();
}

// ======================================================================================= //
void sendseqNoteOn(char note, char velocity, char channel)
{
  #if IS_LAST_UNIT
    MSerial.write(0x90+channel);
  #else
    MSerial.write(B01100100); // Sequencer to SND Generator
    MSerial.write(channel & B00011111);  // Note On
  #endif
  //
  MSerial.write(note);
  MSerial.write(velocity);
}

// ======================================================================================= //
void sendseqNoteOff(char note, char channel)
{
  #if IS_LAST_UNIT
    MSerial.write(0x80+channel);
  #else
    MSerial.write(B01100100); // Sequencer to SND Generator
    MSerial.write(B0010000 | (destUnitID & channel));  // Note Off
  #endif
  //
  MSerial.write(note);
  MSerial.write(0);  // Velocity = 0
}
