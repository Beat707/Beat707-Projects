/*

  Created by Beat707 (c) 2011 - http://www.Beat707.com
  
  Pattern User Interface
  
*/

// ======================================================================================= //
void InterfaceTickPattern()
{
  dbStepsCalc();
  
  switch (multiButton)
  {  
    // ------------------------------- STOP ------------------------------- //
    case 0:
      if (midiClockRunning) 
      {
        MidiClockStop(); 
      }
      else
      {
        if (enableABpattern) editStepsPos = !editStepsPos;
        curZone = 0;
      }
      if (patternChanged) savePattern(0);
      if (setupChanged) saveSetup();
      checkPatternLoader();
      recordEnabled = 0;
      doLCDupdate = 1;
      break;
      
    // ------------------------------- PLAY ------------------------------- //
    case 1:
      lateAutoSave = 1;
      if (midiClockRunning) MidiClockStop();
      MidiClockStart(true, true);
      #if EXTRA_MIDI_IN_H_2
        #if XTH2_AUTO_STOP
          doAutoStop = 0;
        #endif
      #endif      
      break;
      
    // ------------------------------- LEFT ------------------------------- //
    case 2:
      if (holdingShift) { curZone = 0; holdingShiftUsed = 1; } else { curZone--; if (curZone == 255) curZone = LAST_PATT_ZONE; }
      updateLCDPattern();
      break;
      
    // ------------------------------- RIGHT ------------------------------ //
    case 5:
      if (holdingShift) { curZone = LAST_PATT_ZONE; holdingShiftUsed = 1; } else { curZone++; if (curZone > LAST_PATT_ZONE) curZone = 0; }
      updateLCDPattern();
      break;
      
    // -------------------------------- UP -------------------------------- //
    case 4:
      if (holdingShift && !recordEnabled)
      {
        shiftMode++;
        if (shiftMode > 7) shiftMode = 0;
        holdingShiftUsed = showOnlyOnce = 1;
      }
      else if (curZone == 0)
      {
        if (enableABpattern && ((!autoSteps || !midiClockRunning) && !editStepsPos && !mirrorPatternEdit))
        {
          editStepsPos = 1;
          updateLCDPattern();
          break;
        }
        
        if (nextPattern < (MAXSPATTERNS-1)) nextPattern++;
        if (enableABpattern && ((!autoSteps || !midiClockRunning) && !editStepsPos && !mirrorPatternEdit)) editStepsPos = 0;
        updateLCDPattern();
        break;
      }
      else if (curZone == 1) // BPM Editor //
      {
        if (midiClockBPM != 255) midiClockBPM++;
        if (midiClockRunning) MidiClockNewTime();
        setupChanged = 1;
      }      
      else if (curZone == 2 || curZone == 4)
      {
        currentTrack++;
        if (currentTrack > (DRUMTRACKS+3)) currentTrack = 0;
      }
      else if (curZone == 3)
      {
        if (currentTrack < DRUMTRACKS)
        {
          dmNotes[currentTrack]++;
          if (dmNotes[currentTrack] == 128) dmNotes[currentTrack] = 127;
          setupChanged = 1;
        }
        else if (currentTrack < (DRUMTRACKS+2))
        {
          uint8_t dTrack = currentTrack-DRUMTRACKS;
          if (mirrorPatternEdit)
          {
            newNote = dmSynthTrack[dTrack][patternBufferN][dmSynthTrackStepPos[1]+(16*editDoubleSteps)];
            if (holdingStepButton && newNote == 0) newNote = 13; else newNote += ((holdingStepButton) ? 12 : 1);
            if (newNote > 255) newNote = 255;
            dmSynthTrack[dTrack][patternBufferN][dmSynthTrackStepPos[1]+(16*editDoubleSteps)] = (uint8_t)newNote;
            dmSynthTrack[dTrack][patternBufferN][dmSynthTrackStepPos[1]+(16*editDoubleSteps)+32] = dmSynthTrack[dTrack][patternBufferN][dmSynthTrackStepPos[1]+(16*editDoubleSteps)];
            if (dmSynthTrack[dTrack][patternBufferN][dmSynthTrackStepPos[1]+(16*editDoubleSteps)] > 1) dmSynthTrackLastNoteEdit[dTrack] = dmSynthTrack[dTrack][patternBufferN][dmSynthTrackStepPos[1]+(16*editDoubleSteps)]-((dmSynthTrack[dTrack][patternBufferN][dmSynthTrackStepPos[1]+(16*editDoubleSteps)]<128) ? 0 : 127);
          }
          else
          {
            newNote = dmSynthTrack[dTrack][patternBufferN][dbStepsSpos];
            if (holdingStepButton && newNote == 0) newNote = 13; else newNote += ((holdingStepButton) ? 12 : 1);
            if (newNote > 255) newNote = 255;            
            dmSynthTrack[dTrack][patternBufferN][dbStepsSpos] = (uint8_t)newNote;
            if (dmSynthTrack[dTrack][patternBufferN][dbStepsSpos] > 1) dmSynthTrackLastNoteEdit[dTrack] = dmSynthTrack[dTrack][patternBufferN][dbStepsSpos]-((dmSynthTrack[dTrack][patternBufferN][dbStepsSpos]<128) ? 0 : 127);
          }
          patternChanged = 1;
        }
      }        
      else if (curZone == 5 && currentTrack < (DRUMTRACKS+2))
      {
        dmChannel[currentTrack]++;
        if (dmChannel[currentTrack] > 15) dmChannel[currentTrack] = 15;
        setupChanged = 1;
      }        
      else if (curZone == 6)
      {
        if (timeScale == 1) timeScale = 2; else timeScale = 1;
        if (midiClockRunning) MidiClockNewTime();
        setupChanged = 1;
      }        
      else if (curZone == 7)
      {
        MidiClockStop();
        timerStop();
        midiClockType++;
        if (midiClockType > 2) midiClockType = 0;
        setupChanged = 1;
      }
      else if (curZone == 8)
      {
        autoSteps = !autoSteps;
        setupChanged = 1;
      }
      else if (curZone == 9)
      {
        editDoubleSteps = 0;
        editStepsPos = stepsPos;
        mirrorPatternEdit = !mirrorPatternEdit;
        if (mirrorPatternEdit) enableABpattern = 1;
        setupChanged = 1;          
      }
      else if (curZone == 10)
      {
        midiClockShuffle++;
        if (midiClockShuffle > 6) midiClockShuffle = 6;
        MidiShuffleUpdate();
        setupChanged = 1;
      }
      else if (curZone == 11)
      {
        numberOfSteps++;
        if (numberOfSteps > 16) numberOfSteps = 16;
        setupChanged = 1;
      }
      else if (curZone == 12)
      {
        enableABpattern = !enableABpattern;
        editStepsPos = stepsPos = 0;
        setupChanged = 1;
      }
      else if (curZone == 13)
      {
        midiUSBmode = !midiUSBmode;
        checkMIDIusbMode();
        setupChanged = 1;
      }
      else if (curZone == 14)
      {
        if (midiClockDirection == 2) midiClockDirection = 0; else midiClockDirection++;
        setupChanged = 1;
      }
      #if ANALOG_INPUT_A0
      else if (curZone == 15)
      {
        analogInputMode++;
        if (analogInputMode > 4) analogInputMode = 0;
        analogInputModeNewDelay = millisNI()+ANALOG_MDLY;
        setupChanged = 1;
      }
      #endif
      else if (curZone == LAST_PATT_ZONE)
      {
        nextMode++;
        if (nextMode > 2) nextMode = 0;
      }
       
      updateLCDPattern();
      break;      
      
    // ------------------------------- DOWN ------------------------------- //
    case 3:
      if (holdingShift && !recordEnabled)
      {
        shiftMode--;
        if (shiftMode == 255) shiftMode = 7;
        holdingShiftUsed = 1;
        showOnlyOnce = 1;
      }
      else if (curZone == 0)
      {
        if (enableABpattern && ((!autoSteps || !midiClockRunning) && editStepsPos && !mirrorPatternEdit))
        {
          editStepsPos = 0;
          updateLCDPattern();
          break;
        }
        
        if (nextPattern > 0) nextPattern--;
        if (enableABpattern && ((!autoSteps || !midiClockRunning) && editStepsPos && !mirrorPatternEdit)) editStepsPos = 1;
        updateLCDPattern();
        break;
      }
      else if (curZone == 1) // BPM Editor //
      {
        midiClockBPM--;
        if (midiClockBPM < 25) midiClockBPM = 25;
        if (midiClockRunning) MidiClockNewTime();
        setupChanged = 1;
      }      
      else if (curZone == 2 || curZone == 4)
      {
        currentTrack--;
        if (currentTrack == 255) currentTrack = DRUMTRACKS+3;
      }
      else if (curZone == 3)
      {
        if (currentTrack < DRUMTRACKS)
        {
          dmNotes[currentTrack]--;
          if (dmNotes[currentTrack] == 255) dmNotes[currentTrack] = 0;
          setupChanged = 1;
        }
        else if (currentTrack < (DRUMTRACKS+2))
        {
          uint8_t dTrack = currentTrack-DRUMTRACKS;
          if (mirrorPatternEdit)
          {
            newNote = dmSynthTrack[dTrack][patternBufferN][dmSynthTrackStepPos[1]+(16*editDoubleSteps)] - ((holdingStepButton) ? 12 : 1);
            if (newNote < 0) newNote = 0;
            dmSynthTrack[dTrack][patternBufferN][dmSynthTrackStepPos[1]+(16*editDoubleSteps)] = (uint8_t)newNote;
            dmSynthTrack[dTrack][patternBufferN][dmSynthTrackStepPos[1]+(16*editDoubleSteps)+32] = dmSynthTrack[dTrack][patternBufferN][dmSynthTrackStepPos[1]+(16*editDoubleSteps)];
            if (dmSynthTrack[dTrack][patternBufferN][dmSynthTrackStepPos[1]+(16*editDoubleSteps)] > 1) dmSynthTrackLastNoteEdit[dTrack] = dmSynthTrack[dTrack][patternBufferN][dmSynthTrackStepPos[1]+(16*editDoubleSteps)]-((dmSynthTrack[dTrack][patternBufferN][dmSynthTrackStepPos[1]+(16*editDoubleSteps)]<128) ? 0 : 127);
          }
          else
          {          
            newNote = dmSynthTrack[dTrack][patternBufferN][dbStepsSpos] - ((holdingStepButton) ? 12 : 1);
            if (newNote < 0) newNote = 0;            
            dmSynthTrack[dTrack][patternBufferN][dbStepsSpos] = (uint8_t)newNote;
            if (dmSynthTrack[dTrack][patternBufferN][dbStepsSpos] > 1) dmSynthTrackLastNoteEdit[dTrack] = dmSynthTrack[dTrack][patternBufferN][dbStepsSpos]-((dmSynthTrack[dTrack][patternBufferN][dbStepsSpos] < 128) ? 0 : 127);
          }
          patternChanged = 1;
        }        
      }
      else if (curZone == 5 && currentTrack < (DRUMTRACKS+2))
      {
        dmChannel[currentTrack]--;
        if (dmChannel[currentTrack] == 255) dmChannel[currentTrack] = 0;
        setupChanged = 1;
      }                
      else if (curZone == 6)
      {
        if (timeScale == 1) timeScale = 2; else timeScale = 1;
        if (midiClockRunning) MidiClockNewTime();
        setupChanged = 1;
      }
      else if (curZone == 7)
      {
        MidiClockStop();
        timerStop();
        midiClockType--;
        if (midiClockType == 255) midiClockType = 2;
        setupChanged = 1;
      }
      else if (curZone == 8)
      {
        autoSteps = !autoSteps;
        setupChanged = 1;
      }      
      else if (curZone == 9)
      {
        editDoubleSteps = 0;
        editStepsPos = stepsPos;
        mirrorPatternEdit = !mirrorPatternEdit;
        if (mirrorPatternEdit) enableABpattern = 1;
        setupChanged = 1;          
      }
      else if (curZone == 10)
      {
        midiClockShuffle--;
        if (midiClockShuffle == 255) midiClockShuffle = 0;
        MidiShuffleUpdate();
        setupChanged = 1;
      }
      else if (curZone == 11)
      {
        if (numberOfSteps > 1) numberOfSteps--;
        setupChanged = 1;
      }
      else if (curZone == 12)
      {
        enableABpattern = !enableABpattern;
        editStepsPos = stepsPos = 0;
        setupChanged = 1;
      }
      else if (curZone == 13)
      {
        midiUSBmode = !midiUSBmode;
        checkMIDIusbMode();
        setupChanged = 1;
      }      
      else if (curZone == 14)
      {
        if (midiClockDirection == 0) midiClockDirection = 2; else midiClockDirection--;
        setupChanged = 1;
      }      
      #if ANALOG_INPUT_A0
      else if (curZone == 15)
      {
        analogInputMode--;
        if (analogInputMode == 255) analogInputMode = 4;
        analogInputModeNewDelay = millisNI()+ANALOG_MDLY;
        setupChanged = 1;
      }
      #endif      
      else if (curZone == LAST_PATT_ZONE)
      {
        nextMode--;
        if (nextMode == 255) nextMode = 2;
      }                  
            
      updateLCDPattern();
      break;
      
    // ------------------------------- EDIT/RECORD ------------------------ //
    case 6:
      if (holdingShift) stickyShift = !stickyShift; else recordEnabled = !recordEnabled;
      updateLCDPattern();
      break;

    // ------------------------------- CYCLE MODES ------------------------ //
    case 9:
      if (holdingShift)
      {
        autoSteps = !autoSteps;
        setupChanged = 1;        
      }
      else
      {
        nextMode = 1; 
        loadNextMode(); 
      }
      delayNI(250);        
      break;
  }
}

