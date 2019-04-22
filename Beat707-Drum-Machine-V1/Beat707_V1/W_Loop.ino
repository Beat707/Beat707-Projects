/*

  Created by Beat707 (c) 2011 - http://www.Beat707.com
  
  Main Loop
  
*/

// ======================================================================================= //
void loop()
{  
  #if CHECK_CPU_USAGE
    checkCPUusage();
  #endif
  
  #if BEAT707_BIG_TIME
    if (sendBeatToBigTime == 1) { sendBeatToBigTime = 0; wireSendByte(BIGTIME_ADDR, 0); }
  #endif
  
  #if EXTRA_MIDI_IN_H_2 || MANAGER_DUMP_RECV
    midiInputCheck();
  #endif

  #if !MIDI_NO_OUT_BUFFER
    midiBufferCheck();
  #endif
  
  // ======================================================================================= //
  if (doLCDupdate || (lastMillisLateLCDupdate > 0 && lastMillisLateLCDupdate < millisNI()))
  {
    // Force the LCD to be Updated Now //
    doLCDupdate = lastMillisLateLCDupdate = 0;
    if (curMode == 0) updateLCDPattern(); 
      else if (curMode == 1) updateLCDSong();
      else updateLCDFile();
  }
      
  // ======================================================================================= //
  if (curMode == 0)
  {
    // PATTERN MODE //
    LEDsPatternTick();
    shiftButtonPattern(); 
  }
  else if (curMode == 1)
  {
    // SONG MODE //
    LEDsSongTick();
    shiftButtonSong();
  }
  else if (curMode == 2)
  {
    // FILE MODE //
    stepLEDs[0] = stepLEDs[1] = stepLEDs[2] = 0;
    buttonsInputAndLEDsOutput();
    shiftButtonFile();
  }
  
  // ======================================================================================= //
  if (nextPattern != currentPattern)
  {
    // PreLoad New Pattern //
    
    if (patternChanged) savePattern(0);
    if (setupChanged) saveSetup();
    currentPattern = nextPattern;
    loadPattern(0);

    if (!midiClockRunning)
    {
      patternBufferN = !patternBufferN;
      if (curMode == 0) updateLCDPattern(); else updateLCDSong();
    }
    else
    {
      nextPatternReady = 1;
    }
  }
  
  // ======================================================================================= //
  if (curMode == 1 && songNextPosition)
  {
    songNextPosition = 0;
      
    if (patternSongNext == 0)
    {
      if (setupChanged) saveSetup();
      checkPatternLoader();
      recordEnabled = curZone = curSongPosition = 0;
      loadSongPosition();
    }
    else
    {
      curSongPosition++;
      patternSongRepeat = patternSongRepeatNext;
      patternSong = patternSongNext;
      loadSongNextPosition();
      if (patternSongNext > 1) nextPattern = patternSongNext-2;
    }
    
    updateLCDSong();
  }

  // ======================================================================================= //
  if (lateAutoSave)
  {
      lateAutoSave = 0;
      if (patternChanged) savePattern(0);
      if (setupChanged) saveSetup();
      if (songChanged) saveSongPosition();
  }

  // ======================================================================================= //
  InterfaceButtons();
  #if EXTRA_8_BUTTONS
    Extra8ButtonsInterface();
  #endif
    
  // ======================================================================================= //  
  if (doPatternLCDupdate && !holdingShift) { doPatternLCDupdate = 0; updateLCDPatNumber(); } // Its in the W_LCD_Patt File

  // ======================================================================================= //    
  Hack_and_Mods_Loop();
}
