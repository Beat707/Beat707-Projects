/*

  Created by Guilherme (William) Kalfelz
  @ Beat707 (c) 2014 - http://www.Beat707.com

*/

// ======================================================================================= //
ISR(TIMER1_COMPA_vect)
{
  // seq Clock //
  seqClockCounterDivider++;
  if (seqClockCounterDivider >= (seqClockShuffleData[0][seqClockShuffleCounter]*2))
  { 
    Serial.write(0); // Send MIDI Clock to all Slave Units //
    noteTrackProcess(1);
    //
    seqClockShuffleCounter++;
    if (seqClockShuffleCounter >= 3) seqClockShuffleCounter = 0;
    
    seqClockCounterDivider = 0;
    seqClockCounter++;
    //
    seq_beat_counter++;
    if (seq_beat_counter > 3)
    {
      seq_beat_counter = 0;
      seq_beat++;
      if (seq_beat > 3) seq_beat = 0;
      doLCDupdateBeats = 1;
    }
    //
    if (seqClockCounter >= last_settings.numberOfSteps)
    {
      stepsPos++;
      seqClockCounter = 0;
      doLCDupdate++;
      //
      uint8_t stP = (TSNG(1) >> 5);
      if (stepsPos >= 4 || (last_settings.bMode >= kModeSong && stepsPos >= (stP+1) && stP <= 3))
      {
        // Song Mode //
        stepsPos = 0;
        if (last_settings.bMode >= kModeSong)
        {
          songRepeatCounter++;
          if (songRepeatCounter > (TSNG(1) & B00011111))
          {
            songRepeatCounter = 0;
            if (songEditPos == 99 && !last_settings.loopSong && !last_settings.bMode == kModeSongPerf)
            {
              lateClockStop();
            }
            else
            {
              songEditPos++;
              if (last_settings.bMode == kModeSongPerf)
              {
                if (songEditPos > TSPF(1) || TSNG(0) == 0)
                {
                  if (TSPF(2) > 1) songPerfPosCounter++;
                  if (TSPF(2) == 0 || (TSPF(2) > 1 && songPerfPosCounter > (TSPF(2)-1)))
                  {
                    songPerfPosCounter = 0;
                    if (songPerfPosNext[0] != -1)
                    {
                      songPerfPos = songPerfPosNext[0];
                      songEditPos = TSPF(0);
                      songPerfPosNextUnstack = 1;
                    }
                    else
                    {
                      lateClockStop();
                    }
                  }
                  else if (TSPF(2) == 1)
                  {
                    if (songPerfPosNext[0] != -1)
                    {
                      songPerfPos = songPerfPosNext[0];
                      songPerfPosNextUnstack = 1;
                    }                    
                    songEditPos = TSPF(0);
                    songPerfPosCounter = 0;
                  }
                  else if (TSPF(2) > 1) songEditPos = TSPF(0);
                }
              }
              else
              {
                if (songEditPos > 99 && last_settings.loopSong) songEditPos = 0;
                if (TSNG(0) == 0 && last_settings.loopSong) songEditPos = 0;
              }
              //
              if (TSNG(0) == 0) lateClockStop();
              else nextPattern = TSNG(0)-1;
              tracksOnOffBuffer = TSNG(2) & B00011111;
              //
              stP = (TSNG(1) >> 5);
              if (stP == 4) stepsPos = 2;
              else if (stP == 5) stepsPos = 3;              
              //
              switch ((TSNG(2) >> 5))
              {
                case 1:
                {
                  patternFade = 1;
                  patternFadeCounter = 0;        
                  Serial.write(16); 
                }
                break;
                
                case 2:
                {
                  patternFade = -1;
                  patternFadeCounter = 128;        
                  Serial.write(17); 
                }
                break;      
              }
            }
          }
        }
        //        
        // Next Pattern Check //
        checkNextPattern();
        Serial.write(B00100111);
        Serial.write(tracksOnOffBuffer);
        //
        if (bitRead(tracksOnOff,4) && !bitRead(tracksOnOffBuffer,4))
        {
          if (prevNote > 0) sendseqNoteOff(prevNote);       
          prevNote = 0;          
        }
        tracksOnOff = tracksOnOffBuffer;
      }
      //
      if (autoStepPos == 1) editStepsPos = stepsPos;
      //
      Serial.write(B00100100); // Steps Position
      Serial.write(stepsPos);
      //
      if (autoStepPos == 1 && seq_mirror == 1) Serial.write(14);
      else if (autoStepPos == 1 && seq_mirror == 0) Serial.write(15);
    }
    //
    Serial.write(B00100101); // Seq Position
    Serial.write(seqClockCounter);
  }
  else if (seqClockCounterDivider == (seqClockShuffleData[1][seqClockShuffleCounter]*2))
  {
    Serial.write(0); // Send MIDI Clock to all Slave Units //
    noteTrackProcess(0);
  }
}

