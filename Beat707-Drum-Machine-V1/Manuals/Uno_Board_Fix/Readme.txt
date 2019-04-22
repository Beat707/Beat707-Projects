Created by Beat707 (c) 2011 - http://www.Beat707.com

Beat707.com is a joint collaboration between Wusik.com and RuggedCircuits.com, all rights reserved (c) 2011

--------------------------------------------------------------------------------------------------------

If you have an Arduino UNO board and is having problems loading sketches larger than 28k, you will need to update the Bootloader of your board.

You need an ISP AVR programmer for that. We use USBtinyISP, just like this one:

http://www.ladyada.net/make/usbtinyisp/download.html

Just search on eBay and you will see tons of options, we got a $15 USD one with Free Shipping, and it worked perfectly.

You will need to replace the file optiboot_atmega328.hex included in this folder with the one in the following folder of your Arduino IDE:

/hardware/arduino/bootloaders/optiboot

Them open the Arduino IDE and under Tools select Burn Bootloader and the hardware you will be using to do this, wait until it completes.

---------------------------------------------------------------------------------