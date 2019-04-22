/*

   Created by William Kalfelz @ Beat707 (c) 2018 - http://www.Beat707.com

*/

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void showMenuCopyPaste()
{
  // 12341234 12341234
  // PAT COPY PST INIT
  //
  segments[1][0] = S_P;
  segments[1][1] = S_A;
  segments[1][2] = S_T;
  //
  segments[1][4] = S_C;
  segments[1][5] = S_O;
  segments[1][6] = S_P;
  segments[1][7] = S_Y;
  //
  segments[2][0] = S_P;
  segments[2][1] = S_S;
  segments[2][2] = S_T;
  //
  segments[2][4] = S_I;
  segments[2][5] = S_n;
  segments[2][6] = S_I;
  segments[2][7] = S_T;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void processMenuCopyPaste(byte button)
{
  if (button >= 4 && button <= 7) // Copy
  {
    uint16_t pagePos = 16 + (PT_BANKS * ((64 * 16) + 16));
    eraseSector(pagePos);
    if (!flash.writeAnything(pagePos, (uint8_t) 0, patternData)) showErrorMsg(flash.error());
    pagePos++;
    if (!flash.writeAnything(pagePos, (uint8_t) 0, stepsData)) showErrorMsg(flash.error());

  }
  else if (button >= 8 && button <= 11) // Paste
  {
    uint16_t pagePos = 16 + (PT_BANKS * ((64 * 16) + 16));
    if (!flash.readAnything(pagePos, (uint8_t) 0, patternData)) showErrorMsg(flash.error());
    pagePos++;
    if (!flash.readAnything(pagePos, (uint8_t) 0, stepsData)) showErrorMsg(flash.error());
    somethingChangedPattern = true;
  }
  else if (button >= 12 && button <= 15) // Init
  {
    for (byte xs = 0; xs < STEPS; xs++) { stepsData[xs].init(false); }
    somethingChangedPattern = true;
  }
  //
  curRightScreen = kRightSteps;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void showMenu()
{
  switch (menuPosition)
  {
    case menuMidiCC:
      segments[1][0] = S_N;
      segments[1][1] = S_N;
      segments[1][2] = S_I;
      segments[1][3] = S_d;
      segments[1][4] = S_I;
      //
      segments[1][6] = S_C;
      segments[1][7] = S_H;
      //
      if (curTrack != 15) printNumber(2, 5, configData.trackMidiCH[curTrack] + 1);
      else printDashDash(2, 6);
      break;
      //
#if RECORD_ENABLED
    case menuRecord:
      segments[1][0] = S_R;
      segments[1][1] = S_E;
      segments[1][2] = S_C;
      segments[1][3] = S_O;
      segments[1][4] = S_R;
      segments[1][5] = S_d;
      //
      showOnOrOff (bitRead(recordTrackEnable, curTrack) == 1);
      break;
#endif
    //
    case menuPtPlays:
      segments[1][0] = S_P;
      segments[1][1] = S_A;
      segments[1][2] = S_T;
      segments[1][3] = S_T;
      //
      segments[1][4] = S_P;
      segments[1][5] = S_L;
      segments[1][6] = S_A;
      segments[1][7] = S_Y;
      //
      if (patternData.playsPattern == 0)
      {
        segments[2][5] = S_I;
        segments[2][6] = S_N;
        segments[2][7] = S_F;        
      }
      else printNumber(2, 5, patternData.playsPattern);
      break;
    //
    case menuPtPlaysChain:
      segments[1][0] = S_C;
      segments[1][1] = S_H;
      segments[1][2] = S_A;
      segments[1][3] = S_I;
      segments[1][4] = S_N;
      //
      segments[1][5] = S_P;
      segments[1][6] = S_L;
      segments[1][7] = S_Y;
      //
      if (patternData.playsChain == 0)
      {
        segments[2][5] = S_I;
        segments[2][6] = S_N;
        segments[2][7] = S_F;        
      }
      else if (patternData.playsChain == 1) showOnOrOff(false);
      else printNumber(2, 5, patternData.playsChain - 1);
      break;
    //
    case menuPtNext:
      segments[1][0] = S_P;
      segments[1][1] = S_A;
      segments[1][2] = S_T;
      segments[1][3] = S_T;
      //
      segments[1][4] = S_N;
      segments[1][5] = S_E;
      segments[1][6] = S_X;
      segments[1][7] = S_T;
      //
      if (patternData.nextPattern == 0)
      {
        segments[2][4] = S_N;
        segments[2][5] = S_E;
        segments[2][6] = S_X;
        segments[2][7] = S_T;        
      }
      else printNumber(2, 5, patternData.nextPattern);
      //
      break;
    //
    case menuNote:
      segments[1][0] = S_N;
      segments[1][1] = S_O;
      segments[1][2] = S_T;
      segments[1][3] = S_E;
      //
      if (curTrack < (DRUM_TRACKS - 1))
      {
        printNumber(2, 0, configData.trackNote[curTrack]);
        printMIDInote(configData.trackNote[curTrack], 2, 4, 7);
      }
      else printDashDash(2, 6);
      break;
    //
    case menuProtect:
      showMemoryProtected();
      break;
    //
    case menuAccent1:
    case menuAccent2:
    case menuAccent3:
      segments[1][0] = S_A;
      segments[1][1] = S_C;
      segments[1][2] = S_C;
      segments[1][3] = S_E;
      segments[1][4] = S_N;
      segments[1][5] = S_T;
      //
      segments[2][0] = S_U;
      segments[2][1] = (char)pgm_read_word(&numbers[menuPosition - menuAccent1 + 1]);
      //
      printNumber(2, 4, configData.accentValues[menuPosition - menuAccent1]);
      break;
    //
    case menuProc:
      segments[1][0] = S_P;
      segments[1][1] = S_R;
      segments[1][2] = S_O;
      segments[1][3] = S_C;
      segments[1][4] = S_E;
      segments[1][5] = S_S;
      segments[1][6] = S_S;
      segments[1][7] = S_R;
      //
      if (curTrack != 15)
      {
        if (patternData.trackProcessor[curTrack] == fadeIn)
        {
          segments[2][0] = S_F;
          segments[2][1] = S_A;
          segments[2][2] = S_d;
          segments[2][3] = S_E;
          //
          segments[2][5] = S_I;
          segments[2][6] = S_N;
        }
        else if (patternData.trackProcessor[curTrack] == fadeOut)
        {
          segments[2][0] = S_F;
          segments[2][1] = S_A;
          segments[2][2] = S_d;
          segments[2][3] = S_E;
          //
          segments[2][5] = S_O;
          segments[2][6] = S_U;
          segments[2][7] = S_T;
        }
        else if (patternData.trackProcessor[curTrack] == fadeInD)
        {
          segments[2][0] = S_F;
          segments[2][1] = S_A;
          segments[2][2] = S_d;
          segments[2][3] = S_E;
          //
          segments[2][5] = S_I;
          segments[2][6] = S_N;
          segments[2][7] = S_d;
        }
        else if (patternData.trackProcessor[curTrack] == fadeOutD)
        {
          segments[2][0] = S_F;
          segments[2][1] = S_A;
          segments[2][2] = S_d;
          segments[2][3] = S_E;
          segments[2][4] = S_O;
          segments[2][5] = S_U;
          segments[2][6] = S_T;
          segments[2][7] = S_d;
        }
        else if (patternData.trackProcessor[curTrack] == fadeInOut)
        {
          segments[2][0] = S_F;
          segments[2][1] = S_d;
          //
          segments[2][3] = S_I;
          segments[2][4] = S_N;
          segments[2][5] = S_O;
          segments[2][6] = S_U;
          segments[2][7] = S_T;
        }
        else if (patternData.trackProcessor[curTrack] == fadeOutIn)
        {
          segments[2][0] = S_F;
          segments[2][1] = S_d;
          //
          segments[2][3] = S_O;
          segments[2][4] = S_U;
          segments[2][5] = S_T;
          segments[2][6] = S_I;
          segments[2][7] = S_N;
        }
        else if (patternData.trackProcessor[curTrack] == randomVel)
        {
          segments[2][0] = S_R;
          segments[2][1] = S_A;
          segments[2][2] = S_N;
          segments[2][3] = S_d;
          segments[2][4] = S_O;
          segments[2][5] = S_N;
          segments[2][6] = S_N;
        }
        else printDashDash(2, 6);
      }
      else printDashDash(2, 6);
      break;
      //
#if ECHOS_ENABLED
    case menuEcho:
    case menuEchoTrack:
    case menuEchoTicks:
    case menuEchoSpace:
    case menuEchoAttackDecay:
    case menuEchoType:
      segments[1][0] = S_E;
      segments[1][1] = S_C;
      segments[1][2] = S_H;
      segments[1][3] = S_O;
      //
      segments[1][5] = S_F;
      segments[1][6] = S_X;
      //
      if (menuPosition == menuEcho)
      {
        printNumber(2, 5, echoEdit + 1);
      }
      else if (menuPosition == menuEchoTrack)
      {
        segments[2][0] = S_T;
        segments[2][1] = S_R;
        segments[2][2] = S_A;
        segments[2][3] = S_C;
        if (patternData.echoConfig[echoEdit].track == 0) 
        {
          printDashDash(2, 6); 
        }
        else 
        {
          if (patternData.echoConfig[echoEdit].track <= DRUM_TRACKS)
          {
            printNumber(2, 5, patternData.echoConfig[echoEdit].track);
            segments[2][5] = S_d;
          }
          else
          {
            printNumber(2, 5, patternData.echoConfig[echoEdit].track - DRUM_TRACKS);
            segments[2][5] = S_N;
          }
        }
      }
      else if (menuPosition == menuEchoTicks)
      {
        segments[2][0] = S_T;
        segments[2][1] = S_I;
        segments[2][2] = S_C;
        segments[2][3] = S_S;
        if (patternData.echoConfig[echoEdit].track == 0) printDashDash(2, 6); else printNumber(2, 5, patternData.echoConfig[echoEdit].ticks);
      }
      else if (menuPosition == menuEchoSpace)
      {
        segments[2][0] = S_S;
        segments[2][1] = S_P;
        segments[2][2] = S_S;
        segments[2][3] = S_C;
        if (patternData.echoConfig[echoEdit].track == 0) printDashDash(2, 6); else printNumber(2, 5, patternData.echoConfig[echoEdit].space);
      }
      else if (menuPosition == menuEchoAttackDecay)
      {
        if (patternData.echoConfig[echoEdit].attackDecay > 0)
        {
          segments[2][0] = S_A;
          segments[2][1] = S_T;
          segments[2][2] = S_T;
          segments[2][3] = S_A;
          if (patternData.echoConfig[echoEdit].track == 0) printDashDash(2, 6); else printNumber(2, 5, patternData.echoConfig[echoEdit].attackDecay);
        }
        else if (patternData.echoConfig[echoEdit].attackDecay < 0)
        {
          segments[2][0] = S_d;
          segments[2][1] = S_E;
          segments[2][2] = S_C;
          segments[2][3] = S_Y;
          if (patternData.echoConfig[echoEdit].track == 0) printDashDash(2, 6); else printNumber(2, 5, -1 * patternData.echoConfig[echoEdit].attackDecay);
        }
        else
        {
          segments[2][0] = S_A;
          segments[2][1] = S_DASH;
          segments[2][2] = S_d;
          printDashDash(2, 6);
        }
      }
      else if (menuPosition == menuEchoType)
      {
        if (patternData.echoConfig[echoEdit].track == 0)
        {
          segments[2][0] = S_T;
          segments[2][1] = S_Y;
          segments[2][2] = S_P;
          segments[2][3] = S_E;
          //
          printDashDash(2, 6);
        }
        else if (patternData.echoConfig[echoEdit].type == 0)
        {
          segments[2][0] = S_A;
          segments[2][1] = S_L;
          segments[2][2] = S_L;
          //
          segments[2][4] = S_N;
          segments[2][5] = S_O;
          segments[2][6] = S_T;
          segments[2][7] = S_E;
        }
        else if (patternData.echoConfig[echoEdit].type == 1)
        {
          segments[2][0] = S_T;
          segments[2][1] = S_O;
          segments[2][2] = S_P;
          //
          segments[2][4] = S_N;
          segments[2][5] = S_O;
          segments[2][6] = S_T;
          segments[2][7] = S_E;
        }
        else if (patternData.echoConfig[echoEdit].type == 2)
        {
          segments[2][0] = S_L;
          segments[2][1] = S_O;
          segments[2][2] = 0x3c;
          segments[2][3] = 0x1e;
          //
          segments[2][4] = S_N;
          segments[2][5] = S_O;
          segments[2][6] = S_T;
          segments[2][7] = S_E;
        }
      }
      break;
#endif
    //
    case menuVariationsABCD:
      segments[1][0] = S_P;
      segments[1][1] = S_A;
      segments[1][2] = S_T;
      //
      segments[1][4] = S_U;
      segments[1][5] = S_A;
      segments[1][6] = S_R;
      segments[1][7] = S_S;
      //
      segments[2][4] = S_A;
      if (patternData.totalVariations >= 2) segments[2][5] = S_b;
      if (patternData.totalVariations >= 3) segments[2][6] = S_C;
      if (patternData.totalVariations >= 4) segments[2][7] = S_d;
      break;
    //
    case menuSyncOut:
      segments[1][0] = S_S;
      segments[1][1] = S_Y;
      segments[1][2] = S_N;
      segments[1][3] = S_C;
      //
      segments[1][5] = S_O;
      segments[1][6] = S_U;
      segments[1][7] = S_T;
      //
      showOnOrOff(configData.seqSyncOut);
      break;
    //
    case menuMIDIinPattern:
      segments[1][0] = S_N;
      segments[1][1] = S_N;
      segments[1][2] = S_I;
      segments[1][3] = S_d;
      segments[1][4] = S_I;
      //
      segments[1][6] = S_P;
      segments[1][7] = S_T;
      //
      segments[2][0] = S_C;
      segments[2][1] = S_H;
      //
      printNumber(2, 5, configData.midiInputToPatternChannel + 1);
      break;
    //
    #if HAS_SYSEX
    case menuSysex:
      segments[1][0] = S_S;
      segments[1][1] = S_Y;
      segments[1][2] = S_S;
      segments[1][3] = S_E;
      segments[1][4] = S_X;
      //
      if (patternBankSysExDump == -2)
      {
        printDashDash(1, 6);
      }
      else if (patternBankSysExDump == -1)
      {
        segments[1][5] = S_A;
        segments[1][6] = S_L;
        segments[1][7] = S_L;
      }
      else
      {
        printNumber(1, 5, patternBankSysExDump + 1);
        segments[1][4] = S_X;
      }
      //
      if (sysExDump == 0)
      {
        segments[2][0] = S_d;
        segments[2][1] = S_U;
        segments[2][2] = S_N;
        segments[2][3] = S_N;
        segments[2][4] = S_P;
      }
      else if (sysExDump == 1)
      {
        segments[2][0] = S_R;
        segments[2][1] = S_E;
        segments[2][2] = S_C;
        segments[2][3] = S_E;
        segments[2][4] = S_I;
        segments[2][5] = S_U;
        segments[2][6] = S_E;
      }
      break;
    #endif
    //
    case menuPatternBank:
      segments[1][0] = S_P;
      segments[1][1] = S_A;
      segments[1][2] = S_T;
      segments[1][3] = S_T;
      //
      segments[1][4] = S_b;
      segments[1][5] = S_A;
      segments[1][6] = S_N;
      segments[1][7] = S_K;
      //
      printNumber(2, 5, currentPatternBank + 1);
      break;
    //
    case menuClockType:
      segments[1][0] = S_C;
      segments[1][1] = S_L;
      segments[1][2] = S_C;
      segments[1][3] = S_K;
      //
      if (configData.midiClockInternal)
      {
        segments[2][4] = S_I;
        segments[2][5] = S_N;
        segments[2][6] = S_T;
      }
      else
      {
        segments[2][4] = S_E;
        segments[2][5] = S_X;
        segments[2][6] = S_T;
      }
      break;
    //
    case menuInit:
      segments[1][0] = S_I;
      segments[1][1] = S_N;
      segments[1][2] = S_I;
      segments[1][3] = S_T;
      //
      if (initMode == 0)
      {
        segments[2][4] = S_N;
        segments[2][5] = S_O;
        segments[2][6] = S_N;
        segments[2][7] = S_E;
      }
      else if (initMode == 1)
      {
        segments[2][4] = S_b;
        segments[2][5] = S_A;
        segments[2][6] = S_N;
        segments[2][7] = S_C;
      }
      else
      {
        segments[2][5] = S_A;
        segments[2][6] = S_L;
        segments[2][7] = S_L;
      }
      break;
  }
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void processMenuOK()
{
  switch (menuPosition)
  {
    #if HAS_SYSEX
    case menuSysex:
      if (seqPlaying) printStopSequencer();
      else
      {
        if (sysExDump == 0) sysexDump(patternBankSysExDump); else sysexReceive(patternBankSysExDump);
      }
      break;
    #endif
    //
    case menuInit:
      if (seqPlaying) printStopSequencer();
      else
      {
        if (initMode == 1)
        {
          stopSequencer();
          reset();
          int porc = 0;
          initPatternBank(currentPatternBank, true, porc, true);
          loadPatternBank(currentPatternBank);
          checkMenuClose();
        }
        else if (initMode == 2)
        {
          stopSequencer();
          reset();
          currentPatternBank = 0;
          currentPatternBankFlashPosition = 16;
          flashInit(true);
          checkMenuClose();
        }
        else checkMenuClose();
      }
      break;
  }
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void processMenu(char value)
{
  switch (menuPosition)
  {
    case menuMidiCC:
      if (value > 0 && configData.trackMidiCH[curTrack] < 15) configData.trackMidiCH[curTrack]++;
      else if (value < 0 && configData.trackMidiCH[curTrack] > 0) configData.trackMidiCH[curTrack]--;
      somethingChangedConfig = true;
      break;
      //
#if RECORD_ENABLED
    case menuRecord:
      if (value > 0) bitSet(recordTrackEnable, curTrack);
      else if (value < 0) bitClear(recordTrackEnable, curTrack);
      break;
#endif
    //
    case menuNote:
      if (curTrack < DRUM_TRACKS)
      {
        if (value > 0 && configData.trackNote[curTrack] < 127) configData.trackNote[curTrack]++;
        else if (value < 0 && configData.trackNote[curTrack] > 0) configData.trackNote[curTrack]--;
        somethingChangedConfig = true;
      }
      break;
    //
    case menuAccent1:
    case menuAccent2:
    case menuAccent3:
      if (value > 0 && configData.accentValues[menuPosition - menuAccent1] < 127) configData.accentValues[menuPosition - menuAccent1]++;
      else if (value < 1 && configData.accentValues[menuPosition - menuAccent1] > 0) configData.accentValues[menuPosition - menuAccent1]--;
      somethingChangedConfig = true;
      break;
    //
    case menuProc:
      if (value > 0 && patternData.trackProcessor[curTrack] < 7) patternData.trackProcessor[curTrack]++;
      else if (value < 1 && patternData.trackProcessor[curTrack] > 0) patternData.trackProcessor[curTrack]--;
      somethingChangedPattern = true;
      break;
    //
    case menuProtect:
      configData.writeProtectFlash = !configData.writeProtectFlash;
      saveConfigData(true);
    break;
    //
#if ECHOS_ENABLED
    case menuEcho:
      if (value > 0 && echoEdit < (ECHOS - 1)) echoEdit++;
      else if (value < 1 && echoEdit > 0) echoEdit--;
      somethingChangedPattern = true;
      break;
#endif
    //
    case menuPtNext:
      if (value > 0 && patternData.nextPattern < 128) patternData.nextPattern++;
      else if (value < 1 && patternData.nextPattern > 0) patternData.nextPattern--;
      somethingChangedPattern = true;
      break;
    //
    case menuPtPlays:
      if (value > 0 && patternData.playsPattern < 255) patternData.playsPattern++;
      else if (value < 1 && patternData.playsPattern > 0) patternData.playsPattern--;
      somethingChangedPattern = true;
      break;
    //
    case menuPtPlaysChain:
      if (value > 0 && patternData.playsChain < 255) patternData.playsChain++;
      else if (value < 1 && patternData.playsChain > 0) patternData.playsChain--;
      somethingChangedPattern = true;
      break;
    //
    case menuMIDIinPattern:
      if (value > 0 && configData.midiInputToPatternChannel < 15) configData.midiInputToPatternChannel++;
      else if (value < 1 && configData.midiInputToPatternChannel > 0) configData.midiInputToPatternChannel--;
      somethingChangedConfig = true;
      break;
    //
    case menuClockType:
      configData.midiClockInternal = !configData.midiClockInternal;
      setupTimerForExternalMIDISync(!configData.midiClockInternal);
      somethingChangedConfig = true;
      break;
      //
#if ECHOS_ENABLED
    case menuEchoTrack:
      if (value > 0 && patternData.echoConfig[echoEdit].track < (DRUM_TRACKS + NOTE_TRACKS)) patternData.echoConfig[echoEdit].track++;
      else if (value < 1 && patternData.echoConfig[echoEdit].track > 0) patternData.echoConfig[echoEdit].track--;
      somethingChangedPattern = true;
      break;
    //
    case menuEchoTicks:
      if (value > 0 && patternData.echoConfig[echoEdit].ticks < 127) patternData.echoConfig[echoEdit].ticks++;
      else if (value < 1 && patternData.echoConfig[echoEdit].ticks > 0) patternData.echoConfig[echoEdit].ticks--;
      somethingChangedPattern = true;
      break;
    //
    case menuEchoSpace:
      if (value > 0 && patternData.echoConfig[echoEdit].space < 127) patternData.echoConfig[echoEdit].space++;
      else if (value < 1 && patternData.echoConfig[echoEdit].space > 0) patternData.echoConfig[echoEdit].space--;
      somethingChangedPattern = true;
      break;
    //
    case menuEchoAttackDecay:
      if (value > 0 && patternData.echoConfig[echoEdit].attackDecay < 127) patternData.echoConfig[echoEdit].attackDecay++;
      else if (value < 1 && patternData.echoConfig[echoEdit].attackDecay > -127) patternData.echoConfig[echoEdit].attackDecay--;
      somethingChangedPattern = true;
      break;
    //
    case menuEchoType:
      if (value > 0 && patternData.echoConfig[echoEdit].type <= 1) patternData.echoConfig[echoEdit].type++;
      else if (value < 1 && patternData.echoConfig[echoEdit].type > 0) patternData.echoConfig[echoEdit].type--;
      somethingChangedPattern = true;
      break;
#endif
    //
    case menuVariationsABCD:
      if (value > 0 && patternData.totalVariations < 4) patternData.totalVariations++;
      else if (value < 1 && patternData.totalVariations > 1) patternData.totalVariations--;
      somethingChangedPattern = true;
      break;
    //
    case menuSyncOut:
      configData.seqSyncOut = !configData.seqSyncOut;
      somethingChangedConfig = true;
      break;
    //
    #if HAS_SYSEX
    case menuSysex:
      if (value > 0 && patternBankSysExDump < (PT_BANKS - 1)) patternBankSysExDump++;
      else if (value < 1 && patternBankSysExDump > -2) patternBankSysExDump--;
      break;
    #endif
    //
    case menuPatternBank:
      if (seqPlaying) printStopSequencer();
      else
      {
        checkIfDataNeedsSaving();
        //
        if (value > 0 && currentPatternBank < (PT_BANKS - 1)) currentPatternBank++;
        else if (value < 1 && currentPatternBank > 0) currentPatternBank--;
        //
        loadPatternBank(currentPatternBank);
        changedPatternBank = true;
      }
      break;
    //
    case menuInit:
      if (value > 0 && initMode < 2) initMode++;
      else if (value < 1 && initMode > 0) initMode--;
      break;
  }
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void checkMenuClose()
{
  if (curRightScreen == kRightMenuCopyPaste || curRightScreen == kRightMenu) curRightScreen = kRightSteps;
}
