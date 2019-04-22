/*

  Created by Guilherme (William) Kalfelz
  @ Beat707 (c) 2014 - http://www.Beat707.com

*/

// ======================================================================================= //
void checkUpDown()
{
  if ((holdUpDown != 0 || holdLeftRight != 0) && editingOn)
  {
    holdUpDownCounter++;
    if (holdUpDownCounter > ((holdUpDown != 0) ? UP_DOWN_REPEATE_RATE : LEFT_RIGHT_REPEATE_RATE))
    {
      holdUpDownCounter = 0;
      if (holdLeftRight != 0)
      {
        if (holdLeftRight > 0) buttonToZone(6); else buttonToZone(4);        
      }
      else
      {
        if (holdUpDown > 0) buttonToZone(1); else buttonToZone(9); 
      }
    }
  }
}

// ======================================================================================= //
void buttonToZone(uint8_t button)
{
  if (last_settings.bMode == kModeSongPerf)
  {
    // ================================================================================ //
    // =========== SONG PERFORMANCE MODE ============================================== //
    // ================================================================================ //
    //
    if (editZone == kZoneSPmode)
    {
      switch (button)
      {
      case 1: updateSPmode(1); break; 
      case 5: updateSPmode(0); break; 
      case 9: updateSPmode(-1); break; 
      case 6: editZone = kZoneSPsection; break; 
      }    
    }
    else if (editZone == kZoneSPsection)
    {
      switch (button)
      {
      case 1: updateSongPsec(1); break; 
      case 5: updateSongPsec(0); break; 
      case 9: updateSongPsec(-1); break; 
      case 4: editZone = kZoneSPmode; break; 
      case 6: editZone = kZoneSPrpt; break; 
      }
    }    
    else if (editZone == kZoneSPrpt)
    {
      switch (button)
      {
      case 1: updateSongPRpt(1); break; 
      case 5: updateSongPRpt(0); break; 
      case 9: updateSongPRpt(-1); break; 
      case 4: editZone = kZoneSPsection; break; 
      case 6: editZone = kZoneSPstart; break; 
      }
    }
    else if (editZone == kZoneSPstart)
    {
      switch (button)
      {
      case 1: updateSongPstart(1); break; 
      case 5: updateSongPstart(0); break; 
      case 9: updateSongPstart(-1); break; 
      case 4: editZone = kZoneSPrpt; break; 
      case 6: editZone = kZoneSPend; break; 
      }
    }
    else if (editZone == kZoneSPend)
    {
      switch (button)
      {
      case 1: updateSongPend(1); break;
      case 5: updateSongPend(0); break;
      case 9: updateSongPend(-1); break;
      case 4: editZone = kZoneSPstart; break;
      case 6:
        {
          editingOn = 0;
          editZone = kZoneSPsection;
          showOptions = 1;
          showOptionsOption = 0;
          updateNoteTrackLEDs();
          Serial.write(4); // Hide Navigation Keys //
        }
        break;
      }
    }
  }
  else if (last_settings.bMode == kModeSong)
  {
    // ================================================================================ //
    // =========== SONG MODE ========================================================== //
    // ================================================================================ //
    //
    if (editZone >= kZoneSongPos && editZone <= kZoneSongSp)
    {
      switch (button)
      {
      case 15: updateSongPos(1); break; 
      case 14: updateSongPos(-1); break; 
      case 12: updateSongPos(0); break; 
      }    
    }
    //    
    if (editZone == kZoneSongPos || upDownForceToTNpos == 1) // ------- PAGE 1 ------------ //
    {
      switch (button)
      {
      case 1: updateSongPos(1); break; 
      case 5: updateSongPos(0); break; 
      case 9: updateSongPos(-1); break; 
      case 6: editZone = kZoneSongPat; break; 
      }
    }
    else if (editZone == kZoneSongPat)
    {
      switch (button)
      {
      case 1: updateSongPat(1); break; 
      case 5: updateSongPat(0); break; 
      case 9: updateSongPat(-1); break; 
      case 4: editZone = kZoneSongPos; break; 
      case 6: if (TSNG(0) > 0) editZone = kZoneSongRpt; else editZone = kZoneSongBPM; break; 
      }
    }    
    else if (editZone == kZoneSongRpt)
    {
      switch (button)
      {
      case 1: updateSongRpt(1); break; 
      case 5: updateSongRpt(0); break; 
      case 9: updateSongRpt(-1); break; 
      case 4: editZone = kZoneSongPat; break; 
      case 6: editZone = kZoneSongSteps; break; 
      }
    }
    else if (editZone == kZoneSongSteps)
    {
      switch (button)
      {
      case 1: updateSongSteps(1); break; 
      case 5: updateSongSteps(0); break; 
      case 9: updateSongSteps(-1); break; 
      case 4: editZone = kZoneSongRpt; break; 
      case 6: editZone = kZoneSongTrack1; break; 
      }
    }
    else if (editZone >= kZoneSongTrack1 && editZone <= kZoneSongTrack5)
    {
      switch (button)
      {
      case 1: updateSongTrack(1); break; 
      case 5: updateSongTrack(0); break; 
      case 9: updateSongTrack(-1); break; 
      case 4: editZone--; break; 
      case 6: editZone++; break; 
      }
    }
    else if (editZone == kZoneSongSp)
    {
      switch (button)
      {
      case 1: updateSongSp(1); break; 
      case 5: updateSongSp(0); break; 
      case 9: updateSongSp(-1); break; 
      case 4: editZone = kZoneSongTrack5; break; 
      case 6: editZone = kZoneSongBPM; break;
      }
    }
    else if (editZone == kZoneSongBPM) // ------- PAGE 2 ------------ //
    {
      switch (button)
      {
      case 1: updateSongBPM(1); break; 
      case 5: updateSongBPM(0); break; 
      case 9: updateSongBPM(-1); break; 
      case 4: if (!seqClockRunning) { if (TSNG(0) == 0) editZone = kZoneSongPat; else editZone = kZoneSongSp; } break; 
      case 6: if (!seqClockRunning) editZone = kZoneSongLoop; break;
      }
    }
    else if (editZone == kZoneSongLoop) // ------- PAGE 2 ------------ //
    {
      switch (button)
      {
      case 1: updateSongLoop(1); break; 
      case 5: updateSongLoop(0); break; 
      case 9: updateSongLoop(-1); break; 
      case 4: editZone = kZoneSongBPM; break; 
      case 6:
        {
          editingOn = 0;
          editZone = kZoneSongPos;
          showOptions = 1;
          showOptionsOption = 0;
          updateNoteTrackLEDs();
          Serial.write(4); // Hide Navigation Keys //
        }
        break;
      }
    }
  }
  else if (last_settings.bMode == kModePat)
  {
    // ================================================================================ //
    // =========== PATTERN MODE ======================================================= //
    // ================================================================================ //
    //
    if (editZone == kZoneBPM) // ------- PAGE 1 ------------ //
    {
      switch (button)
      {
      case 1: updateBPM(1); break; 
      case 5: updateBPM(0); break; 
      case 9: updateBPM(-1); break; 
      case 6: editZone = kZoneSteps; break; 
      }
    }
    else if (editZone == kZoneSteps)
    {
      switch (button)
      {
      case 1: updateSteps(1); break; 
      case 5: updateSteps(0); break; 
      case 9: updateSteps(-1); break; 
      case 4: editZone = kZoneBPM; break; 
      case 6: editZone = kZonePat; break;       
      }
    }            
    else if (editZone == kZonePat)
    {
      switch (button)
      {
      case 1: updatePattern(1); break; 
      case 5: updatePattern(0); break; 
      case 9: updatePattern(-1); break; 
      case 4: editZone = kZoneSteps; break; 
      case 6: editZone = kZoneTrack; break; 
      }
    }
    else if (editZone == kZoneTrack)
    {
      switch (button)
      {
      case 1: updateTrack(1); break; 
      case 5: updateTrack(0); break; 
      case 9: updateTrack(-1); break; 
      case 4: editZone = kZonePat; break; 
      case 6: editZone = kZoneNT_pos; break;        
      } 
    }
    else if (editZone == kZoneNT_pos || upDownForceToTNpos == 1) // ------- PAGE 2 ------------ //  
    {
      switch (button)
      {
      case 1: updateNtPos(1); break; 
      case 5: updateNtPos(0); break; 
      case 9: updateNtPos(-1); break; 
      case 4: editZone = kZoneTrack; break; 
      case 6: editZone = kZoneNT_value; break;        
      } 
    }
    else if (editZone == kZoneNT_value)
    {
      switch (button)
      {
      case 1: updateNtVal(1); break; 
      case 5: updateNtVal(0); break; 
      case 9: updateNtVal(-1); break; 
      case 4: editZone = kZoneNT_pos; break; 
      case 6: editZone = kZoneNT_type; break;        
      } 
    }  
    else if (editZone == kZoneNT_type)
    {
      switch (button)
      {
      case 1: updateNtType(1); break; 
      case 5: updateNtType(0); break; 
      case 9: updateNtType(-1); break; 
      case 4: editZone = kZoneNT_value; break; 
      case 6: editZone = kZoneNT_transpose1; break;        
      } 
    }
    else if (editZone >= kZoneNT_transpose1 && editZone <= kZoneNT_transpose4) // ------- PAGE 3 ------------ //  
    {
      switch (button)
      {
        case 1: updateTranspose(1); break;
        case 5: updateTranspose(0); break;
        case 9: updateTranspose(-1); break;
        case 4: editZone = editZone-1; break;
        case 6: 
        {
          if (editZone != kZoneNT_transpose4)
          {
            editZone = editZone+1; 
          }
          else 
          {
            editingOn = 0;
            editZone = kZoneBPM;
            showOptions = 1;
            showOptionsOption = 0;
            updateNoteTrackLEDs();
            Serial.write(4); // Hide Navigation Keys //
          }
        }
        break;
      } 
    }  
    //
    // ---------------------- //
    //
    if (editZone >= kZoneNT_value && editZone <= kZoneNT_type)
    {
      switch (button)
      {
      case 15: updateNtPos(1); break; 
      case 14: updateNtPos(-1); break; 
      }    
    }
  }
  //
  doLCDupdate++;
}

