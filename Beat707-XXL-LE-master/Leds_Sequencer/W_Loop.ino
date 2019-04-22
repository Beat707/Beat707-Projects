/*

  Created by Guilherme (William) Kalfelz
  @ Beat707 (c) 2014 - http://www.Beat707.com
  
  Main Loop
  
*/

// ======================================================================================= //
void loop()
{   
  while (Serial.available() > 0)
  {
    clockTick();
    //
    byte bByte = Serial.read();
    byte nBytes = bByte >> 5;
    switch (bByte)
    {
      case 0: // Sequence Trigger //
      {
        #if !IS_LAST_UNIT
          Serial.write(0); // Broadcast the Sequence Trigger
        #endif
        seqClockStart(1);
      }
      break;

      case B01000101: //  Master to All Sequencers
      {
        waitSerialNextByte();
        byte xCode = Serial.read();
        waitSerialNextByte();
        byte xData = Serial.read();
        //
        #if !IS_LAST_UNIT
          Serial.write(B01000101); // Broadcast Message //
          Serial.write(xCode);
          Serial.write(xData);
        #endif
        //
        if (xCode == 0) last_settings.seqClockBPM = xData; // Set BPM Tempo
          else if (xCode == 1) last_settings.numberOfSteps = xData; // Set Number of Steps
      }
      break;

      case B00111000: // Master Sequencer Message //
      {
        waitSerialNextByte();
        byte sRead = Serial.read();
        byte xCode = sRead >> 4; 
        #if !IS_LAST_UNIT
          MSerial.write(B00111000); //Broadcast Message //
          MSerial.write(sRead);
        #endif
        if (xCode == 4) // Stop the Sequencer //
        {
          seqClockStop();
        }
        else if (xCode == 5) // Mirror Mode Toggle
        {
          mirrorStepsEdit = sRead & B00001111;
        }
        else if (xCode == 7) // Steps Selection Change
        {
          if ((sRead & B00001111) == 4)
          {
            autoStepPos = 1;
            editStepsPos = stepsPos;            
          }
          else
          {
            autoStepPos = 0;
            editStepsPos = sRead & B00001111;
          }
        }
      }
      break;

      case B00100001: // Touch Sensor - 1 extra bytes //
      {
        waitSerialNextByte();
        byte sRead = Serial.read();
        byte xCode = sRead >> 5; 
        byte xButton = sRead & B00011111;
        if (xCode == 0) // Touch Sensor Click
        {
          if (xButton >= 4) 
          {
            byte xButton4 = xButton-4;
            if (holdingLeft == 1) // Select Track //
            {
              if (xButton4 < (DRUMTRACKS+2)) currentTrack = xButton4;
            }
            else if (holdingRight == 1)
            {
              if (interfaceZone == kInterface_Zone_BPM)
              {
                if (xButton4 == 1) updateBPM(1);
                else if (xButton4 == 5) updateBPM(0);
                else if (xButton4 == 9) updateBPM(-1);
              }
              else if (interfaceZone == kInterface_Zone_Steps)
              {
                if (xButton4 == 1) updateSteps(1);
                else if (xButton4 == 5) updateSteps(0);
                else if (xButton4 == 9) updateSteps(-1);            
              }
              else if (xButton4 == 14) // Number of Steps Selector //
              {
                interfaceZone = kInterface_Zone_Steps;
              }
              else if (xButton4 == 15) // Tempo Selector //
              {
                interfaceZone = kInterface_Zone_BPM;
              }
              else if (xButton4 >= 8) // Pattern Selector //
              {
                nextPattern = xButton4-8;
                if (!seqClockRunning) currentPattern = nextPattern;
              }
              else if (xButton4 <= 3) // Steps Selector //
              {
                #if IS_MASTER_UNIT
                  autoStepPos = 0;
                  editStepsPos = xButton4;
                  mirrorStepsEdit = 0;
                #endif
                //
                #if HAS_SLAVE_UNITS
                  MSerial.write(B00111000); // Slave Sequencers Message
                  MSerial.write(B00110000 | editStepsPos); // Steps Selection Change                
                #endif
              }
              else  // Auto Steps Selector //
              {
                #if IS_MASTER_UNIT
                  autoStepPos = 1;
                  editStepsPos = stepsPos;
                  if (xButton4 == 4) mirrorStepsEdit = 1; else mirrorStepsEdit = 0;
                #endif
                //
                #if HAS_SLAVE_UNITS
                  MSerial.write(B00111000); // Slave Sequencers Message
                  MSerial.write(B00110100); // Steps Selection Change to Auto
                #endif
              }
            }
            else 
            {
              buttonClick(xButton4);
            }
          }
          else if (xButton == 2)
          {
            if (holdingRight == 1) // Copy Pattern to Next //
            {
              copyToNextPattern();
              somethingChanged = 1;
            }
            //
            #if IS_MASTER_UNIT
              //mirrorStepsEdit = !mirrorStepsEdit;
            #endif
            //
            #if HAS_SLAVE_UNITS
              //MSerial.write(B00111000); // Slave Sequencers Message
              //MSerial.write(B01010000 | mirrorStepsEdit); // Mirror Mode On/Off
            #endif
          }
          else if (xButton == 3)
          {
            if (holdingLeft == 1) // Init Pattern //
            {
              initPattern(currentPattern);
              somethingChanged = 1;
            }
            #if IS_MASTER_UNIT
              else 
              {
                if (seqClockRunning == 0) seqClockStart(1); else seqClockStop();
              }
            #endif
          }
        }
        else if (xCode == 1) // Touch Sensor Hold
        {
          if (xButton == 2) holdingLeft = 1;
          else if (xButton == 3) holdingRight = 1;
          else if (holdingRight == 1 && interfaceZone != kInterface_Zone_None)
          {
            if ((xButton-4) == 1) holdUpDown = 1;
            else if ((xButton-4) == 9) holdUpDown = -1;
          }
          else if (xButton >= 4 && holdingLeft == 0 && holdingRight == 0) buttonClick(xButton-4, 1);
        }
        else if (xCode == 2) // Touch Sensor Release
        {
          holdUpDown = 0;
          if (xButton == 2) holdingLeft = 0;
          else if (xButton == 3)
          {
            holdingRight = 0;
            interfaceZone = kInterface_Zone_None;      
          }
        }
      }
      break;
      
      #if IS_LAST_UNIT
        // Add Conversion from SND Generator Message to MIDI Output Message !!! //
      #endif
      
      default: // Other codes // ------------------------------------------- //
      {
        #if !IS_LAST_UNIT
          Serial.write(bByte); // Broadcast Messages //
        #endif
        while (nBytes > 0)
        {
          waitSerialNextByte();
          #if !IS_LAST_UNIT
            Serial.write(Serial.read());
          #endif
          nBytes--;
        }
      }      
    }
  }
  //
  // --------------------------------------- //
  //
  clockTick();
  setLEDs();
  drawLEDs();
  checkUpDown();
}
