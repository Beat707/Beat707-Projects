
/*
 * Beat707 Mikro - created by William (Guilherme) Kalfelz (c) 2016 - www.Beat707.com - www.Wusik.com - www.William-k.com
 * 
 */

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void startPatternStream(byte xpattern)
{
  streamPatternGo = true;
  showPatternLoading(true);
  SDLoadPatternSettings(filePatternSettings, true);
  streamCheck = 99;
  streamPosition = streamVariation = 0;
  streamPattern = xpattern;
  slaveSendCode(slvStartStream, 0, 0, 0);
  streamDataCheck();
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void finishPatternStream()
{
  if (slaveRequestByte(slvIsPlaying) == 0) SDLoadEntirePattern(filePattern);
  streamPosition = streamVariation = 0;
  streamPatternGo = false;
  curPattern = streamPattern;
  changeTrack(0);
  // !!! If in song mode go to the next pattern in line
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void streamDataCheck()
{
  streamCheck++;
  if (streamCheck > CHECK_STREAM_COUNTER) streamCheck = 0; else return;
  if (streamPatternGo && slaveRequestByte(slvStreamSize) > 0)
  {
    if (slaveRequestByte(slvIsPlaying) == 0 || streamVariation >= VARIATIONS)
    {
      finishPatternStream();
      return;
    }
    //
    uint16_t steps1 = 0;
    uint16_t steps2 = 0;
    //
    SDSFileOpen(filePattern, fileRead, true);
    //
    SDSkipBytes((streamVariation*SD_PATTERN_VARIATION_SIZE)+2);
    for (byte w=0; w<STEPS; w++)
    {
      if (streamPosition > 0) SDSkipBytes(streamPosition);
      char nStep = fR();
      if (nStep == 'X') { bitSet(steps1, w); }
      else if (nStep == 'D') { bitSet(steps1, w); bitSet(steps2, w); }
      else if (nStep == 'd') { bitSet(steps2, w); }
      SDSkipBytes((STEPS-1)-streamPosition+1);
    }
    SDReadSkipNewLine();
    patternStreamBuffer[0] = (steps1 >> 8) & 0xFF;
    patternStreamBuffer[1] = steps1 & 0xFF;
    patternStreamBuffer[2] = (steps2 >> 8) & 0xFF;
    patternStreamBuffer[3] = steps2 & 0xFF;
    //
    byte xp = 4;
    for (byte w=0; w<NOTE_TRACKS; w++)
    {
      byte sS1 = 0;
      byte sS2 = 0;
      SDSkipBytes(streamPosition*7);
      char xcode = fR();
      sS1 = SDReadASCII3Digits();
      sS2 = SDReadASCII3Digits();
      //
      if (xcode == 'X') { sS2 = 0x80 | (sS2 & 0x7F); }
      else if (xcode == 'D') { sS1 = 0x80 | (sS1 & 0x7F); }
      else if (xcode == 'd') { sS1 = 0x80 | (sS1 & 0x7F); sS2 = 0x80 | (sS2 & 0x7F); }
      else if (xcode == 'L') { sS1 = (sS1 & 0x7F); }
      //
      SDSkipBytes((((STEPS-1)*7)-(streamPosition*7))+NEW_LINE_SIZE);
      patternStreamBuffer[xp] = sS1;
      patternStreamBuffer[xp+1] = sS2;
      xp += 2;
    }
    //
    tmpFile.close();
    //
    slaveBeginWrite(slvSendStream);
    Wire.write(patternStreamBuffer, ((NOTE_TRACKS*DOUBLE_STEPS)+4));
    slaveEndTransmission();
    //
    streamPosition++;
    if (streamPosition >= 16)
    {
      streamPosition = 0;
      streamVariation++;
    }
  }
}