// ======================================================================================= //
// ======================================================================================= //
// ======================================================================================= //
// ======================================================================================= //

void shiftButtonPattern()
{
  if (!shiftClick)
  {
    if (holdingShift)
    {
      holdingShift = 0;
      holdingShiftUsed = 0;
      
      if (shiftMode == 3) // Init Pattern //
      {
        shiftMode = 0;
        lcd.clear(); lcdPrint(INIT_PATTERNQ);
        if (hitShiftToConfirm())
        {
            memset(dmSteps,0,sizeof(dmSteps));
            memset(dmSynthTrack,0,sizeof(dmSynthTrack));
            #if INI_PATT_FULL_ACNT
              patternAccentInit();
            #endif
            patternChanged = 1;
            lcdOK();
        }
      }
      else if (shiftMode == 4) // Copy Pattern //
      {
        shiftMode = 0;
        uint8_t prevPattern = currentPattern;
        currentPattern = MAXSPATTERNS; // copy and paste pattern area
        savePattern(0);
        currentPattern = prevPattern;
        lcdOK();
      }
      else if (shiftMode == 5) // Copy Pattern Accents Only //
      {
        shiftMode = 0;
        uint8_t prevPattern = currentPattern;
        currentPattern = MAXSPATTERNS; // copy and paste pattern area
        savePattern(true);
        currentPattern = prevPattern;        
        lcdOK();
      }
      else if (shiftMode == 6) // Paste Pattern //
      {
        shiftMode = 0;
        lcd.clear(); lcdPrint(PASTE_PATTERNQ);
        if (hitShiftToConfirm())
        {
          uint8_t prevPattern = currentPattern;
          currentPattern = MAXSPATTERNS; // copy and paste pattern area
          loadPattern(0);
          currentPattern = prevPattern;
          patternBufferN = !patternBufferN;          
          patternChanged = 1;
          lcdOK();
        }
      }
      else if (shiftMode == 7) // Paste Merge Pattern //
      {
        shiftMode = 0;
        lcd.clear(); lcdPrint(PASTE_PATTERN);
        if (hitShiftToConfirm())
        {
          uint8_t prevPattern = currentPattern;
          currentPattern = MAXSPATTERNS; // copy and paste pattern area
          loadPattern(true);
          currentPattern = prevPattern;
          patternBufferN = !patternBufferN;          
          patternChanged = 1;
          lcdOK();
        }
      }
      updateLCDPattern();
    }
  }
  else
  {
    if (!holdingShift)
    {
      for (int x=0; x<10; x++) LEDsPatternTick();
      buttonsInputAndLEDsOutput();
      holdingShift = 1;      
      if (shiftClick)
      {
        showOnlyOnce = 1;
        updateLCDPattern();
      }
      else
      {
        holdingShift = 0;
        holdingShiftUsed = 0;        
        // Double Quickly Shift Click //
        if (lastMillisShiftClick > millisNI())
        {
          #if !DISABLE_SHFT_SHCUTS
            editDoubleSteps = 0;
            editStepsPos = stepsPos;
            mirrorPatternEdit = !mirrorPatternEdit;
            if (mirrorPatternEdit) enableABpattern = 1;
            setupChanged = doLCDupdate = 1;
          #endif
        }
        else
        {
          // Shift was pressed very quickly //
          if (curZone == LAST_PATT_ZONE) loadNextMode();
            else if (recordEnabled) recordShowCurPos = !recordShowCurPos;
            else
            {
              #if !DISABLE_SHFT_SHCUTS
                editDoubleSteps = !editDoubleSteps;
                updateLCDPatNumber();
              #endif
            }
        }
        lastMillisShiftClick = millisNI()+500;
      }
    }
  }
}

