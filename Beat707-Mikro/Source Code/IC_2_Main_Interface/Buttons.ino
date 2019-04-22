/*
 * Beat707 Mikro - created by William (Guilherme) Kalfelz (c) 2016 - www.Beat707.com - www.Wusik.com - www.William-k.com
 * 
 */

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define PB_WRITE_HIGH(pin) PORTB |= _BV(pin)
#define PB_WRITE_LOW(pin) PORTB &= ~_BV(pin)
#define PB_INPUT(pin) DDRB &= ~_BV(pin)
#define PB_INPUT_PULLUP(pin) PB_INPUT(pin); PB_WRITE_HIGH(pin)
#define PB_OUTPUT(pin) DDRB |= _BV(pin)
#define PB_READ(pin) (PINB & _BV(pin))
  
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define PC_WRITE_HIGH(pin) PORTC |= _BV(pin)
#define PC_WRITE_LOW(pin) PORTC &= ~_BV(pin)
#define PC_INPUT(pin) DDRC &= ~_BV(pin)
#define PC_INPUT_PULLUP(pin) PC_INPUT(pin); PC_WRITE_HIGH(pin)
#define PC_OUTPUT(pin) DDRC |= _BV(pin)
#define PC_READ(pin) (PINC & _BV(pin))
  
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define PD_WRITE_HIGH(pin) PORTD |= _BV(pin)
#define PD_WRITE_LOW(pin) PORTD &= ~_BV(pin)
#define PD_INPUT(pin) DDRD &= ~_BV(pin)
#define PD_INPUT_PULLUP(pin) PD_INPUT(pin); PD_WRITE_HIGH(pin)
#define PD_OUTPUT(pin) DDRD |= _BV(pin)
#define PD_READ(pin) (PIND & _BV(pin))
  
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void setupButtons()
{
  #if V2_BOARD
    PC_INPUT_PULLUP(PC0);
    PC_INPUT_PULLUP(PC1);
  #else
    PB_INPUT_PULLUP(PB6);
    PB_INPUT_PULLUP(PB7);
  #endif
  //
  PD_INPUT_PULLUP(PD2);
  PD_INPUT_PULLUP(PD3);
  PD_INPUT_PULLUP(PD4);
  PD_INPUT_PULLUP(PD5);
  PD_INPUT_PULLUP(PD6);
  PD_INPUT_PULLUP(PD7);
  //
  PB_INPUT_PULLUP(PB0);
  //
  PC_INPUT_PULLUP(PC2);
  PC_INPUT_PULLUP(PC3);
  //
  buttonsClear();
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void checkButtons()
{
  checkButton(buttonUp,       PD_READ(PD5), true,  true);
  checkButton(buttonDown,     PD_READ(PD6), true,  true);
  checkButton(buttonLeft,     PD_READ(PD4), true,  true);
  checkButton(buttonPageUp,   PD_READ(PD2), true,  true);
  checkButton(buttonPageDown, PD_READ(PD3), true,  true);
  checkButton(buttonMenu,     PC_READ(PC3), true,  false);
  checkButton(buttonPlay,     PB_READ(PB0), false, false);
  checkButton(buttonStop,     PD_READ(PD7), false, false);
  checkButton(buttonRecord,   PC_READ(PC2), false, false);
  //
  #if V2_BOARD
    checkButton(buttonShift,    PC_READ(PC0), true,  false);
    checkButton(buttonRight,    PC_READ(PC1), true,  true);
  #else
    checkButton(buttonShift,    PB_READ(PB7), true,  false);
    checkButton(buttonRight,    PB_READ(PB6), true,  true);
  #endif
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void checkButton(byte button, byte value, boolean supportHold, boolean holdAutoIncrease)
{
  if (value == LOW)
  {
    int holdCC = buttonsHoldCounter;
    if (button == buttonShift) holdCC = buttonsShiftHoldCounter;
    //
    if (dButtons1[button] <= holdCC) dButtons1[button]++;
    if (dButtons1[button] > buttonsClickCounter)
    {
      if (dButtons2[button] != buttonEventClick && dButtons1[button] < holdCC && (button == buttonShift || button == buttonMenu)) holdDownPressedAnything = false;
      dButtons2[button] = buttonEventClick;
    }
    if (dButtons1[button] > holdCC && supportHold)
    {
      if (dButtons2[button] != buttonEventHold)
      {
        dButtons2[button] = buttonEventHold;        
        if (holdAutoIncrease)
        {
          if (dButtonsCounter[button] == 0) 
          {
            if (button != buttonShift && button != buttonMenu) holdDownPressedAnything = true;
            dButtonsEvent[button] = buttonEventClick;
            dButtons2[button] = 0;
          }
          dButtonsCounter[button]++;
          if (dButtonsCounter[button] > buttonsRepeatCounter) dButtonsCounter[button] = 0;
        }
        else
        {
          dButtonsEvent[button] = buttonEventHold;
        }
      }
    }  
  }
  else
  {
    dButtons1[button] -= buttonsDecay;
    if (dButtons1[button] < 0)
    {
      dButtons1[button] = 0;
      if (supportHold)
      {
        if (dButtons2[button] == buttonEventHold) 
        {
          if (button != buttonShift && button != buttonMenu) holdDownPressedAnything = true;
          if (holdAutoIncrease) dButtonsEvent[button] = buttonEventClick;
            else dButtonsEvent[button] = buttonEventRelease;
        }
      }
      if (dButtons2[button] == buttonEventClick) 
      {
        if (button != buttonShift && button != buttonMenu) holdDownPressedAnything = true;
        dButtonsEvent[button] = buttonEventClick;
      }
      dButtons2[button] = buttonEventNone;
    }
  }
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void buttonsClear()
{
  memset(dButtons1,0,sizeof(dButtons1));
  memset(dButtons2,0,sizeof(dButtons2));
  memset(dButtonsEvent,0,sizeof(dButtonsEvent));
  memset(dButtonsCounter,0,sizeof(dButtonsCounter));
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
boolean isShiftDown()
{
  return (dButtonsEvent[buttonShift] == buttonEventHold);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
boolean isMenuDown()
{
  return (dButtonsEvent[buttonMenu] == buttonEventHold);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
boolean isButtonShift(byte button)
{
  boolean xr = false;
  if (dButtonsEvent[buttonShift] == buttonEventHold && dButtonsEvent[button] == buttonEventClick)
  {
    xr = true;
   dButtonsEvent[button] = buttonEventNone;
  }
  return xr;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
boolean isButtonNoShift(byte button)
{
  if (dButtonsEvent[buttonShift] != buttonEventHold && dButtonsEvent[button] == buttonEventClick)
  {
    dButtonsEvent[button] = buttonEventNone;
    return true;
  }
  return false;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
boolean isButtonWhatever(byte button)
{
  boolean xr = false;
  if (dButtonsEvent[button] == buttonEventClick)
  {
    xr = true;
    dButtonsEvent[button] = buttonEventNone;
  }
  return xr;
}
