/*

  Created by Beat707 (c) 2011 - http://www.Beat707.com
  
  Song LCD Display
  
*/

// ======================================================================================= //
void updateLCDSong()
{ 
  if (holdingShift && !recordEnabled && showOnlyOnce)
  {
    showOnlyOnce = 0;
  }
  else
  {
    lcd.setCursor(0,0); 
    lcdPrint(POSP);
    if (curZone == 0) lcd.write(0); else lcdPrintEmptyChars(1);
    if (curSongPosition < 0) lcdPrintNumber(songLoopPos+1); else lcdPrintNumber(curSongPosition+1);
    lcdPrintEmptyChars(2);
    lcdPrint(BPM);
    if (curZone == 1) lcd.write(0); else lcdPrintEmptyChars(1);
    if (midiClockType != 1) lcdPrintNumber3Dgts(midiClockBPM); else lcdPrint(SLV);
    
    lcd.setCursor(0,1);
    if (curZone == 4)
    {
      lcdNextMode();
    }
    else
    {
      lcdPrint(PATTERN);
      if (curZone == 2) lcd.write(0); else lcdPrintEmptyChars(1);
      if (patternSong == 0) lcdPrint(END);
        else if (patternSong == 1)  lcdPrint(LOP);
        else { lcdPrintNumber(patternSong-1); lcdPrintEmptyChars(1); }
      lcdPrintEmptyChars(1);
      lcd.write('R');
      if (curZone == 3) lcd.write(0); else lcdPrintEmptyChars(1);
      lcdPrintNumber(patternSongRepeat);
    }
  }
}
