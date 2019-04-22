Created by Beat707 (c) 2011 - http://www.Beat707.com

Beat707.com is a joint collaboration between Wusik.com and RuggedCircuits.com, all rights reserved (c) 2011

--------------------------------------------------------------------------------------------------------

Please, consider making a donation to support continued software development: http://donate.Beat707.com

--------------------------------------------------------------------------------------------------------

Based on the following hardware: http://www.sparkfun.com/products/9022
Don't forget the actual buttons: http://www.sparkfun.com/products/7835
Requires USBtinyISP board (around $ 10 on eBay - or any other AVR ISP programmer that is compatible with the Arduino IDE) a MIDI Cable, 220 Resistor and 5V+ Power Supply.
Connect MIDI cable pins to V+ (using the 220 resistor) and SCK on the Headers. (Digital Pin 13 on the Arduino IDE)

--------------------------------------------------------------------------------------------------------

You will need to update your Arduino IDE files in order for it to use the USBtinyISP (or any other AVR ISP programmer) and also have the Arduino 20Mhz board listed under boards.

1) in the Arduino IDE folder, look for hardware/arduino and inside it, edit boards.txt, add the text below as it is, or use the included docs/Hardware_Arduino_Files/boards.txt file from the GitHub files. When uploading the code, be sure to select the 20Mhz board first. If you are using a different ISP programmer, edit the atmega328_20.upload.using= line to reflect that.
2) still in the Arduino IDE folder, inside hardware/arduino/bootloaders/atmega, copy the docs/Bootloaders_Atmega/ATmegaBOOT_168_atmega328_20MHz.hex file to this folder. We won't really use the bootloader, but we need it so the Arduino IDE can upload it and also set the configuration of the chip correctly. (fuses)
3) open the Arduino IDE, select the correct Arduino 20Mhz board, and use the Burn Upload function. Again, we don't need to bootloader, but, we need the Arduino IDE to set a special chip configuration which is done during this process. (fuses) One of those special configurations is called EESAVE, so when you re-program the chip, it doesn't erase the EEPROM, where we store the patterns at.
4) now you are ready to upload the Beat707 LE Sketch.

A special note about the USBtinyISP, if you have problems using it, try using a USB-HUB with it. Here we had all sort of problems until we connected to a small USB-HUB, and now it works perfectly.

##############################################################

atmega328_20.name=Arduino 20MHz ATmega328 EESAVE ISP

atmega328_20.upload.protocol=stk500
atmega328_20.upload.maximum_size=30720
atmega328_20.upload.speed=57600
atmega328_20.upload.using=usbtinyisp

atmega328_20.bootloader.low_fuses=0xFF
atmega328_20.bootloader.high_fuses=0xD2
atmega328_20.bootloader.extended_fuses=0x05
atmega328_20.bootloader.path=atmega
atmega328_20.bootloader.file=ATmegaBOOT_168_atmega328_20MHz.hex
atmega328_20.bootloader.unlock_bits=0x3F
atmega328_20.bootloader.lock_bits=0x0F

atmega328_20.build.mcu=atmega328p
atmega328_20.build.f_cpu=20000000L
atmega328_20.build.core=arduino

##############################################################

--------------------------------------------------------------------------------------------------------

Latest version direct download link:

https://github.com/Beat707/Beat707-LE/archives/master

Official GitHub download area:

https://github.com/Beat707/Beat707-LE

--------------------------------------------------------------------------------------------------------

June 19 2011 - Version 1.0.0

- First Release

---------------------------------------------------------------------------------