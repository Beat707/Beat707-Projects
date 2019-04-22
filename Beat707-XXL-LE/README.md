Beat707 XXL LE (Limited Edition)

Created by Guilherme (William) Kalfelz
@ Beat707 (c) 2014 - http://www.Beat707.com

------------------------------------------------------------------------------------------------

Beat707 XXL LE has only MIDI output, no LCD and no SD Card. Uses the ATmega328 EEPROM to store data. This unit can handle 8 drum tracks, 2 accent tracks and 6 patterns. Each pattern is actually a combi of 4 x 16 steps plus extra double steps. So in total we have 128 steps per pattern. We have setup a very easy way to edit all those steps. We have steps A, B, C, D for each set of 16 steps. Also, you can set a set to "double-step" and it will blink instead of light-up. All options are set into the program itself which can be altered and uploaded using the Arduino IDE.

- Uses only Two ATmega328 Chips and basic components.
- Work with a 500mA 5V power supply.
- Can use 4 pin mic left and right connectors, instead of 6 pin mic connectors.
- Settings such as track drum notes and midi channels are set in code, while BPM and Steps# can be changed in the interface itself and is saved along the other data into the internal EEPROM.
- Multiple units can be side-chained.