// ======================================================================================= //
// ======================================================================================= //
// ======================================================================================= //
// ======================================================================================= //

void LEDsPatternTick() // & Buttons //
{
  dbStepsCalc();
  
  if (midiClockDirection == 1) midiClockCounter2 = 15-midiClockCounter;
    else if (midiClockDirection == 2) midiClockCounter2 = random(0, 15);
    else midiClockCounter2 = midiClockCounter;
  
  // Holding SHIFT //
  if (holdingShift && !recordEnabled)
  { 
    stepLEDs[0] = stepLEDs[1] = stepLEDs[2] = 0;
    if (shiftMode == 0) bitWrite(stepLEDs[0],currentTrack,1);
      else if (shiftMode == 1) stepLEDs[0] = dmMutes;
      else if (shiftMode == 2) 
      { 
        if (dmMutes != 0)
        {
          stepLEDs[0] = dmMutes ^ 65535;
        }
      }
      
    if (midiClockRunning && midiClockCounterDivider < (bitRead(stepLEDs[0],midiClockCounter2) ? 12 : 6)) bitWrite(stepLEDs[0],midiClockCounter2,!bitRead(stepLEDs[0],midiClockCounter2));

    stepLEDs[1] = stepLEDs[2] = stepLEDs[0];
  }
  else  // No Shift LED Updates //
  {
    if (recordEnabled)
    {  
      showLEDsRecOutput();
    } 
    else // Not In Record Mode //
    {
      if (currentTrack < DRUMTRACKS) 
      {
        stepLEDs[0] = dmSteps[patternBufferN][currentTrack+dbSteps];
      }
      else if (currentTrack < DRUMTRACKS+2) 
      { 
        stepLEDs[0] = 0; 
        uint8_t dTrack = currentTrack-DRUMTRACKS;
        for (char x=0; x<16; x++) 
        { 
          if (dmSynthTrack[dTrack][patternBufferN][x+dbStepsS] > 0) bitSet(stepLEDs[0],x);
        }
        if (mirrorPatternEdit || dmSynthTrackStepPos[0] == editStepsPos)
        {
          if (noteStepPositionBlinker < 2) bitWrite(stepLEDs[0],dmSynthTrackStepPos[1],!bitRead(stepLEDs[0],dmSynthTrackStepPos[1]));      
          if (lastMillisStepPosBlinker < millisNI())
          {
            noteStepPositionBlinker++;
            if (noteStepPositionBlinker > 8) noteStepPositionBlinker = 0;
            lastMillisStepPosBlinker = millisNI()+80;
          }
        }
      }
      else
      {
        stepLEDs[0] = dmSteps[patternBufferN][currentTrack-2+dbSteps];
      }
            
      stepLEDs[1] = stepLEDs[2] = stepLEDs[0];
      stepLEDs[1] &= dmSteps[patternBufferN][DRUMTRACKS+dbSteps];
      stepLEDs[2] &= dmSteps[patternBufferN][DRUMTRACKS+dbSteps];
      stepLEDs[2] &= dmSteps[patternBufferN][DRUMTRACKS+1+dbSteps];

      if (midiClockRunning)
      {
        for (char q=0; q<3; q++)
        {
          if (!autoSteps) { if (editStepsPos == stepsPos && midiClockCounterDivider < (bitRead(stepLEDs[q],midiClockCounter2) ? 12 : 6)) bitWrite(stepLEDs[q],midiClockCounter2,!bitRead(stepLEDs[q],midiClockCounter2)); }
            else { if (midiClockCounterDivider < (bitRead(stepLEDs[q],midiClockCounter2) ? 12 : 6)) bitWrite(stepLEDs[q],midiClockCounter2,!bitRead(stepLEDs[q],midiClockCounter2)); }
        }
      }
    }
  }  
  
  // ----------------- BUTTON IN & LEDS OUT ----------------------- //  
  
  buttonsInputAndLEDsOutput();
  if (stepButtons)
  {
    if (recordEnabled)
    {
      uint8_t velocity = 120;
      if (midiClockRunning) velocity = 87+(bitRead(dmSteps[patternBufferN][DRUMTRACKS],midiClockCounter2)*20)+(bitRead(dmSteps[patternBufferN][DRUMTRACKS+1],midiClockCounter2)*20);
      for (char i=0; i<DRUMTRACKS; i++)
      { 
        if (bitRead(stepButtons,i))
        {
          if (midiClockRunning) 
          {
            if (mirrorPatternEdit)
            {
              bitWrite(dmSteps[patternBufferN][i+((DRUMTRACKS+2)*editDoubleSteps)],midiClockCounter2,1);
              bitWrite(dmSteps[patternBufferN][i+(((DRUMTRACKS+2)*editDoubleSteps)+((DRUMTRACKS+2))*2)],midiClockCounter2,1);
            }
            else bitWrite(dmSteps[patternBufferN][i+dbSteps],midiClockCounter2,1);
          }
          sendMidiNoteOn(dmNotes[i],velocity, dmChannel[i], 0xFF);
          sendMidiNoteOff(dmNotes[i], dmChannel[i], 0xFF);
          for (char q=0; q<3; q++) { bitWrite(stepLEDs[q],i,1); }
        } 
      }
      lastMillisRecButtons = millisNI()+25;
      buttonsInputAndLEDsOutput();
      patternChanged = 1;
    }
    else
    {
      if (holdingShift)
      {
        // Holding SHIFT //
        holdingShiftUsed = 1;
        if (shiftMode == 0)
        {
          for (char i=0; i<(DRUMTRACKS+2); i++) 
          { 
            if (bitRead(stepButtons,i))  
            { 
              if (i == DRUMTRACKS && currentTrack == DRUMTRACKS) currentTrack = DRUMTRACKS+2;
              else if (i == (DRUMTRACKS+1) && currentTrack == (DRUMTRACKS+1)) currentTrack = DRUMTRACKS+3;
                else currentTrack = i; 
              break; 
            } 
          }
          updateLCDPattern();
        }
        else if (shiftMode == 1)
        {
          for (char i=0; i<(DRUMTRACKS+2); i++) { if (bitRead(stepButtons,i)) { bitWrite(dmMutes,i,!bitRead(dmMutes,i)); break; } }
        }
        else if (shiftMode == 2)
        {
          if (dmMutes > 0) soloCheckerTemp = 1; else soloCheckerTemp = 0;

          for (char i=0; i<(DRUMTRACKS+2); i++) 
          { 
            if (bitRead(stepButtons,i)) 
            { 
              if (soloCheckerTemp && !bitRead(dmMutes,i)) { dmMutes = 0; break; }
              bitWrite(dmMutes,i,0);
            } else bitWrite(dmMutes,i,1);
          }
        }
      }
      else
      {
        if (currentTrack < DRUMTRACKS) 
        {
          if (mirrorPatternEdit)
          {
              dmSteps[patternBufferN][currentTrack+((DRUMTRACKS+2)*editDoubleSteps)] ^= stepButtons;
              dmSteps[patternBufferN][currentTrack+(((DRUMTRACKS+2)*editDoubleSteps)+((DRUMTRACKS+2)*2))] ^= stepButtons;
          }
          else dmSteps[patternBufferN][currentTrack+dbSteps] ^= stepButtons;
          patternChanged = 1;
        }
        else if (currentTrack >= (DRUMTRACKS+2))
        {
          if (mirrorPatternEdit)
          {
              dmSteps[patternBufferN][currentTrack-2+((DRUMTRACKS+2)*editDoubleSteps)] ^= stepButtons;
              dmSteps[patternBufferN][currentTrack-2+(((DRUMTRACKS+2)*editDoubleSteps)+((DRUMTRACKS+2)*2))] ^= stepButtons;
          }         
          else dmSteps[patternBufferN][currentTrack-2+dbSteps] ^= stepButtons;
          patternChanged = 1;
        }
        else
        { 
          for (char i=0; i<16; i++) 
          { 
            if (bitRead(stepButtons,i)) 
            { 
              if ((mirrorPatternEdit || dmSynthTrackStepPos[0] == editStepsPos) && dmSynthTrackStepPos[1] == i)
              {
                uint8_t dTrack = currentTrack-DRUMTRACKS;

                if (mirrorPatternEdit)
                {
                  if (dmSynthTrack[dTrack][patternBufferN][i+(16*editDoubleSteps)] == 1) dmSynthTrack[dTrack][patternBufferN][i+(16*editDoubleSteps)] = 0;
                  else if (dmSynthTrack[dTrack][patternBufferN][i+(16*editDoubleSteps)] > 128) dmSynthTrack[dTrack][patternBufferN][i+(16*editDoubleSteps)] = 1;
                  else if (dmSynthTrack[dTrack][patternBufferN][i+(16*editDoubleSteps)] > 0) dmSynthTrack[dTrack][patternBufferN][i+(16*editDoubleSteps)] += 127;
                    else dmSynthTrack[dTrack][patternBufferN][i+(16*editDoubleSteps)] = dmSynthTrackLastNoteEdit[dTrack];

                  if (dmSynthTrack[dTrack][patternBufferN][i+(16*editDoubleSteps)+32] == 1) dmSynthTrack[dTrack][patternBufferN][i+(16*editDoubleSteps)+32] = 0;
                  else if (dmSynthTrack[dTrack][patternBufferN][i+(16*editDoubleSteps)+32] > 128) dmSynthTrack[dTrack][patternBufferN][i+(16*editDoubleSteps)+32] = 1;
                  else if (dmSynthTrack[dTrack][patternBufferN][i+(16*editDoubleSteps)+32] > 0) dmSynthTrack[dTrack][patternBufferN][i+(16*editDoubleSteps)+32] += 127;
                    else dmSynthTrack[dTrack][patternBufferN][i+(16*editDoubleSteps)+32] = dmSynthTrackLastNoteEdit[dTrack];
                    
                  if (dmSynthTrack[dTrack][patternBufferN][i+(16*editDoubleSteps)] > 1) dmSynthTrackLastNoteEdit[dTrack] = dmSynthTrack[dTrack][patternBufferN][i+(16*editDoubleSteps)]-((dmSynthTrack[dTrack][patternBufferN][i+(16*editDoubleSteps)] < 128) ? 0 : 127);
                }
                else
                {
                  if (dmSynthTrack[dTrack][patternBufferN][i+dbStepsS] == 1) dmSynthTrack[dTrack][patternBufferN][i+dbStepsS] = 0;
                  else if (dmSynthTrack[dTrack][patternBufferN][i+dbStepsS] > 128) dmSynthTrack[dTrack][patternBufferN][i+dbStepsS] = 1;
                  else if (dmSynthTrack[dTrack][patternBufferN][i+dbStepsS] > 0) dmSynthTrack[dTrack][patternBufferN][i+dbStepsS] += 127;
                    else dmSynthTrack[dTrack][patternBufferN][i+dbStepsS] = dmSynthTrackLastNoteEdit[dTrack];
                    
                  if (dmSynthTrack[dTrack][patternBufferN][i+dbStepsS] > 1) dmSynthTrackLastNoteEdit[dTrack] = dmSynthTrack[dTrack][patternBufferN][i+dbStepsS]-((dmSynthTrack[dTrack][patternBufferN][i+dbStepsS]<128) ? 0 : 127);
                }
                  
               patternChanged = 1;
              }
              dmSynthTrackStepPos[0] = ((mirrorPatternEdit) ? 0 : editStepsPos);
              dmSynthTrackStepPos[1] = i; 
              break; 
            } 
          }
          noteStepPositionBlinker = 0;
          lastMillisStepPosBlinker = millisNI()+80;
          updateLCDPattern();
        }
      }
    }
  }
}

