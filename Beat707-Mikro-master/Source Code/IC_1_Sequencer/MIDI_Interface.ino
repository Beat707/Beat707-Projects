/*
 * Beat707 Mikro - created by William (Guilherme) Kalfelz (c) 2016 - www.Beat707.com - www.Wusik.com - www.William-k.com
 * 
 */

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MIDIallNotesOff()
{
  for (byte x=0; x<midiChannels; x++)
  {
    Serial.write(0xB0+x);
    Serial.write(0x7B);
    Serial.write((byte)0x00);
  }
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void sendMidiEvent(byte type, byte byte1, byte byte2, byte channel)
{
  Serial.write(type+channel);
  Serial.write(byte1);
  Serial.write(byte2);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void startMIDIinterface(boolean overUSB)
{
  if (overUSB) Serial.begin(38400); else Serial.begin(31250); // 31250 MIDI Interface //
  Serial.flush();
  resetSequencer();
  MIDIallNotesOff();
}
