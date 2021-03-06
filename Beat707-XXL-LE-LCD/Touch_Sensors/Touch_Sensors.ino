/*

  Beat707 XXL LE LCD

  Created by Guilherme (William) Kalfelz
  @ Beat707 (c) 2014
  http://www.Beat707.com
  
  This sketch will check all 18 capacitative
  touch sensors and send via Serial a code to 
  tell which button was clicked / hold / released
  
*/

#define IS_SLAVE_DEVICE 0
//
int16_t buttonState[22];
boolean buttonWasHold[22];
int16_t buttonHoldCounter[22];
boolean buttonWasOver[22];
volatile uint8_t* port[22];
volatile uint8_t* ddr[22];
volatile uint8_t* pin[22];
uint8_t check;
uint8_t x;
uint8_t cycles;
byte bitmask[22];
uint8_t counter = 2;
uint8_t somethingClicked = 0;

// --------------------------------------------------------------------------------------------------------------------------------------------
void setup() 
{
  delay(100);
  Serial.begin(31250);
  //
  for (x=0; x<22; x++)
  {
    buttonState[x] = 0;
    buttonWasHold[x] = false;
    buttonHoldCounter[x] = 0;
    buttonWasOver[x] = false;
  }
  for (x=2; x<=13; x++)
  {
    pinMode(x, INPUT);
    port[x] = portOutputRegister(digitalPinToPort(x));
    ddr[x] = portModeRegister(digitalPinToPort(x));
    bitmask[x] = digitalPinToBitMask(x);
    pin[x] = portInputRegister(digitalPinToPort(x));    
  }
  for (x=0; x<6; x++)
  {
    pinMode(A0+x, INPUT);
    port[14+x] = portOutputRegister(digitalPinToPort(A0+x));
    ddr[14+x] = portModeRegister(digitalPinToPort(A0+x));
    bitmask[14+x] = digitalPinToBitMask(A0+x);
    pin[14+x] = portInputRegister(digitalPinToPort(A0+x));    
  }
  
  counter = 2;
}

// --------------------------------------------------------------------------------------------------------------------------------------------
void loop() 
{
  #if IS_SLAVE_DEVICE
    if (Serial.available() > 0)
    {
      Serial.write(Serial.read());
    }
  #endif

  // ----------- //
  
  checkButton(counter);
  counter++;
  if (counter >= 20) counter = 2;
}

// --------------------------------------------------------------------------------------------------------------------------------------------
void checkButton(uint8_t pin)
{
    check = readCapacitivePin(pin);
    if (check <= 1)
    {
      if (buttonState[pin] > 0)
      {
        buttonState[pin] -= 4;
        if (buttonState[pin] <= 0 && buttonWasOver[pin])
        {
          buttonState[pin] = 0;
          buttonWasOver[pin] = false;
          if (buttonWasHold[pin])
          {
            buttonWasHold[pin] = false;
            Serial.write(B00100001); // Touch Sensor
            Serial.write(B01000000 | pin); // Button Release
            if (somethingClicked == 0 && (pin == 2 || pin == 3) && buttonHoldCounter[pin] < 990)
            {
              Serial.write(B00100001); // Touch Sensor
              Serial.write(pin); // Button Hit              
            }
            buttonHoldCounter[pin] = 0;            
          }
          else
          {
            somethingClicked = 1;
            Serial.write(B00100001); // Touch Sensor
            Serial.write(pin); // Button Hit
          }
        }
      }
    }
    else if (check > 4)
    {
      if (buttonState[pin] < 240)
      {
        buttonState[pin] += 16;
        if (buttonState[pin] < 180)
        {
          buttonHoldCounter[pin] = 0;
          buttonWasHold[pin] = false;
        }
      }
      else
      {
        buttonWasOver[pin] = true;
        //
        if (pin == 2 || pin == 3)
        {
          buttonHoldCounter[pin]++;
          if (buttonHoldCounter[pin] > 1000) buttonHoldCounter[pin] = 1000;
          if (buttonHoldCounter[pin] == 80)
          {
            if (!buttonWasHold[pin])
            {
              somethingClicked = 0;
              buttonWasHold[pin] = true;
              Serial.write(B00100001); // Touch Sensor 
              Serial.write(B00100000 | pin); // Button Hold
            }
          }          
        }
        else
        {
          buttonHoldCounter[pin]++;
          if (buttonHoldCounter[pin] > 1000)
          {
            buttonHoldCounter[pin] = 1000;
            if (!buttonWasHold[pin])
            {
              buttonWasHold[pin] = true;
              Serial.write(B00100001); // Touch Sensor 
              Serial.write(B00100000 | pin); // Button Hold
            }
          }
        }
      }
    }  
}

// --------------------------------------------------------------------------------------------------------------------------------------------
uint8_t readCapacitivePin(uint8_t pinToMeasure) 
{ 
  *port[pinToMeasure] &= ~(bitmask[pinToMeasure]);
  *ddr[pinToMeasure]  |= bitmask[pinToMeasure];
  
  noInterrupts();
  *ddr[pinToMeasure] &= ~(bitmask[pinToMeasure]);
  *port[pinToMeasure] |= bitmask[pinToMeasure];

  cycles = 5;
  if (*pin[pinToMeasure] & bitmask[pinToMeasure]) { cycles =  0;}
  else if (*pin[pinToMeasure] & bitmask[pinToMeasure]) { cycles =  1;}
  else if (*pin[pinToMeasure] & bitmask[pinToMeasure]) { cycles =  2;}
  else if (*pin[pinToMeasure] & bitmask[pinToMeasure]) { cycles =  3;}
  else if (*pin[pinToMeasure] & bitmask[pinToMeasure]) { cycles =  4;}

  interrupts();
  *port[pinToMeasure] &= ~(bitmask[pinToMeasure]);
  *ddr[pinToMeasure]  |= bitmask[pinToMeasure];

  return cycles;
}
