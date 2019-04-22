/*
 * Beat707 Mikro - created by William (Guilherme) Kalfelz (c) 2016 - www.Beat707.com - www.Wusik.com - www.William-k.com
 * 
 */

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void songValuesUpdate(char changeVar)
{
  doRedrawDisplay();
  //
  switch (dArea)
  {
  }
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void songInterface()
{
  //
  // ------------------------------------------ //
  if (isButtonWhatever(buttonUp)) songValuesUpdate(1);
    else if (isButtonWhatever(buttonDown)) songValuesUpdate(-1);  
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void checkSaveSong()
{
  SDSaveSongSettings();
  SDSaveChordCustom();
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void changeSong()
{
  checkSavePattern();
  checkSaveSong();
  byte prevSong = curSong;
  updateVariable(curSong, 98, 1, 10);
  //
  SDSFileOpen(fileSong, fileRead, false);
  if (!tmpFile)
  {
    curSong = prevSong;
  }
  else
  {
    tmpFile.close();
    SDSaveCurrentSong();
    loadSong();
  }
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void setSongSettingsChanged()
{
  songSettingsChanged = true;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void changeSongName()
{
  if (isShiftDown())
  {
    if (changeVar < 0)
    {
      if (songName[dZone2] == 'A') songName[dZone2] = 'a';
      else if (songName[dZone2] == 'a') songName[dZone2] = '0';
      else songName[dZone2] = ' ';
    }
    else
    {
      if (songName[dZone2] == ' ') songName[dZone2] = '0';
      else if (songName[dZone2] == '0') songName[dZone2] = 'a';
      else songName[dZone2] = 'A';
    }
  }
  else
  {
    if (changeVar < 0 && songName[dZone2] > ' ') songName[dZone2]--;
    else if (changeVar > 0 && songName[dZone2] < 'z') songName[dZone2]++;
  }
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void loadSong()
{
  SDLoadSongSettings();
  SDLoadChordCustom();  
  SDLoadEntirePattern(filePattern);
  SDReadTrackName();
  SDReadPatternName();
  curTrack = curVariation = curStep = 0;
  changeTrack(0);  
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void goToSongMode()
{
  checkSavePattern();
  checkSaveSong();
  currentMode = songMode;
  doRedrawDisplay(true);
}
