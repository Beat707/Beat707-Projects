/*

  Created by Beat707 (c) 2011 - http://www.Beat707.com
  
  Hacks and Mods: as used by several areas of the code
  Check the Config.h file and also the online Hack Area at: 
  http://www.beat707.com/w/downloads/hacks-and-mods  
  
*/

// ======================================================================================= //    
void Hack_and_Mods_Loop()
{
  // ------------------------------------------------------------------------------------- //
  #if ANALOG_INPUT_A0
    #if ANALOG_INPUT_BT
    if (digitalReadW(2) == HIGH) prevAnalogButtonCheckState = HIGH;
    if (curMode != 2 && analogInputModeNewDelay < millisNI() && digitalReadW(2) == LOW)
    #else
    if (curMode != 2 && !holdingShift && analogInputModeNewDelay < millisNI())
    #endif
    {
      #if ANALOG_INPUT_BT
        if (prevAnalogButtonCheckState == HIGH)
        {
          analogInputModeNewDelay = millisNI()+ANALOG_MDLY;
          prevAnalogButtonCheckState = LOW;
          return;
        }
      #endif     
      
      int val = analogRead(A0);
      if (val != prevAnalogA0value)
      {
        prevAnalogA0value = val;
        switch (analogInputMode)
        {
          case 0: midiClockBPM = map(val, 0, 1023, 25, 255); if (midiClockRunning) MidiClockNewTime(); break;
          case 1: nextPattern = map(val, 0, 1023, 0, ANALOG_PATT_MAX); break;
          case 2: numberOfSteps = map(val, 0, 1023, 1, 16); break;
          case 3: currentTrack = map(val, 0, 1023, 0, (DRUMTRACKS+3)); break;
          case 4: 
            dbStepsCalc();
            if (currentTrack < DRUMTRACKS)
            {
              dmNotes[currentTrack] = map(val, 0, 1023, 0, 127);
            }
            else if (currentTrack < (DRUMTRACKS+2))
            {
              uint8_t dTrack = currentTrack-DRUMTRACKS;
              if (mirrorPatternEdit)
              {
                dmSynthTrack[dTrack][patternBufferN][dmSynthTrackStepPos[1]+(16*editDoubleSteps)] = map(val, 0, 1023, 0, 255);;
                dmSynthTrack[dTrack][patternBufferN][dmSynthTrackStepPos[1]+(16*editDoubleSteps)+32] = dmSynthTrack[dTrack][patternBufferN][dmSynthTrackStepPos[1]+(16*editDoubleSteps)];
                if (dmSynthTrack[dTrack][patternBufferN][dmSynthTrackStepPos[1]+(16*editDoubleSteps)] > 1) dmSynthTrackLastNoteEdit[dTrack] = dmSynthTrack[dTrack][patternBufferN][dmSynthTrackStepPos[1]+(16*editDoubleSteps)];
              }
              else
              {
                dmSynthTrack[dTrack][patternBufferN][dmSynthTrackStepPos[1]+(16*editDoubleSteps)] = map(val, 0, 1023, 0, 255);;
                if (dmSynthTrack[dTrack][patternBufferN][dbStepsSpos] > 1) dmSynthTrackLastNoteEdit[dTrack] = dmSynthTrack[dTrack][patternBufferN][dbStepsSpos];
              }
              patternChanged = 1;
            }
            break;
        }
        setupChanged = doLCDupdate = 1;
      }
    }
  #endif

  // ------------------------------------------------------------------------------------- //
  #if GATE_OUTS
    for (int gx=0; gx<3; gx++)
    {
      if (gateOutDelay[gx] != 0 && gateOutDelay[gx] < millisNI())
      {
        gateOutDelay[gx] = 0;
        switch (gx)
        {
          case 0:
            digitalWriteW(A0, LOW);
            break;
            
          case 1:
            digitalWriteW(2, LOW);
            break;
            
          case 2:
            digitalWriteW(3, LOW);
            break;
        }
      }
    }
  #endif
  
  // ======================================================================================= //    
  #if ENCODER_INPUT
    for (char x=0; x<2; x++)
    {
      while (globalEncoder[x] > 0)
      {
        multiButton = 3+x;
        globalEncoder[x]--;
        InterfaceButtons();
      }
    }
  #endif  
}

