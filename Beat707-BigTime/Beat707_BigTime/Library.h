/*

  Created by Beat707 (c) 2011 - http://www.Beat707.com
  
  Extra Librarys: TWI (2-Wire) for ATtiny chips and extra settings
  
*/

#define TWI_SLAVE_ADDR  0x26
#define CLOCK_PIN 3
#define DATA_PIN 4
#define LATCH 1
#define setClockHigh() PORTB |= B00001000
#define setClockLow() PORTB &= B11110111
#define setDataHigh() PORTB |= B00010000
#define setDataLow() PORTB &= B11101111
#define SHOW_INIT_DISPLAY 1 // When set it will show "----" as the Default Display until a reset or beat is detected

#ifndef TinyWireS_h
#define TinyWireS_h
  #include <inttypes.h>
  class USI_TWI_S
  { 	
    public:
   	USI_TWI_S();
      void begin(uint8_t I2C_SLAVE_ADDR);
      void send(uint8_t data);
      uint8_t available();
      uint8_t receive();	
  };
  extern USI_TWI_S TinyWireS;
#endif

#ifndef _USI_TWI_SLAVE_H_
#define _USI_TWI_SLAVE_H_
  #include <stdbool.h> 
  void    usiTwiSlaveInit( uint8_t );
  void    usiTwiTransmitByte( uint8_t );
  uint8_t usiTwiReceiveByte( void );
  bool    usiTwiDataInReceiveBuffer( void );
  #define TWI_RX_BUFFER_SIZE  ( 8 )
  #define TWI_RX_BUFFER_MASK  ( TWI_RX_BUFFER_SIZE - 1 )
  #define TWI_TX_BUFFER_SIZE ( 8 )
  #define TWI_TX_BUFFER_MASK ( TWI_TX_BUFFER_SIZE - 1 )
#endif

