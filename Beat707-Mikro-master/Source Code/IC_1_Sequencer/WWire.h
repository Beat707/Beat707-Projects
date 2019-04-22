/*
 * Beat707 Mikro - created by William (Guilherme) Kalfelz (c) 2016 - www.Beat707.com - www.Wusik.com - www.William-k.com
 * 
 */

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include <avr/io.h>
#include <avr/interrupt.h>
#include <compat/twi.h>

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define SLAVE_ID 37
#define I2C_SEND_DATA_BUFFER_SIZE 0x20
#define I2C_RECEIVE_DATA_BUFFER_SIZE 0x20
#define WWIRE_SPEED 100L
#define TWCR_CMD_MASK 0x0F
#define TWSR_STATUS_MASK 0xF8
#define I2C_OK 0x00
#define I2C_ERROR_NODEV 0x01

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef enum { I2C_IDLE = 0, I2C_BUSY = 1, I2C_MASTER_TX = 2, I2C_MASTER_RX = 3, I2C_SLAVE_TX = 4, I2C_SLAVE_RX = 5 } eI2cStateType;
static volatile eI2cStateType I2cState;
static byte I2cDeviceAddrRW;
static byte I2cSendData[I2C_SEND_DATA_BUFFER_SIZE];
static byte I2cSendDataIndex;
static byte I2cSendDataLength;
static byte I2cReceiveData[I2C_RECEIVE_DATA_BUFFER_SIZE];
static byte I2cReceiveDataIndex;
static byte I2cReceiveDataLength;
static void (*i2cSlaveReceive)(byte receiveDataLength, byte* recieveData);
static byte (*i2cSlaveTransmit)(byte transmitDataLengthMax, byte* transmitData);

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif
#define outb(a,b) a = b
#define inb(port) (port)

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void i2cInit(void)
{
  sbi(PORTC, 4);
  sbi(PORTC, 5);
  i2cSlaveReceive = 0;
  i2cSlaveTransmit = 0;
  cbi(TWSR, TWPS0);
  cbi(TWSR, TWPS1);
  TWBR = ((F_CPU / WWIRE_SPEED) - 16) / 2;
  I2cState = I2C_IDLE;
  TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA);
  sei();
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void i2cSetLocalDeviceAddr(byte deviceAddr, byte genCallEn)
{
  outb(TWAR, deviceAddr << 1);
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void i2cSetSlaveReceiveHandler(void (*i2cSlaveRx_func)(byte receiveDataLength, byte* recieveData))
{
  i2cSlaveReceive = i2cSlaveRx_func;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void i2cSetSlaveTransmitHandler(byte (*i2cSlaveTx_func)(byte transmitDataLengthMax, byte* transmitData))
{
  i2cSlaveTransmit = i2cSlaveTx_func;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
inline void i2cSendStart(void)
{
  outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|_BV(TWINT)|_BV(TWSTA));
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
inline void i2cSendStop(void)
{
  outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|_BV(TWINT)|_BV(TWEA)|_BV(TWSTO));
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
inline void i2cWaitForComplete(void)
{
  while( !(inb(TWCR) & _BV(TWINT)) );
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
inline void i2cSendByte(byte data)
{
  outb(TWDR, data);
  outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|_BV(TWINT));
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
inline void i2cReceiveByte(byte ackFlag)
{
  if( ackFlag ) outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|_BV(TWINT)|_BV(TWEA));
    else outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|_BV(TWINT));
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
inline byte i2cGetReceivedByte(void)
{
  return( inb(TWDR) );
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
inline byte i2cGetStatus(void)
{
  return( inb(TWSR) );
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void i2cMasterSend(byte deviceAddr, byte length, byte* data)
{
  byte i;
  while(I2cState);
  I2cState = I2C_MASTER_TX;
  I2cDeviceAddrRW = (deviceAddr & 0xFE);  // RW cleared: write operation
  for(i=0; i<length; i++)
    I2cSendData[i] = *data++;
  I2cSendDataIndex = 0;
  I2cSendDataLength = length;
  i2cSendStart();
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void i2cMasterReceive(byte deviceAddr, byte length, byte* data)
{
  byte i;
  while(I2cState);
  I2cState = I2C_MASTER_RX;
  I2cDeviceAddrRW = (deviceAddr|0x01);  // RW set: read operation
  I2cReceiveDataIndex = 0;
  I2cReceiveDataLength = length;
  i2cSendStart();
  while(I2cState);
  for(i=0; i<length; i++)
    *data++ = I2cReceiveData[i];
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
byte i2cMasterSendNI(byte deviceAddr, byte length, byte* data)
{
  byte retval = I2C_OK;
  cbi(TWCR, TWIE);
  i2cSendStart();
  i2cWaitForComplete();
  i2cSendByte( deviceAddr & 0xFE );
  i2cWaitForComplete();
  if( inb(TWSR) == TW_MT_SLA_ACK)
  {
    while(length)
    {
      i2cSendByte( *data++ );
      i2cWaitForComplete();
      length--;
    }
  }
  else
  {
    retval = I2C_ERROR_NODEV;
  }
  i2cSendStop();
  while( !(inb(TWCR) & _BV(TWSTO)) );
  sbi(TWCR, TWIE);
  //
  return retval;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
byte i2cMasterReceiveNI(byte deviceAddr, byte length, byte *data)
{
  byte retval = I2C_OK;
  cbi(TWCR, TWIE);
  i2cSendStart();
  i2cWaitForComplete();
  i2cSendByte( deviceAddr | 0x01 );
  i2cWaitForComplete();
  if( inb(TWSR) == TW_MR_SLA_ACK)
  {
    while(length > 1)
    {
      i2cReceiveByte(true);
      i2cWaitForComplete();
      *data++ = i2cGetReceivedByte();
      length--;
    }
    i2cReceiveByte(false);
    i2cWaitForComplete();
    *data++ = i2cGetReceivedByte();
  }
  else
  {
    retval = I2C_ERROR_NODEV;
  }
  i2cSendStop();
  sbi(TWCR, TWIE);
  //
  return retval;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SIGNAL(TWI_vect)
{
  byte xstatus = inb(TWSR) & TWSR_STATUS_MASK;
  //
  switch(xstatus)
  {
    // Master General
    case TW_START:            // 0x08: Sent start condition
    case TW_REP_START:          // 0x10: Sent repeated start condition
      #ifdef I2C_DEBUG
      rprintfInit(uart1AddToTxBuffer);
      rprintf("I2C: M->START\r\n");
      rprintfInit(uart1SendByte);
      #endif
      // send device address
      i2cSendByte(I2cDeviceAddrRW);
      break;
    
    // Master Transmitter & Receiver status codes
    case TW_MT_SLA_ACK:         // 0x18: Slave address acknowledged
    case TW_MT_DATA_ACK:        // 0x28: Data acknowledged
      #ifdef I2C_DEBUG
      rprintfInit(uart1AddToTxBuffer);
      rprintf("I2C: MT->SLA_ACK or DATA_ACK\r\n");
      rprintfInit(uart1SendByte);
      #endif
      if(I2cSendDataIndex < I2cSendDataLength)
      {
        // send data
        i2cSendByte( I2cSendData[I2cSendDataIndex++] );
      }
      else
      {
        // transmit stop condition, enable SLA ACK
        i2cSendStop();
        // set state
        I2cState = I2C_IDLE;
      }
      break;
    case TW_MR_DATA_NACK:       // 0x58: Data received, NACK reply issued
      #ifdef I2C_DEBUG
      rprintfInit(uart1AddToTxBuffer);
      rprintf("I2C: MR->DATA_NACK\r\n");
      rprintfInit(uart1SendByte);
      #endif
      // store final received data byte
      I2cReceiveData[I2cReceiveDataIndex++] = inb(TWDR);
      // continue to transmit STOP condition
    case TW_MR_SLA_NACK:        // 0x48: Slave address not acknowledged
    case TW_MT_SLA_NACK:        // 0x20: Slave address not acknowledged
    case TW_MT_DATA_NACK:       // 0x30: Data not acknowledged
      #ifdef I2C_DEBUG
      rprintfInit(uart1AddToTxBuffer);
      rprintf("I2C: MTR->SLA_NACK or MT->DATA_NACK\r\n");
      rprintfInit(uart1SendByte);
      #endif
      // transmit stop condition, enable SLA ACK
      i2cSendStop();
      // set state
      I2cState = I2C_IDLE;
      break;
    case TW_MT_ARB_LOST:        // 0x38: Bus arbitration lost
    //case TW_MR_ARB_LOST:        // 0x38: Bus arbitration lost
      #ifdef I2C_DEBUG
      rprintfInit(uart1AddToTxBuffer);
      rprintf("I2C: MT->ARB_LOST\r\n");
      rprintfInit(uart1SendByte);
      #endif
      // release bus
      outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|_BV(TWINT));
      // set state
      I2cState = I2C_IDLE;
      // release bus and transmit start when bus is free
      //outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|BV(TWINT)|BV(TWSTA));
      break;
    case TW_MR_DATA_ACK:        // 0x50: Data acknowledged
      #ifdef I2C_DEBUG
      rprintfInit(uart1AddToTxBuffer);
      rprintf("I2C: MR->DATA_ACK\r\n");
      rprintfInit(uart1SendByte);
      #endif
      // store received data byte
      I2cReceiveData[I2cReceiveDataIndex++] = inb(TWDR);
      // fall-through to see if more bytes will be received
    case TW_MR_SLA_ACK:         // 0x40: Slave address acknowledged
      #ifdef I2C_DEBUG
      rprintfInit(uart1AddToTxBuffer);
      rprintf("I2C: MR->SLA_ACK\r\n");
      rprintfInit(uart1SendByte);
      #endif
      if(I2cReceiveDataIndex < (I2cReceiveDataLength-1))
        // data byte will be received, reply with ACK (more bytes in transfer)
        i2cReceiveByte(true);
      else
        // data byte will be received, reply with NACK (final byte in transfer)
        i2cReceiveByte(false);
      break;
  
    // Slave Receiver status codes
    case TW_SR_SLA_ACK:         // 0x60: own SLA+W has been received, ACK has been returned
    case TW_SR_ARB_LOST_SLA_ACK:    // 0x68: own SLA+W has been received, ACK has been returned
    case TW_SR_GCALL_ACK:       // 0x70:     GCA+W has been received, ACK has been returned
    case TW_SR_ARB_LOST_GCALL_ACK:    // 0x78:     GCA+W has been received, ACK has been returned
      #ifdef I2C_DEBUG
      rprintfInit(uart1AddToTxBuffer);
      rprintf("I2C: SR->SLA_ACK\r\n");
      rprintfInit(uart1SendByte);
      #endif
      // we are being addressed as slave for writing (data will be received from master)
      // set state
      I2cState = I2C_SLAVE_RX;
      // prepare buffer
      I2cReceiveDataIndex = 0;
      // receive data byte and return ACK
      outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|_BV(TWINT)|_BV(TWEA));
      break;
    case TW_SR_DATA_ACK:        // 0x80: data byte has been received, ACK has been returned
    case TW_SR_GCALL_DATA_ACK:      // 0x90: data byte has been received, ACK has been returned
      #ifdef I2C_DEBUG
      rprintfInit(uart1AddToTxBuffer);
      rprintf("I2C: SR->DATA_ACK\r\n");
      rprintfInit(uart1SendByte);
      #endif
      // get previously received data byte
      I2cReceiveData[I2cReceiveDataIndex++] = inb(TWDR);
      // check receive buffer status
      if(I2cReceiveDataIndex < I2C_RECEIVE_DATA_BUFFER_SIZE)
      {
        // receive data byte and return ACK
        i2cReceiveByte(true);
        //outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|BV(TWINT)|BV(TWEA));
      }
      else
      {
        // receive data byte and return NACK
        i2cReceiveByte(false);
        //outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|BV(TWINT));
      }
      break;
    case TW_SR_DATA_NACK:       // 0x88: data byte has been received, NACK has been returned
    case TW_SR_GCALL_DATA_NACK:     // 0x98: data byte has been received, NACK has been returned
      #ifdef I2C_DEBUG
      rprintfInit(uart1AddToTxBuffer);
      rprintf("I2C: SR->DATA_NACK\r\n");
      rprintfInit(uart1SendByte);
      #endif
      // receive data byte and return NACK
      i2cReceiveByte(false);
      //outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|BV(TWINT));
      break;
    case TW_SR_STOP:          // 0xA0: STOP or REPEATED START has been received while addressed as slave
      #ifdef I2C_DEBUG
      rprintfInit(uart1AddToTxBuffer);
      rprintf("I2C: SR->SR_STOP\r\n");
      rprintfInit(uart1SendByte);
      #endif
      // switch to SR mode with SLA ACK
      outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|_BV(TWINT)|_BV(TWEA));
      // i2c receive is complete, call i2cSlaveReceive
      if(i2cSlaveReceive) i2cSlaveReceive(I2cReceiveDataIndex, I2cReceiveData);
      // set state
      I2cState = I2C_IDLE;
      break;
  
    // Slave Transmitter
    case TW_ST_SLA_ACK:         // 0xA8: own SLA+R has been received, ACK has been returned
    case TW_ST_ARB_LOST_SLA_ACK:    // 0xB0:     GCA+R has been received, ACK has been returned
      #ifdef I2C_DEBUG
      rprintfInit(uart1AddToTxBuffer);
      rprintf("I2C: ST->SLA_ACK\r\n");
      rprintfInit(uart1SendByte);
      #endif
      // we are being addressed as slave for reading (data must be transmitted back to master)
      // set state
      I2cState = I2C_SLAVE_TX;
      // request data from application
      if(i2cSlaveTransmit) I2cSendDataLength = i2cSlaveTransmit(I2C_SEND_DATA_BUFFER_SIZE, I2cSendData);
      // reset data index
      I2cSendDataIndex = 0;
      // fall-through to transmit first data byte
    case TW_ST_DATA_ACK:        // 0xB8: data byte has been transmitted, ACK has been received
      #ifdef I2C_DEBUG
      rprintfInit(uart1AddToTxBuffer);
      rprintf("I2C: ST->DATA_ACK\r\n");
      rprintfInit(uart1SendByte);
      #endif
      // transmit data byte
      outb(TWDR, I2cSendData[I2cSendDataIndex++]);
      if(I2cSendDataIndex < I2cSendDataLength)
        // expect ACK to data byte
        outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|_BV(TWINT)|_BV(TWEA));
      else
        // expect NACK to data byte
        outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|_BV(TWINT));
      break;
    case TW_ST_DATA_NACK:       // 0xC0: data byte has been transmitted, NACK has been received
    case TW_ST_LAST_DATA:       // 0xC8:
      #ifdef I2C_DEBUG
      rprintfInit(uart1AddToTxBuffer);
      rprintf("I2C: ST->DATA_NACK or LAST_DATA\r\n");
      rprintfInit(uart1SendByte);
      #endif
      // all done
      // switch to open slave
      outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|_BV(TWINT)|_BV(TWEA));
      // set state
      I2cState = I2C_IDLE;
      break;
  
    // Misc
    case TW_NO_INFO:          // 0xF8: No relevant state information
      // do nothing
      #ifdef I2C_DEBUG
      rprintfInit(uart1AddToTxBuffer);
      rprintf("I2C: NO_INFO\r\n");
      rprintfInit(uart1SendByte);
      #endif
      break;
    case TW_BUS_ERROR:          // 0x00: Bus error due to illegal start or stop condition
      #ifdef I2C_DEBUG
      rprintfInit(uart1AddToTxBuffer);
      rprintf("I2C: BUS_ERROR\r\n");
      rprintfInit(uart1SendByte);
      #endif
      // reset internal hardware and release bus
      outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|_BV(TWINT)|_BV(TWSTO)|_BV(TWEA));
      // set state
      I2cState = I2C_IDLE;
      break;
  }
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
eI2cStateType i2cGetState(void)
{
  return I2cState;
}
