/*
 * 
 * Beat707 MIDI Box
 * Created by William Kalfelz @ Beat707 (c) 2017 - http://www.Beat707.com
 * 
 */

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "Arduino.h"
#include "wiring_private.h"
#include "Serial_Lite.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ISR(USART_RX_vect)
{
  byte i = checkBufferOutOfBounds(XSerial.rx_buffer.head);
  if (i != XSerial.rx_buffer.tail)
  {
    XSerial.rx_buffer.buffer[XSerial.rx_buffer.head] = UDR;
    XSerial.rx_buffer.head = i;
  }
}
  
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ISR(USART_UDRE_vect)
{
  if (XSerial.tx_buffer.head == XSerial.tx_buffer.tail) 
  {
    cbi(UCSRB, UDRIE); // Buffer empty, so disable interrupts
  }
  else 
  {
    // There is more data in the output buffer. Send the next byte
    unsigned char c = XSerial.tx_buffer.buffer[XSerial.tx_buffer.tail];
    XSerial.tx_buffer.tail = checkBufferOutOfBounds(XSerial.tx_buffer.tail);
    UDR = c;
  }
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
inline byte checkBufferOutOfBounds(byte nvalue)
{
  nvalue++;
  if (nvalue >= SERIAL_BUFFER_SIZE) nvalue -= SERIAL_BUFFER_SIZE;
  return nvalue;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
inline void XHardwareSerial::begin()
{
  *_ucsra = 1 << _u2x;
  //
  *_ubrrh = uint16_t((F_CPU / 4 / 31250 - 1) / 2) >> 8;
  *_ubrrl = ((F_CPU / 4 / 31250 - 1) / 2);
  //
  sbi(*_ucsrb, _rxen);
  sbi(*_ucsrb, _txen);
  sbi(*_ucsrb, _rxcie);
  cbi(*_ucsrb, _udrie);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
inline byte XHardwareSerial::readByte(void)
{
  byte c = rx_buffer.buffer[rx_buffer.tail];
  rx_buffer.tail = checkBufferOutOfBounds(rx_buffer.tail);
  return c;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void XHardwareSerial::writeByte(byte c)
{
  byte i = checkBufferOutOfBounds(tx_buffer.head);
  while (i == tx_buffer.tail) { ; }
  tx_buffer.buffer[tx_buffer.head] = c;
  tx_buffer.head = i; 
  sbi(*_ucsrb, _udrie);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
XHardwareSerial XSerial;
