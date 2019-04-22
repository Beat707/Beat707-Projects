/*
 * 
 * Created by William Kalfelz @ Beat707 (c) 2018 - http://www.Beat707.com
 * 
 */

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void createScreenMute()
{
  leds[0] = leds[1] = leds[2] = 0;
  if (seqPlaying) leds[0] = leftLEDsVarSeq;
  //
  resetSegments(0, 3);
  if (showBPMdot) segments[2][7] |= B10000000;
  //
  if (forceAccent) // Shows Pattern Instead // ---------------------------------------------------------------------------------------------------------------------- //
  {
    leds[0] ^= patternBitsSelector;
    leds[1] ^= patternBitsSelector >> 8;
    //
    if (seqPlaying)
    {
      leds[1] ^= chaseLEDs[0];
      leds[2] ^= chaseLEDs[1];  
    }
    //
    segments[0][0] = S_P;
    if (nextPattern != currentPattern) printNumber(0, 1, nextPattern + 1); else printNumber(0, 1, currentPattern + 1);   
    segments[0][1] = S_T;
    if (streamNextPattern || loadPatternNow) segments[0][3] |= B10000000;
    //
    segments[2][0] = S_P;
    segments[2][2] = S_N;
    segments[2][4] = S_C;
    segments[2][5] = S_L;
    segments[2][6] = S_S;
  }
  else // ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
  {
    if (seqPlaying) bitSet(leds[0], (seqPosition/4)); else bitSet(leds[0], 0);
    bitSet(leds[0], variation + 4);
    if (seqPlaying) { if (seqPosition < 8) bitSet(leds[1], seqPosition); else bitSet(leds[2], seqPosition - 8); }
    //
    byte xseg = 0;
    byte xboard = 0;
    for (byte x=0; x<(DRUM_TRACKS + NOTE_TRACKS); x++)
    {
      if (bitRead(configData.muteTrack, x) == 1) segments[xboard][xseg] = S__; else segments[xboard][xseg] = S_N;
      //
      xseg++;
      if (xseg >= 8)
      {
        xseg = 0;
        xboard++;
      }
    }
  }
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void checkInterfaceMute()
{
  for (byte x=0; x<8; x++)
  {
    for (byte i=0; i<3; i++)
    {
      if (buttonEvent[i][x] >= kButtonClicked)
      {
        if (forceAccent) // Shows Pattern Mode Instead // -------------------------------------------------------------------- //
        {
          if (i == 2 && x == 7 && buttonEvent[i][x] >= kButtonClicked)
          {
            forceAccent = false;
          }
          else if (i == 2 && (x == 0 || x == 1))
          {
            changePattern(-1);
          }
          else if (i == 2 && (x == 2 || x == 3))
          {
            changePattern(1);
          }
          else if (i == 2 && (x > 3))
          {
            forceAccent = false;
            curRightScreen = kRightSteps;
          }
          else if (i <= 1)
          {
            byte leButton = x + (i * 8);
            checkIfDataNeedsSaving();
            if (leButton < 8) patternBitsSelector &= 0xFF00; else patternBitsSelector &= 0xFF;
            bitSet(patternBitsSelector, leButton);
            //
            nextPattern = 0;
            for (byte x = 0; x < 8; x++)
            {
              if (bitRead(patternBitsSelector, x + 8)) nextPattern += x;
              if (bitRead(patternBitsSelector, x)) nextPattern += (x * 8);
            }
            if (currentPattern != nextPattern) loadPattern(nextPattern);
          }
        }
        else // --------------------------------------------------------------------------------------------------------------------------------- //
        {
          if (i == 2 && x == 7) // Play Stop //
          {
            if (buttonEvent[i][x] >= kButtonClicked) forceAccent = true;
          }
          else
          {
            somethingChangedConfig = true;
            if (buttonEvent[i][x] >= kButtonClicked)
            {
              if (bitRead(configData.muteTrack, x + (i *8)) == 1) bitClear(configData.muteTrack, x + (i *8)); else bitSet(configData.muteTrack, x + (i *8));
            }
          }
        }
        //
        buttonEvent[i][x] = 0;
        updateScreen = true;
      }
    }
  }  
}
