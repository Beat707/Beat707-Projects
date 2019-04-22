/*

  Created by Beat707 (c) 2011 - http://www.Beat707.com
  
  File LCD Display

*/

// ======================================================================================= //
void updateLCDFile()
{ 
  if (holdingShift && !recordEnabled && showOnlyOnce)
  {
    showOnlyOnce = 0;
    if (curZone == 10)
    {
      lcd.setCursor(1,1);
      lcdPrintString(fileSongName);
      lcd.setCursor(fileSongNameEdit+1,1);
    }
  }
  else
  {  
    lcd.setCursor(0,0); 
    lcdPrint(SONG);
    if (curZone == 0) lcd.write(0); else lcdPrintEmptyChars(1);
    lcdPrintNumber(fileSelected+1);
    lcdPrintEmptyChars(3);
    if (curZone == 1) lcd.write(0); else lcdPrintEmptyChars(1);
    if (fileMode == 0) lcdPrint(LOAD);
      else if (fileMode == 1) lcdPrint(SAVE);
      else if (fileMode == 2) lcdPrint(ERASE);
    lcdPrintEmptyChars(2);
      
    lcd.setCursor(0,1);
    if (curZone == 5)
    {
      lcdNextMode();
    }
    else if (curZone == 4)
    {
      lcd.write(0);
      lcdPrint(SYSTEM_INIT);
      lcdPrintEmptyChars(4);
    }
    else if (curZone == 3)
    {
      lcdPrint(SYSEX_MIDI_ID);
      lcd.write(0);
      lcdPrintNumber(sysMIDI_ID+1);
    }
    else
    {
      lcdPrintEmptyChars(16);
      lcd.setCursor(0,1);
      if (curZone == 2) lcd.write(0); else lcd.write('"');
      lcdPrintString(fileSongName);
      lcd.setCursor(15,1);
      if (curZone == 2) lcdPrintEmptyChars(1); else lcd.write('"');
      if (curZone == 10) lcd.setCursor(fileSongNameEdit+1,1);
    }
  }
}
