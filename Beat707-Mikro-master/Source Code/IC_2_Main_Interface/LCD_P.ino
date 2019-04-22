/*
 * Beat707 Mikro - created by William (Guilherme) Kalfelz (c) 2016 - www.Beat707.com - www.Wusik.com - www.William-k.com
 * 
 */

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void updateDisplayPattern()
{
  if (!redrawDisplay || streamPatternGo) return;
  redrawDisplay = false;
  if (reloadScreen)
  {
    reloadScreen = false;
    lcd.noCursor();
    SDReadScreen(dArea, entireScreen);
  }
  //
  switch(dArea)
  {
    // -------------------------------------------------------- //
    case areaMain:
    case areaNoteTrackEdit:
      //
      homeCursor();
      printNumber2Dgts(nextPattern);
      if (nextPattern == curPattern) writeDisplay(LCD_VARIATION + curVariation); else writeDisplay(loadingIcon);
      if (curPlayingVariation != 0) writeDisplay(LCD_VARIATION + curPlayingVariation - 1); else spaceDisplay();
      printTrackNumber(curTrack, true);
      //
      setCursorDisplay(SECOND_LINE(0));
      if (dArea != areaNoteTrackEdit)
      {
        if (curTrack < noteTrack)
        {
          for (byte x=0; x<STEPS; x++)
          {
            if (bitRead(drumSteps[0],x) == 0 && bitRead(drumSteps[1],x) == 0)
            {
              writeDisplay(STEP_OFF); 
            }
            else 
            {
              if (bitRead(drumSteps[1],x) == 1 && bitRead(drumSteps[0],x) == 1) writeDisplay(STEP_DOUBLE);      
              else if (bitRead(drumSteps[1],x) == 1 && bitRead(drumSteps[0],x) == 0) writeDisplay(STEP_DOUBLE_IN);      
              else writeDisplay(STEP_ON);
            }
          }          
        }
        else
        {
          for (byte x=0; x<STEPS; x++)
          {
            if (noteTrackMode[curTrack-noteTrack] == noteTrackModeNotes)
            {
              byte xcode = getTrackNoteCode(x);
              if (getTrackNote(x) == 0 && xcode == 0)
              {
                writeDisplay(STEP_OFF); 
              }
              else 
              {
                if (xcode == noteOff) writeDisplay(stepNoteOffChar);
                else if (xcode == noteDouble) writeDisplay(STEP_DOUBLE);
                else if (xcode == noteDoubleIn) writeDisplay(STEP_DOUBLE_IN);
                else if (xcode == noteSlide) writeDisplay(stepSlideChar);            
                else writeDisplay(STEP_ON);
              }
            }
            else
            {
              if (noteSteps[x] > 0) writeDisplay(STEP_ON); else writeDisplay(STEP_OFF); 
            }
          }
        }
      }
      else
      {
        setCursorDisplay(SECOND_LINE(4));
        printNumber2Dgts(curStep+1);
        //
        if (noteTrackMode[curTrack-noteTrack] == noteTrackModeNotes)
        {
          spacesDisplay(2);
          byte xcode = getTrackNoteCode(curStep);
          if (getTrackNote(curStep) == 0 && getTrackNoteCode(curStep) == 0) { printTextStringDisplay(stringDotDotDot); spacesDisplay(5); }
          else if (xcode == noteOff) { printTextStringDisplay(stringSpacesOff); }
          else
          {
            if (xcode == noteSlide) spacesDisplay(3); else printNumber3Dgts(getTrackVelocity(curStep));
            spaceDisplay();
            if (xcode == noteDouble) writeDisplay(STEP_DOUBLE);
            else if (xcode == noteDoubleIn) writeDisplay(STEP_DOUBLE_IN);
            else if (xcode == noteSlide) writeDisplay(stepSlideChar);
            else writeDisplay(regularNoteChar);
            //
            byte xnote = getTrackNote(curStep);
            if (xnote == 0) spacesDisplay(4); else printNoteName(xnote);
          }
        }
        else
        {
          spacesDisplay(1);
          printDisplay("CC");
          if (noteSteps[curStep] > 0) 
          { 
            printNumber3Dgts(noteSteps[curStep]-1); 
            spacesDisplay(1);
            printNumber3Dgts(noteSteps[curStep+STEPS]); 
          } 
          else 
          { 
            printTextStringDisplay(stringDotDotDot); 
            spacesDisplay(4);
          }
        }
      }
      //
      if (dLine == 0)
      {
        if (dZone1 == 0) setCursorDisplay(1);
        else if (dZone1 == 1) setCursorDisplay(2);
        else setCursorDisplay(5);
      }
      else
      {
        if (dArea == areaNoteTrackEdit)
        {
          if (dZone2 == 0)
          {
            setCursorDisplay(SECOND_LINE(4));
          }
          else
          {
            if (noteTrackMode[curTrack-noteTrack] == noteTrackModeNotes)
            {
              if (dZone2 == 1) setCursorDisplay(SECOND_LINE(8));
              else if (dZone2 == 2) setCursorDisplay(SECOND_LINE(12));
              else setCursorDisplay(SECOND_LINE(13));
            }
            else
            {
              if (dZone2 == 1) setCursorDisplay(SECOND_LINE(9));
              else setCursorDisplay(SECOND_LINE(13));            
            }
          }
        }
        else
        {        
          setCursorDisplay(SECOND_LINE(dZone2));
        }
      }
      break;

    // -------------------------------------------------------- //

    case areaBPMnMIDI:
      //
      homeCursor();
      if (seqType == masterUnit) printTextStringDisplay(stringMaster); 
        else printTextStringDisplay(stringSlave);
      spacesDisplay(2);
      printNumber3Dgts(seqBPM);
      setCursorDisplay(SECOND_LINE(13));
      printOnOff(seqSyncOut);
      //
      if (dLine == 0)
      {
        if (dZone1 == 0) homeCursor();
        else setCursorDisplay(9);
      }      
      else setCursorDisplay(SECOND_LINE(13));
      break;
    
    // -------------------------------------------------------- //

    case areaTrackSettings:
      //
      setCursorDisplay(1);
      printTrackNumber(curTrack, false);
      if (!isAccentTrack())
      {
        setCursorDisplay(14);
        printNumber2Dgts(trackMidiCH+1);
        setCursorDisplay(SECOND_LINE(0));
        if (curTrack < accentTrack)
        {
          printDisplay("N#");
          spaceDisplay();
          printNumber3Dgts(trackNote);
          spaceDisplay();
        }
        else
        {
          if (noteTrackMode[curTrack-noteTrack] == noteTrackModeNotes) printTextStringDisplay(stringMNotes);
            else printTextStringDisplay(stringMidiCC);
          spaceDisplay();          
        }
        printDisplay(curTrackName);
      }
      else
      {
        spacesDisplay(13);
        setCursorDisplay(SECOND_LINE(0));
        spacesDisplay(16);
      }
      //
      if (!isAccentTrack())
      {
        if (dLine == 0)
        {
          if (dZone1 == 0) setCursorDisplay(2);
          else setCursorDisplay(15);
        }      
        else 
        {
          if (dZone2 == 0) setCursorDisplay(SECOND_LINE(5));
          else setCursorDisplay(SECOND_LINE(7));
        }
      }
      else 
      {
        setCursorDisplay(2);
      }
      break;

    // -------------------------------------------------------- //

    case areaTrackProcessor:
      //
      setCursorDisplay(SECOND_LINE(1));
      printTrackNumber(curTrack, false);
      setCursorDisplay(SECOND_LINE(4));
      if (!isAccentTrack()) SDReadXTraName(trackProcessor); else spacesDisplay(12);
      //
      if (dZone1 == 0) setCursorDisplay(SECOND_LINE(1));
      else setCursorDisplay(SECOND_LINE(4));
      break;

    // -------------------------------------------------------- //

    case areaAutoStepnPatternType:
      //
      setCursorDisplay(14);
      printNumber2Dgts(autoSteps);
      setCursorDisplay(SECOND_LINE(7));
      printDisplay(curPatternName);
      //
      if (dLine == 0) setCursorDisplay(14); else setCursorDisplay(SECOND_LINE(7));
      break;      

    // -------------------------------------------------------- //

    case areaVariations:
      //
      setCursorDisplay(12);
      for (byte x=0; x<(totalVariations+1); x++) writeDisplay('A'+x);
      spacesDisplay(4-(totalVariations+1));
      //
      setCursorDisplay(12);
      break;


    // -------------------------------------------------------- //

    case areaSong:
      //
      setCursorDisplay(5);
      printNumber2Dgts(curSong);
      setCursorDisplay(SECOND_LINE(0));
      printDisplay(songName);
      //
      if (dLine == 0) setCursorDisplay(5); else setCursorDisplay(SECOND_LINE(dZone2));
      break;
    
    // -------------------------------------------------------- //

    case areaMenuCopyPaste:
      //
      if (copyMode == entirePattern) { setCursorDisplay(SECOND_LINE(9)); printTextStringDisplay(stringPattern); }
        else { setCursorDisplay(SECOND_LINE(9)); spacesDisplay(2); printTextStringDisplay(stringTrack); }
      //
      if (dLine == 0)
      {
        if (dZone1 == 0) homeCursor();
        else if (dZone1 == 1) setCursorDisplay(5);
        else setCursorDisplay(11);
      }
      else
      {
        if (copyMode == entirePattern) setCursorDisplay(SECOND_LINE(9)); else setCursorDisplay(SECOND_LINE(11));
      }
      break;

    // -------------------------------------------------------- //

    case areaNewPatternGoToSong:
      //
      if (dLine == 0) setCursorDisplay(9); else setCursorDisplay(SECOND_LINE(7)); 
      break;

    // -------------------------------------------------------- //

    case areaCustomChord:
      //
      setCursorDisplay(14);
      printNumber2Dgts(customChordsZone1+1);
      //
      setCursorDisplay(SECOND_LINE(1));
      for (byte x=0; x<5; x++)
      {
        if (customChords[customChordsZone1][x] > 0) printNoteName(customChords[customChordsZone1][x]-1);
          else printTextStringDisplay(stringDotDotDot);
      }
      //
      if (dLine == 0) setCursorDisplay(14); else setCursorDisplay(SECOND_LINE(1+(dZone2*3))); 
      break;

    // -------------------------------------------------------- //

    case areaNewPatternCloneCur:
    case areaNewPatternCloneTmplt:
      setCursorDisplay(6); 
      break;
  }
  //
  displayCursor();
}
