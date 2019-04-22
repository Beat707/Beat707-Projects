/*
 * 
 * Beat707 MIDI Box
 * Created by William Kalfelz @ Beat707 (c) 2017 - http://www.Beat707.com
 * 
 */

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef XHardwareSerial_h
#define XHardwareSerial_h
//
#define _ubrrh &UBRRH
#define _ubrrl &UBRRL
#define _ucsra &UCSRA
#define _ucsrb &UCSRB
#define _udr UDR
#define _rxen RXEN
#define _txen TXEN
#define _rxcie RXCIE
#define _udrie UDRE
#define _u2x U2X
//
#define SERIAL_BUFFER_SIZE 12

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
struct ring_buffer
{
  public:
    ring_buffer::ring_buffer() { init(); };
    void init()
    {
      memset(buffer, 0, sizeof(buffer));
      head = tail = 0;
    };
    unsigned char buffer[SERIAL_BUFFER_SIZE];
    byte head, tail;
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class XHardwareSerial
{
  public:
      void begin();
      bool hasSomething() { return (rx_buffer.head != rx_buffer.tail); };
      byte readByte(void);
      void writeByte(byte c);
      //
      volatile ring_buffer rx_buffer;
      volatile ring_buffer tx_buffer;
};
#endif

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
extern XHardwareSerial XSerial;
