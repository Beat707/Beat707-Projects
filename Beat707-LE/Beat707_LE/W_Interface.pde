/*

  Created by Beat707 (c) 2011 - http://www.Beat707.com
  
  Interface Buttons and Display
  
*/

// ======================================================================================= //
void readButtons()
{
  for (char q=0; q<16; q++)
  {
    if (buttonClick[q] > 0)
    {
      dbStepsCalc();
      if (buttonClick[q] == 1) // Click
      {
        if (specialMode == 0)
        {
          somethingChanged = 1;
          if (song_settings.mirrorPatternEdit == 1) 
          {
            bitWrite(song_settings.dmSteps[currentPattern][currentTrack+((DRUMTRACKS+2)*editDoubleSteps)],q,!bitRead(song_settings.dmSteps[currentPattern][currentTrack+((DRUMTRACKS+2)*editDoubleSteps)],q));
            bitWrite(song_settings.dmSteps[currentPattern][currentTrack+((DRUMTRACKS+2)*editDoubleSteps)+((DRUMTRACKS+2)*2)],q,!bitRead(song_settings.dmSteps[currentPattern][currentTrack+((DRUMTRACKS+2)*editDoubleSteps)+((DRUMTRACKS+2)*2)],q));
          }
          else bitWrite(song_settings.dmSteps[currentPattern][currentTrack+dbSteps],q,!bitRead(song_settings.dmSteps[currentPattern][currentTrack+dbSteps],q));
        }
        else
        {
          if (specialModeZone == 0)
          {
            if (q == 0) specialModeZone = 1; // BPM
            else if (q == 1) specialModeZone = 2; // Note-Number
            else if (q == 2) specialModeZone = 3; // Channel
            else if (q == 3) specialModeZone = 4; // numberOfSteps
            else if (q == 4) specialModeZone = 5; // midiClockShuffle
            else if (q == 5) specialModeZone = 6; // midiClockDirection
              else specialMode = 0; // Close
            clearDisplay();
          }
          else
          {
            if (q == 5) // Up
            {
              somethingChanged = 1;
              if (specialModeZone == 1 && song_settings.midiClockBPM < 250) { song_settings.midiClockBPM++; MidiClockNewTime(); } // BPM
                else if (specialModeZone == 2 && currentTrack < DRUMTRACKS && song_settings.dmNotes[currentTrack] < 127) song_settings.dmNotes[currentTrack]++; // Note-Number
                else if (specialModeZone == 3 && currentTrack < DRUMTRACKS && song_settings.dmChannel[currentTrack] < 15) song_settings.dmChannel[currentTrack]++; // Channel
                else if (specialModeZone == 4 && song_settings.numberOfSteps < 16) song_settings.numberOfSteps++; // numberOfSteps
                else if (specialModeZone == 5 && song_settings.midiClockShuffle < 6) { song_settings.midiClockShuffle++; MidiShuffleUpdate(); }// midiClockShuffle
                else if (specialModeZone == 6) song_settings.midiClockDirection = 1; // midiClockDirection
            }
            else if (q == 8) // Left
            {
              somethingChanged = 1;
              if (specialModeZone == 1 && song_settings.midiClockBPM >= 35) { song_settings.midiClockBPM -= 10; MidiClockNewTime(); } // BPM
                else if (specialModeZone == 2 && currentTrack < DRUMTRACKS && song_settings.dmNotes[currentTrack] >= 10) song_settings.dmNotes[currentTrack] -= 10; // Note-Number
            }
            else if (q == 9) // Center
            {
              somethingChanged = 1;
              if (specialModeZone == 1) { song_settings.midiClockBPM = 120; MidiClockNewTime(); } // BPM
                else if (specialModeZone == 2 && currentTrack < DRUMTRACKS) song_settings.dmNotes[currentTrack] = 60; // Note-Number
                else if (specialModeZone == 3 && currentTrack < DRUMTRACKS) song_settings.dmChannel[currentTrack] = 9; // Channel              
                else if (specialModeZone == 4) song_settings.numberOfSteps = 16; // numberOfSteps
                else if (specialModeZone == 5) { song_settings.midiClockShuffle = 0; MidiShuffleUpdate(); }// midiClockShuffle
                else if (specialModeZone == 6) song_settings.midiClockDirection = 0; // midiClockDirection
            }
            else if (q == 10) // Right
            {
              somethingChanged = 1;
              if (specialModeZone == 1 && song_settings.midiClockBPM <= 240) { song_settings.midiClockBPM += 10; MidiClockNewTime(); } // BPM
                else if (specialModeZone == 2 && currentTrack < DRUMTRACKS && song_settings.dmNotes[currentTrack] <= 117) song_settings.dmNotes[currentTrack] += 10; // Note-Number
            }
            else if (q == 13) // down
            {
              somethingChanged = 1;
              if (specialModeZone == 1 && song_settings.midiClockBPM > 25) { song_settings.midiClockBPM--; MidiClockNewTime(); } // BPM
                else if (specialModeZone == 2 && currentTrack < DRUMTRACKS && song_settings.dmNotes[currentTrack] > 0) song_settings.dmNotes[currentTrack]--; // Note-Number
                else if (specialModeZone == 3 && currentTrack < DRUMTRACKS && song_settings.dmChannel[currentTrack] > 0) song_settings.dmChannel[currentTrack]--; // Channel              
                else if (specialModeZone == 4 && song_settings.numberOfSteps > 1) song_settings.numberOfSteps--; // numberOfSteps
                else if (specialModeZone == 5 && song_settings.midiClockShuffle > 0) { song_settings.midiClockShuffle--; MidiShuffleUpdate(); } // midiClockShuffle
                else if (specialModeZone == 6) song_settings.midiClockDirection = 2; // midiClockDirection
            }
            else 
            {
              specialModeZone = 0;
              clearDisplay();
            }
          }
        }
      }
      else // Hold
      {
        if (q < 8)
        {
          if (q == 4) { if (currentTrack == 4) currentTrack = 8; else currentTrack = 4; }
            else if (q == 5) { if (currentTrack == 5) currentTrack = 9; else currentTrack = 5; }
            else if (q == 6) { if (currentTrack == 6) currentTrack = ACCENT1; else currentTrack = 6; }
            else if (q == 7) { if (currentTrack == 7) currentTrack = ACCENT2; else currentTrack = 7; }
            else currentTrack = q;
        }
        else if (q == 8)
        {
          specialMode = 1;
          specialModeZone = 0;
          clearDisplay();
        }
        else if (q == 9)
        {
          somethingChanged = 1;
          song_settings.mirrorPatternEdit = !song_settings.mirrorPatternEdit;
        }
        else if (q == 10)
        {
          editDoubleSteps = !editDoubleSteps;
        }
        else if (q == 11)
        {
          if (midiClockRunning == 0)
          {
            MidiClockStart(1);
          }
          else
          {
           MidiClockStop();
           if (somethingChanged)
           {
             somethingChanged = 0;
             greenf[q] = bluef[q] = redf[q] = 15;
             eeprom_write_block((const void*)&song_settings, (void*)0, sizeof(song_settings));
             greenf[q] = bluef[q] = redf[q] = 0;
           }
          }
        }
        else
        {
          uint8_t newPattern = q-12;
          if (newPattern == nextPattern) newPattern += 4;
          nextPattern = newPattern;
          newPatternBlinkCounter = 0;
          if (midiClockRunning == 0) currentPattern = nextPattern;
        }
      }
      buttonClick[q] = 0;
    }
  }
}

