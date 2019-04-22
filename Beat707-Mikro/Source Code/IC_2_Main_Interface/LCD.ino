/*
 * Beat707 Mikro - created by William (Guilherme) Kalfelz (c) 2016 - www.Beat707.com - www.Wusik.com - www.William-k.com
 * 
 */

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void printTrackNumber(byte track, boolean includeName)
{
  if (track < accentTrack)
  {
    printNumber2Dgts(track+1);
    spaceDisplay();
    if (includeName) namesDisplay();        
  }
  else if (track < noteTrack)
  {
    if (includeName) 
    {
      printTextStringDisplay(stringAccent);
      writeDisplay('1' + (track-accentTrack));
      spacesDisplay(4);
    }
    else
    {
      writeDisplay('A');
      writeDisplay('1' + (track-accentTrack));
      spacesDisplay(10);
    }
  }
  else
  {
    writeDisplay('N');
    writeDisplay('1' + (track-noteTrack));
    spaceDisplay();
    if (includeName) namesDisplay();
  }
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void namesDisplay()
{
  printDisplay(curTrackName);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void displayCursor()
{
  if (hideCursor) lcd.noCursor(); else lcd.cursor();
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void printOnOff(boolean value)
{
  if (value) printTextStringDisplay(stringOn); else printTextStringDisplay(stringOff);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void displayClear()
{
  lcd.clear();
  lcd.noCursor();
  homeCursor();
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void setCursorDisplay(byte xvalue)
{
  lcd.setCursor(xvalue & 0xF, (xvalue >> 4) & 0xF);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void homeCursor()
{
  setCursorDisplay(0);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void printErrorMsg(byte errorn, char* xtrmsg)
{
  displayClear();
  switch (errorn)
  {
    case 0: 
    case 1: printDisplay("SD"); break;
    default: printDisplay("W"); break;
  }
  printTextStringDisplay(stringError);
  writeDisplay('1' + errorn);
  setCursorDisplay(SECOND_LINE(0));
  if (xtrmsg != 0) 
  {
    byte p = 0;
    byte xp = 0;
    while (xtrmsg[p] != 0) { p++; }
    if (p >= 16) xp = p - 16;
    printDisplay(xtrmsg+xp);
  }
  //
  ERROR_HALT_CODE
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void writeDisplay(const byte achar)
{
  lcd.write(achar);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void spaceDisplay()
{
  writeDisplay(' ');
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void spacesDisplay(byte number)
{
  for (byte x=0; x<number; x++) { writeDisplay(' '); }
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void printDisplay(const char* string)
{
  byte p = 0;
  while (string[p] != 0) { writeDisplay(string[p]); p++; if (p >= 16) break; }
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void printTextStringDisplay(byte xposition)
{
  SDReadString(xposition);
  byte xvalue = fR();
  while (xvalue != '|') { writeDisplay(xvalue); xvalue = fR(); }
  tmpFile.close();  
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void printNumber2Dgts(byte number)
{
  writeDisplay('0'+(number/10));
  writeDisplay('0'+(number-((number/10)*10)));
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void printNumber3Dgts(byte number)
{
  if (number >= 200) { writeDisplay('2'); number -= 200; }
    else if (number >= 100) { writeDisplay('1'); number -= 100; }
    else writeDisplay('0');
  printNumber2Dgts(number);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void printNoteName(byte note)
{
  if (note >= 120) writeDisplay('^'); else writeDisplay('0' + (note/12));
  SDReadString((note%12)*2);
  writeDisplay(fR());
  writeDisplay(fR());
  tmpFile.close();
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void doRedrawDisplay(boolean reloadS = false)
{
  redrawDisplay = true;
  reloadScreen = reloadS;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void displayInit()
{
  lcd.init();
  lcd.backlight();  
}