#ifndef _TWI_SLAVE_CPP_
#define _TWI_SLAVE_CPP_  
  extern "C" 
  {
    #include <inttypes.h>
  }
  
  USI_TWI_S::USI_TWI_S() { ; }
  void USI_TWI_S::begin(uint8_t slaveAddr) {  usiTwiSlaveInit(slaveAddr); }
  void USI_TWI_S::send(uint8_t data) { usiTwiTransmitByte(data); }
  uint8_t USI_TWI_S::available() { return usiTwiDataInReceiveBuffer(); }
  uint8_t USI_TWI_S::receive() { return usiTwiReceiveByte(); }
  USI_TWI_S TinyWireS = USI_TWI_S();
  
  #include <avr/io.h>
  #include <avr/interrupt.h>
  
  #if defined( __AVR_ATtiny2313__ )
  #  define DDR_USI             DDRB
  #  define PORT_USI            PORTB
  #  define PIN_USI             PINB
  #  define PORT_USI_SDA        PB5
  #  define PORT_USI_SCL        PB7
  #  define PIN_USI_SDA         PINB5
  #  define PIN_USI_SCL         PINB7
  #  define USI_START_COND_INT  USISIF
  #  define USI_START_VECTOR    USI_START_vect
  #  define USI_OVERFLOW_VECTOR USI_OVERFLOW_vect
  #endif
  
  #if defined( __AVR_ATtiny25__ ) | \
       defined( __AVR_ATtiny45__ ) | \
       defined( __AVR_ATtiny85__ )
  #  define DDR_USI             DDRB
  #  define PORT_USI            PORTB
  #  define PIN_USI             PINB
  #  define PORT_USI_SDA        PB0
  #  define PORT_USI_SCL        PB2
  #  define PIN_USI_SDA         PINB0
  #  define PIN_USI_SCL         PINB2
  #  define USI_START_COND_INT  USISIF //was USICIF jjg
  #  define USI_START_VECTOR    USI_START_vect
  #  define USI_OVERFLOW_VECTOR USI_OVF_vect
  #endif
  
  #if defined( __AVR_ATtiny26__ )
  #  define DDR_USI             DDRB
  #  define PORT_USI            PORTB
  #  define PIN_USI             PINB
  #  define PORT_USI_SDA        PB0
  #  define PORT_USI_SCL        PB2
  #  define PIN_USI_SDA         PINB0
  #  define PIN_USI_SCL         PINB2
  #  define USI_START_COND_INT  USISIF
  #  define USI_START_VECTOR    USI_STRT_vect
  #  define USI_OVERFLOW_VECTOR USI_OVF_vect
  #endif
  
  #if defined( __AVR_ATtiny261__ ) | \
       defined( __AVR_ATtiny461__ ) | \
       defined( __AVR_ATtiny861__ )
  #  define DDR_USI             DDRB
  #  define PORT_USI            PORTB
  #  define PIN_USI             PINB
  #  define PORT_USI_SDA        PB0
  #  define PORT_USI_SCL        PB2
  #  define PIN_USI_SDA         PINB0
  #  define PIN_USI_SCL         PINB2
  #  define USI_START_COND_INT  USISIF
  #  define USI_START_VECTOR    USI_START_vect
  #  define USI_OVERFLOW_VECTOR USI_OVF_vect
  #endif
  
  #if defined( __AVR_ATmega165__ ) | \
       defined( __AVR_ATmega325__ ) | \
       defined( __AVR_ATmega3250__ ) | \
       defined( __AVR_ATmega645__ ) | \
       defined( __AVR_ATmega6450__ ) | \
       defined( __AVR_ATmega329__ ) | \
       defined( __AVR_ATmega3290__ )
  #  define DDR_USI             DDRE
  #  define PORT_USI            PORTE
  #  define PIN_USI             PINE
  #  define PORT_USI_SDA        PE5
  #  define PORT_USI_SCL        PE4
  #  define PIN_USI_SDA         PINE5
  #  define PIN_USI_SCL         PINE4
  #  define USI_START_COND_INT  USISIF
  #  define USI_START_VECTOR    USI_START_vect
  #  define USI_OVERFLOW_VECTOR USI_OVERFLOW_vect
  #endif
  
  #if defined( __AVR_ATmega169__ )
  #  define DDR_USI             DDRE
  #  define PORT_USI            PORTE
  #  define PIN_USI             PINE
  #  define PORT_USI_SDA        PE5
  #  define PORT_USI_SCL        PE4
  #  define PIN_USI_SDA         PINE5
  #  define PIN_USI_SCL         PINE4
  #  define USI_START_COND_INT  USISIF
  #  define USI_START_VECTOR    USI_START_vect
  #  define USI_OVERFLOW_VECTOR USI_OVERFLOW_vect
  #endif
  
  #define SET_USI_TO_SEND_ACK( ) \
  { \
    USIDR = 0; \
    DDR_USI |= ( 1 << PORT_USI_SDA ); \
    USISR = \
         ( 0 << USI_START_COND_INT ) | \
         ( 1 << USIOIF ) | ( 1 << USIPF ) | \
         ( 1 << USIDC )| \
         ( 0x0E << USICNT0 ); \
  }
  
  #define SET_USI_TO_READ_ACK( ) \
  { \
    DDR_USI &= ~( 1 << PORT_USI_SDA ); \
    USIDR = 0; \
    USISR = \
         ( 0 << USI_START_COND_INT ) | \
         ( 1 << USIOIF ) | \
         ( 1 << USIPF ) | \
         ( 1 << USIDC ) | \
         ( 0x0E << USICNT0 ); \
  }
  
  #define SET_USI_TO_TWI_START_CONDITION_MODE( ) \
  { \
    USICR = \
         ( 1 << USISIE ) | ( 0 << USIOIE ) | \
         ( 1 << USIWM1 ) | ( 0 << USIWM0 ) | \
         ( 1 << USICS1 ) | ( 0 << USICS0 ) | ( 0 << USICLK ) | \
         ( 0 << USITC ); \
    USISR = \
          ( 0 << USI_START_COND_INT ) | ( 1 << USIOIF ) | ( 1 << USIPF ) | \
          ( 1 << USIDC ) | ( 0x0 << USICNT0 ); \
  }
  
  #define SET_USI_TO_SEND_DATA( ) \
  { \
    DDR_USI |=  ( 1 << PORT_USI_SDA ); \
    USISR    =  \
         ( 0 << USI_START_COND_INT ) | ( 1 << USIOIF ) | ( 1 << USIPF ) | \
         ( 1 << USIDC) | \
         ( 0x0 << USICNT0 ); \
  }
  
  #define SET_USI_TO_READ_DATA( ) \
  { \
    DDR_USI &= ~( 1 << PORT_USI_SDA ); \
    USISR    = \
         ( 0 << USI_START_COND_INT ) | ( 1 << USIOIF ) | \
         ( 1 << USIPF ) | ( 1 << USIDC ) | \
         ( 0x0 << USICNT0 ); \
  }
  
  typedef enum
  {
    USI_SLAVE_CHECK_ADDRESS                = 0x00,
    USI_SLAVE_SEND_DATA                    = 0x01,
    USI_SLAVE_REQUEST_REPLY_FROM_SEND_DATA = 0x02,
    USI_SLAVE_CHECK_REPLY_FROM_SEND_DATA   = 0x03,
    USI_SLAVE_REQUEST_DATA                 = 0x04,
    USI_SLAVE_GET_DATA_AND_SEND_ACK        = 0x05
  } overflowState_t;
  
  static uint8_t                  slaveAddress;
  static volatile overflowState_t overflowState;
  static uint8_t          rxBuf[ TWI_RX_BUFFER_SIZE ];
  static volatile uint8_t rxHead;
  static volatile uint8_t rxTail;
  static uint8_t          txBuf[ TWI_TX_BUFFER_SIZE ];
  static volatile uint8_t txHead;
  static volatile uint8_t txTail;
  
  static void flushTwiBuffers(void)
  {
    rxTail = 0;
    rxHead = 0;
    txTail = 0;
    txHead = 0;
  }
  
  void usiTwiSlaveInit(uint8_t ownAddress)
  {
    flushTwiBuffers( );
    slaveAddress = ownAddress;
    DDR_USI |= ( 1 << PORT_USI_SCL ) | ( 1 << PORT_USI_SDA );
    PORT_USI |= ( 1 << PORT_USI_SCL );
    PORT_USI |= ( 1 << PORT_USI_SDA );
    DDR_USI &= ~( 1 << PORT_USI_SDA );
    USICR =
         ( 1 << USISIE ) |
         ( 0 << USIOIE ) |
         ( 1 << USIWM1 ) | ( 0 << USIWM0 ) |
         ( 1 << USICS1 ) | ( 0 << USICS0 ) | ( 0 << USICLK ) |
         ( 0 << USITC );
    USISR = ( 1 << USI_START_COND_INT ) | ( 1 << USIOIF ) | ( 1 << USIPF ) | ( 1 << USIDC );
  }
  
  void usiTwiTransmitByte(uint8_t data)
  {
    uint8_t tmphead;
    tmphead = ( txHead + 1 ) & TWI_TX_BUFFER_MASK;
    while ( tmphead == txTail );
    txBuf[ tmphead ] = data;
    txHead = tmphead;
  }
  
  uint8_t usiTwiReceiveByte(void)
  {
    while ( rxHead == rxTail );
    rxTail = ( rxTail + 1 ) & TWI_RX_BUFFER_MASK;
    return rxBuf[ rxTail ];
  }
  
  bool usiTwiDataInReceiveBuffer(void)
  {
    return rxHead != rxTail;
  }
  
  ISR( USI_START_VECTOR )
  {
    overflowState = USI_SLAVE_CHECK_ADDRESS;
    DDR_USI &= ~( 1 << PORT_USI_SDA );
  
    while (( PIN_USI & ( 1 << PIN_USI_SCL ) ) && !( ( PIN_USI & ( 1 << PIN_USI_SDA ) ) )) { ; }
  
    if ( !( PIN_USI & ( 1 << PIN_USI_SDA ) ) )
    {
      USICR =
           ( 1 << USISIE ) |
           ( 1 << USIOIE ) |
           ( 1 << USIWM1 ) | ( 1 << USIWM0 ) |
           ( 1 << USICS1 ) | ( 0 << USICS0 ) | ( 0 << USICLK ) |
           ( 0 << USITC );
    }
    else
    {
      USICR =
           ( 1 << USISIE ) |
           ( 0 << USIOIE ) |
           ( 1 << USIWM1 ) | ( 0 << USIWM0 ) |
           ( 1 << USICS1 ) | ( 0 << USICS0 ) | ( 0 << USICLK ) |
           ( 0 << USITC );
    }
  
    USISR =
         ( 1 << USI_START_COND_INT ) | ( 1 << USIOIF ) |
         ( 1 << USIPF ) |( 1 << USIDC ) |
         ( 0x0 << USICNT0);
  }
  
  ISR( USI_OVERFLOW_VECTOR )
  {
    switch ( overflowState )
    {
      case USI_SLAVE_CHECK_ADDRESS:
        if ( ( USIDR == 0 ) || ( ( USIDR >> 1 ) == slaveAddress) )
        {
            if ( USIDR & 0x01 )
          {
            overflowState = USI_SLAVE_SEND_DATA;
          }
          else
          {
            overflowState = USI_SLAVE_REQUEST_DATA;
          }
          SET_USI_TO_SEND_ACK( );
        }
        else
        {
          SET_USI_TO_TWI_START_CONDITION_MODE( );
        }
        break;
  
      case USI_SLAVE_CHECK_REPLY_FROM_SEND_DATA:
        if ( USIDR )
        {
          SET_USI_TO_TWI_START_CONDITION_MODE( );
          return;
        }
  
      case USI_SLAVE_SEND_DATA:
        if ( txHead != txTail )
        {
          txTail = ( txTail + 1 ) & TWI_TX_BUFFER_MASK;
          USIDR = txBuf[ txTail ];
        }
        else
        {
          SET_USI_TO_TWI_START_CONDITION_MODE( );
          return;
        }
        overflowState = USI_SLAVE_REQUEST_REPLY_FROM_SEND_DATA;
        SET_USI_TO_SEND_DATA( );
        break;
  
      case USI_SLAVE_REQUEST_REPLY_FROM_SEND_DATA:
        overflowState = USI_SLAVE_CHECK_REPLY_FROM_SEND_DATA;
        SET_USI_TO_READ_ACK( );
        break;
  
      case USI_SLAVE_REQUEST_DATA:
        overflowState = USI_SLAVE_GET_DATA_AND_SEND_ACK;
        SET_USI_TO_READ_DATA( );
        break;
  
      case USI_SLAVE_GET_DATA_AND_SEND_ACK:
        rxHead = ( rxHead + 1 ) & TWI_RX_BUFFER_MASK;
        rxBuf[ rxHead ] = USIDR;
        overflowState = USI_SLAVE_REQUEST_DATA;
        SET_USI_TO_SEND_ACK( );
        break;
  
    }
  }
#endif
