/*
 * 
 * Created by William Kalfelz @ Beat707 (c) 2018 - http://www.Beat707.com
 * 
 */

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void createScreen()
{
  if (curRightScreen == kMuteMenu)
  {
    createScreenMute();
    return;
  }
  //
  // Sequence Position ---------------------------------------------------------------------------------------------------------------- //
  leds[0] = leds[1] = leds[2] = 0;
  if (seqPlaying) leds[0] = leftLEDsVarSeq; else bitSet(leds[0], 0);
  //
  if (!seqPlaying)
  {
    byte xVar = variation;
    if (forceVariation >= 0) xVar = forceVariation;
    if (mirror) leds[0] |= B11110000; 
    else 
    {
      if (forceVariation >= 0)
      {
        leds[0] |= B11110000;
        bitClear(leds[0], xVar + 4);
      }
      else bitSet(leds[0], xVar + 4);
    }    
  }
  //
  // Pattern / Track Selection -------------------------------------------------------------------------------------------------------- //
  leds[1] = leds[2] = 0;
  //
  #if ECHOS_ENABLED
    if (curRightScreen == kRightMenu && menuPosition >= menuEcho && menuPosition <= menuEchoType)
    {
      leds[1] = leds[2] = B01010101; 
    } else 
  #endif
  if (curRightScreen == kRightMenu && (menuPosition == menuVariationsABCD || menuPosition == menuPtPlays || menuPosition == menuPtPlaysChain || menuPosition == menuPtNext))
  {
   leds[1] = leds[2] = B01010101; 
  }
  else if (curRightScreen == kRightMenu && menuPosition != menuRecord && menuPosition != menuMidiCC && menuPosition != menuNote && menuPosition != menuProc && menuPosition != menuVariationsABCD)
  {
    leds[1] = leds[2] = 0xFF;
  }
  else if (curRightScreen == kRightPatternSelection)
  {
    leds[1] = patternBitsSelector;
    leds[2] = patternBitsSelector >> 8;
  }
  else if (menuPosition == menuVariationsABCD)
  {
   leds[1] = leds[2] = B01010101; 
  }
  else
  {
    if (curTrack < DRUM_TRACKS)
    {
      if (curTrack < 8) bitSet(leds[1], curTrack); else bitSet(leds[2], curTrack - 8);
    }
    else
    {
      leds[1] = leds[2] = 0xFF;
      bitClear(leds[1], curTrack - DRUM_TRACKS);
    }
    //
    if (seqPlaying && curRightScreen != kRightTrackSelection)
    {
      leds[1] = leds[1] ^ chaseLEDs[0];
      leds[2] = leds[2] ^ chaseLEDs[1];
    }
  }
  //
  // Left Screen ---------------------------------------------------------------------------------------------------------------------- //
  if (curLeftScreen == kLeftMain)
  {
    if (editingNoteTranspose != -127 && !editingNote && curRightScreen == kRightSteps && curTrack >= DRUM_TRACKS)
    {
      if (noteTransposeEditAllSteps) 
      {
        segments[0][0] = S_A;
        printNumber(0, 1, editingNoteTranspose);
      }
      else 
      {
        segments[0][0] = S_N;
        byte holdingNote = 0;
        for (byte x=0; x<8; x++)
        {
          for (byte i=0; i<2; i++)
          {
            if (bitRead(buttonEventWasHolding[i + 1], x))
            {
              if (holdingNote == 0) holdingNote = stepsData[x + (i * 8)].noteSteps[curTrack-DRUM_TRACKS][editVariation];
              else
              {
                holdingNote = 0;
                break;
              }
            }
          }
        }
        //
        if (holdingNote == 0) printNumber(0, 1, editingNoteTranspose); 
        else 
        {
          segments[0][2] = 0;
          printNumber(0, 1, holdingNote);
        }
      }
    }
    else if (configData.midiClockInternal) 
    {
      segments[0][0] = S_b;
      printNumber(0, 1, configData.BPM);
    }
    else
    {
      segments[0][0] = S_b;
      segments[0][1] = S_E;
      segments[0][2] = S_X;
      segments[0][3] = S_T;
    }
    #if RECORD_ENABLED
      if (recordEnabled) segments[0][0] = S_R;
    #endif
    if (showBPMdot) segments[0][3] |= B10000000;
    segments[0][4] = S_P;
    if (nextPattern != currentPattern) printNumber(0, 5, nextPattern + 1); else printNumber(0, 5, currentPattern + 1);
    segments[0][5] = S_T;
    if (streamNextPattern || loadPatternNow) segments[0][7] |= B10000000;
  }
  //
  resetSegments(1, 2);
  //
  if (temporaryMessageCounter > 0)
  {
    if (showTemporaryMessage == kMemoryProtectMessage) showMemoryProtected();
    else if (showTemporaryMessage == kPatternRepeatMessage)
    {
      segments[1][0] = S_R;
      segments[1][1] = S_E;
      segments[1][2] = S_P;
      segments[1][3] = S_E;    
      segments[1][4] = S_A;    
      segments[1][5] = S_T;    
      //
      if (repeatMode == kRepeatModeNormal)
      {
        segments[2][1] = S_N;
        segments[2][2] = S_O;
        segments[2][3] = S_R;
        segments[2][4] = S_N;
        segments[2][5] = S_N;
        segments[2][6] = S_A;
        segments[2][7] = S_L;
      }
      else if (repeatMode == kRepeatModeChain)
      {
        segments[2][3] = S_C;
        segments[2][4] = S_H;
        segments[2][5] = S_A;
        segments[2][6] = S_I;
        segments[2][7] = S_N;
      }
      else
      {
        segments[2][2] = S_P;
        segments[2][3] = S_A;
        segments[2][4] = S_T;
        segments[2][5] = S_E;
        segments[2][6] = S_R;
        segments[2][7] = S_N;        
      }
    }
    //
    temporaryMessageCounter++;
    if (temporaryMessageCounter >= TEMPORARY_MESSAGE_TIME)  temporaryMessageCounter = 0;
    return;
  }
  //
  // Right Screen --------------------------------------------------------------------------------------------------------------------- //
  if (curRightScreen == kRightMenuCopyPaste)
  {
    showMenuCopyPaste();
  }
  else if (curRightScreen == kRightMenu)
  {
    showMenu();
  }
  else if (curRightScreen == kRightSteps || 
      curRightScreen == kRightTrackSelection ||
      curRightScreen == kRightABCDVariations ||
      curRightScreen == kRightPatternSelection)
  {
    //
    #if TRACK_DEBUG_MIDI
      if (curRightScreen == kRightTrackSelection)
      {
        printNumber2(1, 3, lastMIDIinput[3]);
        printNumber2(1, 7, lastMIDIinput[2]);
        printNumber2(2, 3, lastMIDIinput[1]);
        printNumber2(2, 7, lastMIDIinput[0]);
        return;
      }
    #endif
    //
    byte xVar = variation;
    if (forceVariation >= 0) xVar = forceVariation;
    //  
    if (curTrack < DRUM_TRACKS)
    {
      for (byte xs=0; xs < 2; xs++)
      {
        for (byte x=0; x < 8; x++)
        {       
          getStepVelocity(x + (xs*8), curTrack, xVar, false);
          //
          if (curTrack == 15)
          {
            segments[xs + 1][x] = (char)pgm_read_word(&stepChars[stepVelocity]);
          }
          else
          {
            if (stepVelocity == 1) // use accent track
            {
              getStepVelocity(x + (xs*8), 15, xVar, false);
              segments[xs + 1][x] = (char)pgm_read_word(&stepChars[stepVelocity]);
            }
            else if (stepVelocity == 3)
            {
              segments[xs + 1][x] = (char)pgm_read_word(&stepChars[3]);
            }
            else if (stepVelocity == 2)
            {
              segments[xs + 1][x] = (char)pgm_read_word(&stepChars[1]);
            }
            //
            if (bitRead(stepsData[x + (xs*8)].stepsDouble[xVar], curTrack)) segments[xs + 1][x] |= B10000000;
          }
        }
      }
    }
    else // NOTE TRACKS ---===----==---==-- //
    {
      if (editingNote)
      {
        getStepVelocity(editStep, curTrack, editVariation, false);
        byte xm = bitRead(stepsData[editStep].noteStepsExtras[curTrack-DRUM_TRACKS][1], 1 + (editVariation * 2)) << 1;
        xm |= bitRead(stepsData[editStep].noteStepsExtras[curTrack-DRUM_TRACKS][1], 0 + (editVariation * 2));
        //
        if (xm == 3) showOnOrOff(false);
        else if (stepsData[editStep].noteSteps[curTrack-DRUM_TRACKS][editVariation] > 0 && stepVelocity != 0)
        {
          printMIDInote(stepsData[editStep].noteSteps[curTrack-DRUM_TRACKS][editVariation], 2, 0, 3);
          segments[2][6] = S_U;
          segments[2][7] = (char)pgm_read_word(&numbers[stepVelocity]);
          //
          if (xm == 1) 
          {
            segments[1][0] = S_G;
            segments[1][1] = S_L;
            segments[1][2] = S_d;
            segments[1][3] = S_E;
          }
          else if (xm == 2) 
          {
            segments[1][0] = S_d;
            segments[1][1] = S_b;
            segments[1][2] = S_L;
            segments[1][3] = S_E;
          }
        }
        else
        {
          for (byte xs=0; xs < 8; xs++) { segments[2][xs + 8] = B01000000; }
        }
      }
      else // ----------------------------------------------------------------------------------- //
      {
        for (byte xs=0; xs < 2; xs++)
        {
          for (byte x=0; x < 8; x++)
          {
            byte xm = bitRead(stepsData[x + (xs*8)].noteStepsExtras[curTrack-DRUM_TRACKS][1], 1 + (xVar * 2)) << 1;
            xm |= bitRead(stepsData[x + (xs*8)].noteStepsExtras[curTrack-DRUM_TRACKS][1], 0 + (xVar * 2));
            getStepVelocity(x + (xs*8), curTrack, xVar, false);
            //
            if (xm != 3 && stepsData[x + (xs*8)].noteSteps[curTrack-DRUM_TRACKS][xVar] > 0 && stepVelocity != 0)
              segments[xs + 1][x] = (char)pgm_read_word(&stepChars[stepVelocity]);
            //
            if (xm == 1) segments[xs + 1][x] |= B01000000;
            else if (xm == 2) segments[xs + 1][x] |= B10000000;
            else if (xm == 3) segments[xs + 1][x] |= B01000000;            
          }
        }       
      }
    }
  }
}

