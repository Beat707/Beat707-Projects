/*

  Created by Guilherme (William) Kalfelz
  @ Beat707 (c) 2014 - http://www.Beat707.com
  
  Interface Buttons and Display
  
*/

// ======================================================================================= //
void buttonClick(uint8_t button, uint8_t holdclick = 0)
{
  if (currentTrack >= DRUMTRACKS) return;
  //
  if (mirrorStepsEdit == 1) 
  {
    bitWrite(dmSteps[currentPattern][currentTrack+(DRUMTRACKS*holdclick)],button,!bitRead(dmSteps[currentPattern][currentTrack+(DRUMTRACKS*holdclick)],button));
    bitWrite(dmSteps[currentPattern][currentTrack+(DRUMTRACKS*holdclick)+(DRUMTRACKS*2)],button,!bitRead(dmSteps[currentPattern][currentTrack+(DRUMTRACKS*holdclick)+(DRUMTRACKS*2)],button));
    bitWrite(dmSteps[currentPattern][currentTrack+(DRUMTRACKS*holdclick)+(DRUMTRACKS*4)],button,!bitRead(dmSteps[currentPattern][currentTrack+(DRUMTRACKS*holdclick)+(DRUMTRACKS*4)],button));
    bitWrite(dmSteps[currentPattern][currentTrack+(DRUMTRACKS*holdclick)+(DRUMTRACKS*6)],button,!bitRead(dmSteps[currentPattern][currentTrack+(DRUMTRACKS*holdclick)+(DRUMTRACKS*6)],button));
  }
  else bitWrite(dmSteps[currentPattern][currentTrack+((DRUMTRACKS*holdclick)+((DRUMTRACKS*2)*editStepsPos))],button,!bitRead(dmSteps[currentPattern][currentTrack+((DRUMTRACKS*holdclick)+((DRUMTRACKS*2)*editStepsPos))],button));
}

// ======================================================================================= //
void setLEDs()
{
  if (currentTrack >= DRUMTRACKS)
  {
    transportLeds = 0;
    stepLeds = noteTrackLEDs;
    if (showNavigationKeys == 1) stepLeds = 0x272;
    return;
  }
  //
  if (seqClockRunning == 1)
  {
    if (prevseqClockCounter != seqClockCounter)
    {
      prevseqClockCounter = seqClockCounter;
      //
      if (dmSteps[currentPattern][currentTrack+dbSteps+DRUMTRACKS] > 0 &&
          (prevseqClockCounter == 0 || prevseqClockCounter == 4 || 
          prevseqClockCounter == 8 || prevseqClockCounter == 12))
            transportLeds = dmSteps[currentPattern][currentTrack+dbSteps+DRUMTRACKS];
      //
      if (BLINK_TRANSPORT_MODE != 2 && (BLINK_TRANSPORT_MODE == 0 || 
          prevseqClockCounter == 0 || prevseqClockCounter == 4 || 
          prevseqClockCounter == 8 || prevseqClockCounter == 12)) 
              bitSet(transportLeds,prevseqClockCounter);
      transportBlinkCounter = 0;
    }
    else
    {
      if (transportBlinkCounter == TRANSPORT_BLINK_RATE)
      { 
        bitClear(transportLeds,prevseqClockCounter);
        transportBlinkCounter++;
        if (dmSteps[currentPattern][currentTrack+dbSteps+DRUMTRACKS] > 0) transportLeds = 0;
        if (transportBlinkClear == 1)
        {
          transportBlinkClear = 0;
          transportLeds = 0;
        }
      }
      else if (transportBlinkCounter < TRANSPORT_BLINK_RATE) transportBlinkCounter++;
    }
  }
  //
  if (ledsReady)
  {
    // Set Step LED On/Off 
    stepLeds = dmSteps[currentPattern][currentTrack+dbSteps];
    //
    if (showNavigationKeys == 1)
    {
      transportLeds = 0;
      stepLeds = 0x272;
    }
  }
}

// ======================================================================================= //
void drawLEDs()
{
  if (ledCounter < 12)
  {
    if ((bitRead(stepLeds,ledCounter) == 1 && bitRead(transportLeds,ledCounter) == 1) ||
        (bitRead(stepLeds,ledCounter) == 0 && bitRead(transportLeds,ledCounter) == 0)) digitalWrite(2 + ledCounter, LOW); 
    else if (bitRead(stepLeds,ledCounter) == 1 || bitRead(transportLeds,ledCounter) == 1) digitalWrite(2 + ledCounter, HIGH);
    else digitalWrite(2 + ledCounter, LOW); 
  }
  else
  {
    uint8_t ledCounter2 = ledCounter-12;
    if ((bitRead(stepLeds,ledCounter) == 1 && bitRead(transportLeds,ledCounter) == 1) ||
        (bitRead(stepLeds,ledCounter) == 0 && bitRead(transportLeds,ledCounter) == 0)) digitalWrite(A0 + ledCounter2, LOW); 
    else if (bitRead(stepLeds,ledCounter) == 1 || bitRead(transportLeds,ledCounter) == 1) digitalWrite(A0 + ledCounter2, HIGH);    
    else digitalWrite(A0 + ledCounter2, LOW);
  }
  ledCounter++;
  if (ledCounter > 15) ledCounter = 0;
  //
  // Extra LEDs //
  if (extLEDblink > 0)
  {
    extLEDblink++;
    if (extLEDblink > EXTRA_LED_BLINK_RATE)
    {
      digitalWrite(A4, LOW);
      extLEDblink = 0;
    }
  }
}

// ======================================================================================= //
void blinkExtraLEDs()
{
  if (extraLEDs > 0)
  {
    extLEDblink = 1;
    digitalWrite(A4, HIGH);    
  }
}
