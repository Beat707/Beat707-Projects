/*

  Created by Beat707 (c) 2011 - http://www.Beat707.com
  
  Song User Interface
  
*/

// ======================================================================================= //
void InterfaceTickSong()
{
  switch (multiButton)
  {
    // ------------------------------- STOP ------------------------------- //
    case 0:
      if (songChanged) saveSongPosition();
      if (!midiClockRunning) curSongPosition = 0;
      if (midiClockRunning) MidiClockStop();
      if (setupChanged) saveSetup();
      checkPatternLoader();
      recordEnabled = 0;
      curZone = 0;
      loadSongPosition();
      updateLCDSong();
      break;
      
    // ------------------------------- PLAY ------------------------------- //
    case 1:
      startSong();
      break;
    
    // ------------------------------ RECORD ------------------------------ //  
    case 6:
      if (setupChanged) saveSetup();
      if (songChanged) saveSongPosition();
      curMode = nextMode = 0;
      songPattEdit = 1;
      recordEnabled = editDoubleSteps = shiftClick = stepLEDs[0] = stepLEDs[1] = stepLEDs[2] = dmMutes = curZone = 0;
      if (nextPattern != (patternSong-2) || currentPattern != (patternSong-2))
      {
        currentPattern = nextPattern = patternSong-2;
        loadPattern(0);
        patternBufferN = !patternBufferN;
        nextPatternReady = 0;
      }
      doLCDupdate = 1;
      break;    
      
    // ------------------------------- LEFT ------------------------------- //
    case 2:
      if (holdingShift) previousSongPosition();
      else
      {
        curZone--;
        if (curZone == 255) curZone = 4;
        if (midiClockRunning && curZone != 0 && curZone != 1) curZone = 1;
      }
      updateLCDSong();
      break;
      
    // ------------------------------- RIGHT ------------------------------ //
    case 5:
      if (holdingShift) nextSongPosition();
      else
      {       
        curZone++;
        if (curZone > 4) curZone = 0;
        if (midiClockRunning && curZone != 0 && curZone != 1) curZone = 0;
      }
      updateLCDSong();
      break;
    
    // -------------------------------- UP -------------------------------- //
    case 4:
      if (curZone == 1) // BPM Editor //
      {
        if (midiClockBPM != 255) midiClockBPM++;
        if (midiClockRunning) MidiClockNewTime();
        setupChanged = 1;
      }
      else if (curZone == 0)  // Song Position //
      {
        nextSongPosition();
        if (midiClockRunning) startSong();
      }
      else if (curZone == 3 || holdingShift) 
      {
        repeatPattern(1); 
      }
      else if (curZone == 2)  // Pattern //
      {
        patternSong++;
        if (patternSong >= (MAXSPATTERNS+1)) patternSong  = MAXSPATTERNS+1;
        songChanged = 1;
      }
      else if (curZone == 4)
      {
        nextMode++;
        if (nextMode > 2) nextMode = 0;
      }      
      updateLCDSong();
      break;
      
    // ------------------------------- DOWN ------------------------------- //
    case 3:
      if (curZone == 1) // BPM Editor //
      {
        midiClockBPM--;
        if (midiClockBPM < 25) midiClockBPM = 25;
        if (midiClockRunning) MidiClockNewTime();
        setupChanged = 1;
      }
      else if (curZone == 0)  // Song Position //
      {
        previousSongPosition();
        if (midiClockRunning) startSong();
      }
      else if (curZone == 3 || holdingShift) 
      {
        repeatPattern(-1); 
      }      
      else if (curZone == 2)  // Pattern //
      {
        patternSong--;
        if (patternSong == 255) patternSong  = 0;
        songChanged = 1;
      }    
      else if (curZone == 4)
      {
        nextMode--;
        if (nextMode == 255) nextMode = 2;
      }            
      
      updateLCDSong();
      break;
      
    // ------------------------------- CYCLE MODES ------------------------ //
    case 9:
      nextMode = 2; loadNextMode(); delayNI(250);
      break;      
  }
}

// ======================================================================================= //
void shiftButtonSong()
{
  if (!shiftClick)
  {
    if (holdingShift)
    {
      holdingShift = 0;
      holdingShiftUsed = 0;
    }
  }
  else
  {
    if (!holdingShift)
    {
      holdingShift = 1;
      delayNI(150);
      buttonsInputAndLEDsOutput();
      if (shiftClick)
      {
        showOnlyOnce = 1;
        updateLCDSong();
      }
      else
      {
        // Shift was pressed very quickly //
        holdingShift = 0;
        holdingShiftUsed = 0;
        if (curZone == 4) loadNextMode();
          else recordShowCurPos = !recordShowCurPos;        
      }
    }
  }
}

// ======================================================================================= //

void LEDsSongTick()
{
  // Update LEDS //
  stepLEDs[0] = stepLEDs[1] = stepLEDs[2] = 0;
  dbStepsCalc();

  if (midiClockRunning) showLEDsRecOutput(); // Its in the W_I_Patt File
  
  // ----------------- BUTTON IN & LEDS OUT ----------------------- //
  
  buttonsInputAndLEDsOutput();
}

// ======================================================================================= //
// ======================================================================================= //
// ======================================================================================= //
// ======================================================================================= //


void nextSongPosition()
{
    if (patternSong > 1)
    {
      if (songChanged) saveSongPosition();
      curSongPosition++;
      if (curSongPosition >= MAXSONGPOS) curSongPosition = MAXSONGPOS-1;
      loadSongPosition();
    }
}  

void previousSongPosition()
{
    if (songChanged) saveSongPosition();
    curSongPosition--;
    if (curSongPosition == 255) curSongPosition = 0;
    loadSongPosition();
}

void repeatPattern(char add)
{
  patternSongRepeat += add;
  if (patternSongRepeat == 255) patternSongRepeat = 0;
  if (patternSongRepeat > 99) patternSongRepeat = 99;
  songChanged = 1;
}

// ======================================================================================= //
void startSong()
{
  if (songChanged) saveSongPosition();
  if (midiClockRunning) MidiClockStop();
  if (patternSong == 1) { curSongPosition = 0; loadSongPosition(); }
  if (patternSong != 0)
  {
    currentPattern = nextPattern = patternSong-2;
    loadPattern(0);
    patternBufferN = !patternBufferN;
    loadSongNextPosition();
    if (patternSongNext > 1) nextPattern = patternSongNext-2;
    patternSongRepeatCounter = 0;
    MidiClockStart(true, true);
    curZone = 0;
  }
  updateLCDSong();
}
