/*

  Created by Beat707 (c) 2011 - http://www.Beat707.com
  
  Midi Output, Timer and related functions
  
*/

// ======================================================================================= //
// MIDI ClockTimer Call - here's where all the MIDI Sequencing Action Happens - this is called by the Timer2 Interrupt
ISR(TIMER1_COMPA_vect)
{
  #if SEND_MIDI_SYNC
    if (sync24PPQ == 0) MSerial.write(0xF8); // Midi Clock Sync Tick (96 PPQ)
    sync24PPQ++;
    if (sync24PPQ >= 4) sync24PPQ = 0;
  #endif
  
  if (midiClockProcess || midiClockProcessDoubleSteps)
  {    
    if (song_settings.midiClockDirection == 1) midiClockCounter2 = 15-midiClockCounter;
      else if (song_settings.midiClockDirection == 2) midiClockCounter2 = random(0, 15);
        else midiClockCounter2 = midiClockCounter;        
    
    uint8_t dBB = (((DRUMTRACKS+2)*midiClockProcessDoubleSteps)+(((DRUMTRACKS+2)*2)*stepsPos));
    uint8_t velocity = 87+(bitRead(song_settings.dmSteps[currentPattern][DRUMTRACKS+dBB],midiClockCounter2)*20)+(bitRead(song_settings.dmSteps[currentPattern][DRUMTRACKS+1+dBB],midiClockCounter2)*20);
    bufferMIDIpos = bufferMIDIsize = 0;
        
    for (char xdtm=0; xdtm<DRUMTRACKS; xdtm++)
    {
      if (bitRead(song_settings.dmSteps[currentPattern][xdtm+dBB],midiClockCounter2))
      {
        sendMidiNoteOff(song_settings.dmNotes[xdtm], song_settings.dmChannel[xdtm]);
        sendMidiNoteOn(song_settings.dmNotes[xdtm],velocity, song_settings.dmChannel[xdtm]);
      }
    }
    
    midiClockProcess = midiClockProcessDoubleSteps = 0;    
  }
  
  // Midi Clock //
  midiClockCounterDivider++;
  if (midiClockCounterDivider >= (midiClockShuffleData[0][midiClockShuffleCounter]*song_settings.timeScale))
  {
    midiClockShuffleCounter++;
    if (midiClockShuffleCounter >= 3) midiClockShuffleCounter = 0;
    
    midiClockProcess = 1;
    midiClockCounterDivider = 0;
    midiClockCounter++;
    if (midiClockCounter >= song_settings.numberOfSteps)
    {
      stepsPos++;
      midiClockCounter = 0;
      
      if (stepsPos >= 2)
      {
        stepsPos = 0;
        currentPattern = nextPattern;
        if (specialMode == 0)
        {
          TRANSPORT_COLOR[0] = TRANSPORT_COLOR[3] = TRANSPORT_COLOR[5] = TRANSPORT_COLOR[6] = TRANSPORT_COLOR[9] = TRANSPORT_COLOR[10] = TRANSPORT_COLOR[12] = TRANSPORT_COLOR[15] = TRANSPORT_COLOR_STRENG;
          transportBlinkClear = 1;
        }
        /*if (curMode == 1)
        {
          patternSongRepeatCounter++;
          if (patternSongNext == 0 && patternSongRepeatCounter > patternSongRepeat)
          {
            MidiClockStop();
            songNextPosition = 1;
          }     
        }*/
      }
      
      editStepsPos = stepsPos;
    }
  }
  else if (midiClockCounterDivider == (midiClockShuffleData[1][midiClockShuffleCounter]*song_settings.timeScale)) midiClockProcessDoubleSteps = 1; 
}

// ======================================================================================= //
void MidiShuffleUpdate()
{
  midiClockShuffleData[0][1] = 12+song_settings.midiClockShuffle;
  midiClockShuffleData[1][1] = 6+song_settings.midiClockShuffle;
  midiClockShuffleData[0][2] = 12-song_settings.midiClockShuffle;
  midiClockShuffleData[1][2] = 6-song_settings.midiClockShuffle;
}

// ======================================================================================= //
void MidiClockStart(uint8_t restart)
{
  MidiShuffleUpdate();  
  midiClockRunning = 1;
  prevMidiClockCounter = 15;
  if (specialMode == 0)
  {
    TRANSPORT_COLOR[0] = TRANSPORT_COLOR[3] = TRANSPORT_COLOR[5] = TRANSPORT_COLOR[6] = TRANSPORT_COLOR[9] = TRANSPORT_COLOR[10] = TRANSPORT_COLOR[12] = TRANSPORT_COLOR[15] = TRANSPORT_COLOR_STRENG;
    transportBlinkClear = 1;  
  }
  stepsPos = editStepsPos = editDoubleSteps = midiClockShuffleCounter = 0;
  if (restart)
  {
    midiClockProcess = 1;
    midiClockProcessDoubleSteps = sync24PPQ = 0;
    midiClockCounter = midiClockCounterDivider = 0;
  }
  #if SEND_MIDI_SYNC
    sendSpecialMidiData = 0xFA; // MIDI Start
  #endif
  timerStart();  
}

// ======================================================================================= //
void MidiClockStop()
{
  midiClockRunning = stepsPos = editStepsPos = editDoubleSteps = 0;
  #if SEND_MIDI_SYNC
    sendSpecialMidiData = 0xFC; // MIDI Stop
  #endif
  timerStop();
  nop();
  sendAllNoteOff = 1;
  memset(TRANSPORT_COLOR,0,sizeof(TRANSPORT_COLOR));
}

// ======================================================================================= //
void MidiClockNewTime()
{
  if (midiClockRunning) timerSetFrequency();
}

// ======================================================================================= //
inline void sendMidiNoteOn(char note, char velocity, char channel)
{
    bufferMIDI[bufferMIDIsize] = 0x90+channel;
    bufferMIDI[bufferMIDIsize+1] = note;
    bufferMIDI[bufferMIDIsize+2] = velocity;
    bufferMIDIsize += 3;
}

// ======================================================================================= //
inline void sendMidiNoteOff(char note, char channel)
{
    bufferMIDI[bufferMIDIsize] = 0x80+channel;
    bufferMIDI[bufferMIDIsize+1] = note;
    bufferMIDI[bufferMIDIsize+2] = 0;
    bufferMIDIsize += 3;
}

// ======================================================================================= //
void midiBufferCheck()
{
  #if SEND_MIDI_SYNC
    if (sendSpecialMidiData > 0)
    {
      MSerial.write(sendSpecialMidiData);
      sendSpecialMidiData = 0;
    }
  #endif
  
  if (sendAllNoteOff == 1)
  {
    sendAllNoteOff = bufferMIDIsize = 0;
    for (char xd=0; xd<(DRUMTRACKS); xd++)
    {
      bufferMIDI[bufferMIDIsize] = 0x80+song_settings.dmChannel[xd];
      bufferMIDI[bufferMIDIsize+1] = song_settings.dmNotes[xd];
      bufferMIDI[bufferMIDIsize+2] = 0;
      bufferMIDIsize += 3;
      //
      bufferMIDI[bufferMIDIsize] = 0xB0+song_settings.dmChannel[xd];
      bufferMIDI[bufferMIDIsize+1] = 0x7B;
      bufferMIDI[bufferMIDIsize+2] = 0;
      bufferMIDIsize += 3;
    }
  }
  
  if (bufferMIDIsize > 0)
  {
    MSerial.write(bufferMIDI[bufferMIDIpos]);
    bufferMIDIpos++;
    if (bufferMIDIpos > bufferMIDIsize) bufferMIDIsize = 0;
  }
}
