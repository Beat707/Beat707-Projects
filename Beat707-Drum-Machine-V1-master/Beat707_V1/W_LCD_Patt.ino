/*

  Created by Beat707 (c) 2011 - http://www.Beat707.com
  
  Pattern LCD Display
  
*/

// ======================================================================================= //
void updateLCDPattern()
{ 
  // ======================================================================================= //
  if (holdingShift && !recordEnabled && showOnlyOnce)
  {
    showOnlyOnce = 0;
    lcd.clear();
    lcdPrint(SHIFT_MODE);
    lcd.setCursor(0,1);
    printCursor();
    switch (shiftMode)
    {
      case 0: lcdPrint(TRACK_SELECTION);   break;
      case 1: lcdPrint(MUTE_TRACKS);       break;
      case 2: lcdPrint(SOLO_TRACKS);       break;
      case 3: lcdPrint(INIT_PATTERN);      break;
      case 4: lcdPrint(COPY_PATTERN);      break;
      case 5: lcdPrint(COPY_PTR_ACCENTS);  break;
      case 6: lcdPrint(PASTE_PATTERN);     break;
      case 7: lcdPrint(PST_MERGEPATTERN);  break;
    }
  }
  else
  {
    lcd.setCursor(0,0); 
    if (midiClockRunning && (nextPattern != currentPattern || nextPatternReady)) lcdPrint(NEXT); else lcdPrint(PATT);
    if (curZone == 0) printCursor(); else lcdPrintEmptyChars(1);
    updateLCDPatNumber();
    lcdPrintEmptyChars(1);
    lcdPrint(BPM);
    if (curZone == 1) printCursor(); else lcdPrintEmptyChars(1);
    if (midiClockType != 1) lcdPrintNumber3Dgts(midiClockBPM); else lcdPrint(SLV);

    lcd.setCursor(0,1);
    if (curZone == 6)
    {
      lcdPrint(SPEED_MODE);
      printCursor();
      lcdPrintEmptyChars(1);
      lcdPrint(N1_SLASH);
      if (timeScale == 2) lcdPrint(N16); else lcdPrint(N32);
    }
    else if (curZone == 7)
    {
      lcdPrint(SYNC_MODE);
      printCursor();
      if (midiClockType == 0) lcdPrint(NONE);
        else if (midiClockType == 1) lcdPrint(SLAVE);
        else if (midiClockType == 2) lcdPrint(MASTER);
      lcdPrintEmptyChars(2);
    }
    else if (curZone == 8)
    {
      lcdPrint(AUTOSTEPSEDT);
      printCursor();
      if (autoSteps) lcdPrint(ON_); else lcdPrint(OFF_);
      lcdPrintEmptyChars(1);
    }
    else if (curZone == 9)
    {
      lcdPrint(MIRROREDMODE);
      printCursor();
      if (mirrorPatternEdit) lcdPrint(ON_); else lcdPrint(OFF_);
      lcdPrintEmptyChars(1);
    }
    else if (curZone == 10)
    {
      lcdPrint(CLOCKSHUFFLE);
      printCursor();
      lcdPrintNumber(midiClockShuffle);
      lcdPrintEmptyChars(1);
    }
    else if (curZone == 11)
    {
      lcdPrint(N_OF_STEPS);
      printCursor();
      lcdPrintNumber(numberOfSteps);
      lcdPrintEmptyChars(1);
    }
    else if (curZone == 12)
    {
      lcdPrint(PATTERN_AB);
      printCursor();
      if (enableABpattern) lcdPrint(ON_); else lcdPrint(OFF_);
      lcdPrintEmptyChars(3);
    }
    else if (curZone == 13)
    {
      lcdPrint(USB_MODE);
      printCursor();
      if (midiUSBmode == 1) lcdPrint(ON_); else lcdPrint(OFF_);
      lcdPrintEmptyChars(5);
    }
    else if (curZone == 14)
    {
      lcdPrint(SEQDIRECTION);
      printCursor();
      if (midiClockDirection == 1) lcdPrint(REV); 
        else if (midiClockDirection == 2) lcdPrint(RND); 
        else lcdPrint(NRM);
    }
    #if ANALOG_INPUT_A0
    else if (curZone == 15)
    {
      lcdPrintString("AnalogIn");
      printCursor();
      switch (analogInputMode)
      {
        case 0: lcdPrint(BPM); break;
        case 1: lcdPrint(PATTERN); break;
        case 2: lcdPrintString("Steps #"); break;
        case 3: lcdPrintString("Track"); break;
        case 4: lcdPrintString("Note #"); break;
      }
      lcdPrintEmptyChars(4);
    }
    #endif
    else if (curZone == LAST_PATT_ZONE)
    {
      lcdNextMode();
    }
    else if (curZone == 4 || curZone == 5)
    {
      lcd.write('T');
      if (curZone == 4) printCursor(); else lcdPrintEmptyChars(1);
      if (currentTrack == DRUMTRACKS) lcdPrint(S1);
        else if (currentTrack == (DRUMTRACKS+1)) lcdPrint(S2);
        else if (currentTrack == (DRUMTRACKS+2)) lcdPrint(A1);
        else if (currentTrack == (DRUMTRACKS+3)) lcdPrint(A2);
        else lcdPrintNumber(currentTrack+1);
      if (curZone == 5) printCursor(); else lcdPrintEmptyChars(1);
      #if GATE_OUTS
        if (currentTrack <= 2) lcdPrintString("Gate Pin");
          else lcdPrint(MIDICHAN);
      #else
        lcdPrint(MIDICHAN);
      #endif
      lcdPrintEmptyChars(1);
      if (currentTrack >= DRUMTRACKS+2)
      {
        lcdPrintEmptyChars(2);
      }
      else
      {
        #if GATE_OUTS
          if (currentTrack == 0) lcdPrintString("A0");
            else if (currentTrack == 1) lcdPrintString("D2");
            else if (currentTrack == 2) lcdPrintString("D3");
            else lcdPrintNumber(dmChannel[currentTrack]+1);
        #else
          lcdPrintNumber(dmChannel[currentTrack]+1);
        #endif
      }
    }
    else
    {
      lcd.write('T');
      if (curZone == 2) printCursor(); else lcdPrintEmptyChars(1);
      if (currentTrack == DRUMTRACKS) lcdPrint(S1);
        else if (currentTrack == (DRUMTRACKS+1)) lcdPrint(S2);
        else if (currentTrack == (DRUMTRACKS+2)) lcdPrint(A1);
        else if (currentTrack == (DRUMTRACKS+3)) lcdPrint(A2);
        else lcdPrintNumber(currentTrack+1);
      if (currentTrack < DRUMTRACKS) 
      {
        if (curZone == 3) printCursor(); else lcdPrintEmptyChars(1);
        lcdPrintNumber3Dgts((dmNotes[currentTrack]));
      }
      else if (currentTrack < (DRUMTRACKS+2))
      {
        lcdPrintEmptyChars(1);
        #define SynthNote dmSynthTrack[currentTrack-DRUMTRACKS][patternBufferN][ (dmSynthTrackStepPos[0]*32)+dmSynthTrackStepPos[1]+(16*editDoubleSteps) ]
        lcdPrintNumber((dmSynthTrackStepPos[0]*16)+dmSynthTrackStepPos[1]+1);
        
        if (curZone == 3) printCursor(); else lcdPrintEmptyChars(1);
        
        if (SynthNote == 0) lcdPrint(NONE);
        else if (SynthNote == 1) lcdPrint(NOTE_OFF);
        else
        {
          if (SynthNote > 128) 
          {
            lcdPrintNumber(((SynthNote-128)/12));
            printNoteName();
            lcdPrintEmptyChars(1);
            lcdPrint(SLD);
          }
          else
          {
            lcdPrintNumber((SynthNote-1)/12); 
            printNoteName();
          }
        }
      }

      if (currentTrack == (DRUMTRACKS+2)) { lcdPrintEmptyChars(1); lcdPrint(ACCENT); lcdPrintEmptyChars(1); lcd.write('1'); }
      else if (currentTrack == (DRUMTRACKS+3)) { lcdPrintEmptyChars(1); lcdPrint(ACCENT); lcdPrintEmptyChars(1); lcd.write('2'); }
      else if (currentTrack < DRUMTRACKS && dmNotes[currentTrack] >= 35 && dmNotes[currentTrack] <= 70) { lcdPrintEmptyChars(1); lcdPrint(dmNotes[currentTrack]-35+4); }
      lcdPrintEmptyChars(8);
    }
  }
}