// ======================================================================================= //
void noteTrackProcess(uint8_t isDouble)
{
  int8_t xVel = -1;
  if (patternFade == 1)
  {
    patternFadeCounter++;
    xVel = patternFadeCounter;
    if (patternFadeCounter >= 127) patternFade = 0;
  }
  else if (patternFade == -1)
  {
    patternFadeCounter--;    
    xVel = patternFadeCounter;
    if (patternFadeCounter == 0) patternFade = 0;
  }  
  //
  if (!bitRead(tracksOnOff,4)) return;
  //
  uint8_t dType = last_settings.noteSteps[currentPattern][0][seqClockCounter];
  uint8_t dValue = last_settings.noteSteps[currentPattern][1][seqClockCounter];
  //  
  if (dType > 0 && (isDouble == 0 || dType == 3))
  {
    uint8_t dBBac = isDouble+(2*stepsPos);
    uint8_t velocity = 0;
    if (dType <= 4)
    {
      velocity = 87+(bitRead(ACCENT1[dBBac],seqClockCounter)*20)+(bitRead(ACCENT2[dBBac],seqClockCounter)*20);
      int16_t note = dValue + last_settings.patternsTranspose[currentPattern][stepsPos];
      if (note > 127) note = 127;
      if (note < 0) note = 0;
      dValue = (uint8_t) note;
      if (note == 0) return;
    }
    if (xVel != -1) velocity = xVel;
    //
    switch(dType)
    {
      case 1: // Gate
      case 3: // Flam
      {
         if (prevNote > 0) sendseqNoteOff(prevNote);
         sendseqNoteOn(dValue, velocity);
         prevNote = dValue;
      }
      break;
       
      case 2: // Slide
      {
         sendseqNoteOn(dValue, velocity);
         if (prevNote > 0) sendseqNoteOff(prevNote);       
         prevNote = dValue;
      }
      break;
       
      case 4: // Off
      {
         if (prevNote > 0) sendseqNoteOff(prevNote);       
         prevNote = 0;
      }
      break;     
     
      default: // Midi CC - ToDo //
      {
        sendseqMIDIcc(dType-5, dValue);
      }
      break;
    }
  }
}

// ======================================================================================= //
void sendseqNoteOn(char note, char velocity)
{
  Serial.write(B01100100); // Sequencer to SND Generator
  Serial.write(MIDI_CHANNEL_NOTE_TRACK & B00011111);  // Note On
  Serial.write(note);
  Serial.write(velocity);
}

// ======================================================================================= //
void sendseqNoteOff(char note)
{
  Serial.write(B01100100); // Sequencer to SND Generator
  Serial.write(B00100000 | (MIDI_CHANNEL_NOTE_TRACK & B00011111));  // Note Off
  Serial.write(note);
  Serial.write(0);
}

// ======================================================================================= //
void sendseqMIDIcc(char type, char value)
{
  Serial.write(B01100100); // Sequencer to SND Generator
  Serial.write(B01100000 | (MIDI_CHANNEL_NOTE_TRACK & B00011111));  // Midi CC
  Serial.write(type);
  Serial.write(value);
}
