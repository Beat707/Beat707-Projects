/*
 * Beat707 Mikro - created by William (Guilherme) Kalfelz (c) 2016 - www.Beat707.com - www.Wusik.com - www.William-k.com
 * 
 */

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ISR(TIMER1_COMPA_vect) 
{
  tickSequencer++;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void doTickSequencer()
{
  if (tickSequencer == 0) return;
  //
  if (seqSyncOut)
  {
    if (ppq24Counter == 0) Serial.write(0xF8); // Midi Clock Sync Tick 24PPQ
    ppq24Counter++;
    if (ppq24Counter >= 4) ppq24Counter = 0;
  }
  //
  if (seqCounter == 0 || seqCounter == 12)
  {
    midiClockBeats++;
    if (midiClockBeats >= 16) midiClockBeats = 0;
    
    // ----------------------- //

    byte xVel = velAccent0;
    byte isDouble = 1;
    if (seqCounter == 0) isDouble = 0;
    xVel += bitRead(drumSteps[playingVariation][accentTrack][isDouble], seqPosition) * velAccent1;
    xVel += bitRead(drumSteps[playingVariation][accentTrack+1][isDouble], seqPosition) * velAccent2;
    //
    for (byte x=0; x<DRUM_TRACKS; x++)
    {
      if (bitRead(drumSteps[playingVariation][x][isDouble], seqPosition) == 1)
      {
        trackNoteOn(x, trackNote[x], getMPVelocity(x, trackProcessor[x], xVel), false); 
      }
    }

    // ----------------------- //

    for (byte x=0; x<NOTE_TRACKS; x++)
    {
      switch (noteTrackMode[x])
      {
        case noteTrackModeNotes:
          {
            byte xcode = getTrackNoteCode(x); // regularNote = 0, noteOff, noteDouble, noteDoubleIn, noteSlide, // The last is when Velocity=0 and Note>0
            byte xNote = getTrackNote(x);
            boolean doNote = false;
            if (xNote > 0) doNote = true;
            if (isDouble == 1 && xcode != noteDouble && xcode != noteDoubleIn) doNote = false;
            if (isDouble == 0 && xcode == noteDoubleIn) doNote = false;
            //
            if (xcode == noteOff) trackPrevNoteOff(16+x);
            else if (doNote) trackNoteOn(noteTrack+x, xNote, getMPVelocity(x, trackProcessor[16+x], getTrackVelocity(x)), (xcode == noteSlide));
          }
          break;

        case noteTrackModeCC:
          {
            if (isDouble == 0 && noteSteps[playingVariation][x][seqPosition] > 0)
              sendMidiEvent(midiCC, noteSteps[playingVariation][x][seqPosition]-1, noteSteps[playingVariation][x][seqPosition+STEPS], trackMidiCH[x]);
          }
          break;
      }
    }
  }
  //
  seqCounter++;
  if (seqCounter >= 24)
  {
    if (streamSize < (STEPS*VARIATIONS)) streamSize++;
    seqCounter = 0;
    seqPosition++;
    if (seqPosition >= STEPS) 
    {
      seqPosition= 0;
      playingVariation++;
      if (playingVariation > totalVariations)
      {
        playingVariation = 0;
        //
        if (switchSettings)
        {
          switchSettings = false;
          totalVariations = totalVariationsBuffer;
          for (byte x=0; x<TOTAL_TRACKS; x++)
          {
            if (trackProcessor[x] != trackProcessorBuffer[x] || trackMidiCH[x] != trackMidiCHBuffer[x]) trackPrevNoteOff(x);
            trackProcessor[x] = trackProcessorBuffer[x];
            trackMidiCH[x] = trackMidiCHBuffer[x];
            if (x < DRUM_TRACKS) trackNote[x] = trackNoteBuffer[x];
          }
        }
      }
    }    
  }
  tickSequencer--;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
byte getMPVelocity(byte xtrack, byte xproc, byte xorgvelocity)
{
  switch(xproc)
  {    
    case fadeIn:    
      return max(procFadeMin,min(127,xorgvelocity-(128-((seqPosition + (playingVariation * 16)) * 2))));
      break;
      
    case fadeOut:   
      return max(procFadeMin,min(127,xorgvelocity-(((seqPosition + (playingVariation * 16)) * 2))));
      break;
      
    case fadeInD:
      if (playingVariation == 3) return max(procFadeMin,min(127,xorgvelocity-(128-(seqPosition * 9))));
      break;

    case fadeOutD:
      if (playingVariation == 3) return max(procFadeMin,min(127,xorgvelocity-((seqPosition * 9))));
      break;

    case fadeInOut:
      if (playingVariation <= 1) return max(procFadeMin,min(127,xorgvelocity-(128-((seqPosition + (playingVariation * 16)) * 4))));
        else return max(procFadeMin,min(127,xorgvelocity-(((seqPosition + ((playingVariation-2) * 16)) * 4))));
      break;

    case fadeOutIn:
      if (playingVariation <= 1) return max(procFadeMin,min(127,xorgvelocity-(((seqPosition + (playingVariation * 16)) * 4))));
        else return max(procFadeMin,min(127,xorgvelocity-(128-((seqPosition + ((playingVariation-2) * 16)) * 4))));
      break;

    case randomVel:
      return max(procFadeMin,min(127,xorgvelocity-byte(random(procFadeMin, 127))));
      break;
  }
  //
  return xorgvelocity;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void sendChordNotes(byte xtype, byte xnote1, byte xnote2, byte xvelocity, byte xtrack)
{
  sendMidiEvent(xtype, min(127, xnote1), xvelocity, trackMidiCH[xtrack]);
  if (xnote2 > 0) sendMidiEvent(xtype, min(127, xnote2), xvelocity, trackMidiCH[xtrack]);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void trackMPNotes(byte xtrack, byte xnote, byte xvelocity, byte xtype)
{
  if (trackProcessor[xtrack] < chordMaj) return;
  //
  byte dType = midiNoteOn;
  if (xtype == noteOff)
  {
    dType = midiNoteOff;
    xvelocity = 0;
  }
  //
  switch (trackProcessor[xtrack])
  {
    case chordMaj:    sendChordNotes(dType, xnote + 4, xnote + 7, xvelocity, xtrack); break;
    case chordMin:    sendChordNotes(dType, xnote + 3, xnote + 7, xvelocity, xtrack); break;
    case chord57:     sendChordNotes(dType, xnote + 5, xnote + 7, xvelocity, xtrack); break;
    case octave1:     sendChordNotes(dType, xnote + 12, 0, xvelocity, xtrack); break;
    case octave12:    sendChordNotes(dType, xnote + 12, xnote + 24, xvelocity, xtrack); break;
    case chordCustom:
      {
        byte noteT = (xnote%12);
        byte noteOct = (xnote/12)*12;
        for (byte x=0; x<5; x++)
        {
          if (customChords[noteT][x] > 0) sendMidiEvent(dType, min(127, customChords[noteT][x] + noteOct - 1), xvelocity, trackMidiCH[xtrack]);
        }
      }
      break;
  }
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void trackPrevNoteOff(byte xtrack)
{
  if (prevTrackNote[xtrack] != 0)
  {
    trackMPNotes(xtrack, prevTrackNote[xtrack], 0, noteOff);
    if (trackProcessor[xtrack] != chordCustom) sendMidiEvent(midiNoteOff, prevTrackNote[xtrack], 0, trackMidiCH[xtrack]);
    prevTrackNote[xtrack] = 0; 
  }
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void trackNoteOn(byte xtrack, byte xnote, byte xvelocity, boolean xslide)
{
  if (!xslide) trackPrevNoteOff(xtrack);
  else if (xvelocity == 0 && xtrack >= 16) xvelocity = prevTrackVelocity[xtrack-16];
  //
  if (trackProcessor[xtrack] != chordCustom) sendMidiEvent(midiNoteOn, xnote, xvelocity, trackMidiCH[xtrack]);
  trackMPNotes(xtrack, xnote, xvelocity, noteOn);
  //
  if (xslide) trackPrevNoteOff(xtrack);
  //
  prevTrackNote[xtrack] = xnote;
  if (xtrack >= 16 && xvelocity > 0) prevTrackVelocity[xtrack-16] = xvelocity;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void resetSequencer()
{
  seqPosition = seqCounter = ppq24Counter = playingVariation = midiClockBeats = streamSize = 0;
  streamPosition = streamVariation = 0;
  //
  for (byte x=0; x<DRUM_TRACKS; x++)
  {
    if (prevTrackNote[x] != 0) sendMidiEvent(midiNoteOff, prevTrackNote[x], 0, trackMidiCH[x]);
    prevTrackNote[x] = 0;
  }
  //
  for (byte x=0; x<NOTE_TRACKS; x++)
  {
    if (noteTrackMode[x] == noteTrackModeNotes && prevTrackNote[x] != 0) sendMidiEvent(midiNoteOff, prevTrackNote[noteTrack+x], 0, trackMidiCH[x]);
    prevTrackNote[noteTrack+x] = prevTrackVelocity[x] = 0;
  }
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void startSequencer()
{
  seqPlaying = 1;
  resetSequencer();  
  tickSequencer = 1;
  doTickSequencer();
  if (seqSyncOut) Serial.write(0xFA); // MIDI Start
    
  // Start Timer //
  TCCR1A = TCCR1B = 0;
  bitWrite(TCCR1B, CS11, 1);
  bitWrite(TCCR1B, WGM12, 1);
  updateSequencerSpeed();
  bitWrite(TIMSK1, OCIE1A, 1);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void updateSequencerSpeed()
{
  // Calculates the Frequency for the Timer, used by the PPQ clock (Pulses Per Quarter Note) 96 in this case //
  // This uses the 16-bit Timer1, unused by the Arduino, unless you use the analogWrite or Tone functions //
  //
  OCR1A = (F_CPU / 8) / ((((seqBPM)*(96))/60)) - 1;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void stopSequencer(void)
{
  // Stop Timer //
  bitWrite(TIMSK1, OCIE1A, 0);
  TCCR1A = TCCR1B = OCR1A = 0;
  tickSequencer = 0;
  //
  if (seqSyncOut) Serial.write(0xFC); // MIDI Stop
  //
  seqPlaying = 0;
  resetSequencer();
  MIDIallNotesOff();
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
byte getTrackNote(byte xtrack)
{
  return (noteSteps[playingVariation][xtrack][seqPosition+STEPS] & 0x7F); // Getting only 7 bits
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
byte getTrackVelocity(byte xtrack)
{
  return (noteSteps[playingVariation][xtrack][seqPosition] & 0x7F); // Getting only 7 bits
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
byte getTrackNoteCode(byte xtrack)
{
  byte xcode = (noteSteps[playingVariation][xtrack][seqPosition+STEPS] >> 7) & 0x01;
  xcode |= ((noteSteps[playingVariation][xtrack][seqPosition] >> 6) & 0x02);
  if (xcode == 0 && getTrackVelocity(xtrack) == 0 && getTrackNote(xtrack) > 0) xcode = noteSlide;
  return xcode;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void transposeAll(char value)
{
  for (byte t=0; t<NOTE_TRACKS; t++)
  {
    for (byte v=0; v<VARIATIONS; v++)
    { 
      for (byte x=0; x<STEPS; x++)
      {
        byte xnote = (noteSteps[v][t][x+STEPS] & 0x7F);
        //
        if (xnote > 0)
        {
          if (value > 0 && xnote < 127) xnote++;
          else if (value < 0 && xnote > 1) xnote--;
          //
          byte xcode = (noteSteps[v][t][x+STEPS] & 0x80);
          noteSteps[v][t][x+STEPS] = xnote | xcode;
        }
      }
    } 
  }
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void slideStep(char xslide, byte xvariation, byte xstep)
{
  if (curTrack < noteTrack)
  {
    byte x1 = bitRead(drumSteps[xvariation][curTrack][0], xstep);
    byte x1d = bitRead(drumSteps[xvariation][curTrack][1], xstep);
    byte x2 = bitRead(drumSteps[xvariation][curTrack][0], xstep+xslide);
    byte x2d = bitRead(drumSteps[xvariation][curTrack][1], xstep+xslide);
    //
    bitWrite(drumSteps[xvariation][curTrack][0], xstep, x2);
    bitWrite(drumSteps[xvariation][curTrack][1], xstep, x2d);
    bitWrite(drumSteps[xvariation][curTrack][0], xstep+xslide, x1);
    bitWrite(drumSteps[xvariation][curTrack][1], xstep+xslide, x1d);
  }
  else
  {
    byte x1 = noteSteps[xvariation][curTrack-noteTrack][xstep];
    byte x1d = noteSteps[xvariation][curTrack-noteTrack][xstep+STEPS];
    byte x2 = noteSteps[xvariation][curTrack-noteTrack][xstep+xslide];
    byte x2d = noteSteps[xvariation][curTrack-noteTrack][xstep+STEPS+xslide];
    //
    noteSteps[xvariation][curTrack-noteTrack][xstep] = x2;
    noteSteps[xvariation][curTrack-noteTrack][xstep+STEPS] = x2d;
    noteSteps[xvariation][curTrack-noteTrack][xstep+xslide] = x1;
    noteSteps[xvariation][curTrack-noteTrack][xstep+STEPS+xslide] = x1d;
  }
}
