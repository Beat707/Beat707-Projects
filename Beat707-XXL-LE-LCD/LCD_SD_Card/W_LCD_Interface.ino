/*

  Created by Guilherme (William) Kalfelz
  @ Beat707 (c) 2014 - http://www.Beat707.com

*/

// ======================================================================================= //
void updateLCD()
{
  if (doLCDupdate == 0) return;
  doLCDupdate--;
  lcd.noCursor();
  //
  if (showProcessing)
  {
    lcd.clear();
    if (showProcessingMessage != 0)
    {
      lcdPrint(showProcessingMessage);
      lcd.setCursor(0,1);
    }
    lcdPrint(PROCESSING);
    return;
  }
  else if (showOptions)
  {     
    lcd.clear();
    lcd.write(127);
    lcdPrint(CANCEL);
    if (showOptionsOption > 0)
    {
     lcd.setCursor(8,0);          
     lcdPrint(CONFIRM);
     lcd.write(126);
    }
    lcd.setCursor(0,1);
    if (showOptionsOptionConfirm == 1)
    {
      lcdPrint(ARE_YOU_SURE);
    }
    else if (last_settings.bMode == kModeSong)
    {
      switch(showOptionsOption)
      {
        case 0: lcdPrint(OPTIONS_MESSAGE); break;
        case 1: lcdPrint(TO_PATTERN_MODE); break;
        case 2: lcdPrint(TO_SONG_PERFORM); break;
        case 3: lcdPrint(INSERT_POSITION); break;
        case 4: lcdPrint(REMOVE_POSITION); break;
        case 5: lcdPrint(INIT_SONG_DATA); break;
        case 6: lcdPrint(SAVE_ALL_DATA); break;
      }      
    }
    else if (last_settings.bMode == kModePat)
    {
      switch(showOptionsOption)
      {
        case 0: lcdPrint(OPTIONS_MESSAGE); break;
        case 1: lcdPrint(TO_SONG_MODE); break;
        case 2: lcdPrint(TO_SONG_PERFORM); break;
        case 3: lcdPrint(COPY_PATTERN); break;
        case 4: lcdPrint(PASTE_PATTERN); break;
        case 5: lcdPrint(COPY_TO_NEXT_PAT); break;
        case 6: lcdPrint(INIT_PATTERN); break;
        case 7: lcdPrint(INIT_CUR_TRACK); break;
        case 8: lcdPrint(INIT_ALL_DATA); break;
        case 9: lcdPrint(SAVE_ALL_DATA); break;
      }
    }
    else
    {
      switch(showOptionsOption)
      {
        case 0: lcdPrint(OPTIONS_MESSAGE); break;
        case 1: lcdPrint(TO_SONG_MODE); break;
        case 2: lcdPrint(TO_PATTERN_MODE); break;
        case 3: lcdPrint(SAVE_ALL_DATA); break;
      }      
    }
    return;
  }
  else if (last_settings.bMode == kModeSongPerf)
  {
    // ================================================================================ //
    // =========== SONG PERFORMANCE MODE ============================================== //
    // ================================================================================ //
    //
    updateNoteTrackLEDs();
    if (editZone <= kZoneSPend)
    {    
      if (last_settings.perfDataMode == 1 && seqClockRunning == 1)
      {
        lcd.setCursor(0,0);
        lcdPrintNumber(songPerfPos+1);
        lcdSpace();
        //
        int xC = 13;
        uint8_t line = 0;
        for (int x=0; x<32; x++)
        {
          if (songPerfPosNext[x] == -1) break;
          else
          {
            if ((int(xC)-3) < 0)
            {
              line++;
              if (line > 1)
              {
                lcd.setCursor(14,1);
                lcd.write('>');
                break; 
              }
              else if (xC > 0) lcdSpaces(xC);
              lcd.setCursor(0,1);
              xC = 16;
            }
            xC -= 3;
            lcdPrintNumber(songPerfPosNext[x]+1);
            lcdSpace();
          }
        }
        lcdSpaces(xC);
        if (line == 0)
        {
          lcd.setCursor(0,1);
          lcdSpaces(16);
        }
      }
      else // ------------------------ //
      {
        // 1234567890123456
        // Delay  01   Rp--
        // Inst.  01   Rp**
        // Start 01  End 01
        //
        lcd.setCursor(0,0);
        switch (last_settings.perfDataMode)
        {
          case 0: lcdPrint(NORMAL_); break;
          case 1: lcdPrint(STACK__); break;
        }
        lcdPrintNumber(songPerfPos+1);
        lcdSpaces(3);
        lcdPrint(RP_);
        lcd.setCursor(14,0);
        if (TSPF(2) == 0) lcdPrint(__);
        else if (TSPF(2) == 1) lcdPrint(_AA_);
        else lcdPrintNumber(TSPF(2)-1);
        //
        lcd.setCursor(0,1);
        lcdPrint(START_);
        lcdPrintNumber(TSPF(0)+1);
        lcdSpaces(2);
        lcdPrint(END_);
        lcdPrintNumber(TSPF(1)+1);
      }
    }
    if (seqClockRunning == 1) updateBeatLCD();
  }
  else if (last_settings.bMode == kModeSong)
  {
    // ================================================================================ //
    // =========== SONG MODE ========================================================== //
    // ================================================================================ //
    //
    updateNoteTrackLEDs();
    if (editZone <= kZoneSongSp)
    {    
      // 1234567890123456
      // 01 Patt 01 Rp 01
      // ABCD oxoxo Sp --
      //
      // 01 Patt 01 b R01
      // ABCD oxoxo Sp --
      //
      lcd.setCursor(0,0);
      lcdPrintNumber(songEditPos+1);
      lcdSpace();
      lcdPrint(PATT_);
      if (TSNG(0) == 0)
      {
        lcdPrint(__);
        lcdSpaces(6);
        lcd.setCursor(0,1);
        lcdSpaces(16);
      }
      else
      {
        lcdPrintNumber(TSNG(0));
        lcdSpace();
        if (seqClockRunning)
        {
          lcdSpaces(2);
          lcd.write('R');
        }
        else
        {
          lcdPrint(RP_);
        }
        lcdPrintNumber(TSNG(1) & B00011111);
        //
        lcd.setCursor(0,1);
        switch ((TSNG(1) >> 5))
        {
          case 0: lcdPrint(A____); break;
          case 1: lcdPrint(AB___); break;
          case 2: lcdPrint(ABC__); break;
          case 3: lcdPrint(ABCD_); break;
          case 4: lcdPrint(__CD_); break;
          case 5: lcdPrint(___D_); break;
        }
        for (uint8_t xu=0; xu<5; xu++)
        {
          if (bitRead(TSNG(2), xu)) lcd.write(161); else lcd.write('.');
        }
        lcdSpace();
        lcdPrint(SP_);
        switch ((TSNG(2) >> 5))
        {
          case 1: lcdPrint(FI); break;
          case 2: lcdPrint(FO); break;
          default: lcdPrint(__); break;
        }
      }
      if (seqClockRunning == 1) updateBeatLCD();
    }
    else if (editZone <= kZoneSongLoop)
    {    
      // 1234567890123456
      // BPM 120
      // Loop Song    Off
      //
      lcd.clear();
      lcdPrint(BPM_);
      lcdPrintNumber3Dgts(last_settings.seq_bpm);
      if (!seqClockRunning)
      {
        lcd.setCursor(0,1);
        lcdPrint(LOOP_SONG);
        lcd.setCursor(13,1);
        if (last_settings.loopSong) lcdPrint(_ON_); else lcdPrint(OFF);
      }
    }    
  }
  else if (last_settings.bMode == kModePat)
  {
    // ================================================================================ //
    // =========== PATTERN MODE ======================================================= //
    // ================================================================================ //
    //
    if (editZone <= kZoneTrack)
    {
      // 1234567890123456
      // BPM 120 Stps ***
      // Patt 01 T1 1 ***
      //
      lcd.setCursor(0,0);
      lcdPrint(BPM_);
      lcdPrintNumber3Dgts(last_settings.seq_bpm);
      lcdSpace();
      lcdPrint(STPS_);
      lcd.setCursor(0,1);
      lcdPrint(PATT_);
      lcdPrintNumber(nextPattern+1);
      //
      lcdSpace();
      if (currentTrack == 5) lcdPrint(ON);
      else if (currentTrack == 6) lcdPrint(PT);
      else
      {
        lcd.write('T');
        if (currentTrack == 4) lcd.write('N'); else lcd.write('1'+currentTrack);
      }
      lcdSpaces(3);
      //
      updateStepsLCD();
      if (seqClockRunning == 1) updateBeatLCD();
      //
      if (nextPattern != currentPattern)
      {
        lcd.setCursor(4,1);
        lcd.write('n');
      }
      //
      if (autoStepPos == 0)
      {
        lcd.setCursor(12,1);
        lcd.write('.');   
      }
      else if (seq_mirror == 1)
      {
        lcd.setCursor(12,1);
        lcd.write(161);
      }
    }
    else if (editZone >= kZoneNT_pos && editZone <= kZoneNT_type) // ------------------------------------------------------------ //
    {
      // 1234567890123456
      // Pos 00  Note 000
      // Type Note OnOff
      // Type M.CC 001 (to 032)   
      //
      lcd.setCursor(0,0);
      lcdPrint(POS_);
      lcdPrintNumber(editNTrackPos+1);
      lcdSpaces(2);
      uint8_t tN = TNOTE(0);
      uint8_t tN1 = TNOTE(1);
      if (tN == 0 ) 
      {
        lcdSpaces(5);
        lcdPrint(___);
      }
      else if (tN >= 1 && tN <= 4)
      {
        lcdPrint(NOTE_);
        printNoteName(tN1);
        if ((tN1/12) > 9) lcd.write('o'); else lcd.write('0'+(tN1/12));
      }
      else
      {
        lcdPrint(VAL__);
        lcdPrintNumber3Dgts(tN1);
      }
      lcdClearnLine(1);
      lcdPrint(TYPE_);
      //
      if (tN <= 4)
      {
        switch(tN)
        {
          // Max of 11 chars //
          case 0: lcdPrint(EMPTY); break;
          case 1: lcdPrint(NOTE_); lcdPrint(GATE); break;
          case 2: lcdPrint(NOTE_); lcdPrint(SLIDE); break;
          case 3: lcdPrint(NOTE_); lcdPrint(FLAM); break;
          case 4: lcdPrint(NOTE_); lcdPrint(OFF); break;
        }
      }
      else
      {
        lcdPrint(M_CC_);
        lcdPrintNumber3Dgts(tN-5);
      }
    }
    else if (editZone >= kZoneNT_transpose1 && editZone <= kZoneNT_transpose4) // ------------------------------------------------------------ //
    {
      // 1234567890123456
      // Transpose  Notes 
      // -99 -99  -99 -99
      //
      lcd.setCursor(0,0);
      lcdPrint(TRANSPOSE__NOTES);
      lcd.setCursor(0,1);
      lcdPrintNumberPN(last_settings.patternsTranspose[currentPattern][0]);
      lcdSpace();
      lcdPrintNumberPN(last_settings.patternsTranspose[currentPattern][1]);
      lcdSpace(); lcdSpace();
      lcdPrintNumberPN(last_settings.patternsTranspose[currentPattern][2]);
      lcdSpace();
      lcdPrintNumberPN(last_settings.patternsTranspose[currentPattern][3]);
    }    
  }
  //
  updateCursor();
}