// ======================================================================================= //
void setLEDs()
{
  if (specialMode == 0)
  {
    if (midiClockRunning == 1)
    {
      if (prevMidiClockCounter != midiClockCounter)
      {
        prevMidiClockCounter = midiClockCounter;
        TRANSPORT_COLOR[prevMidiClockCounter] = TRANSPORT_COLOR_STRENG;
        transportBlinkCounter = 0;
      }
      else
      {
        if (transportBlinkCounter == TRANSPORT_BLINK_RATE)
        { 
          TRANSPORT_COLOR[prevMidiClockCounter] = 0; 
          transportBlinkCounter++;
          if (transportBlinkClear == 1)
          {
            transportBlinkClear = 0;
            memset(TRANSPORT_COLOR,0,sizeof(TRANSPORT_COLOR));
          }
        }
        else if (transportBlinkCounter < TRANSPORT_BLINK_RATE) transportBlinkCounter++;
      }
    }
    
    dbStepsCalc();
    
    for (char q=0; q<16; q++)
    {
      if (bitRead(song_settings.dmSteps[currentPattern][currentTrack+dbSteps],q) == 1)
      {
        STEPS_COLOR[q] = 5;
        if (bitRead(song_settings.dmSteps[currentPattern][ACCENT1+dbSteps],q) == 1) STEPS_COLOR[q] += 5;
        if (bitRead(song_settings.dmSteps[currentPattern][ACCENT2+dbSteps],q) == 1) STEPS_COLOR[q] += 5;
      } else STEPS_COLOR[q] = 0;
    }

    memset(tempColor,0,sizeof(tempColor));
    if (currentTrack < 8) tempColor[currentTrack] = CUR_TRACK_COLOR_STRENG;
      else tempColor[currentTrack-4] = CUR_TRACKAC_COLOR_STRENG;
    for (char q=0; q<8; q++) CUR_TRACK_COLOR[q] = tempColor[q];
    
    memset(tempColor,0,sizeof(tempColor));
    if (nextPattern < 4) tempColor[nextPattern] = CUR_PATTERN03_COLOR_STRENG;
      else tempColor[nextPattern-4] = CUR_PATTERN47_COLOR_STRENG;

    if (midiClockRunning == 1 && nextPattern != currentPattern)
    {
      newPatternBlinkCounter++;
      if (newPatternBlinkCounter > 2000) newPatternBlinkCounter = 0;
        else if (newPatternBlinkCounter > 800) memset(tempColor,0,sizeof(tempColor));      
    }
    for (char q=0; q<4; q++) CUR_PATTERN_COLOR[12+q] = tempColor[q];
    
    if (song_settings.mirrorPatternEdit == 1) MIRROR_PATTMODE_COLOR[9] = MIRROR_PATTMODE_COLOR_STRENG; else MIRROR_PATTMODE_COLOR[9] = 0;
    if (editDoubleSteps == 1) ED_DOUBLE_STEPS_COLOR[10] = ED_DOUBLE_STEPS_COLOR_STRENG; else ED_DOUBLE_STEPS_COLOR[10] = 0;
  }
  else
  {
    if (specialModeZone == 0)
    {
      SPECIAL_MODE_COLOR[0] = SPECIAL_MODE_COLOR_STRENG;
      SPECIAL_MODE_COLOR[1] = SPECIAL_MODE_COLOR_STRENG;
      SPECIAL_MODE_COLOR[2] = SPECIAL_MODE_COLOR_STRENG;
      SPECIAL_MODE_COLOR[3] = SPECIAL_MODE_COLOR_STRENG;
      SPECIAL_MODE_COLOR[4] = SPECIAL_MODE_COLOR_STRENG;
      SPECIAL_MODE_COLOR[5] = SPECIAL_MODE_COLOR_STRENG;
      SPECIAL_MODE_CLOSE_COLOR[15] = SPECIAL_MODE_CLOSE_COLOR_STRENG;
    }
    else
    {
      SPECIAL_MODE_COLOR[5] = SPECIAL_MODE_COLOR_STRENG; // Up
      if (specialModeZone == 1 || specialModeZone == 2) SPECIAL_MODE_COLOR[8] = SPECIAL_MODE_COLOR_STRENG; // Left
      SPECIAL_MODE_COLOR[9] = SPECIAL_MODE_CENTER_COLOR_STRENG; // Center
      if (specialModeZone == 1 || specialModeZone == 2) SPECIAL_MODE_COLOR[10] = SPECIAL_MODE_COLOR_STRENG; // Right
      SPECIAL_MODE_COLOR[13] = SPECIAL_MODE_COLOR_STRENG; // Down
      SPECIAL_MODE_CLOSE_COLOR[15] = SPECIAL_MODE_CLOSE_COLOR_STRENG;
      
      memset(tempColor,0,sizeof(tempColor));
      if (specialModeZone == 2 || specialModeZone == 3)
      {
        if (currentTrack < 8) tempColor[currentTrack] = SPECIAL_MODE_CURTRACK_COLOR_STRENG;
          else tempColor[currentTrack-4] = SPECIAL_MODE_CURTRACKAC_COLOR_STRENG;
      }
      for (char q=0; q<8; q++) SPECIAL_MODE_CURTRACK_COLOR[q] = tempColor[q];
    }
  }
}
