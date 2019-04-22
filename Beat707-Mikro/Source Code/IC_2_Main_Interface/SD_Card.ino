/*
 * Beat707 Mikro - created by William (Guilherme) Kalfelz (c) 2016 - www.Beat707.com - www.Wusik.com - www.William-k.com
 * 
 */

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDInit()
{
  if (!SD.begin(SDCARDSS)) { printErrorMsg(SD_CARD_ERROR,0); } 
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDSSetFilename(byte pos, byte value)
{
  fileG[pos] = value;
  fileGT[pos] = value;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDSFileWriteFinish()
{
  tmpFile.close();
  if (SD.exists(fileG)) SD.remove(fileG);
  SD.rename(fileGT, fileG);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDSFileOpen(byte type, byte writeload, boolean showErrorMsg)
{
  if (type >= filePatternCopy && type <= fileNoteTrackCopy)
  {
    SDSSetFilename(8, 'c');
    SDSSetFilename(9, 'p');
    SDSSetFilename(11, 's');
    SDSSetFilename(12, 'c'); 
    SDSSetFilename(13, 'c');
    if (type == fileDrumTrackCopy) SDSSetFilename(11, 'd'); 
      else if (type == fileNoteTrackCopy) SDSSetFilename(11, 'n'); 
      else if (type == filePatternCopy) SDSSetFilename(11, 'p'); 
  }
  else
  {
    if (curSong == 0)
    {
      SDSSetFilename(8, 't');
      SDSSetFilename(9, 'p');
    }
    else
    {
      SDSSetFilename(8, ('0'+(curSong/10)));
      SDSSetFilename(9, ('0'+(curSong-((curSong/10)*10))));
    }
    SDSSetFilename(12, ('0'+(curPattern/10)));
    SDSSetFilename(13, ('0'+(curPattern-((curPattern/10)*10))));
    if (type == filePattern) { SDSSetFilename(11, 'p'); }
    else if (type == filePatternSettings) { SDSSetFilename(11, 's'); }
    else if (type == fileChordCustom)
    {
      SDSSetFilename(11, 'c');
      SDSSetFilename(12, 'h');
      SDSSetFilename(13, 'd');          
    }
    else
    {
      SDSSetFilename(11, 's');
      SDSSetFilename(12, 'n');
      SDSSetFilename(13, 'g');    
    }
  }
  sdCardFileOpenRetry = 0;
  while (sdCardFileOpenRetry < 9)
  {
    if (writeload == fileWrite) 
    {
      if (SD.exists(fileGT)) SD.remove(fileGT);
      tmpFile = SD.open(fileGT, FILE_WRITE);
    } 
    else 
    {
      tmpFile = SD.open(fileG);
    }
    sdCardFileOpenRetry++;
    if (tmpFile) break;
    if (!showErrorMsg) return;
  }
  if (!tmpFile && showErrorMsg) printErrorMsg(SD_CARD_ERRORSC,fileG);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDFileOpen(byte type, byte sector, byte sizesector)
{
  fileS[9] = '0' + type;
  SDFileSOpen();
  int xskip = sizesector * sector;
  if (xskip >= tmpFile.size()) xskip = tmpFile.size() - sizesector;
  if (xskip > 0) tmpFile.seek(xskip);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDFileSOpen()
{
  sdCardFileOpenRetry = 0;
  while (sdCardFileOpenRetry < 9)
  {
    tmpFile = SD.open(fileS);
    sdCardFileOpenRetry++;
    if (tmpFile) break;
  }
  if (!tmpFile) printErrorMsg(SD_CARD_ERRORSC,fileS);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int SDFileSize(byte type)
{
  int xsize = 0;
  fileS[9] = '0' + type;
  SDFileSOpen();
  xsize = tmpFile.size();
  tmpFile.close();
  return xsize;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDReadScreen(byte scn, byte xwhat)
{
  SDFileOpen(0, scn, 34 + NEW_LINE_SIZE);
  if (xwhat == entireScreen || xwhat == firstLineOnly)
  {
    homeCursor();
    for (byte x=0; x<16; x++) { writeDisplay(fR()); }
    SDReadSkipNewLine();
  }
  //
  if (xwhat == entireScreen || xwhat == lastLineOnly)
  {
    if (xwhat == lastLineOnly) SDSkipBytes(16 + NEW_LINE_SIZE);
    setCursorDisplay(SECOND_LINE(0));
    for (byte x=0; x<16; x++) { writeDisplay(fR()); }
  }
  tmpFile.close();
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDReadXTraName(byte scn)
{
  SDFileOpen(1, scn, 12 + NEW_LINE_SIZE);
  for (byte x=0; x<12; x++) { writeDisplay(fR()); }
  tmpFile.close();
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDReadSkipNewLine()
{
  SDSkipBytes(NEW_LINE_SIZE);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDWriteNewLine()
{
 fW(0x0d);
 fW(0x0a); 
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDReadTrackName()
{
  memset(curTrackName,' ',sizeof(curTrackName));
  curTrackName[9] = 0;
  //
  if (!isAccentTrack())
  {
    byte xt = 3;
    if (curTrack < accentTrack) xt = 2;
    SDFileOpen(xt, trackName[curTrack], 9 + NEW_LINE_SIZE);
    for (byte x=0; x<9; x++) { curTrackName[x] = fR(); }
    tmpFile.close();
  }
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDReadPatternName()
{
  memset(curPatternName,' ',sizeof(curPatternName));
  curPatternName[9] = 0;
  //
  SDFileOpen(4, curPatternNameValue, 9 + NEW_LINE_SIZE);
  for (byte x=0; x<9; x++) { curPatternName[x] = fR(); }
  tmpFile.close();
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDReadCustomLCDChars(byte xmirror)
{
  SDFileOpen(5 + xmirror, 0, 0);
  byte customChar[8];
  for (byte x=0; x<8; x++)
  {
    for (byte y=0; y<8; y++)
    {
      customChar[y] = 0;
      for (byte w=0; w<5; w++)
      {
        if (fR() == 'x') bitSet(customChar[y], 4-w);
      }
      SDReadSkipNewLine();
    }
    //
    lcd.createChar(x, customChar);
    SDReadSkipNewLine();
  }
  tmpFile.close();
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDWriteASCII2Digits(byte number)
{
  fW('0'+(number/10));
  fW('0'+(number-((number/10)*10)));
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
byte SDReadASCII2Digits()
{
  byte value = 0;
  value += (fR() - '0') * 10;
  value += (fR() - '0');
  //
  return value;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDWriteASCII3Digits(byte number)
{
  if (number >= 200) { fW('2'); number -= 200; }
    else if (number >= 100) { fW('1'); number -= 100; }
    else fW('0');
  SDWriteASCII2Digits(number);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int SDReadASCII3Digits()
{
  int value = 0;
  byte dB1 = fR();
  byte dB2 = fR();
  byte dB3 = fR();
  value += (dB1 - '0') * 100;
  value += (dB2 - '0') * 10;
  value += (dB3 - '0');
  //
  return value;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDWriteSpace()
{
  fW(' ');
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDWriteBoolean(boolean value)
{
  if (value) fW('1'); else fW('0');
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
boolean SDReadBoolean()
{
  byte xb = fR();
  if (xb == '1') return true; else return false;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDSkipBytes(int value)
{
  tmpFile.seek(tmpFile.position() + value);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDGetCurrentSong()
{
  SDFileOpen('g'-'0', 0, 0);
  curSong = SDReadASCII2Digits();
  tmpFile.close();
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDSaveCurrentSong()
{
  fileS[9] = 'g';
  SD.remove(fileS);
  tmpFile = SD.open(fileS, FILE_WRITE);
  if (!tmpFile) printErrorMsg(SD_CARD_ERRORSC,fileS);
  SDWriteASCII2Digits(curSong);
  tmpFile.close();
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDLoadSongSettings()
{
  SDSFileOpen(fileSong, fileRead, true);
  for (byte x=0; x<16; x++) { songName[x] = fR(); }
  SDReadSkipNewLine();
  SDSkipBytes(3); seqBPM = SDReadASCII3Digits(); SDReadSkipNewLine();
  SDSkipBytes(4); seqType = SDReadASCII2Digits(); SDReadSkipNewLine();
  SDSkipBytes(4); songLoopPosition = SDReadASCII2Digits(); SDReadSkipNewLine();
  SDSkipBytes(5); seqSyncOut = SDReadBoolean();
  // 
  tmpFile.close();
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDSaveSongSettings()
{
  if (!songSettingsChanged) return;
  songSettingsChanged = false;
  //
  SDReadScreen(areaSavingMessage, lastLineOnly);
  lateRedrawDisplay = true;
  //
  SDSFileOpen(fileSong, fileWrite, true);
  tmpFile.write(songName, 16); SDWriteNewLine();
  fW('B'); fW('P'); fW('M'); SDWriteASCII3Digits(seqBPM); SDWriteNewLine();
  fW('T'); fW('y'); fW('p'); fW('e'); SDWriteASCII2Digits(seqType); SDWriteNewLine();
  fW('L'); fW('o'); fW('o'); fW('p'); SDWriteASCII2Digits(songLoopPosition); SDWriteNewLine();
  fW('S'); fW('y'); fW('n'); fW('c'); fW('O'); SDWriteBoolean(seqSyncOut); SDWriteNewLine();
  //
  SDSFileWriteFinish();
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDLoadPatternSettings(byte fileType, boolean sendToBuffer)
{
  byte prevTrack = curTrack;
  byte xtemp = 0;
  boolean xsend = false;
  //
  SDSFileOpen(fileType, fileRead, true);
  SDSkipBytes(2);
  xtemp = SDReadASCII2Digits();
  if (xtemp != curPatternNameValue) xsend = true;
  curPatternNameValue = xtemp;
  //
  SDSkipBytes(2);
  xtemp = SDReadASCII2Digits();
  if (xtemp != totalVariations) xsend = true;
  totalVariations = xtemp;
  //
  SDReadSkipNewLine();
  if (xsend) slaveSendSequencerSettings(sendToBuffer);
  //
  for (byte x=0; x<DRUM_TRACKS; x++)
  {
    xsend = false;
    curTrack = x;
    SDSkipBytes(5);
    xtemp = SDReadASCII2Digits()-1;
    if (xtemp != trackMidiCH) xsend = true;
    trackMidiCH = xtemp;
    //
    SDSkipBytes(3); 
    xtemp = SDReadASCII3Digits();
    if (xtemp != trackNote) xsend = true;
    trackNote = xtemp;
    //
    SDSkipBytes(2); 
    xtemp = SDReadASCII2Digits();
    if (xtemp != trackName[x]) xsend = true;
    trackName[x] = xtemp;
    //
    SDSkipBytes(2); 
    xtemp = SDReadASCII2Digits();
    if (xtemp != trackProcessor) xsend = true;
    trackProcessor = xtemp;
    //
    SDReadSkipNewLine();
    if (xsend) slaveSendTrackSettings(sendToBuffer);
  }
  // 
  for (byte x=noteTrack; x<(TOTAL_TRACKS); x++)
  {
    xsend = false;
    curTrack = x;
    SDSkipBytes(5);
    xtemp = SDReadASCII2Digits()-1;
    if (xtemp != trackMidiCH) xsend = true;
    trackMidiCH = xtemp;
    //
    SDSkipBytes(2); 
    xtemp = SDReadASCII2Digits();
    if (xtemp != trackName[x]) xsend = true;
    trackName[x] = xtemp;
    //
    SDSkipBytes(2); 
    xtemp = SDReadASCII2Digits();
    if (xtemp != trackProcessor) xsend = true;
    trackProcessor = xtemp;
    //
    SDSkipBytes(3); 
    xtemp = SDReadASCII2Digits();
    if (xtemp != noteTrackMode[x-noteTrack]) xsend = true;
    noteTrackMode[x-noteTrack] = xtemp;
    //    
    SDReadSkipNewLine();
    if (xsend) slaveSendTrackSettings(sendToBuffer);
  }
  // 
  tmpFile.close();
  curTrack = prevTrack;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDSavePattern(byte fileType1, byte fileType2)
{
  if (fileType1 != filePatternCopy) 
  {
    if (!somethingChanged) return;
    somethingChanged = false;
  }
  if (fileType1 == filePatternCopy) SDReadScreen(areaCopyingMessage, lastLineOnly);
    else SDReadScreen(areaSavingMessage, lastLineOnly); 
  //
  byte prevVar = curVariation;
  byte prevTrack = curTrack;
  SDSFileOpen(fileType1, fileWrite, true);
  //
  for (byte x=0; x<VARIATIONS; x++)
  {
    curVariation = x;
    fW('A' + curVariation);
    SDWriteSpace();
    for (byte w=0; w<(DRUM_TRACKS+ACCENT_TRACKS); w++)
    {
      curTrack = w;
      slaveGetDrumSteps();
      for (byte s=0; s<STEPS; s++)
      {
        byte nStep = bitRead(drumSteps[0],s);
        byte dStep = bitRead(drumSteps[1],s);
        //
        if (nStep == 1 && dStep == 0) fW('X');
        else if (nStep == 1 && dStep == 1) fW('D');
        else if (nStep == 0 && dStep == 1) fW('d');
        else fW('.');
      }
      SDWriteSpace();
    }
    SDWriteNewLine();
    //
    for (byte w=noteTrack; w<TOTAL_TRACKS; w++)
    {
      curTrack = w;
      slaveGetNoteSteps(0);
      slaveGetNoteSteps(1);
      for (byte s=0; s<STEPS; s++)
      {
        if (noteTrackMode[w-noteTrack] == noteTrackModeNotes)
        {
          byte xcode = getTrackNoteCode(s);
          if (xcode == noteOff) fW('X');
          else if (xcode == noteDouble) fW('D');
          else if (xcode == noteDoubleIn) fW('d');
          else if (xcode == noteSlide) fW('L');
          else fW('N');
          //
          SDWriteASCII3Digits(getTrackVelocity(s));
          SDWriteASCII3Digits(getTrackNote(s));
        }
        else
        {
          fW('M');
          SDWriteASCII3Digits(noteSteps[s]);
          SDWriteASCII3Digits(noteSteps[s+STEPS]);
        }
      }
      SDWriteNewLine();
    }
    SDWriteNewLine();
  }
  // 
  SDSFileWriteFinish();
  //
  SDSFileOpen(fileType2, fileWrite, true);
  fW('P'); fW('N'); SDWriteASCII2Digits(curPatternNameValue); SDWriteSpace();
  fW('V'); SDWriteASCII2Digits(totalVariations); SDWriteNewLine();
  //
  for (byte x=0; x<DRUM_TRACKS; x++)
  {
    curTrack = x;
    slaveGetTrackSettings();
    fW('T'); SDWriteASCII2Digits(x+1); SDWriteSpace();
    fW('M'); SDWriteASCII2Digits(trackMidiCH+1); SDWriteSpace();
    fW('N'); fW('T'); SDWriteASCII3Digits(trackNote); SDWriteSpace();
    fW('N'); SDWriteASCII2Digits(trackName[x]); SDWriteSpace();
    fW('P'); SDWriteASCII2Digits(trackProcessor);
    SDWriteNewLine();
  }
  // 
  for (byte x=noteTrack; x<TOTAL_TRACKS; x++)
  {
    curTrack = x;
    slaveGetTrackSettings();
    fW('T'); fW('N'); fW('1'+x-noteTrack); SDWriteSpace();
    fW('M'); SDWriteASCII2Digits(trackMidiCH+1); SDWriteSpace();
    fW('N'); SDWriteASCII2Digits(trackName[x]); SDWriteSpace();
    fW('P'); SDWriteASCII2Digits(trackProcessor); SDWriteSpace();
    fW('M'); fW('D'); SDWriteASCII2Digits(noteTrackMode[x-noteTrack]);
    SDWriteNewLine();
  }
  // 
  SDSFileWriteFinish();  
  //
  curTrack = prevTrack;
  curVariation = prevVar;
  changeTrack(0);
  reloadScreen = true;
  updateDisplayPattern();
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDLoadPattern(byte variation, byte fileType)
{
  byte prevVar = curVariation;
  byte prevTrack = curTrack;
  SDSFileOpen(fileType, fileRead, true);
  //
  curVariation = variation;
  SDSkipBytes((variation*SD_PATTERN_VARIATION_SIZE)+2);
  for (byte w=0; w<(DRUM_TRACKS+ACCENT_TRACKS); w++)
  {
    curTrack = w;
    drumSteps[0] = drumSteps[1] = 0;
    for (byte s=0; s<STEPS; s++)
    {
      char nStep = fR();
      if (nStep == 'X') { bitSet(drumSteps[0], s); }
      else if (nStep == 'D') { bitSet(drumSteps[0], s); bitSet(drumSteps[1], s); }
      else if (nStep == 'd') { bitSet(drumSteps[1], s); }
    }
    SDSkipBytes(1);
    slaveSetDrumSteps();
  }
  SDReadSkipNewLine();
  //
  for (byte w=noteTrack; w<TOTAL_TRACKS; w++)
  {
    curTrack = w;
    for (byte s=0; s<STEPS; s++)
    {
      noteSteps[s] = noteSteps[s+STEPS] = 0;
      char xcode = fR();
      noteSteps[s] = SDReadASCII3Digits();
      noteSteps[s+STEPS] = SDReadASCII3Digits();
      //
      if (xcode == 'X') setTrackNoteOff(s);
      else if (xcode == 'D') setTrackDouble(s);
      else if (xcode == 'd') setTrackDoubleIn(s);
      else if (xcode == 'L') setTrackSlide(s);
    }
    SDReadSkipNewLine();
    slaveSetNoteSteps(0);
    slaveSetNoteSteps(1);
  }
  // 
  tmpFile.close();
  curTrack = prevTrack;
  curVariation = prevVar;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDLoadEntirePattern(byte fileType)
{
  SDLoadPatternSettings(fileType+1, false);
  SDLoadPattern(0, fileType);
  if (totalVariations >= 1) SDLoadPattern(1, fileType);
  if (totalVariations >= 2) SDLoadPattern(2, fileType);
  if (totalVariations == 3) SDLoadPattern(3, fileType);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDSaveCurrentTrack()
{
  SDReadScreen(areaCopyingMessage, lastLineOnly);
  //
  byte prevVar = curVariation;
  if (curTrack < noteTrack) SDSFileOpen(fileDrumTrackCopy, fileWrite, true);
    else SDSFileOpen(fileNoteTrackCopy, fileWrite, true);
  //
  for (byte x=0; x<VARIATIONS; x++)
  {
    curVariation = x;
    fW('A' + curVariation);
    SDWriteSpace();
    if (curTrack < 16)
    {
      slaveGetDrumSteps();
      for (byte s=0; s<STEPS; s++)
      {
        byte nStep = bitRead(drumSteps[0],s);
        byte dStep = bitRead(drumSteps[1],s);
        //
        if (nStep == 1 && dStep == 0) fW('X');
        else if (nStep == 1 && dStep == 1) fW('D');
        else if (nStep == 0 && dStep == 1) fW('d');
        else fW('.');
      }
    }
    else
    {
      slaveGetNoteSteps(0);
      slaveGetNoteSteps(1);
      for (byte s=0; s<16; s++)
      {
        if (noteTrackMode[curTrack-noteTrack] == noteTrackModeNotes)
        {
          byte xcode = getTrackNoteCode(s);
          if (xcode == noteOff) fW('X');
          else if (xcode == noteDouble) fW('D');
          else if (xcode == noteDoubleIn) fW('d');
          else if (xcode == noteSlide) fW('L');
          else fW('N');
          //
          SDWriteASCII3Digits(getTrackVelocity(s));
          SDWriteASCII3Digits(getTrackNote(s));
        }
        else
        {
          fW('M');
          SDWriteASCII3Digits(noteSteps[s]);
          SDWriteASCII3Digits(noteSteps[s+STEPS]);
        }
      }
    }
    //
    SDWriteNewLine();
  }
  // 
  fW('M'); SDWriteASCII2Digits(trackMidiCH+1); SDWriteSpace();
  fW('N'); fW('T'); SDWriteASCII3Digits(trackNote); SDWriteSpace();
  fW('N'); SDWriteASCII2Digits(trackName[curTrack]); SDWriteSpace();
  fW('P'); SDWriteASCII2Digits(trackProcessor); SDWriteSpace();
  fW('M'); fW('D'); SDWriteASCII2Digits(noteTrackMode[curTrack-noteTrack]);
  // 
  SDSFileWriteFinish();  
  //
  curVariation = prevVar;
  changeTrack(0);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDLoadCurrentTrack()
{
  byte prevVar = curVariation;
  if (curTrack < noteTrack) SDSFileOpen(fileDrumTrackCopy, fileRead, true);
    else SDSFileOpen(fileNoteTrackCopy, fileRead, true);
  //
  for (byte x=0; x<VARIATIONS; x++)
  {
    curVariation = x;
    SDSkipBytes(2);
    if (curTrack < noteTrack)
    {
      drumSteps[0] = drumSteps[1] = 0;
      for (byte s=0; s<STEPS; s++)
      {
        char nStep = fR();
        if (nStep == 'X') { bitSet(drumSteps[0], s); }
        else if (nStep == 'D') { bitSet(drumSteps[0], s); bitSet(drumSteps[1], s); }
        else if (nStep == 'd') { bitSet(drumSteps[1], s); }
      }
      slaveSetDrumSteps();
    }
    else
    {
      for (byte s=0; s<STEPS; s++)
      {
        noteSteps[s] = noteSteps[s+STEPS] = 0;
        char xcode = fR();
        noteSteps[s] = SDReadASCII3Digits();
        noteSteps[s+STEPS] = SDReadASCII3Digits();
        //
        if (xcode == 'X') setTrackNoteOff(s);
        else if (xcode == 'D') setTrackDouble(s);
        else if (xcode == 'd') setTrackDoubleIn(s);
        else if (xcode == 'L') setTrackSlide(s);
      }
      slaveSetNoteSteps(0);
      slaveSetNoteSteps(1);
    }
    //
    SDReadSkipNewLine();
  }
  //
  SDSkipBytes(1);
  trackMidiCH = SDReadASCII2Digits()-1;
  SDSkipBytes(3); 
  trackNote = SDReadASCII3Digits();
  SDSkipBytes(2); 
  trackName[curTrack] = SDReadASCII2Digits();
  SDSkipBytes(2); 
  trackProcessor = SDReadASCII2Digits();
  SDSkipBytes(3); 
  noteTrackMode[curTrack-noteTrack] = SDReadASCII2Digits();
  slaveSendTrackSettings(false);
  //
  tmpFile.close();
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDLoadChordCustom()
{
  SDSFileOpen(fileChordCustom, fileRead, true);
  //
  for (byte x=0; x<12; x++)
  {
    SDSkipBytes(3);
    for (byte y=0; y<5; y++)
    {
      customChords[x][y] = SDReadASCII2Digits();
      SDSkipBytes(1);
    }
    SDReadSkipNewLine();
    slaveSetCustomChord(x);
  }
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDSaveChordCustom()
{
  if (!customChordChanged) return;
  customChordChanged = false;
  //
  SDReadScreen(areaSavingMessage, lastLineOnly);
  lateRedrawDisplay = true;
  //  
  SDSFileOpen(fileChordCustom, fileWrite, true);
  //
  for (byte x=0; x<12; x++)
  {
    SDWriteASCII2Digits(x);
    SDWriteSpace();
    for (byte y=0; y<5; y++)
    {
      SDWriteASCII2Digits(customChords[x][y]);
      SDWriteSpace();
    }
    SDWriteNewLine();
  }
  //
  SDSFileWriteFinish();
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
byte SDReadNextConfigValue()
{
  SDSkipBytes(26);
  //
  byte xvalue = fR();
  if (xvalue == '\'')
  {
    xvalue = fR();
    fR();
  }
  else
  {
    xvalue = (xvalue - '0') * 100;
    xvalue += SDReadASCII2Digits();
  }
  //
  SDReadSkipNewLine();
  //
  return xvalue;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDReadString(byte xposition) // Now, the file can only hold up to 255 characters - and we don't close the file here //
{
  fileS[9] = 't';
  SDFileSOpen();
  tmpFile.seek(xposition);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SDReadConfigurations()
{
  SDFileOpen('s'-'0', 0, 0);
  if ((SDReadNextConfigValue() == 1)) slaveSendCode(slvStartUSBserial, 0, 0, 0); else slaveSendCode(slvStartMIDIserial, 0, 0, 0); 
  slaveSendCode(slvSetFadeMin, SDReadNextConfigValue(), 0, 1);
  buttonsClickCounter = SDReadNextConfigValue(); 
  buttonsHoldCounter = SDReadNextConfigValue(); 
  buttonsRepeatCounter = SDReadNextConfigValue(); 
  buttonsDecay = SDReadNextConfigValue(); 
  buttonsShiftHoldCounter = SDReadNextConfigValue(); 
  nextPatternCC = (SDReadNextConfigValue()*10); 
  //
  #if V2_BOARD
    buttonsClickCounter *= 4;
    buttonsHoldCounter *= 6;
    buttonsRepeatCounter *= 6;
    buttonsShiftHoldCounter *= 4;
    nextPatternCC *= 8;
  #endif
  //
  stepNoteOffChar = SDReadNextConfigValue(); 
  regularNoteChar = SDReadNextConfigValue(); 
  stepSlideChar = SDReadNextConfigValue(); 
  loadingIcon = SDReadNextConfigValue(); 
  if (SDReadNextConfigValue() == 1) currentMode = songMode;
  if (SDReadNextConfigValue() == 0) blinkBeat = false;
  tmpFile.close();
}
