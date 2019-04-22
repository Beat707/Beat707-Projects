Beat707 XXL LE (Limited Edition) with LCD Support (uses I2C/TwoWire LCDs)

Created by Guilherme (William) Kalfelz
@ Beat707 (c) 2014 - http://www.Beat707.com

------------------------------------------------------------------------------------------------

Beat707 XXL LE LCD has only MIDI output.
Uses the ATmega328 EEPROM to store data.
This unit can handle 4 drum tracks, 2 accent tracks (hardcoded) and 16 x 4 ABCD steps = 64 patterns.
Plus, a song mode with 99 positions with Pattern # and Repeat #.
Each pattern is actually a combi of 4 x 16 steps plus extra double steps for a total of 128 steps for each of the 16 patterns.
So in total we have 128 steps per pattern. We have setup a very easy way to edit all those steps.
We have steps A, B, C, D for each set of 16 steps.
Also, you can set a set to "double-step" and it will blink instead of light-up.
All options are set into the program itself which can be altered and uploaded using the Arduino IDE.

- Uses only Two ATmega328 Chips and basic components.
- Work with a 500mA 5V power supply.
- Can use 4 pin mic left and right connectors, instead of 6 pin mic connectors.
- Multiple units can be side-chained.
- LCD support