/*
 * 
 * Created by William Kalfelz @ Beat707 (c) 2017 - http://www.Beat707.com
 * 
 */

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void timerStart()
{
  TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM11);
  TCCR1B = _BV(WGM12) | _BV(WGM13) | _BV(CS10);
  ICR1 = (726-1);
  TIMSK1 = _BV(TOIE1);
  OCR1A = OCR1B = 0;
  sei();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void timerStop()
{
  bitWrite(TIMSK1, OCIE1A, 0);
  TCCR1A = TCCR1B = OCR1A = 0; 
  soundPlaying = -1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ISR(TIMER1_OVF_vect)
{
  switch (currentMode)
  {
    case kAntiPopUp:
      OCR1A = OCR1B = (uint16_t) antiPopOnStart;
      antiPopOnStart += antiPopOnStartRate;
      if (antiPopOnStart >= 363.0f) 
      {
        Serial.println("AntiPopUp Finished");
        #if !USE_AUTO_ENGINE
          currentMode = kIdle;
        #else
          currentMode = kPlayingSound;
        #endif
      }
    break;
    //
    case kAntiPopDown:
      OCR1A = OCR1B = (uint16_t) antiPopOnStart;
      antiPopOnStart -= antiPopOnStartRate;
      soundPlaying = -1;
      if (antiPopOnStart <= 0.0f) 
      {
        antiPopOnStart = 0.0f;
        Serial.println("AntiPopDown Finished");
        currentMode = kIdle;
        shutDownEngine = true;
      }
    break;
    //
    case kPlayingSound:
      if (soundPlaying != -1)
      {
        OCR1A = OCR1B = soundOut[soundOutPos2][soundOutPos1];
        if (soundOutPos1 == 0)
        {
          /*if (readNextSample) 
          {
            Serial.print("Buffer Error - Pos: ");
            Serial.println(soundPosition, DEC);
          }*/
          readNextSample = true;
        }
        soundOutPos1++;
        soundLen -= 2;
        if (soundLen <= 0) 
        {
          Serial.println("Sound Ended");
          if (playLooped)
          {
            Serial.println("Loop Sound");
            soundPosition = listOfSounds[soundPlaying].soundAddress;
            soundLen = listOfSounds[soundPlaying].soundSize;
          }
          else
          {
            soundPlaying = -1;
            antiPopOnStart = 363.0f;
            currentMode = kAntiPopDown;
          }
        }
        else if (soundOutPos1 >= BUFFER_SIZE)
        {
          soundOutPos1 = 0;
          if (soundOutPos2 == 0) soundOutPos2 = 1; else soundOutPos2 = 0;
        }
      }
      else OCR1A = OCR1B = 363;   
    break;
  }
}
