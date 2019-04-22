/*
 * 
 * Created by William Kalfelz @ Beat707 (c) 2018 - http://www.Beat707.com
 * 
 */

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define DOUBLE_NOTE 3
#define MAX_NOTES 6
#define PPQ 24

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ISR(TIMER1_COMPA_vect) 
{
  if (configData.midiClockInternal) 
  {
    if (configData.seqSyncOut) Serial.write(0xF8); // Midi Clock Out //
    ledsBufferFlip();
    outputMIDIBuffer();
    calculateSequencer++;
  }
  else 
  {
    stopTimer(true);
    handleMIDIInput();
    startTimer(true);
  }
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void outputMIDIBuffer()
{
  while (calculateSequencer > 0) { doTickSequencer(); }
  sendOutputMIDIBuffer();
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void sendOutputMIDIBuffer()
{
  byte slot = 0;
  byte xpos = 0;
  //
  while (1)
  {
    if (midiOutputBuffer[slot][0][xpos] > 0)
    {
      Serial.write(midiOutputBuffer[slot][0][xpos]);
      Serial.write(midiOutputBuffer[slot][1][xpos]);
      Serial.write(midiOutputBuffer[slot][2][xpos]);
      midiOutputBuffer[slot][0][xpos] = 0;
    }
    //
    if (slot == 0) slot = 1;
    else
    {
      slot = 0;
      xpos++;
      if (xpos >= MIDI_OUTPUT_BUFFER) break;
      if ((midiOutputBuffer[0][0][xpos] == 0) && (midiOutputBuffer[1][0][xpos] == 0)) break;
    }
  }
  midiOutputBufferPosition[0] = midiOutputBufferPosition[1] = 0; 
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void doTickSequencer()
{
  while (calculateSequencer > 0)
  {
    if (seqPlaying)
    {
      // --------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
      #if ECHOS_ENABLED
        // Check For Echo //
        for (byte xe = 0; xe < ECHOS; xe++)
        {
          if (echoCounter[xe][0] > 0)
          {
            echoCounter[xe][1]--;
            if (echoCounter[xe][1] == 0) 
            {
              echoCounter[xe][0]--;
              if (echoCounter[xe][0] > 0) 
              {
                echoCounter[xe][1] = echoSpace[xe];
                //
                trackNoteOn(echoTrack[xe], echoNote[xe], getMPVelocity(patternData.trackProcessor[echoTrack[xe]-1], echoVelocity[xe]));
                //
                int newV = echoVelocity[xe] + echoAttackDecay[xe];
                if (newV < 0) newV = 0;
                if (newV > 127) newV = 127;
                echoVelocity[xe] = newV;
              }
            }
          }
        }
      #endif
      //
      // --------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
      //
      // Check Mute Tracks //
      if (prevMuteTrack != configData.muteTrack)
      {
        for (byte x=0; x<(NOTE_TRACKS+DRUM_TRACKS+1); x++)
        {
          if (bitRead(prevMuteTrack, x) != bitRead(configData.muteTrack, x) && bitRead(configData.muteTrack, x) == 1)
          {
            if (x >= NOTE_TRACKS) stopDrumTrackPrevNote(x - NOTE_TRACKS, true); else stopDrumTrackPrevNote(x, false);
          }
        }
        prevMuteTrack = configData.muteTrack;
      }
      //
      if (seqCounter == 0 || seqCounter == DOUBLE_NOTE)
      {
        midiClockBeats++;
        if (midiClockBeats >= 16) midiClockBeats = 0;
        //
        byte accent = bitRead(stepsData[seqPosition].steps[15], 1 + (variation * 2)) << 1;
        accent |= bitRead(stepsData[seqPosition].steps[15], 0 + (variation * 2));
        //
        // --------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
        //
        for (byte x=0; x<(DRUM_TRACKS-1); x++)
        {
          if (bitRead(configData.muteTrack, x) != 1)
          {
            byte xvel = bitRead(stepsData[seqPosition].steps[x], 1 + (variation * 2)) << 1;
            xvel |= bitRead(stepsData[seqPosition].steps[x], 0 + (variation * 2));
            bool isDouble = bitRead(stepsData[seqPosition].stepsDouble[variation], x);
            //
            if ((xvel > 0 && seqCounter == 0) || (seqCounter == DOUBLE_NOTE && isDouble))
            {
              byte orgxvel = xvel;
              if (xvel == 1) xvel = accent; // use accent track 
              else if (xvel == 2) xvel = 1;
              //
              if (xvel <= 0 || xvel > 3) xvel = 1;
              byte theVelocity = configData.accentValues[xvel-1];
              //
              #if ECHOS_ENABLED
                // Process Echo //
                for (byte xe = 0; xe < ECHOS; xe++)
                {
                  if (((patternData.echoConfig[xe].track - 1) == x) && 
                  ( (patternData.echoConfig[xe].type == echoTypeOnAllNotes) || ((patternData.echoConfig[xe].type == echoTypeForceMaxVelocity && orgxvel == 3)) || ((patternData.echoConfig[xe].type == echoTypeForceLowVelocity && orgxvel == 2)) ))
                  {
                    echoCounter[xe][0] = patternData.echoConfig[xe].ticks;
                    echoCounter[xe][1] = patternData.echoConfig[xe].space;
                    echoAttackDecay[xe] = patternData.echoConfig[xe].attackDecay;
                    echoSpace[xe] = patternData.echoConfig[xe].space;
                    echoTrack[xe] = patternData.echoConfig[xe].track - 1;
                    echoNote[xe] = configData.trackNote[x];
                    if (patternData.echoConfig[xe].attackDecay > 0) echoVelocity[xe] = patternData.echoConfig[xe].attackDecay; else echoVelocity[xe] = configData.accentValues[xvel-1];
                    theVelocity = echoVelocity[xe];
                  }
                }
              #endif
              //
              trackNoteOn(x, configData.trackNote[x], getMPVelocity(patternData.trackProcessor[x], theVelocity));
            }
          }
        }
        //
        // --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
        //
        for (byte x=0; x<(NOTE_TRACKS-1); x++)
        {
          if (bitRead(configData.muteTrack, x + DRUM_TRACKS) != 1)
          {       
            byte xvel = bitRead(stepsData[seqPosition].noteStepsExtras[x][0], 1 + (variation * 2)) << 1;
            xvel |= bitRead(stepsData[seqPosition].noteStepsExtras[x][0], 0 + (variation * 2));
            //
            byte extra = bitRead(stepsData[seqPosition].noteStepsExtras[x][1], 1 + (variation * 2)) << 1;
            extra |= bitRead(stepsData[seqPosition].noteStepsExtras[x][1], 0 + (variation * 2));
            //
            bool isSlide = (extra == 1);
            bool isDouble = (extra == 2);
            bool isNoteOff = (extra == 3);
            byte xnote = stepsData[seqPosition].noteSteps[x][variation];
            //
            if ((xnote > 0 && xvel > 0 && seqCounter == 0) || (xnote > 0 && seqCounter == DOUBLE_NOTE && isDouble))
            {
              if (isNoteOff)
              {
                sendMidiEvent(midiNoteOff, xnote, 0, configData.trackMidiCH[DRUM_TRACKS+x], 1);
                if (xnote == prevPlayedNote[DRUM_TRACKS+x]) prevPlayedNote[DRUM_TRACKS+x] = 0;
              }
              else 
              {
                byte orgxvel = xvel;
                byte theVelocity = configData.accentValues[xvel-1];
                //
                #if ECHOS_ENABLED
                  // Process Echo //
                  for (byte xe = 0; xe < ECHOS; xe++)
                  {
                    if (((patternData.echoConfig[xe].track - DRUM_TRACKS - 1) == x) && 
                    ( (patternData.echoConfig[xe].type == echoTypeOnAllNotes) || ((patternData.echoConfig[xe].type == echoTypeForceMaxVelocity && orgxvel == 3)) || ((patternData.echoConfig[xe].type == echoTypeForceLowVelocity && orgxvel == 2)) ))
                    {
                      echoCounter[xe][0] = patternData.echoConfig[xe].ticks;
                      echoCounter[xe][1] = patternData.echoConfig[xe].space;
                      echoAttackDecay[xe] = patternData.echoConfig[xe].attackDecay;
                      echoSpace[xe] = patternData.echoConfig[xe].space;
                      echoTrack[xe] = patternData.echoConfig[xe].track - 1;
                      echoNote[xe] = xnote;
                      if (patternData.echoConfig[xe].attackDecay > 0) echoVelocity[xe] = patternData.echoConfig[xe].attackDecay; else echoVelocity[xe] = configData.accentValues[xvel-1];
                      theVelocity = echoVelocity[xe];
                    }
                  }
                #endif           
                //     
                noteTrackNoteOn(DRUM_TRACKS+x, xnote, getMPVelocity(patternData.trackProcessor[DRUM_TRACKS+x], theVelocity), isSlide);
              }
            }
          }
        }
        //
        // ---===-------===-------===-------===-------===-------===-------===-------===-------===---- //
        //
        #if RECORD_ENABLED
          if (recordEnabled && seqCounter == 0)
          {
            for (byte xm = 0; xm < recordBufferPosition; xm++)
            {
              if (recordTrackEnable == 0)
              {
                recordInputCheck(recordBuffer[0][xm], recordBuffer[1][xm], recordBuffer[2][xm], curTrack);
              }
              else
              {
                for (byte xt = 0; xt < (DRUM_TRACKS+NOTE_TRACKS); xt++)
                {
                  if (bitRead(recordTrackEnable, xt) == 1) recordInputCheck(recordBuffer[0][xm], recordBuffer[1][xm], recordBuffer[2][xm], xt);
                }
              }
            }
            //
            recordBufferPosition = 0;
          }
        #endif
        //
        // ---===-------===-------===-------===-------===-------===-------===-------===-------===---- //
        //
        if (seqCounter == DOUBLE_NOTE && (seqPosition+1) >= STEPS && (variation+1) >= prevPatternTotalVariations)
        {
          if (streamNextPattern)
          {
            streamNextPattern = false;
            loadPatternNow = true;
            currentPlaysPattern = currentPlaysChain = 0;
          }
          else
          {
            if (patternData.playsPattern != 0 && (repeatMode == kRepeatModeNormal || repeatMode == kRepeatModeChain))
            {
              currentPlaysPattern++;
              if ((currentPlaysPattern - 1) == (patternData.playsPattern - 1))
              {
                currentPlaysPattern = 0;
                //
                if (patternData.playsChain != 1)
                {
                  currentPlaysChain++;
                  if (patternData.playsChain == 0 || repeatMode == kRepeatModeChain) currentPlaysChain = 0;
                  if (patternData.playsChain != 0 && currentPlaysChain >= (patternData.playsChain - 1))
                  {
                    currentPlaysChain = 0;
                    nextPattern = currentPattern + 1;
                    loadPatternNow = true;
                  }
                  else
                  {
                    if (patternData.nextPattern == 0) nextPattern = currentPattern + 1; else nextPattern = patternData.nextPattern - 1;
                    loadPatternNow = true;                  
                  }
                }
                else
                {
                    if (patternData.nextPattern == 0) nextPattern = currentPattern + 1; else nextPattern = patternData.nextPattern - 1;
                    loadPatternNow = true;                                   
                }
              }
            }
          }
        }
      }
    }
    //
    // ---===-------===-------===-------===-------===-------===-------===-------===-------===---- //
    // ---===-------===-------===-------===-------===-------===-------===-------===-------===---- //
    // ---===-------===-------===-------===-------===-------===-------===-------===-------===---- //
    //
    seqCounter++; 
    if (seqCounter >= MAX_NOTES)
    {
      seqCounter = 0;
      seqPosition++;
      showBPMdotBuffer = false;
      //
      switch (seqPosition)
      {
        case 0: showBPMdotBuffer = true; break;
        case 4: showBPMdotBuffer = true; break;
        case 8: showBPMdotBuffer = true; break;
        case 12: showBPMdotBuffer = true; break;
      }
      //
      if (seqPosition >= STEPS)
      {
        seqPosition = 0; 
        showBPMdotBuffer = true;
        if (seqPlaying)
        {
          variation++;
          if (variation >= prevPatternTotalVariations)
          {
            variation = 0;
            checkPatternStream();
            if (loadPatternNow) showErrorMsg(99);
            prevPatternTotalVariations = patternData.totalVariations;
          }
        }    
      }
      //
      // Redo The Left LEDs Indicators //
      if (seqPlaying && curRightScreen != kRightTrackSelection)
      {
        chaseLEDsBuffer[0] = chaseLEDsBuffer[1] = 0;
        if (seqPosition < 8) bitSet(chaseLEDsBuffer[0], seqPosition);
        else bitSet(chaseLEDsBuffer[1], seqPosition - 8);
      }
      ledsVarSeqUpdate();
    }
    //
    calculateSequencer--;
  }
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#if RECORD_ENABLED
  void recordInputCheck(byte data1, byte data2, byte channel, byte track)
  {
    byte xvariation = variation;
    byte theStep = seqPosition;
    //
    if (theStep > 0)  theStep--;
    else 
    {
      theStep = STEPS - 1;
      if (xvariation > 0) xvariation--;
      else xvariation = 3;
    }
    //    
    byte xVar = xvariation;
    if (forceVariation >= 0) xVar = forceVariation;
    if (mirror) xVar = 0;
    //
    if (track < DRUM_TRACKS)
    {
      if (configData.trackMidiCH[track] == channel && configData.trackNote[track] == data1 && data2 > 0)
      {
          bitClear(stepsData[theStep].steps[track], (xVar * 2));
          bitClear(stepsData[theStep].steps[track], (xVar * 2) + 1);
          //
          somethingChangedPattern = true;
          stepsData[theStep].steps[track] |= 1 << (xVar * 2);
          //
          if (mirror)
          {
            stepsData[theStep].steps[track] = 0x00;
            stepsData[theStep].steps[track] |= 1;
            stepsData[theStep].steps[track] |= 1 << 2;
            stepsData[theStep].steps[track] |= 1 << 4;
            stepsData[theStep].steps[track] |= 1 << 6;
          }
      }
    }
    else
    {
      if (configData.trackMidiCH[track] == channel)
      {
        if (data2 > 0)
        {
          clearStepsExtrasBits(theStep, xVar, track-DRUM_TRACKS);
          patternData.lastNote[track-DRUM_TRACKS] = data1;
          //      
          lastVelocity = 1;
          if (data2 >= configData.accentValues[2]) lastVelocity = 3;
          else if (data2 >= configData.accentValues[1]) lastVelocity = 2;
          #if RECORD_FORCE_VEL
            lastVelocity = 3;
          #endif
          //
          stepsData[theStep].noteSteps[track-DRUM_TRACKS][xVar] = patternData.lastNote[track-DRUM_TRACKS];
          stepsData[theStep].noteStepsExtras[track-DRUM_TRACKS][0] |= lastVelocity << (xVar * 2);
          //
          checkIfMirrorAndCopy(theStep, track-DRUM_TRACKS);
          somethingChangedPattern = true;         
        }
        else if (stepsData[theStep].noteSteps[track-DRUM_TRACKS][xVar] == 0)
        {
          clearStepsExtrasBits(theStep, xVar, track-DRUM_TRACKS);
          patternData.lastNote[track-DRUM_TRACKS] = data1;
          //      
          stepsData[theStep].noteSteps[track-DRUM_TRACKS][xVar] = patternData.lastNote[track-DRUM_TRACKS];
          stepsData[theStep].noteStepsExtras[track-DRUM_TRACKS][0] |= lastVelocity << (xVar * 2);
          stepsData[theStep].noteStepsExtras[track-DRUM_TRACKS][1] |= 3 << (xVar * 2);
          //
          checkIfMirrorAndCopy(theStep, track-DRUM_TRACKS);
          somethingChangedPattern = true;          
        }
      }
    }
  }
#endif

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ledsVarSeqUpdate()
{
  leftLEDsVarSeqBuffer = 0;
  bitSet(leftLEDsVarSeqBuffer, (seqPosition/4));
  byte xVar = variation;
  if (forceVariation >= 0) xVar = forceVariation;
  if (mirror) leftLEDsVarSeqBuffer |= B11110000; 
  else 
  {
    if (forceVariation >= 0)
    {
      leftLEDsVarSeqBuffer |= B11110000;
      bitClear(leftLEDsVarSeqBuffer, xVar + 4);
    }
    else bitSet(leftLEDsVarSeqBuffer, xVar + 4);
  }
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ledsBufferFlip()
{
  leftLEDsVarSeq = leftLEDsVarSeqBuffer;
  chaseLEDs[0] = chaseLEDsBuffer[0];
  chaseLEDs[1] = chaseLEDsBuffer[1];
  showBPMdot = showBPMdotBuffer;
  updateScreen = true;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
byte getMPVelocity(byte xproc, byte xorgvelocity)
{
  switch(xproc)
  {    
    case fadeIn:    
      return max(procFadeMin,min(127,xorgvelocity-(128-((seqPosition + (variation * 16)) * 2))));
      break;
      
    case fadeOut:   
      return max(procFadeMin,min(127,xorgvelocity-(((seqPosition + (variation * 16)) * 2))));
      break;
      
    case fadeInD:
      if (variation == 3) return max(procFadeMin,min(127,xorgvelocity-(128-(seqPosition * 9))));
      break;

    case fadeOutD:
      if (variation == 3) return max(procFadeMin,min(127,xorgvelocity-((seqPosition * 9))));
      break;

    case fadeInOut:
      if (variation <= 1) return max(procFadeMin,min(127,xorgvelocity-(128-((seqPosition + (variation * 16)) * 4))));
        else return max(procFadeMin,min(127,xorgvelocity-(((seqPosition + ((variation-2) * 16)) * 4))));
      break;

    case fadeOutIn:
      if (variation <= 1) return max(procFadeMin,min(127,xorgvelocity-(((seqPosition + (variation * 16)) * 4))));
        else return max(procFadeMin,min(127,xorgvelocity-(128-((seqPosition + ((variation-2) * 16)) * 4))));
      break;

    case randomVel:
      return max(procFadeMin,min(127,xorgvelocity-byte(random(procFadeMin, 127))));
      break;
  }
  //
  return xorgvelocity;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void trackNoteOn(byte xtrack, byte xnote, byte xvelocity)
{
  sendMidiEvent(midiNoteOff, xnote, 0, configData.trackMidiCH[xtrack], 0);
  sendMidiEvent(midiNoteOn, xnote, xvelocity, configData.trackMidiCH[xtrack], 0);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void noteTrackNoteOn(byte xtrack, byte xnote, byte xvelocity, bool slide)
{
  if ((!slide && prevPlayedNote[xtrack-DRUM_TRACKS] > 0) || prevPlayedNote[xtrack-DRUM_TRACKS] == xnote)  sendMidiEvent(midiNoteOff, prevPlayedNote[xtrack-DRUM_TRACKS], 0, configData.trackMidiCH[xtrack], 1);
  sendMidiEvent(midiNoteOn, xnote, xvelocity, configData.trackMidiCH[xtrack], 1);
  if (slide && prevPlayedNote[xtrack-DRUM_TRACKS] > 0 && prevPlayedNote[xtrack-DRUM_TRACKS] != xnote)  sendMidiEvent(midiNoteOff, prevPlayedNote[xtrack-DRUM_TRACKS], 0, configData.trackMidiCH[xtrack], 1);
  prevPlayedNote[xtrack-DRUM_TRACKS] = xnote;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void resetSequencer()
{
  seqPositionPrev = seqPosition;
  seqCounterPrev = seqCounter;
  variationPrev = variation;
  midiClockBeatsPrev = midiClockBeats;
  currentPlaysPattern = currentPlaysChain = 0;  
  //
  #if RECORD_ENABLED
    recordBufferPosition = 0;
  #endif
  midiOutputBufferPosition[0] = midiOutputBufferPosition[1] = 0;
  memset(midiOutputBuffer, 0, sizeof(midiOutputBuffer));
  //  
  #if ECHOS_ENABLED
    for (byte xe = 0; xe < ECHOS; xe++)
    {
      echoCounter[xe][0] = echoCounter[xe][1] = 0;
    }
  #endif
  seqPosition = seqCounter = variation = midiClockBeats = 0;
  showBPMdot = showBPMdotBuffer = true;
  //
  for (byte x=0; x<DRUM_TRACKS; x++)
  {
    stopDrumTrackPrevNote(x, true);
  }
  //
  for (byte x=0; x<NOTE_TRACKS; x++)
  {
    stopDrumTrackPrevNote(x, false);
  }
  //
  if (configData.writeProtectFlash) ShowTemporaryMessage(kMemoryProtectMessage);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void stopDrumTrackPrevNote(byte track, bool isDrumTrack)
{
  if (isDrumTrack)
  {
    sendMidiEvent(midiNoteOff, configData.trackNote[track], 0, configData.trackMidiCH[track], 0);
  }
  else
  {
    if (prevPlayedNote[track] > 0)
    {
      sendMidiEvent(midiNoteOff, prevPlayedNote[track], 0, configData.trackMidiCH[DRUM_TRACKS+track], 1);
      prevPlayedNote[track] = 0;
    } 
  }
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void startSequencer(bool docontinue)
{
  bool alreadyPlaying = seqPlaying;
  stopTimer(false);
  seqPlaying = true;
  if (docontinue)
  {
    seqPosition = seqPositionPrev;
    seqCounter = seqCounterPrev;
    variation = variationPrev;
    midiClockBeats = midiClockBeatsPrev;
  }
  else resetSequencer();  
  prevPatternTotalVariations = patternData.totalVariations;
  if (configData.seqSyncOut && configData.midiClockInternal) Serial.write(0xFA); // MIDI Start
  if (alreadyPlaying)
  {
    midiOutputBufferPosition[0] = midiOutputBufferPosition[1] = 0;
    memset(midiOutputBuffer, 0, sizeof(midiOutputBuffer));
    calculateSequencer = 1;
    doTickSequencer(); 
  }
  //
  outputMIDIBuffer();
  calculateSequencer = 1;
  //
  ledsVarSeqUpdate();
  startTimer(false);
  leftLEDsVarSeq = leftLEDsVarSeqBuffer;
  chaseLEDs[0] = chaseLEDs[1] = 0;
  chaseLEDs[0] = B10000000;
  updateScreen = true;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void startTimer(bool force)
{
  if (configData.midiClockInternal || force)
  {
    TCCR1A = TCCR1B = 0;
    if (configData.midiClockInternal) bitWrite(TCCR1B, CS11, 1); else bitWrite(TCCR1B, CS10, 1);
    bitWrite(TCCR1B, WGM12, 1);
    updateSequencerSpeed(false);
    bitWrite(TIMSK1, OCIE1A, 1); 
  }
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void stopTimer(bool force)
{
  if (configData.midiClockInternal || force)
  {
    bitWrite(TIMSK1, OCIE1A, 0);
    TCCR1A = TCCR1B = OCR1A = 0; 
  }
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void updateSequencerSpeed(bool force)
{
  // Calculates the Frequency for the Timer, used by the PPQ clock (Pulses Per Quarter Note) //
  // This uses the 16-bit Timer1, unused by the Arduino, unless you use the analogWrite or Tone functions //
  //
  if (configData.midiClockInternal || force) OCR1A = (F_CPU / 8) / ((((configData.BPM)*(PPQ))/60)) - 1;    
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void setupTimerForExternalMIDISync(bool active)
{
#if !DEBUG_MIDI_INPUT  
  if (active)
  {
    stopTimer(true);
    OCR1A = EXTERNAL_CLOCK_TIMER;
    startTimer(true);
  }
  else
  {
    stopTimer(true);
    updateSequencerSpeed(true);
    startTimer(true);
  }
#endif
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void stopSequencer(void)
{
  // Stop Timer //
  stopTimer(false);
  //
  if (configData.seqSyncOut && configData.midiClockInternal) Serial.write(0xFC); // MIDI Stop
  //
  seqPlaying = false;
  resetSequencer();
  MIDIallNotesOff();
  outputMIDIBuffer();
  //
  checkIfDataNeedsSaving();  
  if (streamNextPattern)
  {
    streamNextPattern = false;
    loadPatternNow = true;
  }
  //
  startTimer(false);
  updateScreen = true;  
  calculateSequencer = 1;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MIDIallNotesOff()
{
  for (byte x=0; x<midiChannels; x++)
  {
    sendMidiEvent(0xB0, 0x7B, 0x00, x, 0);
  }
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void sendMidiEvent(byte type, byte byte1, byte byte2, byte channel, byte slot)
{
  if ((slot == 0 && midiOutputBufferPosition[0] == MIDI_OUTPUT_BUFFER) || (slot == 1 && midiOutputBufferPosition[1] == MIDI_OUTPUT_BUFFER)) sendOutputMIDIBuffer();
  //
  midiOutputBuffer[slot][0][midiOutputBufferPosition[slot]] = type+channel;
  midiOutputBuffer[slot][1][midiOutputBufferPosition[slot]] = byte1;
  midiOutputBuffer[slot][2][midiOutputBufferPosition[slot]] = byte2;
  midiOutputBufferPosition[slot]++;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void startMIDIinterface()
{
  #if DEBUG_SERIAL
    Serial.begin(9600);
    Serial.println("Startup");
  #else
    #if MIDI_OVER_USB 
      Serial.begin(38400); 
    #else 
      Serial.begin(31250); // 31250 MIDI Interface //
    #endif
    Serial.flush();
    resetSequencer();
    MIDIallNotesOff();
  #endif
} 