// ======================================================================================= //    
#if GATE_OUTS
  void Gate_Outs_Midi(uint8_t xdtm, uint8_t velocity)
  {
    if (xdtm <= 2)
    {
      switch (xdtm)
      {
        case 0:
          digitalWriteW(A0, HIGH);
          break;
          
        case 1:
          digitalWriteW(2, HIGH);
          break;
          
        case 2:
          digitalWriteW(3, HIGH);
          break;
      }
      gateOutDelay[xdtm] = millisNI()+GATE_OUTS_TIME;            
    }
    else
    {
      sendMidiNoteOff(dmNotes[xdtm], dmChannel[xdtm], 0xFF);
      sendMidiNoteOn(dmNotes[xdtm],velocity, dmChannel[xdtm], 0xFF);
    }
  }
#endif


#if EXTRA_8_BUTTONS
  // ======================================================================================= //
  /*
    Extra 8 Buttons User Interface (only used if EXTRA_8_BUTTONS is set to 1)
    You can also use the holdingShift variable to check for the Shift key
    Also, if you plan on adding a button on a determinated mode, use the following variable: curMode // 0=Pattern Mode, 1=Song Mode, 2=File Mode
  */  
  void Extra8ButtonsInterface()
  {  
    switch (multiButton)
    {  
      // ------------------------------- Button 1 ------------------------------- //
      case 21:
        if (!holdingButton)
        {
          if (midiClockRunning) multiButton = 0; // Stop //
            else multiButton = 1; // Play //
        
          InterfaceButtons();
        }
      break;
      
      // ------------------------------- Button 2 ------------------------------- //
      case 22:
        // User Code Here //
      break;
      
      // ------------------------------- Button 3 ------------------------------- //
      case 23:
        // User Code Here //
      break;
      
      // ------------------------------- Button 4 ------------------------------- //
      case 24:
        // User Code Here //
      break;
      
      // ------------------------------- Button 5 ------------------------------- //
      case 25:
        // User Code Here //
      break;
      
      // ------------------------------- Button 6 ------------------------------- //
      case 26:
        // User Code Here //
      break;
      
      // ------------------------------- Button 7 ------------------------------- //
      case 27:
        // User Code Here //
      break;
      
      // ------------------------------- Button 8 ------------------------------- //
      case 28:
        // User Code Here //
      break;
    }
  }
#endif

