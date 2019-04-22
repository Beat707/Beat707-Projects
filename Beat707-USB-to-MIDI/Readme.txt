Created by Beat707 (c) 2011 - http://www.Beat707.com

Beat707.com is a joint collaboration between Wusik.com and RuggedCircuits.com, all rights reserved (c) 2011

--------------------------------------------------------------------------------------------------------

Please, consider making a donation to support continued software development: http://donate.Beat707.com

--------------------------------------------------------------------------------------------------------

! Important ! Update your Java files before anything else: http://java.com/en/download/

The included Source Files were made with Processing: http://processing.org/ (they are not Arduino files)

--------------------------------------------------------------------------------------------------------

Latest version direct download link:

https://github.com/Beat707/Beat707-USB-to-MIDI/zipball/master

Official GitHub download area:

https://github.com/Beat707/Beat707-USB-to-MIDI

--------------------------------------------------------------------------------------------------------

How to use: its actually very simple. First be sure you got the latest Beat707 software running on your Arduino board, as it has a new option to enable MIDI over USB. Start your Beat707 device and, from the Interface, find the USB MODE option and enable it. Plug the USB cable to your computer, wait a few seconds so the computer detects it and assigns a Serial port to it. Now startup Beat707_USB2MIDI, the code will check all available Serial ports until it finds a Beat707 device and initializes communication. You don't need to do anything else, just wait a few seconds. Once you see the MIDI Output Port selection window, you are ready to go. If you change the MIDI Output Port, the code will inform the Beat707 unit which will store the new setting on its EEPROM, so the next time you start the application it will be where you had selected.

If you plan on using a computer software like EnergyXT on Windows, you will need a MIDI Loopback Driver such as the MIDI Yoke from MIDI-OX: http://www.midiox.com/

--------------------------------------------------------------------------------------------------------

May 20 2011 - Version 1.0.0

- First Release with Midi Output only.
- Automated Serial Port Check, no need to know which port Beat707 is connected to.
- Low Latency and Low CPU overhead, thanks to own code, no external libraries used.
- The selected MIDI Out Port is saved in the Beat707 EEPROM

--------------------------------------------------------------------------------------------------------

June 24 2011 - Version 1.0.2

- Updated code to reflect latest changes in the Arduino sources. (requires the latest Beat707 DM software)
- Better handling of Arduino boards with auto-reset. 

--------------------------------------------------------------------------------------------------------