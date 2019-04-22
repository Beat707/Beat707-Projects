  /*

  Created by Beat707 (c) 2011 - http://www.Beat707.com
  
  Midi Output, Timer and related functions
  
*/

// ======================================================================================= //
// MIDI ClockTimer Call - here's where all the MIDI Sequencing Action Happens - this is called by the Timer2 Interrupt
ISR(TIMER1_COMPA_vect) { midiTimer(); }

// ======================================================================================= //
void midiTimer()
{
  if (midiClockType == 2)
  {
    if (sync24PPQ == 0) MSerial.write(0xF8); // Midi Clock Sync Tick (96 PPQ)
    sync24PPQ++;
    if (sync24PPQ >= 4) sync24PPQ = 0;
  }
  
  if (midiClockProcess || midiClockProcessDoubleSteps)
  {
    #if BEAT707_BIG_TIME
      midiClockBeats++;
      if (midiClockBeats >= 8)
      {
        midiClockBeats = 0;
        sendBeatToBigTime = 1;
      }
    #endif    
    
    if (midiClockDirection == 1) midiClockCounter2 = 15-midiClockCounter;
      else if (midiClockDirection == 2) midiClockCounter2 = random(0, 15);
        else midiClockCounter2 = midiClockCounter;        
    
    uint8_t dBB = (((DRUMTRACKS+2)*midiClockProcessDoubleSteps)+(((DRUMTRACKS+2)*2)*stepsPos));
    uint8_t dBBs = ((16*midiClockProcessDoubleSteps)+(32*stepsPos));    
    uint8_t velocity = 87+(bitRead(dmSteps[patternBufferN][DRUMTRACKS+dBB],midiClockCounter2)*20)+(bitRead(dmSteps[patternBufferN][DRUMTRACKS+1+dBB],midiClockCounter2)*20);
    
    char midiBuffer = 0xFF;
    #if !MIDI_NO_OUT_BUFFER
      if (bufferMIDIpos[0] == 0) midiBuffer = 0;
        else if (bufferMIDIpos[1] == 0) midiBuffer = 1;
    #endif
    
    for (char xdtm=0; xdtm<DRUMTRACKS; xdtm++)
    {
      if (bitRead(dmSteps[patternBufferN][xdtm+dBB],midiClockCounter2) && !bitRead(dmMutes,xdtm))
      {        
        #if GATE_OUTS
          Gate_Outs_Midi(xdtm, velocity);
        #else
        sendMidiNoteOff(dmNotes[xdtm], dmChannel[xdtm], midiBuffer);
        sendMidiNoteOn(dmNotes[xdtm],velocity, dmChannel[xdtm], midiBuffer);
        #endif
      }
    }
    
    for (char xdtm=0; xdtm<2; xdtm++)
    {
      if (!bitRead(dmMutes,DRUMTRACKS+xdtm))
      {        
        if (dmSynthTrack[xdtm][patternBufferN][midiClockCounter2+dBBs] == 1)
        {
          if (dmSynthTrackPrevNote[0][xdtm] > 0) sendMidiNoteOff(dmSynthTrackPrevNote[0][xdtm], dmChannel[DRUMTRACKS+xdtm], midiBuffer);
          if (dmSynthTrackPrevNote[1][xdtm] > 0) sendMidiNoteOff(dmSynthTrackPrevNote[1][xdtm], dmChannel[DRUMTRACKS+xdtm], midiBuffer);
          dmSynthTrackPrevNote[0][xdtm] = dmSynthTrackPrevNote[1][xdtm] = 0;
        }
        else if (dmSynthTrack[xdtm][patternBufferN][midiClockCounter2+dBBs] > 1)
        {
          if (dmSynthTrack[xdtm][patternBufferN][midiClockCounter2+dBBs] < 128)
          {
            if (dmSynthTrackPrevNote[0][xdtm] > 0) sendMidiNoteOff(dmSynthTrackPrevNote[0][xdtm], dmChannel[DRUMTRACKS+xdtm], midiBuffer);
            if (dmSynthTrackPrevNote[1][xdtm] > 0) sendMidiNoteOff(dmSynthTrackPrevNote[1][xdtm], dmChannel[DRUMTRACKS+xdtm], midiBuffer);
            sendMidiNoteOn(dmSynthTrack[xdtm][patternBufferN][midiClockCounter2+dBBs]-1,velocity, dmChannel[DRUMTRACKS+xdtm], midiBuffer);
            dmSynthTrackPrevNote[0][xdtm] = dmSynthTrack[xdtm][patternBufferN][midiClockCounter2+dBBs]-1;
            dmSynthTrackPrevNote[1][xdtm] = 0;
          }
          else
          {
            if (dmSynthTrackPrevNote[1][xdtm] > 0) sendMidiNoteOff(dmSynthTrackPrevNote[1][xdtm], dmChannel[DRUMTRACKS+xdtm], midiBuffer);
            sendMidiNoteOn(dmSynthTrack[xdtm][patternBufferN][midiClockCounter2+dBBs]-1-127,velocity, dmChannel[DRUMTRACKS+xdtm], midiBuffer);
            dmSynthTrackPrevNote[1][xdtm] = dmSynthTrack[xdtm][patternBufferN][midiClockCounter2+dBBs]-1-127;
          }
        }
      }
    }
    
    midiClockProcess = midiClockProcessDoubleSteps = 0;    
  }
  
  // Midi Clock //
  if (midiClockType == 1) midiClockCounterDivider += 2; else midiClockCounterDivider++;
  if (midiClockCounterDivider >= (midiClockShuffleData[0][midiClockShuffleCounter]*timeScale))
  {
    midiClockShuffleCounter++;
    if (midiClockShuffleCounter >= 3) midiClockShuffleCounter = 0;
    
    midiClockProcess = 1;
    midiClockCounterDivider = 0;
    midiClockCounter++;
    if (midiClockCounter >= numberOfSteps)
    {
      stepsPos++;
      midiClockCounter = 0;
      
      if (stepsPos >= 2 || !enableABpattern)
      {
        stepsPos = 0;
        #if EXTRA_MIDI_IN_H_2
          #if XTH2_AUTO_STOP
            if (doAutoStop == 1)
            {
          #endif
              midiClockRunning = midiClockProcess = 0;
              return;
          #if XTH2_AUTO_STOP
            }
          #endif
        #endif        
        if (curMode == 1)
        {
          patternSongRepeatCounter++;
          if (patternSongNext == 0 && patternSongRepeatCounter > patternSongRepeat)
          {
            MidiClockStop();
            songNextPosition = 1;
          }     
        } 
        
        checkPatternLoader();
      }
      
      if (autoSteps || curMode == 1) { editStepsPos = stepsPos; if (!holdingShift) doPatternLCDupdate = 1; }
    }
  }
  else if (midiClockCounterDivider == (midiClockShuffleData[1][midiClockShuffleCounter]*timeScale)) midiClockProcessDoubleSteps = 1; 
}

