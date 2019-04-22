Pick a folder according to your selected programmer and copy its contents to your Arduino_IDE\Hardware folder.

After this process you should have the following:

Arduino_IDE\Hardware\tiny\... and so on.

Before you start, you will need to use AvrDude (found under Arduino_IDE\hardware\tools\avr\bin) and call the following command to se the settings (fuses) of your ATtiny85 chip (if its another chip, check the datasheet for details)
Replace <YOUR_ARDUINO_FOLDER> with your current Arduino_IDE folder. EG: c:\arduino ide\
The command below is for the USBtinyISP, to use the Arduino instead, check the details at the Arduino site or forum.

avrdude -p t85 -c usbtiny -U lfuse:w:0xe2:m -U hfuse:w:0xd7:m -U efuse:w:0xff:m -C "<YOUR_ARDUINO_FOLDER>\hardware\tools\avr\etc\avrdude.conf"

-p is to set the target chip
-c is to set the programmer you will be using
-U will set the fuses for each configuration fuse on the chip
-C is to locate your avrdude configuration file.