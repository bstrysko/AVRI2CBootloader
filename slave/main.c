#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/twi.h>
#include <avr/boot.h>

#include "../master/include/I2CDeviceBootableRegisters.h"

#define I2C_BUFFER_LENGTH SPM_PAGESIZE

uint8_t i2c_rxBufferIndex;
uint8_t i2c_rxBuffer[I2C_BUFFER_LENGTH];
uint8_t i2c_txBufferIndex;
uint8_t i2c_txBufferLength;
uint8_t i2c_txBuffer[I2C_BUFFER_LENGTH];

void i2c_reply(uint8_t ack);
void i2c_stop();

void onWrite(uint8_t reg, uint8_t* buffer, uint8_t bufferSize);
uint8_t onRead(uint8_t reg, uint8_t* buffer);

#define TWI_FREQ 100000L

int main()
{
  i2c_rxBufferIndex = 0;
  i2c_txBufferIndex = 0;

//  DDRC &= ~((1 << 4) | (1 << 5));
  PORTC |= (1 << 4) | (1 << 5);

  TWAR = (0x40 << 1);
  TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWEA);

  sei();

  for(;;)
  {

  }

  while(1);

  return 0;
}

SIGNAL(TWI_vect)
{
  switch(TW_STATUS)
  {
    // Slave Receiver
    case TW_SR_SLA_ACK:   // addressed, returned ack
    case TW_SR_GCALL_ACK: // addressed generally, returned ack
    case TW_SR_ARB_LOST_SLA_ACK:   // lost arbitration, returned ack
    case TW_SR_ARB_LOST_GCALL_ACK: // lost arbitration, returned ack
    {
      // indicate that rx buffer can be overwritten and ack
      i2c_rxBufferIndex = 0;
      i2c_reply(1);
      break;
    }
    case TW_SR_DATA_ACK:       // data received, returned ack
    case TW_SR_GCALL_DATA_ACK: // data received generally, returned ack
    {
      // if there is still room in the rx buffer
      if(i2c_rxBufferIndex < I2C_BUFFER_LENGTH)
      {
        // put byte in buffer and ack
        i2c_rxBuffer[i2c_rxBufferIndex++] = TWDR;
        i2c_reply(1);
      }
      else
      {
        // otherwise nack
        i2c_reply(0);
      }
      break;
    }
    case TW_SR_STOP: // stop or repeated start condition received
    {
       // sends ack and stops interface for clock stretching
      i2c_stop();
      onWrite(i2c_rxBuffer[0], i2c_rxBuffer+1, i2c_rxBufferIndex-1);
      // since we submit rx buffer to "wire" library, we can reset it
      i2c_rxBufferIndex = 0;
      // ack future responses and leave slave receiver state
      TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWEA) | (1 << TWINT);
      break;
    }
    case TW_SR_DATA_NACK:       // data received, returned nack
    case TW_SR_GCALL_DATA_NACK: // data received generally, returned nack
    {
      // nack back at master
      i2c_reply(0);
      break;
    }

    // Slave Transmitter
    case TW_ST_SLA_ACK:          // addressed, returned ack
    case TW_ST_ARB_LOST_SLA_ACK: // arbitration lost, returned ack
    {
      i2c_txBufferIndex = 0;
      i2c_txBufferLength = onRead(i2c_rxBuffer[0], i2c_txBuffer);
    }
    case TW_ST_DATA_ACK: // byte sent, ack returned
      // copy data to output register
      TWDR = i2c_txBuffer[i2c_txBufferIndex++];
      // if there is more to send, ack, otherwise nack
      if(i2c_txBufferIndex < i2c_txBufferLength)
      {
        i2c_reply(1);
      }
      else
      {
        i2c_reply(0);
      }
      break;
    case TW_ST_DATA_NACK: // received nack, we are done 
    case TW_ST_LAST_DATA: // received ack, but we are done already!
      // ack future responses
      i2c_reply(1);
      break;

    // All
    case TW_NO_INFO:   // no state information
      break;
    case TW_BUS_ERROR: // bus error, illegal stop/start
      i2c_stop();
      break;
  }
}

void onWrite(uint8_t reg, uint8_t* buffer, uint8_t bufferSize)
{
  
}

uint8_t onRead(uint8_t reg, uint8_t* buffer)
{
  switch(reg)
  {
    case I2C_DEVICE_BOOTABLE_REGISTER_BOOT_MAGIC:
    {
      buffer[0] = 0xAE;
      return 1;
    }
    case I2C_DEVICE_BOOTABLE_REGISTER_BOOT_STATUS:
    {
      buffer[0] = 0x2;
      return 1;
    }
    case I2C_DEVICE_BOOTABLE_REGISTER_VERSION:
    {
      buffer[0] = 0x01;
      return 1;
    }
    case I2C_DEVICE_BOOTABLE_REGISTER_PAGE_SIZE:
    {
      buffer[0] = SPM_PAGESIZE;
      return 1;
    }
    case I2C_DEVICE_BOOTABLE_REGISTER_PROGRAM_PAGE_NUMBER:
    {
      buffer[0] = 0x0;
      buffer[1] = 0x0;
      return 2;
    }
    default:
    {
      return 0;
    }
  }
}

void i2c_reply(uint8_t ack)
{
  uint8_t v = (1 << TWEN) | (1 << TWIE) | (1 << TWINT);

  v |= ((ack?1:0) << TWEA);

  TWCR = v;
}

void i2c_stop()
{
  TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWEA) | (1 << TWINT) | (1 << TWSTO);

  // wait for stop condition to be exectued on bus
  // TWINT is not set after a stop condition!
  while(TWCR & (1 << TWSTO)){
    continue;
  }
}
