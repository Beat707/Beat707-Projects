Created by Beat707 (c) 2011 - http://www.Beat707.com

Beat707.com is a joint collaboration between Wusik.com and RuggedCircuits.com, all rights reserved (c) 2011

--------------------------------------------------------------------------------------------------------

Requires the MIDI Shield: http://www.ruggedcircuits.com/html/flexible_midi_shield.html

And 8 x 10k Resistors on each output, starting on digital output 2 ... 9 - connect all resistors to Left+Right of a Headphone or Line-Output, and Arduino-Ground to Output-Ground.

--------------------------------------------------------------------------------------------------------

For AUDIO EXAMPLES visit: http://www.beat707.com/downloads/Beat707_SY_LE/

For VIDEO EXAMPLES visit: http://www.youtube.com/user/beat707videos

--------------------------------------------------------------------------------------------------------

Please, consider making a donation to support continued software development: http://donate.Beat707.com

--------------------------------------------------------------------------------------------------------

Latest version direct download link:

https://github.com/Beat707/Beat707-SY-LE/archives/master

Official GitHub download area:

https://github.com/Beat707/Beat707-SY-LE

--------------------------------------------------------------------------------------------------------

August 18 2011 - Version 1.0.0

- First Release (8 voices with 4 modes + Draft Pitch Bend)
- MIDI CC 0 = Mode / 0= 8 Voices / 1= 4 Voices Double Mode / 2= 2 Voices Double Mode + Octave Up / 3= 2 Voices Double Mode + Octave Up + Octave Down
- MIDI CC 1 = PWM Rate-Fine (When PWM Rate-Coarse is 0, this will be PWM position instead)
- MIDI CC 2 = PWM Rate-Coarse
- MIDI CC 3 = Velocity to PWM Mode

---------------------------------------------------------------------------------