/*

  Created by Beat707 (c) 2011 - http://www.Beat707.com
  
  Extra Functions and Classes used by this project
  
*/


// ======================================================================================= //
void volatile nop(void) { asm __volatile__ ("nop"); }

// ======================================================================================= //
#define sbi(var, mask)   ((var) |= (uint8_t)(1 << mask))  // Set Bit
#define cbi(var, mask)   ((var) &= (uint8_t)~(1 << mask)) // Clear Bit

// ======================================================================================= //
ISR(TIMER2_COMPA_vect)
{
  lineByte = 0;
  row = (row + 1) & 3;
  if (row == 0) frame_num = (frame_num + 1) & (COLOR_DEPTH);
  row_offset = row << 2;

  for (uint8_t column = 0; column < 4; column++)
  {
    LED = column + row_offset;		
    if (column != row) lineByte |= (1<<column);
    if (greenf[LED] > frame_num) lineByte |= (1 << (column + GREEN_POS));
    if (bluef[LED] > frame_num) lineByte |= (1 << (column + BLUE_POS));
    if (redf[LED] > frame_num) lineByte |= (1 << (column + RED_POS));
  }

  for(uint8_t i = 0; i < 16; i++)
  {
    cbi(PORTC, XSR_SCK);
    if (lineByte & (1 << i)) sbi(PORTC, XSR_DATA); else cbi(PORTC, XSR_DATA);
    sbi(PORTC, XSR_SCK);
  }
  cbi(PORTC, XSR_LATCH);
  nop();
  sbi(PORTC, XSR_LATCH);
  PORTD &= 0xF0;
  PORTD |= (1 << row);

  buttonCounter++;
  if (buttonCounter > 10)
  {
    buttonCounter = 0;
    buttonRow = PIND; // Get the value of the buttons
    for (char i=0; i<4; i++)
    {
      if (!(buttonRow & (1<<(i+4)))) // Click
      {
        if (bitRead(buttons,(i*4)+row) == 0) // First Click
        {
          bitSet(buttons,(i*4)+row);
          buttonHoldCounter = 1;
        }
        else if (bitRead(buttonsHold,(i*4)+row) == 0 && buttonHoldCounter != 0) // Hold Click
        {
          buttonHoldCounter++;
          if (buttonHoldCounter > BT_HOLD_TIME)
          {
            buttonHoldCounter = 0;
            buttonClick[((i*4)+row)] = 2;
            bitSet(buttonsHold,(i*4)+row);
          }
        }
      }
      else // Release
      {
        if (bitRead(buttons,(i*4)+row) == 1)
        {
          if (bitRead(buttonsHold,(i*4)+row) == 0) buttonClick[((i*4)+row)] = 1;
          bitClear(buttons,(i*4)+row); bitClear(buttonsHold,(i*4)+row);
        }
      }
    }
  }
  
  midiBufferCheck();
}

// ======================================================================================= //
uint8_t dbSteps=0, dbStepsS=0;
void dbStepsCalc()
{
  dbSteps = (((DRUMTRACKS+2)*editDoubleSteps)+(((DRUMTRACKS+2)*2)*editStepsPos) );
  dbStepsS = (16*editDoubleSteps)+(32*editStepsPos);
}

// ======================================================================================= //
void clearDisplay()
{
  memset(redf,0,sizeof(redf));
  memset(greenf,0,sizeof(greenf));
  memset(bluef,0,sizeof(bluef));
}  

// ======================================================================================= //
void timerStart()
{
  TCCR1A = TCCR1B = 0;
  bitWrite(TCCR1B, CS11, 1);
  bitWrite(TCCR1B, WGM12, 1);
  timerSetFrequency();
  bitWrite(TIMSK1, OCIE1A, 1);
}

void timerSetFrequency()
{
  // Calculates the Frequency for the Timer, used by the PPQ clock (Pulses Per Quarter Note) //
  // This uses the 16-bit Timer1, unused by the Arduino, unless you use the analogWrite or Tone functions //
  #define frequency (((song_settings.midiClockBPM)*(PPQ))/60)
  OCR1A = (F_CPU/ 8) / frequency - 1;
}

void timerStop(void)
{
  bitWrite(TIMSK1, OCIE1A, 0);
  TCCR1A = TCCR1B = OCR1A = 0;
}

// ======================================================================================= //
