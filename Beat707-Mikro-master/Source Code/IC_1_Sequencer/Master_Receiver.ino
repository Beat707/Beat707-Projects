/*
 * Beat707 Mikro - created by William (Guilherme) Kalfelz (c) 2016 - www.Beat707.com - www.Wusik.com - www.William-k.com
 * 
 */

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
byte WBufIndex = 0;
byte *WBuffer = 0;
#define masterNextByte() WBuffer[WBufIndex++]
inline void wireSkipBytes(byte xbytes) { WBufIndex += xbytes; }
inline void wireWrite(byte xbyte) { WBuffer[WBufIndex++] = xbyte; }
void wireWrite(byte *xbyte, byte xbytes) { for (byte x=0; x<xbytes; x++) { wireWrite(xbyte[x]); } }

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void receiveEvent(byte receiveDataLength, byte* receiveData)
{
  WBufIndex = 0;
  WBuffer = receiveData;
  //
  wireRequestCode = masterNextByte();
  //
  switch(wireRequestCode)
  {
    case slvSendStream:
      {
        if (streamPosition == 0 && streamVariation == 0) switchSettings = true;
        //
        int number1 = masterNextByte();
        number1 = (number1 << 8) | masterNextByte();
        int number2 = masterNextByte();;
        number2 = (number2 << 8) | masterNextByte();
        //
        for (byte x=0; x<DRUM_TRACKS; x++)
        {
          bitWrite(drumSteps[streamVariation][x][0], streamPosition, bitRead(number1, x));
          bitWrite(drumSteps[streamVariation][x][1], streamPosition, bitRead(number2, x));
        }
        //
        for (byte x=0; x<NOTE_TRACKS; x++)
        {
          noteSteps[streamVariation][x][streamPosition] = masterNextByte();
          noteSteps[streamVariation][x][streamPosition+STEPS] = masterNextByte();
        }
        //
        streamPosition++;
        if (streamPosition >= STEPS)
        {
          streamPosition = 0;
          streamVariation++;
          if (streamVariation >= VARIATIONS) streamVariation = 0;
        }
      }
      if (streamSize > 0) streamSize--;
      break;
      
    case slvClearPattern:
      clearPattern();
      break;

    case slvTransposeAll:
      if (masterNextByte() == 1) transposeAll(1); else transposeAll(-1);
      break;

    case slvStartStream:
      streamPosition = streamVariation = 0;
      streamSize = seqPosition + (playingVariation*16);
      break;

    case slvClearTrack:
      curTrack = masterNextByte();
      if (curTrack < noteTrack)
      {
        drumSteps[0][curTrack][0] = drumSteps[0][curTrack][1] =
        drumSteps[1][curTrack][0] = drumSteps[1][curTrack][1] =
        drumSteps[2][curTrack][0] = drumSteps[2][curTrack][1] =
        drumSteps[3][curTrack][0] = drumSteps[3][curTrack][1] = 0;
      }
      else
      {
        for (byte x=0; x<(16*2); x++) 
        { 
          noteSteps[0][curTrack-noteTrack][x] =
          noteSteps[1][curTrack-noteTrack][x] = 
          noteSteps[2][curTrack-noteTrack][x] = 
          noteSteps[3][curTrack-noteTrack][x] = 0; 
        }
      }
      break;
      
    case slvStartMIDIserial:
      startMIDIinterface(false);
      break;

    case slvNextSingleStepNoMirror:
      nextSingleStepNoMirror = true;
      break;

    case slvNextSingleStepForceMirror:
      nextSingleStepForceMirror = true;
      break;
      
    case slvStartUSBserial:
      startMIDIinterface(true);
      break;
      
    case slvStop:
      stopSequencer();
      break;

    case slvPlay:
      startSequencer();
      break;
          
    case slvGetTrackSettings:
      curTrack = masterNextByte();
      break;

    case slvSetFadeMin:
      procFadeMin = masterNextByte();
      break;

    case slvSetMirrorMode:
      isMirrorMode = (masterNextByte() == 1);
      break;
      
    case slvSetTrackSettings:
      curTrack = masterNextByte();
      trackProcessor[curTrack] = masterNextByte();
      trackMidiCH[curTrack] = masterNextByte();
      if (curTrack < DRUM_TRACKS) trackNote[curTrack] = masterNextByte(); else wireSkipBytes(1);
      if (curTrack >= noteTrack) noteTrackMode[curTrack-noteTrack] = masterNextByte(); else wireSkipBytes(1);
      break;

    case slvSetTrackSettingsBuffer:
      curTrack = masterNextByte();
      trackProcessorBuffer[curTrack] = masterNextByte();
      trackMidiCHBuffer[curTrack] = masterNextByte();
      if (curTrack < DRUM_TRACKS) trackNoteBuffer[curTrack] = masterNextByte(); else wireSkipBytes(1);
      if (curTrack >= noteTrack) noteTrackModeBuffer[curTrack-noteTrack] = masterNextByte(); else wireSkipBytes(1);
      break;

    case slvGetDrumSteps:
      curVariation = masterNextByte();
      curTrack = masterNextByte();      
      break;
      
    case slvGetNoteSteps1:
    case slvGetNoteSteps2:
      curVariation = masterNextByte();
      curTrack = masterNextByte();      
      break;

    case slvSetSequencerSettings:
      {
        seqSyncOut = (masterNextByte() == 1);
        byte newBPM = masterNextByte();
        if (seqBPM != newBPM)
        {
          seqBPM = newBPM;
          if (seqPlaying == 1) updateSequencerSpeed();
        }
        totalVariations = masterNextByte();
        seqType = masterNextByte();
      }
      break;

    case slvSetSequencerSettingsBuffer:
      totalVariationsBuffer = masterNextByte();
      break;
  
    case slvSetDrumSteps:
      curVariation = masterNextByte();
      curTrack = masterNextByte();      
      drumSteps[curVariation][curTrack][0] = getIntNumber();
      drumSteps[curVariation][curTrack][1] = getIntNumber();
      break;

    case slvSetCustomChord:
      {
        byte xchord = masterNextByte();
        for (byte x=0; x<5; x++) { customChords[xchord][x] = masterNextByte(); }
      }
      break;

    case slvSetNoteSteps1:
    case slvSetNoteSteps2:
      curVariation = masterNextByte();
      curTrack = masterNextByte();      
      for (byte x=0; x<STEPS; x++)
      {
        noteSteps[curVariation][curTrack-noteTrack][x+((wireRequestCode-slvSetNoteSteps1)*STEPS)] = masterNextByte();
      }
      break;

    case slvSlideStep:
      {
        char value = -1;
        if (masterNextByte() > 0) value = 1;
        byte xstep = masterNextByte();
        if (isMirrorMode)
        {
          for (byte v=0; v<VARIATIONS; v++)
          {
            slideStep(value, v, xstep);
          }
        }
        else
        {
          slideStep(value, curVariation, xstep);
        }
      }
      break;
      
    case slvSetSingleStep:
      {
        byte xS = masterNextByte();
        if (curTrack < noteTrack)
        {
          bitClear(drumSteps[curVariation][curTrack][0], xS);
          bitClear(drumSteps[curVariation][curTrack][1], xS);
          byte bB1 = masterNextByte();
          if (bB1 == 1) { bitSet(drumSteps[curVariation][curTrack][0], xS); }
          else if (bB1 == 2) { bitSet(drumSteps[curVariation][curTrack][1], xS); }
          else if (bB1 == 3) { bitSet(drumSteps[curVariation][curTrack][1], xS); bitSet(drumSteps[curVariation][curTrack][0], xS); }
          //
          if ((isMirrorMode && !nextSingleStepNoMirror) || nextSingleStepForceMirror)
          {
            for (byte x=0; x<VARIATIONS; x++)
            {
              if (x != curVariation)
              {
                bitWrite(drumSteps[x][curTrack][0], xS, bitRead(drumSteps[curVariation][curTrack][0], xS));
                bitWrite(drumSteps[x][curTrack][1], xS, bitRead(drumSteps[curVariation][curTrack][1], xS));
              }
            }
          }
        }
        else
        {
          noteSteps[curVariation][curTrack-16][xS] = masterNextByte();
          noteSteps[curVariation][curTrack-16][xS+16] = masterNextByte();
          //
          if ((isMirrorMode && !nextSingleStepNoMirror) || nextSingleStepForceMirror)
          {
            for (byte x=0; x<VARIATIONS; x++)
            {
              if (x != curVariation)
              {
                noteSteps[x][curTrack-16][xS] = noteSteps[curVariation][curTrack-16][xS];
                noteSteps[x][curTrack-16][xS+16] = noteSteps[curVariation][curTrack-16][xS+16];
              }
            }
          }          
        }
      }
      nextSingleStepNoMirror = nextSingleStepForceMirror = false;
      break;
  }
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
byte requestEvent(byte transmitDataLengthMax, byte* transmitData)
{  
  WBufIndex = 0;
  WBuffer = transmitData;
  //
  switch(wireRequestCode)
  {      
    case slvStreamSize:
      wireWrite(streamSize);
      break;
      
    case slvIsPlaying:
      wireWrite(seqPlaying);
      break;
          
    case slvInit:
      wireWrite(slvInitOk);
      break;

    case slvGetBeat:
      if (seqPlaying == 1) wireWrite((midiClockBeats & 0xF) | ((playingVariation & 0xF) << 4));
        else wireWrite(0xFF);
      break;

    case slvGetTrackSettings:
      {
        wireWrite(trackProcessor[curTrack]);
        wireWrite(trackMidiCH[curTrack]);
        if (curTrack < DRUM_TRACKS) wireWrite(trackNote[curTrack]); else wireWrite(0);
        if (curTrack >= noteTrack) wireWrite(noteTrackMode[curTrack-noteTrack]); else wireWrite(0);
      }
      break;

    case slvGetDrumSteps:
      {
        wireWrite((drumSteps[curVariation][curTrack][0] >> 8) & 0xFF);
        wireWrite(drumSteps[curVariation][curTrack][0] & 0xFF);
        wireWrite((drumSteps[curVariation][curTrack][1] >> 8) & 0xFF);
        wireWrite(drumSteps[curVariation][curTrack][1] & 0xFF);
      }
      break;

    case slvGetNoteSteps1:
    case slvGetNoteSteps2:
      wireWrite(noteSteps[curVariation][curTrack-noteTrack]+((wireRequestCode-slvGetNoteSteps1)*STEPS), STEPS);
      break;
  }
  //
  return WBufIndex;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int getIntNumber()
{
  int number = 0;
  number = masterNextByte();
  number = (number << 8) | masterNextByte();  
  return number;
}
