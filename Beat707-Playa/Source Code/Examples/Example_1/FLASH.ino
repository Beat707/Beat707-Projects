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

