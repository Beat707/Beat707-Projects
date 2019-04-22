/*
 * Beat707 Mikro - created by William (Guilherme) Kalfelz (c) 2016 - www.Beat707.com - www.Wusik.com - www.William-k.com
 * 
 */

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
byte getTrackNote(byte xstep)
{
  if (curTrack < noteTrack) return 0;
  return (noteSteps[xstep+STEPS] & 0x7F); // Getting only 7 bits
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
byte getTrackVelocity(byte xstep)
{
  if (curTrack < noteTrack) return 0;
  return (noteSteps[xstep] & 0x7F); // Getting only 7 bits
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
byte getTrackNoteCode(byte xstep)
{
  if (curTrack < noteTrack) return 0;
  byte xcode = (noteSteps[xstep+STEPS] >> 7) & 0x01;
  xcode |= ((noteSteps[xstep] >> 6) & 0x02);
  if (xcode == 0 && getTrackVelocity(xstep) == 0 && getTrackNote(xstep) > 0) xcode = noteSlide;
  return xcode;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
boolean isAccentTrack()
{
  if ((curTrack == accentTrack) || (curTrack == (accentTrack+1))) return true; else return false;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
byte getTrackWithoutAccent()
{
  if (curTrack >= noteTrack) return curTrack-2;
  return curTrack;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void setTrackRegularNote(byte xstep)
{
  noteSteps[xstep] = (noteSteps[xstep] & 0x7F);
  noteSteps[xstep+STEPS] = (noteSteps[xstep+STEPS] & 0x7F);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void setTrackNoteOff(byte xstep)
{
  noteSteps[xstep] = (noteSteps[xstep] & 0x7F);
  noteSteps[xstep+STEPS] = 0x80 | (noteSteps[xstep+STEPS] & 0x7F);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void setTrackDouble(byte xstep)
{
  noteSteps[xstep] = 0x80 | (noteSteps[xstep] & 0x7F);
  noteSteps[xstep+STEPS] = (noteSteps[xstep+STEPS] & 0x7F);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void setTrackDoubleIn(byte xstep)
{
  noteSteps[xstep] = 0x80 | (noteSteps[xstep] & 0x7F);
  noteSteps[xstep+STEPS] = 0x80 | (noteSteps[xstep+STEPS] & 0x7F);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void setTrackSlide(byte xstep)
{
  noteSteps[xstep] = 0;
  noteSteps[xstep+STEPS] = (noteSteps[xstep+STEPS] & 0x7F);
}