// ======================================================================================= //
void showLEDsRecOutput()
{  
  if (lastMillisRecButtons < millisNI()) stepLEDs[0] = stepLEDs[1] = stepLEDs[2] = 0;
  
  if (midiClockRunning)
  {
    if (midiClockCounterDivider < (bitRead(stepLEDs[0],midiClockCounter2) ? 12 : 6))
    {
      for (char i=0; i<DRUMTRACKS; i++)
      {
        if (!bitRead(dmMutes,i) && bitRead(dmSteps[patternBufferN][i+dbSteps],midiClockCounter2))
        {
          bitWrite(stepLEDs[0],i,1); 
          if (bitRead(dmSteps[patternBufferN][DRUMTRACKS+dbSteps],midiClockCounter2) || bitRead(dmSteps[patternBufferN][DRUMTRACKS+1+dbSteps],midiClockCounter2)) bitWrite(stepLEDs[1],i,1);
          if (bitRead(dmSteps[patternBufferN][DRUMTRACKS+dbSteps],midiClockCounter2) && bitRead(dmSteps[patternBufferN][DRUMTRACKS+1+dbSteps],midiClockCounter2)) bitWrite(stepLEDs[2],i,1);
        }
      }
      if (dmSynthTrack[0][patternBufferN][midiClockCounter2+dbStepsS] > 1)
      {
        bitSet(stepLEDs[0],DRUMTRACKS);
        if (bitRead(dmSteps[patternBufferN][DRUMTRACKS+dbSteps],midiClockCounter2) || bitRead(dmSteps[patternBufferN][DRUMTRACKS+1+dbSteps],midiClockCounter2)) bitWrite(stepLEDs[1],DRUMTRACKS,1);
        if (bitRead(dmSteps[patternBufferN][DRUMTRACKS+dbSteps],midiClockCounter2) && bitRead(dmSteps[patternBufferN][DRUMTRACKS+1+dbSteps],midiClockCounter2)) bitWrite(stepLEDs[2],DRUMTRACKS,1);
      }
      if (dmSynthTrack[1][patternBufferN][midiClockCounter2+dbStepsS] > 1)
      {
        bitSet(stepLEDs[0],DRUMTRACKS+1);
        if (bitRead(dmSteps[patternBufferN][DRUMTRACKS+dbSteps],midiClockCounter2) || bitRead(dmSteps[patternBufferN][DRUMTRACKS+1+dbSteps],midiClockCounter2)) bitWrite(stepLEDs[1],DRUMTRACKS+1,1);
        if (bitRead(dmSteps[patternBufferN][DRUMTRACKS+dbSteps],midiClockCounter2) && bitRead(dmSteps[patternBufferN][DRUMTRACKS+1+dbSteps],midiClockCounter2)) bitWrite(stepLEDs[2],DRUMTRACKS+1,1);
      }
    }
        
    if (recordShowCurPos) bitWrite(stepLEDs[1],midiClockCounter2,!bitRead(stepLEDs[1],midiClockCounter2));
  }
}
