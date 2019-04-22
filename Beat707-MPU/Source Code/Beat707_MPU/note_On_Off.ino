/*
 * 
 * Beat707 MIDI Box
 * Created by William Kalfelz @ Beat707 (c) 2017 - http://www.Beat707.com
 * 
 */

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
inline bool noteOn(byte note, byte velocity)
{
  bool doEcho = true;
  //
  switch (allSettings.settings[kMode])
  {
    case kMode_KeySwitching:
      if (globalVars[kKeyS_Glob_Active] == 0) doEcho = false;
      if (isInsideLowKeyZone(note))
      {
        doEcho = false;
        globalVars[kKeyS_Glob_Active] = 0;
        if (note == allSettings.settings[kKeyS_Set_KeyRoot1] || note == allSettings.settings[kKeyS_Set_KeyRoot2]) globalVars[kKeyS_Glob_Active] = 1;
      }
    break;
    //
    case kMode_LowKeysToChannel:
      if (isInsideLowKeyZone(note))
      {
        doEcho = false;
        forceMIDIChannel = note - allSettings.settings[kKeySM_Set_KeyLow];
        if (forceMIDIChannel > 15) forceMIDIChannel = 15;
      }
    break;
    //
    case kMode_Sequencer:
      if (isInsideLowKeyZone(note))
      {
        doEcho = false;
        if (note == allSettings.settings[kSequencer_Start]) startSequencer();
        else if (note == allSettings.settings[kSequencer_Stop]) stopSequencer();
        else if (note == allSettings.settings[kSequencer_SpeedUp])
        {
          if (globalVars[kSequencer_Glob_BPM] < (MAX_SEQ_SPEEDS-1)) globalVars[kSequencer_Glob_BPM]++;
        }
        else if (note == allSettings.settings[kSequencer_SpeedDown])
        {
          if (globalVars[kSequencer_Glob_BPM] > 0) globalVars[kSequencer_Glob_BPM]--;
        }
        updateSequencerSpeed();
      }
    break;
  }
  //
  return doEcho;
}
