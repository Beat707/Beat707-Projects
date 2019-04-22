/*
 * 
 * Created by William Kalfelz @ Beat707 (c) 2018 - http://www.Beat707.com
 * 
 */

#if HAS_SYSEX

byte zeroCounterSend = 0;
byte zeroCounterReceive = 0;
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void sendFinalZero()
{
  if (zeroCounterSend > 0)
  {
    Serial.write(zeroCounterSend);
    zeroCounterSend = 0;
  } 
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool sendValueCheckZero(byte value)
{
  if (value == 0)
  {
    if (zeroCounterSend == 0)
    {
      Serial.write(0);
      Serial.write(0);
      zeroCounterSend++;
    }
    else
    {
      zeroCounterSend++;
      if (zeroCounterSend == 0x7f)
      {
        Serial.write(0x7f);
        zeroCounterSend = 0;
      }
    }
  }
  else
  {
    sendFinalZero();
    return true;
  }
  //
  return false;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void send4BitsBytes(byte value)
{
  if (sendValueCheckZero(value))
  {
    Serial.write(((value & B11110000) >> 4) & B00001111);
    Serial.write(value & B00001111);
  }
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
byte receive4BitsBytes()
{
  byte xreturn = 0;
  //
  if (zeroCounterReceive > 0)
  {
    zeroCounterReceive--;
  }
  else
  {
    xreturn = (serialNextByte() << 4);
    xreturn |= serialNextByte();
    if (xreturn == 0)
    {
      zeroCounterReceive = serialNextByte() - 1;
    }
  }
  //
  return xreturn;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool isPatternEmpty()
{
  for (byte x=0; x<STEPS; x++)
  {
    for (byte y=0; y<(DRUM_TRACKS-1); y++) { if (stepsData[x].steps[y] != 0) return false; }
    for (byte y=0; y<4; y++) { if (stepsData[x].stepsDouble[y] != 0) return false; }
    for (byte y=0; y<NOTE_TRACKS; y++) 
    { 
      if (stepsData[x].noteSteps[y][0] != 0) return false; 
      if (stepsData[x].noteSteps[y][1] != 0) return false; 
      if (stepsData[x].noteSteps[y][2] != 0) return false; 
      if (stepsData[x].noteSteps[y][3] != 0) return false; 
      if (stepsData[x].noteStepsExtras[y][0] != 0) return false; 
      if (stepsData[x].noteStepsExtras[y][1] != 0) return false; 
    }
  }
  //
  return true;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool isEntirePatternBankEmpty()
{
  for (byte xd = 0; xd < PATTERNS; xd++)
  {
    loadPattern(xd);
    if (!isPatternEmpty()) return false;
  }
  return true;
}

int porc = 0;
byte porcDivider = 1;
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void updatePorc(byte value)
{
  porc += value;
  if (porcDivider == 1) showWaitMsg(porc*3); else showWaitMsg(porc/porcDivider);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void sysexDump(char patternBank)
{
  if (patternBank == -2) return;
  ////
  stopTimer(false);
  showWaitMsg(-1);
  porc = 0;
  byte prevPatternBank = currentPatternBank;
  byte prevPattern = currentPattern;
  byte nPatternBanks = PT_BANKS;
  byte patternBankSelect = 0;
  porcDivider = 20;
  if (patternBank >= 0) 
  {
    patternBankSelect = patternBank;
    nPatternBanks = 1;
    porcDivider = 1;
  }
  //
  Serial.write(0xF0); // Sysex Start Code //
  Serial.write(0x01); // Sysex ID Code //
  Serial.write(0x02);
  Serial.write(0x28);
  if (patternBank == -1) Serial.write(0x08); else Serial.write(0x04);
  //
  for (byte x = 0; x < nPatternBanks; x++) // patternBanks
  {
    loadPatternBank(patternBankSelect);
    patternBankSelect++;
    //
    if (isEntirePatternBankEmpty())
    {
      Serial.write(0x10);
      updatePorc(64);
    }
    else
    {
      Serial.write(0x12);
      for (int xd = 0; xd < sizeof(configData); xd++) { send4BitsBytes(((byte*)&configData)[xd]); }
      sendFinalZero();
      //
      for (byte xd = 0; xd < PATTERNS; xd++)
      {
        loadPattern(xd);
        if (isPatternEmpty())
        {
          Serial.write(0x18);
        }
        else
        {
          Serial.write(0x22);
          for (int xpp = 0; xpp < sizeof(patternData); xpp++) { send4BitsBytes(((byte*)&patternData)[xpp]); }
          sendFinalZero();
          for (int xpp = 0; xpp < sizeof(stepsData); xpp++) { send4BitsBytes(((byte*)&stepsData)[xpp]); }
          sendFinalZero();
        }
        //
        updatePorc(1);
      }
    }
  }
  //
  Serial.write(0x24); // End Of Message //
  Serial.write(0xF7); // Sysex End Code //
  loadPatternBank(prevPatternBank);
  loadPattern(prevPattern);
  showWaitMsg(-1);
  startTimer(false);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void sysexReceive(char patternBank)
{
  if (patternBank == -2) return;
  //
  stopTimer(false);
  showWaitMsg(-1);
  //  
  byte prevPatternBank = currentPatternBank;
  byte prevPattern = currentPattern;
  byte nPatternBanks = PT_BANKS;
  byte patternBankSelect = 0;
  porcDivider = 20;
  if (patternBank >= 0) 
  {
    patternBankSelect = patternBank;
    nPatternBanks = 1;
    porcDivider = 1;
  }
  //  
  // First Erase The Flash Memory //
  if (patternBank == -1)
  {
    if (!flash.eraseChip()) showErrorMsg(flash.error());
  }
  else
  {
    uint16_t pagePos = 16 + (patternBank * ((64 * 16) + 16));
    eraseSector(pagePos);
    pagePos += 16;
    for (byte p = 0; p < PATTERNS; p ++)
    {
      eraseSector(pagePos);
      pagePos += 16;
      updatePorc(1);
    }
  }
  //
  memset(leds, 0, sizeof(leds));
  resetSegments(0, 2);
  segments[2][0] = S_R;
  segments[2][1] = S_E;
  segments[2][2] = S_A;
  segments[2][3] = S_d;
  segments[2][4] = S_Y;
  //
  segments[2][6] = S_G;
  segments[2][7] = S_O;
  sendScreen();
  //
  /*
   * SysEx Start: 0xF0
   * 3 ID Bytes: 0x01 0x02 0x28
   * Is All PatternBanks: 0x08 - Is Single PatternBank 0x04
   * {
   *   Is PatternBank Empty: 0x10 - Otherwise 0x12
   *   {
   *     Receive configData, using the format above
   *     If PatternBankData empty: 0x14 - Otherwise 0x16
   *     {
   *       Receive patternBankData, using the format above
   *     }
   *     Is Pattern Empty: 0x18 - Otherwise 0x22
   *     {
   *       Receive patternData, using the format above
   *       Receive stepsData, using the format above
   *     }
   *   }
   * }
   * End Message: 0x24
   * SysEx End: 0xF7
   */
  //
  porc = 0;
  emptySerialInput();
  if (serialNextByte() != 0xF0)
  {
    showErrorMsg(18);
    emptySerialInput();
    goto finish;
  }
  //
  if (serialNextByte() != 0x01 ||
      serialNextByte() != 0x02 ||
      serialNextByte() != 0x28)
  {
    showErrorMsg(28);
    emptySerialInput();
    goto finish;   
  }
  else
  {
    byte xType = serialNextByte();
    if ((xType == 0x08 && patternBank != -1) || (xType == 0x04 && patternBank == -1))
    {
      while (serialNextByte() != 0xF7) { ; }
      showErrorMsg(38);
    }
  }
  //
  updatePorc(1);
  for (byte x = 0; x < nPatternBanks; x++) // patternBanks
  {
    currentPatternBank = patternBankSelect;
    currentPattern = 0;
    patternBankSelect++;
    //
    if (serialNextByte() == 0x12)
    {
      for (int xd = 0; xd < sizeof(configData); xd++) { ((byte*)&configData)[xd] = receive4BitsBytes(); }
      saveConfigData(false);
      //
      for (byte p = 0; p < PATTERNS; p++)
      {
        byte xTemp = serialNextByte();
        if (xTemp == 0x22)
        {
          currentPattern = p;
          for (int xd = 0; xd < sizeof(patternData); xd++) { ((byte*)&patternData)[xd] = receive4BitsBytes(); }
          savePatternData(false);
          for (int xd = 0; xd < sizeof(stepsData); xd++) { ((byte*)&stepsData)[xd] = receive4BitsBytes(); }
          saveStepsData();
        }
        else if (xTemp != 0x18)
        {
          showErrorMsg(98);
          goto finish;
        }
        //
        updatePorc(1);
      }
    }
    else
    {
      updatePorc(64); // Empty PatternBank //
    }
  }
  if (serialNextByte() != 0x24) showErrorMsg(48);
  if (serialNextByte() != 0xF7) showErrorMsg(58);
  //
  // Now Check All Memory Spaces that were not saved (hasInit != 0x27) and INIT those spaces //
  patternBankSelect = 0;
  porc = 0;
  if (patternBank >= 0) patternBankSelect = patternBank;
  //
  for (byte x = 0; x < nPatternBanks; x++) // patternBanks
  {
    loadPatternBank(patternBankSelect);
    patternBankSelect++;
    //
    if (configData.hasInit != 0x27)
    {
      reset();
      saveConfigData(false);
    }
    //
    for (byte p = 0; p < PATTERNS; p ++)
    {
      loadPattern(p);
      if (patternData.hasInit != 0x27)
      {
        resetPatternBank();
        savePatternData(false);
        saveStepsData();
      }
      //
      updatePorc(1);
    }
  }
  //
  loadPatternBank(prevPatternBank);
  loadPattern(prevPattern);
  saveHeader(prevPatternBank, true);
  //
  finish:
    showWaitMsg(-1);
    startTimer(false);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
byte serialNextByte()
{
  while (Serial.available() == 0) { ; }
  return Serial.read();
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void emptySerialInput()
{
  while (Serial.available() > 0)
  {
    Serial.read();
  }
}

#endif
