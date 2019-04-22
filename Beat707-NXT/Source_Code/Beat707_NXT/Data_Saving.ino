/*
 * 
 * Created by William Kalfelz @ Beat707 (c) 2018 - http://www.Beat707.com
 * 
 */

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void checkIfDataNeedsSaving()
{
  if (somethingChangedPattern)
  {
    somethingChangedPattern = false;
    if (!configData.writeProtectFlash)
    {
      stopTimer(true);
      savePatternData(true);
      saveStepsData();
      startTimer(true);
    }
  }
  // -------=========---------- //
  if (somethingChangedConfig)
  {
    somethingChangedConfig = false;
    if (!configData.writeProtectFlash) saveConfigData(true);
  }
  //
  if (changedPatternBank)
  {
    changedPatternBank = false;
    if (!configData.writeProtectFlash) saveHeader(currentPatternBank, true);
  }
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void eraseSector(uint16_t pagePos)
{
  #if !DISABLE_FLASH
    if (!flash.eraseSector(pagePos, 0)) showErrorMsg(flash.error());  
  #endif
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void saveConfigData(byte _eraseSector)
{
  #if !DISABLE_FLASH
    stopTimer(true);
    if (_eraseSector) eraseSector(16 + (currentPatternBank * ((64 * 16) + 16)));
    if (!flash.writeAnything(16 + (currentPatternBank * ((64 * 16) + 16)), (uint8_t) 0, configData)) showErrorMsg(flash.error());  
    startTimer(true);
  #endif
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void savePatternData(byte _eraseSector)
{
  #if !DISABLE_FLASH
    if (_eraseSector) eraseSector(16 + (currentPatternBank * ((64 * 16) + 16)) + (currentPattern * 16) + 16);
    if (!flash.writeAnything(16 + (currentPatternBank * ((64 * 16) + 16)) + (currentPattern * 16) + 16, (uint8_t) 0, patternData)) showErrorMsg(flash.error());   
  #endif
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void saveStepsData()
{
  #if !DISABLE_FLASH
    if (!flash.writeAnything(16 + 1 + (currentPatternBank * ((64 * 16) + 16)) + (currentPattern * 16) + 16, (uint8_t) 0, stepsData)) showErrorMsg(flash.error());
  #endif
}