// ======================================================================================= //
void checkPatternLoader()
{
  if (nextPatternReady)
  {
    if (curMode == 1)
    {
      if (patternSongRepeatCounter > patternSongRepeat)
      {
        patternBufferN = !patternBufferN;
        patternSongRepeatCounter = nextPatternReady = 0;
        songNextPosition = doPatternLCDupdate = 1;
      }
    }
    else
    {
      patternBufferN = !patternBufferN;
      nextPatternReady = 0;
      doLCDupdate = 1;
    }
  }
}

// ======================================================================================= //
void MidiShuffleUpdate()
{
  midiClockShuffleData[0][1] = 12+midiClockShuffle;
  midiClockShuffleData[1][1] = 6+midiClockShuffle;
  midiClockShuffleData[0][2] = 12-midiClockShuffle;
  midiClockShuffleData[1][2] = 6-midiClockShuffle;
}

// ======================================================================================= //
void MidiClockStart(uint8_t restart, uint8_t callMidiTimer)
{
  #if !EXTRA_MIDI_IN_H_2
    MidiShuffleUpdate();  
  #endif
  midiClockRunning = 1;
  stepsPos = midiClockShuffleCounter = 0;
  if (autoSteps) { editStepsPos = 0; doPatternLCDupdate = 1; }
  if (restart)
  {
    midiClockProcess = 1;
    midiClockProcessDoubleSteps = sync24PPQ = 0;
    midiClockCounter = midiClockCounterDivider = 0;
  }
  if (midiClockType == 2) MSerial.write(0xFA); // MIDI Start
  if (midiClockType != 1) timerStart();
  if (callMidiTimer == 1) midiTimer();
}