// ======================================================================================= //
// ======================================================================================= //
// ======================================================================================= //
// ======================================================================================= //
void updateSPmode(int mode)
{
  if (mode > 0 && (last_settings.perfDataMode+mode) <= 1) last_settings.perfDataMode += mode;
  else if (mode == 0) last_settings.perfDataMode = 0;
  else if (mode < 0 && (last_settings.perfDataMode+mode) >= 0) last_settings.perfDataMode += mode;
  doLCDupdate++;
}

// ======================================================================================= //
void updateSongPsec(int mode)
{
  if (mode > 0 && (songPerfPos+mode) <= 15) songPerfPos += mode;
  else if (mode == 0) songPerfPos = 0;
  else if (mode < 0 && (songPerfPos+mode) >= 0) songPerfPos += mode;
  doLCDupdate++;
}

// ======================================================================================= //
void updateSongPRpt(int mode)
{
  if (mode > 0 && (TSPF(2)+mode) <= 100) TSPF(2) += mode;
  else if (mode == 0) TSPF(2) = 0;
  else if (mode < 0 && (TSPF(2)+mode) >= 0) TSPF(2) += mode;
  doLCDupdate++;
}

// ======================================================================================= //
void updateSongPstart(int mode)
{
  if (mode > 0 && (TSPF(0)+mode) <= 98) TSPF(0) += mode;
  else if (mode == 0) TSPF(0) = 0;
  else if (mode < 0 && (TSPF(0)+mode) >= 0) TSPF(0) += mode;
  doLCDupdate++;
}

