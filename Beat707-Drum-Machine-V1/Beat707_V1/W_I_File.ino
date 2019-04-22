/*

  Created by Beat707 (c) 2011 - http://www.Beat707.com
  
  File User Interface - All functions related to storing can be found under W_Storage

*/

// ======================================================================================= //
void InterfaceTickFile()
{
  switch (multiButton)
  {    
    // ------------------------------- LEFT ------------------------------- //
    case 2:
      if (curZone == 10)
      {
        fileSongNameEdit--;
        if (fileSongNameEdit == 255)
        {
          fileSongNameEdit = 0;
          curZone = 2;
          lcd.noCursor();
        }
      }
      else
      {
        curZone--;
        if (curZone == 255) curZone = 5;
      }
      updateLCDFile();
      break;
      
    // ------------------------------- RIGHT ------------------------------ //
    case 5: 
      if (curZone == 10)
      {
        fileSongNameEdit++;
        if (fileSongNameEdit > 13) 
        {
          fileSongNameEdit = 0;
          curZone = 2;
          lcd.noCursor();
        }
      }
      else
      {
        curZone++;
        if (curZone > 5) curZone = 0; 
      }
      updateLCDFile();
      break;
    
    // -------------------------------- UP -------------------------------- //
    case 4:
      if (curZone == 0)
      {
        fileSelected++;
        if (fileSelected == MAXSONGSFILE) fileSelected = MAXSONGSFILE-1;
        loadSongName();
      }
      else if (curZone == 1)
      {
        fileMode++;
        if (fileMode > 2) fileMode = 0;
      }
      else if (curZone == 3)
      {
        sysMIDI_ID++;
        if (sysMIDI_ID > 98) sysMIDI_ID = 98;
      }
      else if (curZone == 5)
      {
        nextMode++;
        if (nextMode > 2) nextMode = 0;
      }
      else if (curZone == 10)
      {
        fileSongName[fileSongNameEdit]++;
        if (fileSongName[fileSongNameEdit] == 91) fileSongName[fileSongNameEdit] = 90;
        if (fileSongName[fileSongNameEdit] == 91+32) fileSongName[fileSongNameEdit] = 90+32;
      }
      else if (curZone == 2)
      { 
        curZone = 10; 
        lcd.cursor(); 
        updateLCDFile(); 
      }
      updateLCDFile();
      break;
      
    // ------------------------------- DOWN ------------------------------- //
    case 3:
      if (curZone == 0) 
      {
        fileSelected--;
        if (fileSelected == 255) fileSelected = 0;
        loadSongName();
      }
      else if (curZone == 1)
      {
        fileMode--;
        if (fileMode == 255) fileMode = 2;
      }
      else if (curZone == 3)
      {
        sysMIDI_ID--;
        if (sysMIDI_ID == 255) sysMIDI_ID = 0;
      }      
      else if (curZone == 5)
      {
        nextMode--;
        if (nextMode == 255) nextMode = 2;
      }
      else if (curZone == 10)
      {
        fileSongName[fileSongNameEdit]--;
        if (fileSongName[fileSongNameEdit] < 32) fileSongName[fileSongNameEdit] = 32;
        if (fileSongName[fileSongNameEdit] == 64) fileSongName[fileSongNameEdit] = 65;
        if (fileSongName[fileSongNameEdit] == 64+32) fileSongName[fileSongNameEdit] = 65+32;
      }
      else if (curZone == 2)
      {
        curZone = 10;
        lcd.cursor();
        updateLCDFile();
      }
      updateLCDFile();
      break;
      
    // ------------------------------- CYCLE MODES ------------------------ //
    case 9:
      nextMode = 0; loadNextMode(); delayNI(250);
      break;      
  }
}

// ======================================================================================= //

void shiftButtonFile()
{
  if (!shiftClick)
  {
    if (holdingShift)
    {
      holdingShift = holdingShiftUsed = 0;
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
        if (curZone == 10) fileSongName[fileSongNameEdit] = 32; // Add a Space //
          else shiftAction();
        showOnlyOnce = 1;
        updateLCDFile();
      }
      else
      {
        // Shift was pressed very quickly //
        holdingShift = holdingShiftUsed = 0;
        shiftAction();
      }
    }
  }
}

// ======================================================================================= //

void shiftAction()
{
  if (curZone == 5) loadNextMode();
  else if (curZone == 4) { storageInit(true); curZone = curMode = nextMode = 0; doLCDupdate = 1; }
  else if (curZone == 2) { curZone = 10; lcd.cursor(); updateLCDFile(); }
  else if (curZone == 10)
  { 
    // UpperCase/LowerCase //
    if (fileSongName[fileSongNameEdit] < 65) fileSongName[fileSongNameEdit] = 65;
      else if (fileSongName[fileSongNameEdit] <= 90) fileSongName[fileSongNameEdit] += 32; else fileSongName[fileSongNameEdit] -= 32;
    updateLCDFile(); 
  }
  else
  {
    if (fileMode != 1 && !checkSong())
    {
      lcd.clear();
      lcd.setCursor(1,0);
      lcdPrint(E_EMPTY_SONG_E);
      delayNI(700);
      updateLCDFile();
      return;
    }
    
    if (hitShiftToConfirm())
    {
      lcd.clear();
      lcd.setCursor(2,0);
      lcdPrint(PROCESSING);
      
      if (fileMode == 0) songLoad();
        else if (fileMode == 1) songSave();
        else if (fileMode == 2) songErase();
      
      lcdOK();
      loadSongName();
      updateLCDFile();
    }
  }
}