// ======================================================================================= //
void updateCursor()
{
  if (editingOn == 0) return;
  //
  switch(editZone)
  {
    case kZoneBPM: setLCDCursor(6,0); break;
    case kZoneSteps: setLCDCursor(12,0); break;     
    case kZonePat: setLCDCursor(6,1); break;
    case kZoneTrack: setLCDCursor(9,1); break;
    case kZoneNT_pos: setLCDCursor(5,0); break;
    case kZoneNT_value: setLCDCursor(15,0); break;
    case kZoneNT_type: setLCDCursor(5,1); break;
    case kZoneNT_transpose1: setLCDCursor(2,1); break;
    case kZoneNT_transpose2: setLCDCursor(6,1); break;
    case kZoneNT_transpose3: setLCDCursor(11,1); break;
    case kZoneNT_transpose4: setLCDCursor(15,1); break;
    //
    case kZoneSongPos: setLCDCursor(1,0); break;
    case kZoneSongPat: setLCDCursor(9,0); break;
    case kZoneSongRpt: setLCDCursor(15,0); break;
    case kZoneSongSteps: setLCDCursor(0,1); break;
    case kZoneSongTrack1:
    case kZoneSongTrack2:
    case kZoneSongTrack3:
    case kZoneSongTrack4:
    case kZoneSongTrack5: setLCDCursor(5+(editZone-kZoneSongTrack1),1); break;
    case kZoneSongSp: setLCDCursor(15,1); break;
    //
    case kZoneSongBPM: setLCDCursor(6,0); break;
    case kZoneSongLoop: setLCDCursor(13,1); break;
    //
    case kZoneSPsection: setLCDCursor(8,0); break;
    case kZoneSPstart: setLCDCursor(7,1); break;
    case kZoneSPend: setLCDCursor(15,1); break;
    case kZoneSPrpt: setLCDCursor(15,0); break;
    case kZoneSPmode: setLCDCursor(0,0); break;
  }
}

// ======================================================================================= //
void updateStepsLCD()
{
  xPos = 13;
  uint8_t showSeqSteps = editStepsPos;
  if (autoStepPos == 1) showSeqSteps = stepsPos;
  if (showSeqSteps == 0) customA();
  else if (showSeqSteps == 1) customB();
  else if (showSeqSteps == 2) customC();
  else customD();
}

// ======================================================================================= //
void updateBeatLCD()
{
  lcd.noCursor();
  if (last_settings.bMode == kModeSong) lcd.setCursor(11,0); 
  else if (last_settings.bMode == kModeSongPerf && last_settings.perfDataMode == 1) lcd.setCursor(15,1);
  else if (last_settings.bMode == kModeSongPerf) lcd.setCursor(10,0);
    else lcd.setCursor(11,1);
  lcd.write('1'+seq_beat);
  updateCursor();
}