// ======================================================================================= //
void updateSongPend(int mode)
{
  if (mode > 0 && (TSPF(1)+mode) <= 98) TSPF(1) += mode;
  else if (mode == 0) TSPF(1) = 0;
  else if (mode < 0 && (TSPF(1)+mode) >= 0) TSPF(1) += mode;
  doLCDupdate++;
}

// ======================================================================================= //
void updateSongBPM(int mode)
{
  if (mode > 0 && (last_settings.seq_bpm+mode) <= 255) last_settings.seq_bpm += mode;
  else if (mode == 0) last_settings.seq_bpm = 120;
  else if (mode < 0 && (last_settings.seq_bpm+mode) >= 25) last_settings.seq_bpm += mode;
  seqClockNewTime();
  doLCDupdate++;
}

// ======================================================================================= //
void updateSongLoop(int mode)
{
  if (mode > 0) last_settings.loopSong = 1;
  else if (mode == 0) last_settings.loopSong = 0;
  else if (mode < 0) last_settings.loopSong = 0;
  doLCDupdate++;  
}

// ======================================================================================= //
void updateSongPos(int mode)
{
  if (mode > 0 && (songEditPos+mode) <= 98) songEditPos += mode;
  else if (mode == 0) songEditPos = 0;
  else if (mode < 0 && (songEditPos+mode) >= 0) songEditPos += mode;
  if (TSNG(0) == 0 && editZone > kZoneSongPat) editZone = kZoneSongPat;
  doLCDupdate++;
}
// ======================================================================================= //
void updateSongPat(int mode)
{
  if (mode > 0 && (TSNG(0)+mode) <= 16) TSNG(0) += mode;
  else if (mode == 0) TSNG(0) = 0;
  else if (mode < 0 && (TSNG(0)+mode) >= 0) TSNG(0) += mode;
  doLCDupdate++;
}

