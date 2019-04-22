/*
 * Beat707 Mikro - created by William (Guilherme) Kalfelz (c) 2016 - www.Beat707.com - www.Wusik.com - www.William-k.com
 * 
 */

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void slaveInit()
{
  if (slaveRequestByte(slvInit) != slvInitOk) // Init Return Code //
  {
    printErrorMsg(WIRE_ERROR,0); 
  }
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void slaveBeginWrite(byte xcode)
{
  Wire.beginTransmission(SLAVE_ID);
  Wire.write(xcode);  
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void slaveSendCode(byte bcode, byte bcode2, byte bcode3, byte extraCodes)
{
  slaveBeginWrite(bcode);
  if (extraCodes > 0) Wire.write(bcode2);
  if (extraCodes > 1) Wire.write(bcode3);
  slaveEndTransmission();
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void slaveEndTransmission()
{
  byte xmsg = Wire.endTransmission();
  if (xmsg != 0) printErrorMsg(WIRE_ERROR+xmsg, 0);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void slaveWaitByte()
{
  if (Wire.available() > 0) return;
  //
  int xc = 0;
  while (Wire.available() == 0) 
  { 
    xc++; 
    if (xc > 10000)
    {
      printErrorMsg(WIRE_ERROR, "TO");
      break;
    }
  }
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void slaveRequestBytes(byte code, byte xbytes, byte bcode2 = 0, byte bcode3 = 0, byte extraCodes = 0)
{
  slaveSendCode(code, bcode2, bcode3, extraCodes);
  if (Wire.requestFrom((uint8_t)SLAVE_ID, (uint8_t)xbytes) != xbytes)
  {
    Wire.flush();
    slaveRequestBytes(code, xbytes, bcode2, bcode3, extraCodes);
  }
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
byte slaveRequestNextByte()
{
  slaveWaitByte();
  byte bB = Wire.read();
  return bB;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
byte slaveRequestByte(byte code)
{
  slaveRequestBytes(code, 1);
  return slaveRequestNextByte();
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void slaveGetTrackSettings()
{
  slaveRequestBytes(slvGetTrackSettings, 4, curTrack, 0, 1);
  //
  trackProcessor = slaveRequestNextByte();
  trackMidiCH = slaveRequestNextByte();
  trackNote = slaveRequestNextByte();
  noteTrackMode[curTrack-noteTrack] = slaveRequestNextByte();
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void slaveGetDrumSteps()
{
  if (curTrack >= noteTrack) return;
  slaveRequestBytes(slvGetDrumSteps, 4, curVariation, curTrack, 2);
  drumSteps[0] = getIntNumber();
  drumSteps[1] = getIntNumber();
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void slaveGetNoteSteps(byte area)
{
  if (curTrack < noteTrack) return;
  slaveRequestBytes(slvGetNoteSteps1+area, STEPS, curVariation, curTrack, 2);
  //
  for (byte x=0; x<STEPS; x++)
  {
    noteSteps[x+(area*STEPS)] = slaveRequestNextByte();
  }
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void slavePlay(byte play)
{
 slaveSendCode(slvStop+play, 0, 0, 0);
 checkSavePattern();
 checkSaveSong();
 beatCheckLateRedraw = true;
 slaveCheckBeatCounter = CHECK_BEAT_COUNTER + 1;
 slaveCheckBeat();
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void slaveSendTrackSettings(boolean sendToBuffer)
{
  if (sendToBuffer) slaveBeginWrite(slvSetTrackSettingsBuffer); else slaveBeginWrite(slvSetTrackSettings);
  Wire.write(curTrack);
  Wire.write(trackProcessor);
  Wire.write(trackMidiCH);
  Wire.write(trackNote);
  Wire.write(noteTrackMode[curTrack-noteTrack]);
  slaveEndTransmission();
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void slaveSetCustomChord(byte xchord)
{
  slaveBeginWrite(slvSetCustomChord);
  Wire.write(xchord);
  for (byte x=0; x<5; x++) { Wire.write(customChords[xchord][x]); }
  slaveEndTransmission();
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void slaveSendSequencerSettings(boolean sendToBuffer)
{
  if (sendToBuffer)
  {
    slaveSendCode(slvSetSequencerSettingsBuffer, totalVariations, 0, 1);
  }
  else
  {
    slaveBeginWrite(slvSetSequencerSettings);
    byte xc = 0;
    if (seqSyncOut) xc = 1;
    Wire.write(xc);
    Wire.write(seqBPM);
    Wire.write(totalVariations);
    Wire.write(seqType);
    slaveEndTransmission();
  }
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void slaveSetSingleStep()
{
  if (isMenuDown())
  {
    if (isMirrorMode) slaveSendCode(slvNextSingleStepNoMirror, 0, 0, 0);
      else slaveSendCode(slvNextSingleStepForceMirror, 0, 0, 0);
  }
  //
  slaveBeginWrite(slvSetSingleStep);
  if (curTrack < noteTrack)
  {
    Wire.write(dZone2);
    byte bB = 0;
    bB = (bitRead(drumSteps[0], dZone2) | (bitRead(drumSteps[1], dZone2) << 1));
    Wire.write(bB);
  }
  else
  {
    byte xz = dZone2;
    if (dArea == areaNoteTrackEdit) xz = curStep;
    Wire.write(xz);
    Wire.write(noteSteps[xz]);
    Wire.write(noteSteps[xz+STEPS]);
  }
  slaveEndTransmission();
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void slaveSetDrumSteps()
{
  if (curTrack >= noteTrack) return;
  slaveBeginWrite(slvSetDrumSteps);
  slaveSendTrackVariation();
  sendIntNumber(drumSteps[0]);
  sendIntNumber(drumSteps[1]);
  slaveEndTransmission();  
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void slaveSetNoteSteps(byte area)
{
  if (curTrack < noteTrack) return;
  slaveBeginWrite(slvSetNoteSteps1+area);
  slaveSendTrackVariation();
  Wire.write(noteSteps+(area*STEPS), STEPS);
  slaveEndTransmission();  
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void slaveSendTrackVariation()
{
  Wire.write(curVariation);  
  Wire.write(curTrack);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void sendIntNumber(int number)
{
  wBuf[0] = (number >> 8) & 0xFF;
  wBuf[1] = number & 0xFF;
  Wire.write(wBuf,2);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int getIntNumber()
{
  int number = 0;
  byte bB1 = 0, bB2 = 0;
  bB1 = slaveRequestNextByte();
  bB2 = slaveRequestNextByte();
  number = bB1;
  number = (number << 8) | bB2;  
  return number;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void slaveCheckBeat()
{
  if (!blinkBeat) return;
  //
  slaveCheckBeatCounter++;
  if (slaveCheckBeatCounter > CHECK_BEAT_COUNTER)
  {
    slaveCheckBeatCounter = 0;
    byte xevent = slaveRequestByte(slvGetBeat);
    byte xcurvariation = (xevent >> 4) & 0xF;
    if (xevent < 255) xevent = xevent & 0xF;
    //
    switch (xevent)
    {
      case 1:
        curPlayingVariation = xcurvariation + 1;
        if (streamPatternGo)
        {
          showPatternLoading(true);
        }
        else
        {
          hideCursor = false;
          displayCursor();
        }
        if (dArea <= areaNoteTrackEdit) doRedrawDisplay();
        beatCheckLateRedraw = true;
        break;

      case 9:
        curPlayingVariation = 0;
        if (streamPatternGo)
        {
          showPatternLoading(false);
        }
        else
        {
          hideCursor = true;
          displayCursor();
        }
        if (dArea <= areaNoteTrackEdit) doRedrawDisplay();
        beatCheckLateRedraw = true;
        break;

      case 255:
        if (beatCheckLateRedraw)
        {
          if (streamPatternGo)
          {
            showPatternLoading(true);
          }
          else
          {
            hideCursor = false;
            displayCursor();
          }
          //          
          curPlayingVariation = 0;
          beatCheckLateRedraw = false;
          doRedrawDisplay();
        }
        break;
    }
  }
}