/*
 * 0 Play
 * 1 Stop
 * 2 (-) >
 * 3 (+) > change tempo or any Menu option
 * 4 Force Accent
 * 5 ABCD -> Mirror Mode
 * 6 Pattern Selection  >
 * 7 Track Selection    > Both = Menu
 */

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void checkInterface()
{
  if (!somethingHappened) return;
  somethingHappened = false;
  //
  if (curRightScreen == kMuteMenu)
  {
    checkInterfaceMute();
    return;
  }
  //
  // Start Sequencer //
  if (buttonEvent[0][0] >= kButtonClicked)
  {
    if (seqPlaying && buttonEvent[0][0] == kButtonClicked) 
    {
      if (temporaryMessageCounter != 0)
      {
        repeatMode++;
        if (repeatMode > kRepeatModePattern) repeatMode = kRepeatModeNormal;
      }
      ShowTemporaryMessage(kPatternRepeatMessage);
      buttonEvent[0][0] = 0;
      return;
    }
    //    
    #if RECORD_ENABLED
      if (buttonEvent[0][0] == kButtonClicked)
      {
        if (configData.midiClockInternal) startSequencer(false);
      }
      else if (buttonEvent[0][0] == kButtonHold)
      {
        recordEnabled = !recordEnabled;
        if (!seqPlaying && configData.midiClockInternal) startSequencer(false);
      }
    #else
      if (configData.midiClockInternal) startSequencer(false);
    #endif
    //
    buttonEvent[0][0] = 0;
  }
  //
  // Stop Sequencer //
  if (buttonEvent[0][1] >= kButtonClicked)
  {
    if (buttonEvent[0][1] == kButtonHold)
    {
        if (configData.midiClockInternal) stopSequencer();
    }
    buttonEvent[0][1] = 0;
    editingNoteTranspose = -127;
  }
  //
  // (-) BPM or Menu  // ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
  if (buttonEvent[0][2] >= kButtonClicked)
  {
    if (buttonEvent[0][2] == kButtonClicked)
    {
      if (editingNoteTranspose != -127 && curRightScreen == kRightSteps && curTrack >= DRUM_TRACKS)
      {
        bool foundOne = false;
        for (byte x=0; x<8; x++)
        {
          for (byte i=0; i<2; i++)
          {
            if ((noteTransposeEditAllSteps || bitRead(buttonEventWasHolding[i + 1], x)))
            {
              for (byte xp = 0; xp < 4; xp++)
              {
                if (!mirror) xp = editVariation;
                if (stepsData[x + (i * 8)].noteSteps[curTrack-DRUM_TRACKS][xp] > 0)
                {
                  stepsData[x + (i * 8)].noteSteps[curTrack-DRUM_TRACKS][xp]--;
                  patternData.lastNote[curTrack-DRUM_TRACKS] = stepsData[x + (i * 8)].noteSteps[curTrack-DRUM_TRACKS][xp];
                  //
                  if (!foundOne && editingNoteTranspose > -125) editingNoteTranspose--;
                  foundOne = true;                  
                }
                //
                if (!mirror) break;
              }
              //
              noteTransposeWasChanged = true;
            }
          }
        }
      }
      else if (curRightScreen == kRightMenu)
      {
        processMenu(-1);
      }
      else
      {
        if (configData.BPM > 10) 
        { 
          configData.BPM--;
          somethingChangedConfig = true;
        }
        updateSequencerSpeed(false);
      }
      updateScreen = true;
    }
    buttonEvent[0][2] = 0;
  }
  // (+) BPM or Menu  // ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
  if (buttonEvent[0][3] >= kButtonClicked)
  {
    if (buttonEvent[0][3] == kButtonClicked)
    {
      if (editingNoteTranspose != -127 && curRightScreen == kRightSteps && curTrack >= DRUM_TRACKS)
      {
        bool foundOne = false;
        for (byte x=0; x<8; x++)
        {
          for (byte i=0; i<2; i++)
          {
            if ((noteTransposeEditAllSteps || bitRead(buttonEventWasHolding[i + 1], x)))
            {
              for (byte xp = 0; xp < 4; xp++)
              {
                if (!mirror) xp = editVariation;
                if (stepsData[x + (i * 8)].noteSteps[curTrack-DRUM_TRACKS][xp] < 127)
                {
                  stepsData[x + (i * 8)].noteSteps[curTrack-DRUM_TRACKS][xp]++;
                  patternData.lastNote[curTrack-DRUM_TRACKS] = stepsData[x + (i * 8)].noteSteps[curTrack-DRUM_TRACKS][xp];
                  //
                  if (!foundOne && editingNoteTranspose < 127) editingNoteTranspose++;
                  foundOne = true;                  
                }
                //
                if (!mirror) break;
              }
              //
              noteTransposeWasChanged = true;
            }
          }
        }
      }
      else if (curRightScreen == kRightMenu)
      {
        processMenu(1);
      }
      else
      {
        if (configData.BPM < 255)
        {
          configData.BPM++;
          somethingChangedConfig = true;
        }
        updateSequencerSpeed(false);
      }
      updateScreen = true;
    }
    buttonEvent[0][3] = 0;
  }
  //
  if (curRightScreen == kRightSteps) // Regular Steps Editing // ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
  {
    byte xVar = variation;
    if (forceVariation >= 0) xVar = forceVariation;
    if (mirror) xVar = 0;
    //  
    if (curTrack < DRUM_TRACKS)
    {
      for (byte x=0; x<8; x++)
      {
        for (byte i=0; i<2; i++)
        {
          if (buttonEvent[i + 1][x] > kButtonNone)
          {
            getStepVelocity(x + (i*8), curTrack, xVar, (buttonEvent[i + 1][x] == kButtonClicked));
            //
            if (buttonEvent[i + 1][x] == kButtonClicked)
            {
              somethingChangedPattern = true;
              //
              if (curTrack == 15)
              {
                if (stepVelocity == 0) stepVelocity = 1; 
                else if (stepVelocity == 1) stepVelocity = 2;
                else if (stepVelocity == 2) stepVelocity = 3;
                else if (stepVelocity == 3) stepVelocity = 1;
                stepsData[x + (i*8)].steps[curTrack] |= stepVelocity << (xVar * 2);
              }
              else
              {
                if (forceAccent)
                {
                  if (stepVelocity == 3) stepVelocity = 2;
                  else if (stepVelocity == 2) stepVelocity = 1;
                  else stepVelocity = 3;
                }
                else
                {
                  if (stepVelocity == 0) stepVelocity = 1; else stepVelocity = 0;
                }
                stepsData[x + (i*8)].steps[curTrack] |= stepVelocity << (xVar * 2);
              }
              //
              if (mirror)
              {
                stepsData[x + (i*8)].steps[curTrack] = 0x00;
                stepsData[x + (i*8)].steps[curTrack] |= stepVelocity;
                stepsData[x + (i*8)].steps[curTrack] |= stepVelocity << 2;
                stepsData[x + (i*8)].steps[curTrack] |= stepVelocity << 4;
                stepsData[x + (i*8)].steps[curTrack] |= stepVelocity << 6;
              }
            }
            else if (buttonEvent[i + 1][x] == kButtonHold && curTrack != 15)
            {
              somethingChangedPattern = true;
              //
              if (bitRead(stepsData[x + (i*8)].stepsDouble[xVar], curTrack))
              {
                bitClear(stepsData[x + (i*8)].stepsDouble[xVar], curTrack);
                if (mirror)
                {
                  bitClear(stepsData[x + (i*8)].stepsDouble[1], curTrack);
                  bitClear(stepsData[x + (i*8)].stepsDouble[2], curTrack);
                  bitClear(stepsData[x + (i*8)].stepsDouble[3], curTrack);
                }
              }
              else
              {
                bitSet(stepsData[x + (i*8)].stepsDouble[xVar], curTrack);
                if (mirror)
                {
                  bitSet(stepsData[x + (i*8)].stepsDouble[1], curTrack);
                  bitSet(stepsData[x + (i*8)].stepsDouble[2], curTrack);
                  bitSet(stepsData[x + (i*8)].stepsDouble[3], curTrack);
                }
              }
            }
            //
            buttonEvent[i + 1][x] = 0;
            updateScreen = true;
          }
        }
      }
    }
    else // NOTE TRACKS // ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
    {
      for (byte x=0; x<8; x++)
      {
        for (byte i=0; i<2; i++)
        {
          if (buttonEvent[i + 1][x] > kButtonNone)
          {
            if (editingNote)
            {
              if (i == 1 && x == 7)  // Ok, close //
              {
                if (buttonEvent[i + 1][x] == kButtonClicked) editingNote = false;
              }
              else if (i == 1 && x == 6) // Octave
              {
                if (buttonEvent[i + 1][x] == kButtonClicked)
                {
                  uint16_t xNote = uint16_t(stepsData[editStep].noteSteps[curTrack-DRUM_TRACKS][editVariation]) + 12;
                  if (xNote > 108) xNote = uint16_t(stepsData[editStep].noteSteps[curTrack-DRUM_TRACKS][editVariation]) - ((uint16_t(stepsData[editStep].noteSteps[curTrack-DRUM_TRACKS][editVariation]) / 12) * 12);
                  if (xNote <= 0) xNote = 12;
                  stepsData[editStep].noteSteps[curTrack-DRUM_TRACKS][editVariation] = byte(xNote & B01111111);
                  patternData.lastNote[curTrack-DRUM_TRACKS] = stepsData[editStep].noteSteps[curTrack-DRUM_TRACKS][editVariation];
                  checkIfMirrorAndCopy(editStep, curTrack-DRUM_TRACKS);
                }
              }
              else if (i == 1 && x == 5) // Velocity
              {
                if (buttonEvent[i + 1][x] == kButtonClicked)
                {
                  getStepVelocity(editStep, curTrack, editVariation, true);
                  getNoteStepGlideDoubleOff(editStep, curTrack-DRUM_TRACKS, editVariation, true);
                  //
                  if (noteStepGlideDoubleOff == 3) 
                  {
                    noteStepGlideDoubleOff = 0;
                    stepVelocity = 1;
                  }
                  else if (stepVelocity == 1) stepVelocity = 2;
                  else if (stepVelocity == 2) stepVelocity = 3;
                  else noteStepGlideDoubleOff = 3;
                  lastVelocity = stepVelocity;
                  //
                  stepsData[editStep].noteStepsExtras[curTrack-DRUM_TRACKS][0] |= stepVelocity << (editVariation * 2);
                  stepsData[editStep].noteStepsExtras[curTrack-DRUM_TRACKS][1] |= noteStepGlideDoubleOff << (editVariation * 2); 
                  checkIfMirrorAndCopy(editStep, curTrack-DRUM_TRACKS);
                }
              }
              else if (i == 1 && x == 4) // Type (Glide or NoteOff / Hold=Double)
              {
                if (buttonEvent[i + 1][x] == kButtonClicked)
                {
                  getNoteStepGlideDoubleOff(editStep, curTrack-DRUM_TRACKS, editVariation, true);
                  //
                  if (noteStepGlideDoubleOff == 0) noteStepGlideDoubleOff = 1;
                  else if (noteStepGlideDoubleOff == 1) noteStepGlideDoubleOff = 2;
                  else if (noteStepGlideDoubleOff == 2) noteStepGlideDoubleOff = 3;
                  else noteStepGlideDoubleOff = 0;
                  //
                  stepsData[editStep].noteStepsExtras[curTrack-DRUM_TRACKS][1] |= noteStepGlideDoubleOff << (editVariation * 2); 
                  checkIfMirrorAndCopy(editStep, curTrack-DRUM_TRACKS);
                }
              }
              else // The other first 12 buttons for notes //
              {
                if (buttonEvent[i + 1][x] == kButtonClicked)
                {
                  byte xOct = stepsData[editStep].noteSteps[curTrack-DRUM_TRACKS][editVariation] / 12;
                  stepsData[editStep].noteSteps[curTrack-DRUM_TRACKS][editVariation] = (xOct * 12) + (x + (i * 8));
                  patternData.lastNote[curTrack-DRUM_TRACKS] = stepsData[editStep].noteSteps[curTrack-DRUM_TRACKS][editVariation];
                  checkIfMirrorAndCopy(editStep, curTrack-DRUM_TRACKS);
                }
              }
            }
            else // Not Editing Note // ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
            {
              somethingChangedPattern = true;
              //
              editStep = x + (i * 8);
              editVariation = xVar;
              getStepVelocity(editStep, curTrack, editVariation, false);
              //
              if (buttonEvent[i + 1][x] == kButtonClicked)
              {
                if (stepsData[editStep].noteSteps[curTrack-DRUM_TRACKS][editVariation] == 0 && patternData.lastNote[curTrack-DRUM_TRACKS] == 0)
                {
                  clearStepsExtrasBits(editStep, editVariation, curTrack-DRUM_TRACKS);
                  editingNote = true;
                  patternData.lastNote[curTrack-DRUM_TRACKS] = 60;
                  stepsData[editStep].noteSteps[curTrack-DRUM_TRACKS][editVariation] = patternData.lastNote[curTrack-DRUM_TRACKS];
                  stepsData[editStep].noteStepsExtras[curTrack-DRUM_TRACKS][0] |= lastVelocity << (editVariation * 2); 
                }
                else if (stepsData[editStep].noteSteps[curTrack-DRUM_TRACKS][editVariation] == 0 && patternData.lastNote[curTrack-DRUM_TRACKS] > 0)
                {
                  clearStepsExtrasBits(editStep, editVariation, curTrack-DRUM_TRACKS);
                  stepsData[editStep].noteSteps[curTrack-DRUM_TRACKS][editVariation] = patternData.lastNote[curTrack-DRUM_TRACKS];
                  stepsData[editStep].noteStepsExtras[curTrack-DRUM_TRACKS][0] |= lastVelocity << (editVariation * 2);
                }
                else if (forceAccent)
                {
                  getStepVelocity(editStep, curTrack, editVariation, true);
                  getNoteStepGlideDoubleOff(editStep, curTrack-DRUM_TRACKS, editVariation, true);
                  //
                  if (noteStepGlideDoubleOff == 3) 
                  {
                    noteStepGlideDoubleOff = 0;
                    stepVelocity = 1;
                  }
                  else if (stepVelocity == 1) stepVelocity = 2;
                  else if (stepVelocity == 2) stepVelocity = 3;
                  else noteStepGlideDoubleOff = 3;
                  lastVelocity = stepVelocity;
                  //
                  stepsData[editStep].noteStepsExtras[curTrack-DRUM_TRACKS][0] |= stepVelocity << (editVariation * 2);
                  stepsData[editStep].noteStepsExtras[curTrack-DRUM_TRACKS][1] |= noteStepGlideDoubleOff << (editVariation * 2); 
                  checkIfMirrorAndCopy(editStep, curTrack-DRUM_TRACKS);
                }               
                else if (stepsData[editStep].noteSteps[curTrack-DRUM_TRACKS][editVariation] > 0)
                {
                  getNoteStepGlideDoubleOff(editStep, curTrack-DRUM_TRACKS, editVariation, false);
                  //
                  if (noteStepGlideDoubleOff == 3)
                  {
                    bitClear(stepsData[editStep].noteStepsExtras[curTrack-DRUM_TRACKS][1], 1 + (editVariation * 2));
                    bitClear(stepsData[editStep].noteStepsExtras[curTrack-DRUM_TRACKS][1], 0 + (editVariation * 2));                                     
                    bitClear(stepsData[editStep].noteStepsExtras[curTrack-DRUM_TRACKS][0], 1 + (editVariation * 2));
                    bitClear(stepsData[editStep].noteStepsExtras[curTrack-DRUM_TRACKS][0], 0 + (editVariation * 2));                 
                  }
                  else if (stepVelocity == 0)
                  {
                    bitSet(stepsData[editStep].noteStepsExtras[curTrack-DRUM_TRACKS][0], 1 + (editVariation * 2));
                    bitSet(stepsData[editStep].noteStepsExtras[curTrack-DRUM_TRACKS][0], 0 + (editVariation * 2));
                  }
                  else
                  {
                    bitClear(stepsData[editStep].noteStepsExtras[curTrack-DRUM_TRACKS][0], 1 + (editVariation * 2));
                    bitClear(stepsData[editStep].noteStepsExtras[curTrack-DRUM_TRACKS][0], 0 + (editVariation * 2));                 
                  }
                }
                //
                checkIfMirrorAndCopy(editStep, curTrack-DRUM_TRACKS);
              }
              else if (buttonEvent[i + 1][x] == kButtonRelease)
              {
                editingNoteTranspose = -127;
              }
              else if (buttonEvent[i + 1][x] == kButtonReleaseNothingClicked && !forceAccent)
              {
                if (!noteTransposeWasChanged)
                {
                  editingNote = true;
                  //
                  getStepVelocity(editStep, curTrack, editVariation, false);
                  //
                  if (stepsData[editStep].noteSteps[curTrack-DRUM_TRACKS][editVariation] > 0 && stepVelocity == 0)
                  {
                    clearStepsExtrasBits(editStep, editVariation, curTrack-DRUM_TRACKS);
                    //
                    if (patternData.lastNote[curTrack-DRUM_TRACKS] == 0) patternData.lastNote[curTrack-DRUM_TRACKS] = 60;
                    stepsData[editStep].noteSteps[curTrack-DRUM_TRACKS][editVariation] = patternData.lastNote[curTrack-DRUM_TRACKS];
                    stepsData[editStep].noteStepsExtras[curTrack-DRUM_TRACKS][0] |= lastVelocity << (editVariation * 2);
                    //
                    checkIfMirrorAndCopy(editStep, curTrack-DRUM_TRACKS);
                    editingNote = false;
                  }
                  else if (stepsData[editStep].noteSteps[curTrack-DRUM_TRACKS][editVariation] == 0)
                  {
                    clearStepsExtrasBits(editStep, editVariation, curTrack-DRUM_TRACKS);
                    //
                    if (patternData.lastNote[curTrack-DRUM_TRACKS] == 0) patternData.lastNote[curTrack-DRUM_TRACKS] = 60;
                    stepsData[editStep].noteSteps[curTrack-DRUM_TRACKS][editVariation] = patternData.lastNote[curTrack-DRUM_TRACKS];
                    stepsData[editStep].noteStepsExtras[curTrack-DRUM_TRACKS][0] |= lastVelocity << (editVariation * 2);
                    //
                    checkIfMirrorAndCopy(editStep, curTrack-DRUM_TRACKS);
                    editingNote = false;
                  }
                  else
                  {
                    getNoteStepGlideDoubleOff(editStep, curTrack-DRUM_TRACKS, editVariation, false);
                    if (noteStepGlideDoubleOff == 3) editingNote = false;
                  }
                }
                //
                editingNoteTranspose = -127;
              }
            }
            buttonEvent[i + 1][x] = 0;
            updateScreen = true;
          }
        }
      }      
    }
  }
  //
  // ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
  // ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
  // ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
  //
  if (curRightScreen == kRightTrackSelection ||
      curRightScreen == kRightABCDVariations ||
      curRightScreen == kRightPatternSelection ||
      curRightScreen == kRightMenuCopyPaste ||
      curRightScreen == kRightMenu)
  {
    char leButton = -1;
    for (byte x=0; x<8; x++)
    {
      for (byte i=0; i<2; i++)
      {
        if (buttonEvent[i + 1][x] == kButtonClicked)
        {
          buttonEvent[i + 1][x] = 0;
          updateScreen = true;
          leButton = x + (i * 8);
        }
      }
    }
    //
    if (leButton >= 0)
    {
      #if HAS_SYSEX
        if (curRightScreen == kRightMenu && (menuPosition == menuInit || menuPosition == menuSysex))
        {
          if (menuPosition == menuSysex && leButton != 15)
          {
            if (sysExDump == 0) sysExDump = 1; else sysExDump = 0;
          }
          else processMenuOK();
        }
      #else
        if (curRightScreen == kRightMenu && menuPosition == menuInit)
        {
          processMenuOK();
        }
      #endif
      else if (curRightScreen == kRightTrackSelection ||  curRightScreen == kRightMenu)
      {
        editingNote = false;
        //
        if (selectingDrumTracks && curTrack == leButton) selectingDrumTracks = false;
        else if (!selectingDrumTracks && (curTrack-DRUM_TRACKS) == leButton) selectingDrumTracks = true;
        //
        if (selectingDrumTracks) curTrack = leButton; 
        else if (leButton < NOTE_TRACKS) curTrack = leButton + DRUM_TRACKS;
      }
      else if (curRightScreen == kRightABCDVariations)
      {
        if (leButton <= 3) forceVariation = leButton; else forceVariation = -1;
        mirror = false;
      }
      else if (curRightScreen == kRightMenuCopyPaste)
      {
        processMenuCopyPaste(leButton);
      }
      else if (curRightScreen == kRightPatternSelection)
      {
        checkIfDataNeedsSaving();
        if (leButton < 8) patternBitsSelector &= 0xFF00; else patternBitsSelector &= 0xFF;
        bitSet(patternBitsSelector, leButton);
        //
        nextPattern = 0;
        for (byte x = 0; x < 8; x++)
        {
          if (bitRead(patternBitsSelector, x + 8)) nextPattern += x;
          if (bitRead(patternBitsSelector, x)) nextPattern += (x * 8);
        }
        if (currentPattern != nextPattern) loadPattern(nextPattern);
      }
    }
  }
  //  
  // ---===------===------===------===------===------===------===------===---
  //
  if (curRightScreen == kRightMenu) buttonEvent[0][5] = 0;
  else
  {
    if (buttonEvent[0][5] == kButtonClicked)
    {
      checkMenuClose();
      buttonEvent[0][5] = 0;
      updateScreen = true;
      //
      if (forceAccent)
      {
        curRightScreen = kMuteMenu;
        forceAccent = false;
      }
      else
      {
        if (forceVariation >= 0)
        {
          mirror = false;
          forceVariation = -1;    
        }
        else
        {
          mirror = !mirror;
          forceVariation = -1;
        }
      }
    }
    else if (buttonEvent[0][5] == kButtonHold)
    {
      checkMenuClose();
      buttonEvent[0][5] = 0;
      curRightScreen = kRightABCDVariations;
    }
    else if (buttonEvent[0][5] > kButtonHold)
    {
      updateScreen = true;
      buttonEvent[0][5] = 0;
      curRightScreen = kRightSteps;
    }
  }
  //
  // ----------============------------
  //
  if (buttonEvent[0][7] == kButtonClicked)
  {
    updateScreen = true;
    buttonEvent[0][7] = 0;
  }
  else if (buttonEvent[0][7] == kButtonHold)
  {
    updateScreen = true;
    buttonEvent[0][7] = 0;
    if (curRightScreen == kRightMenu)
    {
      menuPosition++;
      initMode = 0;
      if (menuPosition > lastMenu) menuPosition = menuFirst;
    }
    else if (forceAccent)
    {
      curRightScreen = kRightMenu;
      initMode = menuPosition = 0;
    }
    else if (editingNoteTranspose != -127)
    {
      noteTransposeEditAllSteps = !noteTransposeEditAllSteps;
    }
    else curRightScreen = kRightTrackSelection;
  }
  else if (buttonEvent[0][7] > kButtonHold)
  {
    if (curRightScreen == kRightTrackSelection) curRightScreen = kRightSteps;
    updateScreen = true;
    buttonEvent[0][7] = 0;
  }
  //
  // ----------============------------
  //
  if (buttonEvent[0][4] == kButtonHold)
  {
    checkMenuClose();
    buttonEvent[0][4] = 0;
    forceAccent = true;
  }
  else if (buttonEvent[0][4] > kButtonHold ||
           buttonEvent[0][4] == kButtonClicked)
  {
    forceAccent = false;
    buttonEvent[0][4] = 0;
    updateScreen = true;
  }
  //
  // ----------============------------
  //
  if (buttonEvent[0][6] == kButtonHold)
  {
    if (curRightScreen == kRightSteps) curRightScreen = kRightPatternSelection;
    buttonEvent[0][6] = 0;
    updateScreen = true;
  }
  else if (buttonEvent[0][6] > kButtonHold ||
           buttonEvent[0][6] == kButtonClicked)
  {
    if (curRightScreen == kRightMenu)
    {
      if (menuPosition > menuFirst) menuPosition--;
      else menuPosition = lastMenu;
      initMode = 0;
    }    
    else if (forceAccent)
    {
      curRightScreen = kRightMenuCopyPaste;
    }
    else
    {
      if (buttonEvent[0][6] == kButtonReleaseNothingClicked) changePattern(1);
      curRightScreen = kRightSteps;
    }
    //
    buttonEvent[0][6] = 0;
    updateScreen = true;
  }
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void checkIfMirrorAndCopy(byte thestep, byte track)
{
  if (mirror)
  {
    stepsData[thestep].noteSteps[track][1] = stepsData[thestep].noteSteps[track][0];
    stepsData[thestep].noteSteps[track][2] = stepsData[thestep].noteSteps[track][0];
    stepsData[thestep].noteSteps[track][3] = stepsData[thestep].noteSteps[track][0];
    //
    stepsData[thestep].noteStepsExtras[track][0] &= B00000011;
    stepsData[thestep].noteStepsExtras[track][0] |= stepsData[thestep].noteStepsExtras[track][0] << 2;
    stepsData[thestep].noteStepsExtras[track][0] |= stepsData[thestep].noteStepsExtras[track][0] << 4;
    stepsData[thestep].noteStepsExtras[track][0] |= stepsData[thestep].noteStepsExtras[track][0] << 6;
    //
    stepsData[thestep].noteStepsExtras[track][1] &= B00000011;
    stepsData[thestep].noteStepsExtras[track][1] |= stepsData[thestep].noteStepsExtras[track][1] << 2;
    stepsData[thestep].noteStepsExtras[track][1] |= stepsData[thestep].noteStepsExtras[track][1] << 4;
    stepsData[thestep].noteStepsExtras[track][1] |= stepsData[thestep].noteStepsExtras[track][1] << 6;
  }    
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void clearStepsExtrasBits(byte thestep, byte xVar, byte track)
{
  bitClear(stepsData[thestep].noteStepsExtras[track][0], (editVariation * 2));
  bitClear(stepsData[thestep].noteStepsExtras[track][0], (editVariation * 2) + 1);
  bitClear(stepsData[thestep].noteStepsExtras[track][1], (editVariation * 2));
  bitClear(stepsData[thestep].noteStepsExtras[track][1], (editVariation * 2) + 1);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void resetSegments(byte xs, byte xe)
{
  for (byte xx = xs; xx <= xe; xx++) { memset(segments[xx], 0, sizeof(segments[0])); }
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void printStopSequencer()
{
  leds[0] = leds[1] = leds[2] = 0;
  resetSegments(1, 2);
  segments[1][4] = S_S;
  segments[1][5] = S_T;
  segments[1][6] = S_O;
  segments[1][7] = S_P;
  //
  segments[2][0] = S_S;
  segments[2][1] = S_E;
  segments[2][2] = S_O;
  segments[2][3] = S_U;
  segments[2][4] = S_E;
  segments[2][5] = S_N;
  segments[2][6] = S_C;
  segments[2][7] = S_R;
  //
  sendScreen();
  waitMs(2000);
  updateScreen = true;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void getStepVelocity(byte theStep, byte track, byte variation, bool cleanBits)
{
  if (track < DRUM_TRACKS)
  {
    stepVelocity = bitRead(stepsData[theStep].steps[track], 1 + (variation * 2)) << 1;
    stepVelocity |= bitRead(stepsData[theStep].steps[track], 0 + (variation * 2));  
  }
  else
  {
    stepVelocity = bitRead(stepsData[theStep].noteStepsExtras[track-DRUM_TRACKS][0], 1 + (variation * 2)) << 1;
    stepVelocity |= bitRead(stepsData[theStep].noteStepsExtras[track-DRUM_TRACKS][0], 0 + (variation * 2));
  }
  //
  if (cleanBits)
  {
    if (track < DRUM_TRACKS)
    {
      bitClear(stepsData[theStep].steps[track], 1 + (variation * 2));
      bitClear(stepsData[theStep].steps[track], 0 + (variation * 2));   
    }
    else
    {
      bitClear(stepsData[theStep].noteStepsExtras[track-DRUM_TRACKS][0], (variation * 2));
      bitClear(stepsData[theStep].noteStepsExtras[track-DRUM_TRACKS][0], (variation * 2) + 1);                  
    }
  }
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void getNoteStepGlideDoubleOff(byte theStep, byte track, byte variation, bool cleanBits)
{
  noteStepGlideDoubleOff  = bitRead(stepsData[theStep].noteStepsExtras[track][1], 1 + (variation * 2)) << 1;
  noteStepGlideDoubleOff |= bitRead(stepsData[theStep].noteStepsExtras[track][1], 0 + (variation * 2));
  //
  if (cleanBits)
  {
    bitClear(stepsData[theStep].noteStepsExtras[track][1], (variation * 2));
    bitClear(stepsData[theStep].noteStepsExtras[track][1], (variation * 2) + 1);                  
  }
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char stepBeenHold()
{
  char holdingNote = -2;
  for (byte x=0; x<8; x++)
  {
    for (byte i=0; i<2; i++)
    {
      if (bitRead(buttonEventWasHolding[i + 1], x))
      {
        if (holdingNote == -2) holdingNote = stepsData[x + (i * 8)].noteSteps[curTrack-DRUM_TRACKS][editVariation]; else return -1;
      }
    }
  }
  return holdingNote;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ShowTemporaryMessage(byte message)
{
  showTemporaryMessage = message;
  temporaryMessageCounter = 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void showOnOrOff(bool showOn)
{
  if (showOn)
  {
    segments[2][6] = S_O;
    segments[2][7] = S_N;
  }
  else
  {
    segments[2][5] = S_O;
    segments[2][6] = S_F;
    segments[2][7] = S_F;
  }
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void showMemoryProtected()
{
  segments[1][0] = S_P;
  segments[1][1] = S_R;
  segments[1][2] = S_O;
  segments[1][3] = S_T;
  segments[1][4] = S_E;
  segments[1][5] = S_C;
  segments[1][6] = S_T;
  //
  showOnOrOff(configData.writeProtectFlash);
}