// ======================================================================================= //
void updateSongRpt(int mode)
{
  uint8_t rP = TSNG(1) & B00011111;
  if (mode > 0 && (rP+mode) <= 31) rP += mode;
  else if (mode == 0) rP = 0;
  else if (mode < 0 && (rP+mode) >= 0) rP += mode;
  TSNG(1) = (TSNG(1) & B11100000) | (rP & B00011111);
  doLCDupdate++;  
}

// ======================================================================================= //
void updateSongSteps(int mode)
{
  uint8_t rS = TSNG(1) >> 5;
  if (mode > 0 && (rS+mode) <= 5) rS += mode;
  else if (mode == 0) rS = 0;
  else if (mode < 0 && (rS+mode) >= 0) rS += mode;
  TSNG(1) = (TSNG(1) & B00011111) | (rS << 5);
  doLCDupdate++;
}

// ======================================================================================= //
void updateSongTrack(int mode)
{
  if (mode > 0) bitSet(TSNG(2), (editZone-kZoneSongTrack1));
  else if (mode == 0) bitClear(TSNG(2), (editZone-kZoneSongTrack1));
  else if (mode < 0) bitClear(TSNG(2), (editZone-kZoneSongTrack1));
  doLCDupdate++;  
}

// ======================================================================================= //
void updateSongSp(int mode)
{
  uint8_t rS = TSNG(2) >> 5;
  if (mode > 0 && (rS+mode) <= 2) rS += mode;
  else if (mode == 0) rS = 0;
  else if (mode < 0 && (rS+mode) >= 0) rS += mode;
  TSNG(2) = (TSNG(2) & B00011111) | (rS << 5);
  doLCDupdate++;
}

// ======================================================================================= //
void updateTranspose(int mode)
{
  #define TRANS last_settings.patternsTranspose[currentPattern][editZone-kZoneNT_transpose1]
  //
  if (mode > 0 && (TRANS+mode) <= 99) TRANS += mode;
  else if (mode == 0) TRANS = 0;
  else if (mode < 0 && (TRANS+mode) >= -99) TRANS += mode;
  doLCDupdate++;
}

// ======================================================================================= //
void updateNtPos(int mode)
{
  if (mode > 0 && (editNTrackPos+mode) <= 15) editNTrackPos += mode;
  else if (mode == 0) editNTrackPos = 0;
  else if (mode < 0 && (editNTrackPos+mode) >= 0) editNTrackPos += mode;
  doLCDupdate++;
}

// ======================================================================================= //
void updateNtVal(int mode)
{
  uint8_t nV = TNOTE(1); 
  uint8_t cpBuffer = 1;
  if (mode > 0 && (nV+mode) <= 127) nV += mode;
  else if (mode == 0)
  {
    uint8_t nV0 = TNOTE(0);
    if (nV0 == 0)
    {
      TNOTE(0) = lastTNVals[0];
      nV = lastTNVals[1];
    }
    else if (nV0 >= 5)
    {
      if (nV == 0) nV = 127;
      else nV = 0;
    }
    else 
    {
      if (nV == 60)
      {
        nV = 0;
        TNOTE(0) = 0;
        cpBuffer = 0;
      }
      else nV = 60;
    }
  }
  else if (mode < 0 && (nV+mode) >= 0) nV += mode;
  TNOTE(1) = nV;
  if (cpBuffer)
  {
    lastTNVals[0] = TNOTE(0);
    lastTNVals[1] = nV;
  }
  doLCDupdate++;
}

