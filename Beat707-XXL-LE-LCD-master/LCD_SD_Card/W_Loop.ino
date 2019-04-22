/*

  Created by Guilherme (William) Kalfelz
  @ Beat707 (c) 2014 - http://www.Beat707.com

*/

// ======================================================================================= //
void loop() 
{    
  while (Serial.available() > 0)
  {
    byte bByte = Serial.read();
    byte nBytes = bByte >> 5;
    switch (bByte)
    {
      case B00100001: // Touch Sensor //
      {
        waitSerialNextByte();
        byte sRead = Serial.read();
        byte xCode = sRead >> 5; 
        byte xButton = sRead & B00011111;
        byte xButton4 = xButton-4;
        uint8_t broadcast = 0;
        if (xCode == kClick && ignoreKeysCounter <= 0) // Touch Sensor Click
        {
          if (xButton == kLeft)
          {
            if (showOptions)
            {            
              showOptionsOption = showOptions = showOptionsOptionConfirm = 0;
              ignoreKeysForOneSecond(1);
              updateNoteTrackLEDs();
              doLCDupdate++;   
            }
            else if (editingOn == 0) editMode(1);
            else editMode(0);         
          }
          else if (xButton == kRight && holdingLeft == 1)
          {
            editZone = kZoneBPM;
            doLCDupdate++;
          }
          else if (xButton == kRight)
          {
            if (showOptions) 
            {
              OptionsConfirm();
            }
            else
            {
              if (!seqClockRunning) seqClockStart(1); else seqClockStop();
            }
          }
          else if (xButton4 >= 14 && holdingLeft)
          {
            if (xButton4 == 15)
            {
              showOptionsOption = showOptionsOptionConfirm = 0; 
              showOptions = 1;
              updateNoteTrackLEDs();
              doLCDupdate++;
            }
            else if (xButton4 == 14)
            {
              saveDataToEEPROM();
            }
          }
          else if (xButton >= 4 && holdingLeft && last_settings.bMode == kModePat) 
          {
            if (xButton4 <= 6)
            {
              currentTrack = xButton4;
              updateNoteTrackLEDs();
            }       
            else if (xButton4 >= 8)
            {
              autoStepPos = 0;
              if (xButton4 <= 11) editStepsPos = xButton4-8;
              else
              {
                if (xButton4 == 12) seq_mirror = 1; else seq_mirror = 0;
                autoStepPos = 1;
                editStepsPos = stepsPos;
              }
              updateAutoSteps();
            }
            doLCDupdate++;
          }          
          else if (xButton >= 4 && showOptions)
          {
              showOptionsOption = xButton4+1;
              updateNoteTrackLEDs();
              doLCDupdate++;
          }
          else if (editingOn == 1)
          {
            buttonToZone(xButton4);
          }
          else if (xButton >= 4 && last_settings.bMode == kModeSongPerf && holdingLeft == 0 && holdingRight == 0)
          {
            if (last_settings.perfDataMode == 1 && seqClockRunning) // Stack //
            {
              for (int x=0; x<32; x++) 
              { 
                if (songPerfPosNext[x] == -1)
                {
                  songPerfPosNext[x] = xButton4;
                  break;
                }
              }
            }
            else songPerfPosNext[0] = xButton4;
            //
            if (!seqClockRunning)
            {
              if (songPerfPosNext[0] != -1) songPerfPos = songPerfPosNext[0];
              songPerfPosNext[0] = -1;
              seqClockStart(1);
            }
            doLCDupdate++;
            updateNoteTrackLEDs();            
          }
          else if (xButton >= 4 && last_settings.bMode == kModeSong && !seqClockRunning && holdingLeft == 0 && holdingRight == 1)
          {
            TSNG(0) = xButton4+1;
            doLCDupdate++;
          }
          else if (currentTrack == 4 && xButton >= 4 && holdingLeft == 0 && holdingRight == 0 && last_settings.bMode == kModePat)
          {
            editZone = kZoneNT_value;
            editNTrackPos = xButton4;
            doLCDupdate++;
            updateNoteTrackLEDs();
          }
          else if (currentTrack == 5 && xButton >= 4 && xButton4 <= 4 && holdingLeft == 0 && holdingRight == 0 && last_settings.bMode == kModePat)
          {
            if (bitRead(tracksOnOffBuffer,xButton4)) bitClear(tracksOnOffBuffer,xButton4); else bitSet(tracksOnOffBuffer,xButton4);
            updateNoteTrackLEDs();
          }
          else if (currentTrack == 6 && xButton >= 4 && holdingLeft == 0 && holdingRight == 0 && last_settings.bMode == kModePat)
          {
            nextPattern = xButton4;
            if (!seqClockRunning) checkNextPattern(); else doLCDupdate++;            
            updateNoteTrackLEDs();
          }          
          else if (xButton >= 4 && holdingLeft == 0 && holdingRight == 0 && last_settings.bMode == kModePat)
          {
            broadcast = 1;
          }
          else if (xButton >= 4 && holdingRight && last_settings.bMode == kModePat) 
          {
            nextPattern = xButton4;
            if (!seqClockRunning) checkNextPattern(); else doLCDupdate++;
          }
        }
        else if (xCode == kHold) // Touch Sensor Hold
        {
          if (xButton == kLeft)
          {
            holdingLeft = 1;
            if (showOptions)
            { 
              showOptionsOption = showOptions = 0;
              ignoreKeysForOneSecond(1);
              updateNoteTrackLEDs();
              doLCDupdate++;
            } 
          }
          else if (xButton == kRight)
          {
            holdingRight = 1;
            if (showOptions) OptionsConfirm();
          }
          else if (editingOn == 1)
          {
            if (xButton4 == 1) holdUpDown = 1;
            else if (xButton4 == 9) holdUpDown = -1;
            else if (xButton4 == 4) holdLeftRight = -1;
            else if (xButton4 == 6) holdLeftRight = 1;            
            //
            if (editZone == kZoneNT_value || editZone == kZoneNT_type || last_settings.bMode == kModeSong)
            {
              if ((xButton4) == 15) { holdUpDown = 1; upDownForceToTNpos = 1; }
              else if ((xButton4) == 14) { holdUpDown = -1; upDownForceToTNpos = 1; }
            }
          }
          else if (xButton >= 4 && last_settings.bMode == kModeSong && !seqClockRunning && holdingLeft == 0 && holdingRight == 0)
          {
              if ((xButton4) == 15) updateSongPos(1);
              else if ((xButton4) == 14) updateSongPos(-1);
              else updateSongPos(0);
          }
          else if (xButton >= 4 && last_settings.bMode == kModeSongPerf)
          {
          }
          else if (currentTrack == 4 && xButton >= 4 && holdingLeft == 0 && holdingRight == 0)
          {
            editZone = kZoneNT_value;
            editNTrackPos = xButton4;
            updateNtVal(0);
            updateNoteTrackLEDs();
          }          
          else if (xButton >= 4 && holdingLeft == 0 && holdingRight == 0) broadcast = 1;
          doLCDupdate++;
        }
        else if (xCode == kRelease) // Touch Sensor Release //
        {
          holdUpDown = holdLeftRight = upDownForceToTNpos = 0;
          if (xButton == kLeft) holdingLeft = 0;
          else if (xButton == kRight) holdingRight = 0;
          doLCDupdate++;
        }
        //
        if (broadcast == 1)
        {
          Serial.write(B00100001);
          Serial.write(sRead);          
        }
      }
      break;
      
      default: // Filter other codes // ------------------------------------------- //
      {
        while (nBytes > 0)
        {
          waitSerialNextByte();
          Serial.read();
          nBytes--;
        }
      }      
    }
  }
  //
  checkUpDown();
  updateLCD();
  ignoreKeysForOneSecond(0);
  //
  if (seqClockRunning == 1 && doLCDupdateBeats && ((last_settings.bMode == kModePat && editZone <= kZoneTrack) || last_settings.bMode >= kModeSong) && !showOptions)
  {
    doLCDupdateBeats = 0;
    updateBeatLCD();
  }
  //
  if (doLateClockStop)
  {
    doLateClockStop = 0;
    seqClockStop();
  }
  //
  if (!ledsReady)
  {
    ledsReady = 1;
    Serial.write(18); // Ready to display the LEDs //
  }
  //
  if (songPerfPosNextUnstack)
  {
    songPerfPosNextUnstack = 0;
    for (int x=0; x<31; x++) 
    { 
      songPerfPosNext[x] = songPerfPosNext[x+1];
    }
    songPerfPosNext[31] = -1;
    //
    doLCDupdate++;
    updateNoteTrackLEDs();    
  }
}
