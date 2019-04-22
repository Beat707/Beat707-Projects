/*
 * Beat707 Mikro - created by William (Guilherme) Kalfelz (c) 2016 - www.Beat707.com - www.Wusik.com - www.William-k.com
 * 
 */

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void patternValuesUpdate()
{
  doRedrawDisplay();
  //
  switch (dArea)
  {
    case areaMain:
    case areaNoteTrackEdit:
      //
      if (dLine == 0)
      {
        if (dZone1 == 0) { changePattern(); }
        else if (dZone1 == 1) { updateVariable(curVariation, (VARIATIONS-1)); changeTrack(0); }
        else if (dZone1 == 2) { changeTrack(changeVar, true); }
      }
      else
      {
        if (dArea == areaMain)
        {
          if (curTrack <= (accentTrack+1))
          {
            byte xs1 = bitRead(drumSteps[0], dZone2);
            byte xs2 = bitRead(drumSteps[1], dZone2);              
            if (changeVar > 0)
            {
              if (xs1 == 0 && xs2 == 0) bitSet(drumSteps[0], dZone2);
              else if (xs1 == 1 && xs2 == 0) bitSet(drumSteps[1], dZone2);
              else if (xs1 == 1 && xs2 == 1) bitClear(drumSteps[0], dZone2);
            }
            else
            {
              if (xs1 == 0 && xs2 == 1) bitSet(drumSteps[0], dZone2);
              else if (xs1 == 1 && xs2 == 1) bitClear(drumSteps[1], dZone2);
              else if (xs1 == 1 && xs2 == 0) bitClear(drumSteps[0], dZone2);
            }
          }
          else
          {
            if (noteTrackMode[curTrack-noteTrack] == noteTrackModeNotes)
            {
              byte xcode = getTrackNoteCode(dZone2);
              byte xnote = getTrackNote(dZone2);
              if (changeVar > 0)
              {
                if (xnote == 0 && xcode == 0)
                {
                  noteSteps[dZone2] = lastVelocity;
                  noteSteps[dZone2+STEPS] = lastNote;
                }
                else if (xnote > 0 && xcode != noteDouble)
                {
                  setTrackDouble(dZone2);
                }
                else
                {
                  if (getTrackNote(dZone2) > 0) lastNote = getTrackNote(dZone2);
                  setTrackNoteOff(dZone2);
                }
              }
              else
              {
                if (xnote > 0 && xcode == noteDouble) setTrackRegularNote(dZone2);
                else if (xnote == 0 && xcode == 0) setTrackNoteOff(dZone2);
                  else noteSteps[dZone2+STEPS] = noteSteps[dZone2] = 0;
              }
            }
            else
            {
              if (changeVar < 0)
              {
                noteSteps[dZone2+STEPS] = noteSteps[dZone2] = 0;
              }
              else
              {
                noteSteps[dZone2] = midiCClast[0];
                noteSteps[dZone2+STEPS] = midiCClast[1];
              }
            }
          }
          //
          setStepsChanged();
          if (isShiftDown())
          {
            dZone2 += autoSteps;
            if (dZone2 > 15) dZone2 = 15;
          }
        }
        else // Note Track Notes List //
        {
          if (dZone2 == 0)
          {
            updateVariable(curStep, 15);
          }
          else if (noteTrackMode[curTrack-noteTrack] == noteTrackModeNotes)
          {
            if (getTrackNote(curStep) == 0 && getTrackNoteCode(curStep) == 0 && changeVar > 0)
            {
              noteSteps[curStep] = lastVelocity;
              noteSteps[curStep+STEPS] = lastNote;
              setStepsChanged();
            }            
            else if (dZone2 == 1)
            {
              updateTrackVelocity();
              setStepsChanged();
            }
            else if (dZone2 == 2)
            {
              byte xcode = getTrackNoteCode(curStep);
              if      (changeVar > 0 && xcode == noteSlide)    { if (getTrackNote(curStep) > 0) lastNote = getTrackNote(curStep); setTrackNoteOff(curStep); }
              else if (changeVar > 0 && xcode == regularNote)   { setTrackDouble(curStep); }
              else if (changeVar > 0 && xcode == noteDouble)    { setTrackDoubleIn(curStep); }
              else if (changeVar > 0 && xcode == noteDoubleIn)  { lastVelocity = getTrackVelocity(curStep); setTrackSlide(curStep); }
              //
              else if (changeVar < 0 && xcode == noteOff)       { noteSteps[curStep+STEPS] = lastNote; setTrackSlide(curStep); }
              else if (changeVar < 0 && xcode == noteSlide)    { noteSteps[curStep] = lastVelocity; setTrackDoubleIn(curStep); }
              else if (changeVar < 0 && xcode == noteDoubleIn)  { setTrackDouble(curStep); }
              else if (changeVar < 0 && xcode == noteDouble)    { setTrackRegularNote(curStep); }
              else if (changeVar < 0) { noteSteps[curStep] = noteSteps[curStep+STEPS] = 0; }
              //
              setStepsChanged();
            }
            else if (dZone2 == 3) 
            { 
              updateTrackNote(); 
              setStepsChanged();
            }
          }
          else if (noteTrackMode[curTrack-noteTrack] == noteTrackModeCC)
          {
            if (noteSteps[curStep] == 0 && midiCClast[0] > 0)
            {
                noteSteps[curStep] = midiCClast[0];
                noteSteps[curStep+STEPS] = midiCClast[1];              
            }
            else
            {
              if (dZone2 == 1) updateVariable(noteSteps[curStep], 128, 0, 10);
                else updateVariable(noteSteps[curStep+STEPS], 127, 0, 10);
              //
              midiCClast[0] = noteSteps[curStep];
              midiCClast[1] = noteSteps[curStep+STEPS];              
            }
          }
        }
      }
      break;

      // ----------------------------------------------- //

    case areaBPMnMIDI:
      //
      setSequencerSettingsChanged();
      if (dLine == 0 && dZone1 == 0)
      { 
        updateVariable(seqType, 1); 
        setSongSettingsChanged(); 
      }
      else if (dLine == 0 && dZone1 == 1)
      {
        updateVariable(seqBPM, MAX_BPM, MIN_BPM, 10); 
        setSongSettingsChanged(); 
      }
      else if (dLine == 1) 
      {
        seqSyncOut = (changeVar > 0);
        setSongSettingsChanged();
      }
      break;

      // ----------------------------------------------- //

    case areaTrackSettings:
      //
      if (dLine == 0)
      {
        if (dZone1 == 0) 
        { 
          changeTrack(changeVar, true); 
          doRedrawDisplay(true); 
        }
        else if (dZone1 == 1)
        { 
          updateVariable(trackMidiCH, (midiChannels-1));
          setTrackSettingsChanged();
        }
      }
      else
      {
        if (curTrack >= noteTrack)
        {
          if (dZone2 == 0)
          {
            updateVariable(noteTrackMode[curTrack-noteTrack], 1);
          }
          else
          {
            updateVariable(trackName[curTrack], maxTrackNoteNames);
            SDReadTrackName();
          }
          setTrackSettingsChanged();            
        }
        else
        {
          if (dZone2 == 0)
          {
            updateVariable(trackNote, 127, 1, 10);
            setTrackSettingsChanged();
          }
          else if (dZone2 == 1) 
          {
            updateVariable(trackName[curTrack], maxTrackDrumNames);
            SDReadTrackName();
            setTrackSettingsChanged();
          }
        }
      }
      break;

      // ----------------------------------------------- //

    case areaTrackProcessor:
      //
      if (dZone1 == 0) changeTrack(changeVar, true);
      else if (dZone1 == 1) 
      {
        updateVariable(trackProcessor, maxTrackProcessor);
        setTrackSettingsChanged();
      }
      break;

      // ----------------------------------------------- //

    case areaAutoStepnPatternType:
      //
      if (dLine == 0) 
      {
        updateVariable(autoSteps, STEPS);
      }
      else 
      { 
        updateVariable(curPatternNameValue, maxPatternNames);
        setTrackSettingsChanged(); 
        SDReadPatternName();
      }
      break;

      // ----------------------------------------------- //

    case areaVariations:
      //
      setSequencerSettingsChanged();
      updateVariable(totalVariations, (VARIATIONS-1));
      break;

      // ----------------------------------------------- //

    case areaSong:
      //
      if (dLine == 0) changeSong(); else { changeSongName(); setSongSettingsChanged(); }
      break;

      // ----------------------------------------------- //

    case areaMenuCopyPaste:
      //
      if (changeVar == 1) copyMode = entirePattern; else copyMode = currentTrack;
      break;

    case areaCustomChord:
      //
      if (dLine == 0) 
      {
        if (changeVar > 0 && customChordsZone1 < 11) customChordsZone1++;
        else if (changeVar < 0 && customChordsZone1 > 0) customChordsZone1--;
      }
      else
      {
        updateChordCustom();
      }
      break;

      // ----------------------------------------------- //

    case areaNewPatternCloneCur:    dArea = areaNewPatternCloneTmplt; doRedrawDisplay(true); break;
    case areaNewPatternCloneTmplt:  dArea = areaNewPatternCloneCur; doRedrawDisplay(true); break;
  }
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void patternInterface()
{
  if (dButtonsEvent[buttonMenu] == buttonEventRelease || dButtonsEvent[buttonMenu] == buttonEventClick)
  {
    dButtonsEvent[buttonMenu] = buttonEventNone;
    //
    if ((dArea >= areaMenusStart) && (dArea <= areaMenusEnd))
    {
      if (dArea == areaNewPatternCloneCur || dArea == areaNewPatternCloneTmplt)
      {
        dArea = areaMenusStart+1;
        dLine = 0;
      }
      else
      {
        // Close Menu //
        dArea = prevArea;
      }
    }
    else
    {
      // Open Menu //
      if (holdDownPressedAnything) return;
      prevArea = dArea;
      dArea = areaMenuCopyPaste;
    }
    zeroAreaPointers();
    return;
  }
  //
  if ((dArea >= areaMenusStart) && (dArea <= areaMenusEnd) &&
      (dButtonsEvent[buttonShift] == buttonEventRelease || dButtonsEvent[buttonShift] == buttonEventClick) )
  {
    dButtonsEvent[buttonShift] = buttonEventNone;
    //
    if (dArea == areaNewPatternCloneCur || dArea == areaNewPatternCloneTmplt)
    {
      if (curPattern < 99) createNewPattern();
    }
    else if (dArea == areaNewPatternGoToSong)
    {
      if (dLine == 0)
      {
        dLine = 0;
        dArea = areaNewPatternCloneCur;
        doRedrawDisplay(true);
        return;
      }
      else
      {
        goToSongMode();
        return;
      }
    }
    else if (dArea == areaMenuCopyPaste)
    {
      if (dZone1 == 0) // Copy //
      {
        if (copyMode == entirePattern) SDSavePattern(filePatternCopy, filePatternSettingsCopy); else SDSaveCurrentTrack();
      }
      else if (dZone1 == 1) // Paste //
      {
        if (copyMode == entirePattern)
        {
          SDReadScreen(areaPastingMessage, lastLineOnly);
          SDLoadEntirePattern(filePatternCopy);
        }
        else
        {
          SDReadScreen(areaPastingMessage, lastLineOnly);
          SDLoadCurrentTrack();
        }
        changeTrack(0);
        somethingChanged = true;
      }
      else if (dZone1 == 2) // Clear //
      {
        if (copyMode == entirePattern) slaveSendCode(slvClearPattern, 0, 0, 0); else slaveSendCode(slvClearTrack, curTrack, 0, 1);
        changeTrack(0);
        somethingChanged = true;
      }
    }
    //
    dArea = prevArea;
    zeroAreaPointers();
    return;
  }
  //
  if (dButtonsEvent[buttonShift] == buttonEventRelease)
  {
    dButtonsEvent[buttonShift] = buttonEventNone;
    if (!holdDownPressedAnything) flipMirrorMode();
    return;
  }
  //
  if (isMenuDown())
  {
    if (isButtonNoShift(buttonPageUp))
    {
      somethingChanged = true;
      if (dArea == areaNoteTrackEdit) { if (curStep < 15) curStep++; doRedrawDisplay(); }
        else { slaveSendCode(slvTransposeAll, 1, 0, 1); }
      changeTrack(0);
      return;
    }
    else if (isButtonNoShift(buttonPageDown))
    {
      somethingChanged = true;
      if (dArea == areaNoteTrackEdit) { if (curStep > 0) curStep--; doRedrawDisplay(); }
        else { slaveSendCode(slvTransposeAll, 0, 0, 1); }
      changeTrack(0);
      return;
    }
    else if (isButtonNoShift(buttonLeft) && (dArea == areaMain || dArea == areaNoteTrackEdit) && dLine == 1)
    {
      if (dArea == areaMain && dZone2 != 0)
      {
        slaveSendCode(slvSlideStep, 0, dZone2, 2);
        dZone2--;
      }
      else if (dArea == areaNoteTrackEdit && curStep != 0)
      {
        slaveSendCode(slvSlideStep, 0, curStep, 2);
        curStep--;
      }
      //
      somethingChanged = true;
      changeTrack(0);
      return;
    }
    else if (isButtonNoShift(buttonRight) && (dArea == areaMain || dArea == areaNoteTrackEdit) && dLine == 1)
    {
      if (dArea == areaMain && dZone2 != 15)
      {
        slaveSendCode(slvSlideStep, 1, dZone2, 2);
        dZone2++;
      }
      else if (dArea == areaNoteTrackEdit && curStep != 15)
      {
        slaveSendCode(slvSlideStep, 1, curStep, 2);
        curStep++;
      }
      //
      somethingChanged = true;
      changeTrack(0);
      return;
    }
  }
  //
  if (isButtonShift(buttonPageUp)) { changeTrack(1); return; }
  if (isButtonShift(buttonPageDown)) { changeTrack(-1); return; }
  if (isButtonShift(buttonLeft)) { changeVar = -1; updateVariable(curVariation, (VARIATIONS-1)); changeTrack(0); return; }
  if (isButtonShift(buttonRight)) { changeVar = 1; updateVariable(curVariation, (VARIATIONS-1)); changeTrack(0); return; }
  if (isButtonNoShift(buttonPageUp))
  {
    if (dArea == areaNewPatternCloneCur) { dArea = areaNewPatternCloneTmplt; }
    else if (dArea == areaNewPatternCloneTmplt) { dArea = areaNewPatternCloneCur; }
    else if (dArea == areaCustomChord) 
    { 
      if (dLine == 1)
      {
        dLine = 0;
      }
      else
      {
        dArea = areaNewPatternGoToSong; 
        dLine = 1; 
      }
    }
    else if (dArea == areaNewPatternGoToSong)
    {
      if (dLine == 0)
      {
        dArea = areaMenuCopyPaste;
        dLine = 1;
      } else dLine = 0;
    }
    else if (dArea == areaMenuCopyPaste)
    {
      dLine = 0;
    }
    else if (dArea < areaMenuCopyPaste)
    {
      if (dLine == 1)
      {
        dLine = 0;
        if (dArea == areaNoteTrackEdit && curTrack >= noteTrack) 
        { 
          dArea--; 
          dLine = 1;
          curStepZoneBuffer = dZone2;
          dZone2 = curStep;
        }
      }
      else if (dArea > 0)
      {
        dLine = 1;
        dArea--;
        if (dArea == areaTrackProcessor || dArea == areaVariations) dLine = 0;
        if (dArea == areaNoteTrackEdit && curTrack <= (accentTrack+1)) dArea--;
      }
    }
    updateAreaZones();
    return;
  }
  if (isButtonNoShift(buttonPageDown))
  {
    if (dArea == areaNewPatternCloneCur) { dArea = areaNewPatternCloneTmplt; }
    else if (dArea == areaNewPatternCloneTmplt) { dArea = areaNewPatternCloneCur; }    
    else if (dArea == areaMenuCopyPaste)
    {
      dLine++;
      if (dLine == 2)
      {
        dArea = areaNewPatternGoToSong;
        dLine = 0;
      }
    }
    else if (dArea == areaNewPatternGoToSong)
    {
      if (dLine == 0) 
      {
        dLine = 1; 
      }
      else
      {
        dArea = areaCustomChord;
        dLine = 0;
      }
    }
    else if (dArea == areaCustomChord) 
    { 
      if (dLine == 0)  dLine = 1;
    }
    else if (dArea < areaMenuCopyPaste)
    {
      if (dLine == 0 && dArea != areaTrackProcessor && dArea != areaVariations)
      {
        dLine = 1;
      }
      else if (dArea < areaSong)
      {
        dLine = 0;
        dArea++;
        if (dArea == areaNoteTrackEdit)
        {
          if (curTrack <= (accentTrack+1)) 
          { 
            dArea++; 
          } 
          else 
          { 
            dLine = 1; 
            curStep = dZone2;
            dZone2 = curStepZoneBuffer;
          }
        }
      }
    }
    updateAreaZones();
    return;
  }  
  //
  if (isButtonNoShift(buttonShift)) 
  { 
    flipMirrorMode();
    return;
  }
  //
  if (isButtonNoShift(buttonLeft))
  {
    if (dLine == 0 && dZone1 > 0) dZone1--;
    else if (dLine == 1 && dZone2 > 0) dZone2--;
    doRedrawDisplay();
    return;
  }
  if (isButtonNoShift(buttonRight))
  {
    if (dLine == 0 && dZone1 < dZone1Max) dZone1++;
    else if (dLine == 1 && dZone2 < dZone2Max) dZone2++;
    doRedrawDisplay();
    return;
  }
  //
  // ------------------------------------------ //
  if (isButtonWhatever(buttonUp)) { changeVar = 1; patternValuesUpdate(); }
    else if (isButtonWhatever(buttonDown)) { changeVar = -1; patternValuesUpdate(); }
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void updateAreaZones()
{
  switch (dArea)
  {
    case areaMain:                  dZone1Max = 2;  dZone2Max = 15;  break;
    case areaNoteTrackEdit:         dZone1Max = 3;  dZone2Max = 3;   break;
    case areaBPMnMIDI:              dZone1Max = 1;  dZone2Max = 0;   break;
    case areaTrackSettings:         dZone1Max = 1;  dZone2Max = 1;   break;
    case areaTrackProcessor:        dZone1Max = 1;  dZone2Max = 1;   break;
    case areaAutoStepnPatternType:  dZone1Max = 0;  dZone2Max = 0;   break;
    case areaVariations:            dZone1Max = 0;  dZone2Max = 0;   break;
    case areaSong:                  dZone1Max = 0;  dZone2Max = 15;  break;
    case areaMenuCopyPaste:         dZone1Max = 2;  dZone2Max = 0;   break;
    case areaNewPatternGoToSong:    dZone1Max = 0;  dZone2Max = 0;   break;
    case areaCustomChord:           dZone1Max = 0;  dZone2Max = 4;   break;
  }
  //
  if (dArea == areaNoteTrackEdit && noteTrackMode[curTrack-noteTrack] == noteTrackModeCC) dZone2Max = 2;
  checkAreaZonesBounds();
  doRedrawDisplay(true);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void zeroAreaPointers()
{
  dLine = dZone1 = dZone2 = 0;
  updateAreaZones();
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void flipMirrorMode()
{
  isMirrorMode = !isMirrorMode; 
  slaveSendCode(slvSetMirrorMode, ((isMirrorMode) ? 1 : 0), 0, 1); 
  SDReadCustomLCDChars(isMirrorMode);
  doRedrawDisplay();  
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void setStepsChanged()
{
  somethingChanged = true;
  slaveSetSingleStep();
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void setSequencerSettingsChanged()
{
  somethingChanged = true;
  slaveSendSequencerSettings(false);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void setTrackSettingsChanged()
{
  somethingChanged = true;
  slaveSendTrackSettings(false);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void changePattern()
{
  nextPatternCounter = 0;
  byte prevPattern = curPattern;
  updateVariable(nextPattern, 98, 1, 10);
  //
  if (nextPattern != curPattern) 
  {
    curPattern = nextPattern;
    SDSFileOpen(filePattern, fileRead, false);
    if (!tmpFile)
    {
      curPattern = nextPattern = prevPattern;
    }
    else
    {
      curPattern = prevPattern;
      tmpFile.close();
    }
  }
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void changeTrack(char val, boolean checkShift)
{
  if (val != 0)
  {
    if (checkShift && val != 0 && isShiftDown())
    {
      if (val > 0) 
      {
        if (curTrack == 0) curTrack = accentTrack;
          else curTrack = noteTrack;
      }
      else
      {
        if (curTrack == noteTrack) curTrack = accentTrack;
          else curTrack = 0;
      }
    }
    else
    {
      changeVar = val;
      updateVariable(curTrack, (lastTrack-1));
    }
  }
  //
  slaveGetTrackSettings();
  slaveGetDrumSteps();
  slaveGetNoteSteps(0);
  slaveGetNoteSteps(1);
  SDReadTrackName();
  doRedrawDisplay();
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void checkAreaZonesBounds()
{
  if (dZone1 > dZone1Max) { doRedrawDisplay(false); dZone1 = dZone1Max; }
  if (dZone2 > dZone2Max) { doRedrawDisplay(false); dZone2 = dZone2Max; }
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void updateChordCustom()
{
  if (dZone2 >= 5) return;
  updateVariable(customChords[customChordsZone1][dZone2], 48, 0, 10);
  customChordChanged = true;
  slaveSetCustomChord(customChordsZone1);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void checkSavePattern()
{
  SDSavePattern(filePattern, filePatternSettings);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void createNewPattern()
{
  checkSavePattern();
  if (curPattern >= 98) return;
  SDReadScreen(areaSavingMessage, lastLineOnly);
  //
  while (1) // Find Last Pattern //
  {
    curPattern++;
    SDSFileOpen(filePattern, fileRead, false);
    if (!tmpFile)
    {
      curPattern--;
      break;
    }
    else
    {
      tmpFile.close();
    }
  }    
  //
  if (dArea != areaNewPatternCloneCur) 
  { 
    byte prevPattern = curPattern;
    byte prevSong = curSong;
    //
    curSong = 0; // Template Song //
    curPattern = 1;
    SDReadScreen(areaCopyingMessage, lastLineOnly);
    SDLoadEntirePattern(filePattern);
    //    
    curSong = prevSong;
    curPattern = prevPattern;
  }
  //
  curPattern++;
  somethingChanged = true;
  checkSavePattern();  
  nextPattern = curPattern;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void showPatternLoading(boolean show)
{
  lcd.noCursor();
  homeCursor();
  printNumber2Dgts(curPattern);
  if (show) writeDisplay(loadingIcon); else spaceDisplay();
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void checkNextPattern()
{
  if (curPattern == nextPattern) return;
  //
  nextPatternCounter++;
  if (nextPatternCounter > nextPatternCC)
  {
    nextPatternCounter = 0;
    checkSavePattern();
    curPattern = nextPattern;
    //
    if (slaveRequestByte(slvIsPlaying) == 0)
    {
      showPatternLoading(true);
      SDLoadEntirePattern(filePattern);
      changeTrack(0);
    }
    else
    {
      startPatternStream(curPattern);
    }
  }
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void updateTrackNote()
{
  byte xcode = (noteSteps[curStep+STEPS] & 0x80);
  byte note = (noteSteps[curStep+STEPS] & 0x7F);
  updateVariable(note, 127, 1, 12);
  noteSteps[curStep+STEPS] = note | xcode;
  lastNote = note;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void updateTrackVelocity()
{
  byte xcode = (noteSteps[curStep] & 0x80);
  byte velocity = (noteSteps[curStep] & 0x7F);
  updateVariable(velocity, 127, 0, 10);
  noteSteps[curStep] = velocity | xcode;
  lastVelocity = velocity;
}