// ======================================================================================= //
void updateNtType(int mode)
{
  uint8_t nV = TNOTE(0);
  uint8_t cpBuffer = 1;
  if (mode > 0 && (nV+mode) <= 124) nV += mode;
  else if (mode == 0) 
  {
    nV = 0;
    TNOTE(1) = 0;
    cpBuffer = 0;
  }
  else if (mode < 0 && (nV+mode) >= 0) nV += mode;
  TNOTE(0) = nV;
  if (cpBuffer)
  {
    lastTNVals[1] = TNOTE(1);
    lastTNVals[0] = nV;  
  }
  doLCDupdate++;
}

// ======================================================================================= //
void updateBPM(int mode)
{
  if (mode > 0 && (last_settings.seq_bpm+mode) <= 255) last_settings.seq_bpm += mode;
  else if (mode == 0) last_settings.seq_bpm = 120;
  else if (mode < 0 && (last_settings.seq_bpm+mode) >= 25) last_settings.seq_bpm += mode;
  seqClockNewTime();
  doLCDupdate++;
}

// ======================================================================================= //
void updateSteps(int mode)
{
  if (mode > 0 && (editStepsPos+mode) <= 3)
  {
    editStepsPos += mode;
    seq_mirror = autoStepPos = 0;
  }
  else if (mode == 0)
  { 
    editStepsPos = stepsPos; 
    if (autoStepPos == 0)
    {
      autoStepPos = seq_mirror = 1; 
    }
    else
    {
      if (seq_mirror == 1) seq_mirror = 0; else seq_mirror = 1;
    }
  }
  else if (mode < 0 && (editStepsPos+mode) >= 0) 
  {
    editStepsPos += mode;
    seq_mirror = autoStepPos = 0;
  }
  doLCDupdate++;
  updateAutoSteps();
}

// ======================================================================================= //
void updateTrack(int mode)
{
  if (mode > 0 && (currentTrack+mode) <= 6) currentTrack += mode;
  else if (mode == 0) currentTrack = 0;
  else if (mode < 0 && (currentTrack+mode) >= 0) currentTrack += mode;
  //
  doLCDupdate++;
  updateNoteTrackLEDs();
}

// ======================================================================================= //
void updatePattern(int mode)
{
  if (mode > 0 && (nextPattern+mode) <= 15) nextPattern += mode;
  else if (mode == 0) nextPattern = 0;
  else if (mode < 0 && (nextPattern+mode) >= 0) nextPattern += mode;
  //
  if (!seqClockRunning) checkNextPattern(); else doLCDupdate++;
}