#if EXTRA_MIDI_IN_HACKS
  // ======================================================================================= //    
  void midiInputHacks(uint8_t* data, uint8_t channel)
  {
    /* 
    
      MIDI byte1 = data[0] - (MIDI Channel is removed and set to the channel variable instead
      MIDI byte2 = data[1]
      MIDI byte3 = data[2]
    
      List MIDI Commands from: http://www.omega-art.com/midi/mbytes.html
    
      byte1 =  144 = Note On                  byte2 =  note number        byte3  = velocity (on)
               128 = Note Off                          note number                 velocity (off)
               160 = Polyphonic aftertouch             note number                 amount (0 to 127)
               176 = Control (CC)                      value1                      value2
               192 = Program Change (PC)               pc-value (0 to 127)         
               208 = Channel Aftertouch                amount (0 to 127)           
               224 = Pitch Wheel (Bend)                LSB                         MSB  (14 bits - see the site above for details)
    */
    
    // !!! The Codes below are set as examples - feel free to erase and create your own codes !!! //
    
    switch (data[0])
    {
      case 192: // Program Change to Pattern Selector
        #if EXTRA_MIDI_IN_H_1
          if (data[1] < MAXSPATTERNS)  { nextPattern = data[1]; updateLCDPattern(); }
        #endif
        break;
        
      case 176:
        #if EXTRA_MIDI_IN_H_1
          switch (data[1])
          {
            case 1: // Midi CC - Modulation Wheel to BPM Tempo
              midiClockBPM = map(data[2], 0, 127, 25, 255);
              if (midiClockRunning) MidiClockNewTime();
              setupChanged = doLCDupdate = 1;
              break;
              
            case 2: // Midi CC 2 to Pattern Number of Steps
              numberOfSteps = map(data[2], 0, 127, 1, 16); break;
              setupChanged = doLCDupdate = 1;
              break;
          }
        #endif
        #if EXTRA_MIDI_IN_H_2
          if (data[1] == 123) // All Notes Off
          {
            if (midiClockRunning == 1) MidiClockStop();
          }
        #endif
        break;
        
      case 224:  // Pitch Wheel (Bend)
        #if EXTRA_MIDI_IN_H_1
          if (data[2] > 90 && !midiClockRunning) MidiClockStart(true, true); 
            else if (data[2] < 40 && midiClockRunning) MidiClockStop(); 
        #endif
        break;

      #if EXTRA_MIDI_IN_H_1
        case 144: // This will split the whole keyboard into 3 sections: Drum-Tracks, S1 and S2 - this will only work correctly if MIDIECHO is disabled
        case 128:
          if (data[1] >= 72) { data[1] -= 24; channel = dmChannel[DRUMTRACKS+1]; }
            else if (data[1] >= 36) { channel = dmChannel[DRUMTRACKS]; }
            else { data[1] += 12; channel = dmChannel[0]; }
          sendMidiData(data, channel, 3);
          break;
      #endif
      
      #if EXTRA_MIDI_IN_H_2
        case 144: // Select and Play Pattern //
          if (channel == 15)
          {
            if (data[1] < 24)
            {
              if (midiClockRunning == 1) MidiClockStop();
            }
            else if (data[2] > 0 && data[1] >= 24 && (data[1]-24) < MAXSPATTERNS)
            { 
              if ((data[1]-24) != currentPattern)
              {
                if (patternChanged) savePattern(0);
                currentPattern = nextPattern = (data[1]-24); 
                loadPatternPartial();
                patternBufferN = !patternBufferN;
                MidiClockStart(true, true);
                #if XTH2_AUTO_STOP
                  doAutoStop = 1;
                #endif
                #if !MIDI_NO_OUT_BUFFER
                  midiBufferCheck();
                #endif
                loadPattern(0);
                patternBufferN = !patternBufferN;
                doLCDupdate = 1;
              }
              else
              {
                MidiClockStart(true, true);
                #if XTH2_AUTO_STOP
                  doAutoStop = 1;
                #endif
              }
            }
          }
          break;
      #endif
    }
  }
#endif

// ======================================================================================= //    
#if ENCODER_INPUT
  void EncoderChange()
  {
    enCval1 = digitalReadW(2);
    enCval2 = digitalReadW(3);
    
      // Detect changes
    if (enCval1 != enColdVal1 || enCval2 != enColdVal2) 
    {
      enColdVal1=enCval1;
      enColdVal2=enCval2;
      
        //for each pair there's a position out of four
      if (enCval1 == 1) // stationary position
      {
        if (enCval2 == 1) enCpos = 0; else if (enCval2 == 0) enCpos = 3;
      } 
      else if (enCval1 == 0)
      {
        if (enCval2 == 1) enCpos = 1; else if (enCval2 == 0) enCpos = 2;
      }
      
      enCturn = enCpos-enColdPos;
      enColdPos = enCpos;
     
      if (abs(enCturn) != 2) { if (enCturn == 1 || enCturn == -3) enCturnCount++; else if (enCturn == -1 || enCturn == 3) enCturnCount--; }
      if (enCpos==0) 
      { 
        if (enCturnCount>0) 
        {
          #if ENCODER_SPEED
            if ((millisNI()-encoderMillis) < 25) globalEncoder[0] += 5; else globalEncoder[0]++;
            encoderMillis = millisNI();
          #else
            globalEncoder[0]++; 
          #endif          
        }
        else if (enCturnCount<0) 
        { 
          #if ENCODER_SPEED
            if ((millisNI()-encoderMillis) < 25) globalEncoder[1] += 5; else globalEncoder[1]++;
            encoderMillis = millisNI();
          #else
            globalEncoder[1]++; 
          #endif          
        }
        enCturnCount=0;          
      }
    }
  }
#endif