// ======================================================================================= //
void MidiClockStop()
{
  midiClockRunning = stepsPos = 0;
  if (midiClockType == 2) MSerial.write(0xFC); // MIDI Stop
  if (midiClockType != 1) timerStop();
  sendMidiAllNotesOff();
  #if BEAT707_BIG_TIME
    midiClockBeats = 0;
    wireSendByte(BIGTIME_ADDR, 1); // Reset
  #endif
  #if EXTRA_MIDI_IN_H_2
    MidiShuffleUpdate();  
  #endif  
}

// ======================================================================================= //
void MidiClockNewTime()
{
  if (midiClockType != 1) timerSetFrequency();
}

// ======================================================================================= //
void sendMidiNoteOn(char note, char velocity, char channel, uint8_t midiBuffer)
{
  #if !DISABLE_MIDI
    if (midiBuffer != 0xFF)
    {     
      bufferMIDI[midiBuffer][bufferMIDIpos[midiBuffer]] = 0x90+channel;
      bufferMIDI[midiBuffer][bufferMIDIpos[midiBuffer]+1] = note;
      bufferMIDI[midiBuffer][bufferMIDIpos[midiBuffer]+2] = velocity;
      bufferMIDIpos[midiBuffer] += 3;
    }
    else
    {
      MSerial.write(0x90+channel);
      MSerial.write(note);
      MSerial.write(velocity);
    }
  #endif
}

// ======================================================================================= //
void sendMidiData(uint8_t* data, uint8_t channel, uint8_t nBytes)
{
  #if !DISABLE_MIDI
    MSerial.write(data[0]+channel);
    if (nBytes >= 2) MSerial.write(data[1]);
    if (nBytes >= 3) MSerial.write(data[2]);
  #endif
}

// ======================================================================================= //
void sendMidiNoteOff(char note, char channel, uint8_t midiBuffer)
{
  #if !DISABLE_MIDI
    if (midiBuffer != 0xFF)
    {      
      bufferMIDI[midiBuffer][bufferMIDIpos[midiBuffer]] = 0x80+channel;
      bufferMIDI[midiBuffer][bufferMIDIpos[midiBuffer]+1] = note;
      bufferMIDI[midiBuffer][bufferMIDIpos[midiBuffer]+2] = 0;
      bufferMIDIpos[midiBuffer] += 3;
    }
    else
    {  
      MSerial.write(0x80+channel);
      MSerial.write(note);
      MSerial.write((byte)0x00);
    }
  #endif
}

// ======================================================================================= //
void sendMidiAllNotesOff()
{
  #if !DISABLE_MIDI
    for (char xd=0; xd<(DRUMTRACKS); xd++)
    {
      sendMidiNoteOff(dmNotes[xd], dmChannel[xd], 0xFF);
      
      MSerial.write(0xB0+dmChannel[xd]);
      MSerial.write(0x7B);
      MSerial.write((byte)0x00);
    }

    if (dmSynthTrackPrevNote[0][0] > 0) sendMidiNoteOff(dmSynthTrackPrevNote[0][0], dmChannel[DRUMTRACKS], 0xFF);
    if (dmSynthTrackPrevNote[1][0] > 0) sendMidiNoteOff(dmSynthTrackPrevNote[1][0], dmChannel[DRUMTRACKS], 0xFF);
    if (dmSynthTrackPrevNote[0][1] > 0) sendMidiNoteOff(dmSynthTrackPrevNote[0][1], dmChannel[DRUMTRACKS+1], 0xFF);
    if (dmSynthTrackPrevNote[1][1] > 0) sendMidiNoteOff(dmSynthTrackPrevNote[1][1], dmChannel[DRUMTRACKS+1], 0xFF);
    
    MSerial.write(0xB0+dmChannel[DRUMTRACKS]); MSerial.write(0x7B); MSerial.write((byte)0x00);
    MSerial.write(0xB0+dmChannel[DRUMTRACKS+1]); MSerial.write(0x7B); MSerial.write((byte)0x00);
    
    memset(dmSynthTrackPrevNote,0,sizeof(dmSynthTrackPrevNote));
  #endif
}

// ======================================================================================= //
uint8_t midiInput(void)
{
  while (MSerial.available() == 0) nop();
  incomingByte = MSerial.read();

  if (!rawMIDImode)
  {
    while (incomingByte >= 0xF8)
    {
      while (MSerial.available() == 0) nop();
      incomingByte = MSerial.read();
    }
  }

  return incomingByte;
}