// ======================================================================================= //
// ======================================================================================= //
// ======================================================================================= //
void OptionsConfirm()
{      
  if (last_settings.bMode == kModeSong)
  {
    // ================================================================================ //
    // =========== SONG MODE ========================================================== //
    // ================================================================================ //
    //
    if (seqClockRunning) seqClockStop();
    //
    switch (showOptionsOption)
    {
      case 1: // Change Mode //
      {
        editZone = kZoneBPM;
        last_settings.bMode = kModePat;
      }
      break;
      
      case 2: // Change Mode //
      {
        editZone = kZoneSPsection;
        last_settings.bMode = kModeSongPerf;
        songEditPos = 0;
      }
      break;
      
      case 3: // Insert Position //
      {
        if (songEditPos != 99)
        {
          for (int x=98; x>songEditPos; x--)
          {
            last_settings.songData[0][x] = last_settings.songData[0][x-1];
            last_settings.songData[1][x] = last_settings.songData[1][x-1];
            last_settings.songData[2][x] = last_settings.songData[2][x-1];
          }
          last_settings.songData[0][songEditPos] = 0;
          last_settings.songData[1][songEditPos] = B01100000;
          last_settings.songData[2][songEditPos] = B00011111;          
        }        
      }
      break;
      
      case 4: // Remove Position //
      {
        if (songEditPos == 98)
        {
          last_settings.songData[0][98] = 0;
          last_settings.songData[1][98] = B01100000;
          last_settings.songData[2][98] = B00011111;
        }
        else
        {
          for (int x=songEditPos; x<98; x++)
          {
            last_settings.songData[0][x] = last_settings.songData[0][x+1];
            last_settings.songData[1][x] = last_settings.songData[1][x+1];
            last_settings.songData[2][x] = last_settings.songData[2][x+1];
          }
        }
      }
      break;
      
      case 5: // Init Song Data //
      {
        if (!optionsConfirm()) return;
        songEditPos = 0;
        initSongData();
      }
      break;
      
      case 6: // Save All Data //
      {
        saveDataToEEPROM();
      }
      break;
    }
  }
  else if (last_settings.bMode == kModePat)
  {
    // ================================================================================ //
    // =========== PATTERN MODE ======================================================= //
    // ================================================================================ //
    //
    switch (showOptionsOption)
    {   
      case 1: // Change Mode //
      {
        editZone = kZoneSongPat;
        last_settings.bMode = kModeSong;
        songEditPos = 0;
        if (seqClockRunning) seqClockStop();
      }
      break;
      
      case 2: // Change Mode //
      {
        editZone = kZoneSPsection;
        last_settings.bMode = kModeSongPerf;
        songEditPos = 0;
        if (seqClockRunning) seqClockStop();
      }
      break;
      
      case 3: // Copy Pattern //
      {
        copyNoteTrackToBuffer();
        Serial.write(B00101000); // Pattern Special Functions
        Serial.write(2);
      }
      break;
      
      case 4: // Paste Pattern //
      {
        copyNoteTrackFromBuffer();
        Serial.write(B00101000); // Pattern Special Functions
        Serial.write(3);
      }
      break;
      
      case 5: // Copy To Next Pat //
      {
        if (currentPattern <= 14) 
        {
          copyNoteTrackToBuffer();
          Serial.write(B00101000); // Pattern Special Functions
          Serial.write(2);
          //
          currentPattern++;
          nextPattern = currentPattern;
          Serial.write(B00100011);
          Serial.write(currentPattern);
          copyNoteTrackFromBuffer();
          //
          Serial.write(B00101000);
          Serial.write(3);
        }
      }
      break;
      
      case 6: // Init Pattern //
      {
        if (!optionsConfirm()) return;
        Serial.write(B00101000); // Pattern Special Functions
        Serial.write(1); // Init Pattern
        initNoteTrack();
      }
      break;
  
      case 7: // Init Current Track //
      {
        if (!optionsConfirm()) return;
        if (currentTrack <= 3)
        {
          Serial.write(9+currentTrack);
          Serial.write(B00101000); // Pattern Special Functions
          Serial.write(4); // Init Current Track
        }
        else if (currentTrack == 4)
        {
          initNoteTrack();
        }
      }
      break;
      
      case 8: // Init All Data //
      {
        if (!optionsConfirm()) return;
        showProcessing = 1;
        doLCDupdate++;
        updateLCD();
        initData();
        initSlaveData();
        showProcessing = 0;
      }
      break;
      
      case 9: // Save All Data //
      {
        saveDataToEEPROM();
      }
      break;      
    }
  }
  else
  {
    // ================================================================================ //
    // =========== SONG PERFORMANCE MODE ============================================== //
    // ================================================================================ //    
    //
    switch (showOptionsOption)
    {
      case 1: // Change Mode //
      {
        editZone = kZoneSongPat;
        last_settings.bMode = kModeSong;
        songEditPos = 0;
        if (seqClockRunning) seqClockStop();
      }
      break;
      
      case 2: // Change Mode //
      {
        editZone = kZonePat;
        last_settings.bMode = kModePat;
        songEditPos = 0;
        if (seqClockRunning) seqClockStop();
      }
      break;
      
      case 3: // Save All Data //
      {
        saveDataToEEPROM();
      }
      break;
    }
  }  
  //
  showOptions = showOptionsOption = 0;
  ignoreKeysForOneSecond(1);
  doLCDupdate++;
  updateNoteTrackLEDs();
}

// ======================================================================================= //
uint8_t optionsConfirm()
{
  if (showOptionsOptionConfirm == 0)
  {
    showOptionsOptionConfirm = 1;
    doLCDupdate++;
    ignoreKeysForOneSecond(1);
    return 0;
  }
  //
  showOptionsOptionConfirm = 0;
  return 1;
}
