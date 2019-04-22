@echo off
avrdude -p at90usb82 -F -P usb -c usbtiny -U flash:w:usbserial.hex -U lfuse:w:0xFF:m -U hfuse:w:0xD9:m -U efuse:w:0xF4:m -U lock:w:0x0F:m 
echo.
pause