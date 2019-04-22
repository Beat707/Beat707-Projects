/*
 * Beat707 Mikro - created by William (Guilherme) Kalfelz (c) 2016 - www.Beat707.com - www.Wusik.com - www.William-k.com
 * 
 */

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void updateVariable(byte& xvariable, byte xmax, byte xmin, byte shiftDownMultiplier)
{
  if (isShiftDown()) changeVar *= shiftDownMultiplier;
  updateVariable(xvariable, xmax, xmin);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void updateVariable(byte& xvariable, byte xmax, byte xmin)
{
  if ((xvariable+changeVar) <= xmax && (xvariable+changeVar) >= xmin) xvariable += changeVar;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void updateVariable(byte& xvariable, byte xmax)
{
  updateVariable(xvariable, xmax, 0);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void checkPlayStopMenu()
{
  if (isButtonNoShift(buttonStop)) 
  { 
    slavePlay(0); 
    if (streamPatternGo) finishPatternStream();
    return;
  }
  if (streamPatternGo) return;
  if (isButtonNoShift(buttonPlay)) { slavePlay(1); return; }
  //
  if (isButtonShift(buttonMenu)) 
  { 
    checkSavePattern();
    checkSaveSong();
    return; 
  }
}