// ======================================================================================= //
void updateLCDPatNumber()
{
  if (curMode == 0)
  {
    lcd.setCursor(5,0);
    lcdPrintNumber(nextPattern+1);

    if (midiClockRunning && (nextPattern != currentPattern || nextPatternReady)) { lcd.write(3+(editDoubleSteps*2)); }
      else { if (midiClockRunning) { if ((autoSteps && !stepsPos) || (!autoSteps && !editStepsPos)) lcd.write(3+(editDoubleSteps*2)); else lcd.write(4+(editDoubleSteps*2)); } else { if (editStepsPos == 0) lcd.write(3+(editDoubleSteps*2)); else lcd.write(4+(editDoubleSteps*2)); } }
  }
  else 
  {
    if (midiClockRunning)
    {
      lcd.setCursor(10,1);
      if (stepsPos) lcd.write(4); else lcd.write(3);
      lcd.setCursor(14,1);
      lcdPrintNumber(patternSongRepeatCounter);      
    }
  }
}

// ======================================================================================= //
void printNoteName()
{  
  char* p = (char*)pgm_read_word(&(stringlist[NOTENAMESLIST])) + ((SynthNote-1-((SynthNote > 128) ? 127 : 0)) % 12)*2;
  uint8_t c = pgm_read_byte(p);
  lcd.write(c);
  p++;
  c = pgm_read_byte(p);
  lcd.write(c);
}