// ======================================================================================= //
void midiBufferCheck()
{ 
  char midiBuffer = -1;
  if (bufferMIDIpos[0] > 0) midiBuffer = 0; else if (bufferMIDIpos[1] > 0) midiBuffer = 1;
  
  if (midiBuffer != -1)
  {    
    for (uint8_t bufferOut=0; bufferOut<bufferMIDIpos[midiBuffer]; bufferOut++)
    {
      MSerial.write(bufferMIDI[midiBuffer][bufferOut]);
    }
    bufferMIDIpos[midiBuffer] = 0;
  }
}

// ======================================================================================= //
void midiInputCheck()
{
  while (MSerial.available() > 0) 
  {
    incomingByte = MSerial.read();
    #if MIDIECHO && !MIDIECHO_BYTRACK
      MSerial.write(incomingByte);
    #endif
    
    switch (incomingByte)
    {
      case 0xF8:
         if (midiClockType == 1 && midiClockRunning == 1) 
         { 
           midiTimer(); 
           midiTimer(); 
         }
         break;
         
      #if !EXTRA_MIDI_IN_H_2
        case 0xFA: 
          if (midiClockType == 1) MidiClockStart(true, false);
          break;
          
        case 0xFB:
          if (midiClockType == 1) MidiClockStart(false, false); // Continue
          break;
      #endif
      
    case 0xFC:
      if (midiClockType == 1 || midiUSBmode == 1) MidiClockStop();
      break;
      
    case 240: 
      songDumpReceive();
      break;
    
    default:
      switch (state)
      {
        case 0:
            if (incomingByte >= 128)
            {
              if      (incomingByte <= 143) { midiInputB[0] = 128; state = 1; channel = incomingByte-128; }  // Note Off
              else if (incomingByte <= 159) { midiInputB[0] = 144; state = 1; channel = incomingByte-144; }  // Note On
              else if (incomingByte <= 175) { midiInputB[0] = 160; state = 1; channel = incomingByte-160; }  // Polyphonic aftertouch
              else if (incomingByte <= 191) { midiInputB[0] = 176; state = 1; channel = incomingByte-176; }  // Control (CC)
              else if (incomingByte <= 207) { midiInputB[0] = 192; state = 1; channel = incomingByte-192; }  // Program Change (2 bytes)
              else if (incomingByte <= 223) { midiInputB[0] = 208; state = 1; channel = incomingByte-208; }  // Channel aftertouch (2 bytes)
              else if (incomingByte <= 239) { midiInputB[0] = 224; state = 1; channel = incomingByte-224; }  // Pitch Wheel
            }
          break;
          
         case 1:
           if(incomingByte < 128) 
           { 
             midiInputB[1] = incomingByte;
             if (midiInputB[0] == 192) // Program Change
             {
               #if EXTRA_MIDI_IN_HACKS
                 midiInputHacks(midiInputB,channel);
               #endif               
               #if MIDIECHO && MIDIECHO_BYTRACK
                  if (currentTrack < (DRUMTRACKS+2)) sendMidiData(midiInputB, dmChannel[currentTrack], 2);
               #endif
               midiInputB[0] = midiInputB[1] = midiInputB[2] = state = 0;
             }
             else if (midiInputB[0] == 208) // Channel aftertouch
             {
               #if EXTRA_MIDI_IN_HACKS
                 midiInputHacks(midiInputB,channel);
               #endif               
               #if MIDIECHO && MIDIECHO_BYTRACK
                  if (currentTrack < (DRUMTRACKS+2)) sendMidiData(midiInputB, dmChannel[currentTrack], 2);
               #endif
               midiInputB[0] = midiInputB[1] = midiInputB[2] = state = 0;
             }
             else
             {
               state = 2; 
             }
           }
           break;
         
         case 2:
           if(incomingByte < 128)
           {
             midiInputB[2] = incomingByte;
             #if EXTRA_MIDI_IN_HACKS
               midiInputHacks(midiInputB,channel);
             #endif             
             #if MIDIECHO && MIDIECHO_BYTRACK
               if (currentTrack < (DRUMTRACKS+2))
               {
                 if (midiInputB[0] == 144 && incomingByte > 0) sendMidiNoteOn(midiInputB[1], incomingByte, dmChannel[currentTrack], 0xFF);
                   else if ((midiInputB[0] == 144 && incomingByte == 0) || midiInputB[0] == 128) sendMidiNoteOff(midiInputB[1], dmChannel[currentTrack], 0xFF);
                   else if (midiInputB[0] == 160 || midiInputB[0] == 176 || midiInputB[0] == 224) sendMidiData(midiInputB, dmChannel[currentTrack], 3);
               }
             #endif
             if (midiInputB[0] == 144 && incomingByte > 0 && curMode == 0)
             {
               if (currentTrack < DRUMTRACKS)
               {
                 // REGULAR DRUM TRACKS ---------------------------------------------------------------- //
                 #if MIDI_INPUT_REC
                   if (recordEnabled)
                   {
                     if (midiClockRunning)
                     {
                        dbStepsCalc();
                        for (char i=0; i<DRUMTRACKS; i++)
                        { 
                          if (midiInputB[1] == dmNotes[i])
                          {
                              if (mirrorPatternEdit)
                              {
                                bitWrite(dmSteps[patternBufferN][i+((DRUMTRACKS+2)*editDoubleSteps)],midiClockCounter,1);
                                bitWrite(dmSteps[patternBufferN][i+(((DRUMTRACKS+2)*editDoubleSteps)+((DRUMTRACKS+2))*2)],midiClockCounter,1);
                              }
                              else bitWrite(dmSteps[patternBufferN][i+dbSteps],midiClockCounter,1);
                              patternChanged = 1;
                          }
                        }                       
                     }
                   }
                  #endif
               }
               else if (currentTrack < (DRUMTRACKS+2))
               {
                 // S1/S2 NOTE-STEP TRACKS -------------------------------------------------------------- //
                 if (recordEnabled)
                 {
                   #if MIDI_INPUT_REC
                     if (midiClockRunning)
                     {
                       dbStepsCalc();
                       uint8_t dTrack = currentTrack-DRUMTRACKS;
                       
                       if (mirrorPatternEdit) 
                         dmSynthTrack[dTrack][patternBufferN][midiClockCounter+32] = dmSynthTrack[dTrack][patternBufferN][midiClockCounter] = dmSynthTrackLastNoteEdit[dTrack] = midiInputB[1]+1;
                           else dmSynthTrack[dTrack][patternBufferN][midiClockCounter] = dmSynthTrackLastNoteEdit[dTrack] = midiInputB[1]+1;
                         
                       patternChanged = doLCDupdate = 1;
                     }
                   #endif
                 }
                 #if EXTRA_MIDI_IN_H_2
                   else if (curZone == 3 && channel != 15)
                 #else
                   else if (curZone == 3)
                 #endif
                 {
                   #if MIDI_INPUT_ST
                      dbStepsCalc();
                      uint8_t dTrack = currentTrack-DRUMTRACKS;
                      newNote = midiInputB[1]+1;
                      #if MIDI_INPUT_AUTO_V
                        if (incomingByte <= 40) newNote = 0;
                      #endif
                      #if MIDI_INPUT_AU_LW
                        if (midiInputB[1] <= MIDI_INPUT_AU_LW) newNote = 0;
                      #endif
                      
                      if (mirrorPatternEdit) dmSynthTrack[dTrack][patternBufferN][dmSynthTrackStepPos[1]+(16*editDoubleSteps)+32] = dmSynthTrack[dTrack][patternBufferN][dmSynthTrackStepPos[1]+(16*editDoubleSteps)] = dmSynthTrackLastNoteEdit[dTrack] = newNote;
                        else dmSynthTrack[dTrack][patternBufferN][dbStepsSpos] = dmSynthTrackLastNoteEdit[dTrack] = newNote;
                        
                      #if MIDI_INPUT_AUTO
                        dmSynthTrackStepPos[1] += MIDI_INPUT_AUTO_N;
                        if (dmSynthTrackStepPos[1] >= numberOfSteps)
                        {
                          if (!mirrorPatternEdit) dmSynthTrackStepPos[0] = !dmSynthTrackStepPos[0];
                          dmSynthTrackStepPos[1] -= numberOfSteps;
                        }
                      #endif
                      patternChanged = doLCDupdate = 1;
                   #endif
                 }
               }
             }
             midiInputB[0] = midiInputB[1] = midiInputB[2] = state = 0;
           }
       }
    }
  }
}
