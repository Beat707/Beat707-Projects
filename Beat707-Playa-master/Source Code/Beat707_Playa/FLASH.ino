/*
 * 
 * Created by William Kalfelz @ Beat707 (c) 2017 - http://www.Beat707.com
 * 
 * Capacity =  8388608 bytes
 * 
 */

#define FLASH_CHIPSIZE MB(64)
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void flashInit()
{
  delay(1000);
  Serial.print("Flash Init: ");
  if (flash.begin(FLASH_CHIPSIZE)) Serial.println("Ok"); else Serial.println("Error!");
  delay(200);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void receiveData()
{
  char theSound = -1;
  uint32_t theAddress = 0;
  for (byte x=0; x<MAX_SOUNDS; x++) 
  {
    if (listOfSounds[x].soundSize == 0)
    {
      if (x == 0) theAddress = 0;
      theSound = x;
      break;
    }
    theAddress = listOfSounds[x].soundAddress + listOfSounds[x].soundSize + ((BUFFER_SIZE * 2) + 2);
  }
  //
  if (theSound == -1) Serial.println("No Space Left!");
  else
  {
    listOfSounds[theSound].soundAddress = theAddress;
    listOfSounds[theSound].soundSize = 0;
    //
    Serial.print("Writing to Sound ");
    Serial.println(theSound, DEC);
    Serial.print("Address ");
    Serial.println(theAddress, DEC);
    //
    byte byte1 = serialReadNext();
    byte byte2 = serialReadNext();
    byte byte3 = serialReadNext();
    uint32_t xsize1 = convertBytes(byte1, byte2, byte3);
    byte1 = serialReadNext();
    byte2 = serialReadNext();
    byte3 = serialReadNext();
    uint32_t xsize2 = convertBytes(byte1, byte2, byte3);
    uint32_t xsize = (xsize1 << 12) | xsize2;
    Serial.println("+----------------------------------------------------------------------------------------------------+");
    Serial.print("[");
    //    
    byte xCols = 0;
    int prevPorc = -1;
    bool hasErrors = false;
    while (1)
    {
      byte1 = serialReadNext();
      byte2 = serialReadNext();
      byte3 = serialReadNext();
      //
      if (byte1 == 'X') break;
      //
      byte xretries = 0;
      uint16_t xsample = convertBytes(byte1, byte2, byte3);
      //
      while (1)
      {
        flash.writeAnything(theAddress, xsample, false);
        uint16_t xsample2 = 0;
        flash.readAnything(theAddress, xsample2);
        if (xsample == xsample2) break;
        //
        Serial.print("Retrying");
        Serial.println(xretries, DEC);
        xretries++;
        if (xretries > 250) 
        {
          Serial.println("Retrying Gave Up");
          hasErrors = true;
          break;
        }
      }
      //
      listOfSounds[theSound].soundSize += 2;
      theAddress += 2;
      xCols++;
      if (xCols > 250)
      {
        xCols = 0;
        int newPorc = int((100.0f/float(xsize*2))*float(listOfSounds[theSound].soundSize));
        if (newPorc != prevPorc)
        {
          Serial.print(".");
          prevPorc = newPorc;
        }
      }
    }
    //
    writeSoundList();
    Serial.println("]");
    Serial.println("Sound Saved");
    if (hasErrors) Serial.println("Errors during Flash writing");
  }
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
byte serialReadNext()
{
  while (Serial.available() == 0) { ; }
  return Serial.read();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint16_t convertBytes(byte _byte1, byte _byte2, byte _byte3)
{
  byte xb1 = _byte1 - '0';
  if (_byte1 >= 'A') xb1 = _byte1 - 'A' + 10;
  byte xb2 = _byte2 - '0';
  if (_byte2 >= 'A') xb2 = _byte2 - 'A' + 10;
  byte xb3 = _byte3 - '0';
  if (_byte3 >= 'A') xb3 = _byte3 - 'A' + 10;
  //
  uint16_t thesample = (xb1 * 256) + (xb2 * 16) + xb3;
  // 
  return thesample;
}

