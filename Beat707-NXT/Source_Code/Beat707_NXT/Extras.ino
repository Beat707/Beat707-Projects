/*
 * 
 * Created by William Kalfelz @ Beat707 (c) 2018 - http://www.Beat707.com
 * 
 */

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void reset()
{
  createFlashHeader(currentPatternBank);
  resetSegments(0, 2);
  memset(midiOutputBuffer, 0, sizeof(midiOutputBuffer));
  #if ECHOS_ENABLED
    memset(echoSpace, 0, sizeof(echoSpace));
    memset(echoAttackDecay, 0, sizeof(echoAttackDecay));
    memset(echoTrack, 0, sizeof(echoTrack));
    memset(echoCounter, 0, sizeof(echoCounter));
    memset(echoVelocity, 0, sizeof(echoVelocity));  
    memset(echoNote, 0, sizeof(echoNote));
  #endif
  memset(leds, 0, sizeof(leds));
  memset(buttons, 0, sizeof(buttons));  
  memset(buttonEvent, 0, sizeof(buttonEvent));
  memset(buttonDownTime, 0, sizeof(buttonDownTime));
  memset(prevPlayedNote, 0, sizeof(prevPlayedNote));
  configData.init();
  bitSet(patternBitsSelector,0);
  bitSet(patternBitsSelector,8);
  midiOutputBufferPosition[0] = midiOutputBufferPosition[1] = 0;
  #if RECORD_ENABLED
    memset(recordBuffer, 0, sizeof(recordBuffer));
    recordBufferPosition = 0;
  #endif
  //
  resetPatternBank();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void resetPatternBank()
{
  for (byte x=0; x < 4; x++) 
  { 
    stepsData[0 + (x*4)].init(true); 
    stepsData[1 + (x*4)].init(true); 
    stepsData[2 + (x*4)].init(true); 
    stepsData[3 + (x*4)].init(true); 
    //
    stepsData[0 + (x*4)].steps[15] = B11111111;
    stepsData[1 + (x*4)].steps[15] = B10101010;
    stepsData[2 + (x*4)].steps[15] = B01010101;
    stepsData[3 + (x*4)].steps[15] = B10101010;
  }
  patternData.init();  
  //
  #if DEBUG_MIDI_INPUT
    memset(midiInput, 0, sizeof(midiInput));
    midiInputItens = 0;
  #endif 
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void waitMs(int mstime)
{
  for (int xx=0; xx<mstime; xx++) 
  { 
    for (int xms=0; xms<1000; xms++)
    {
      __asm__("nop"); 
    }
  }
}
