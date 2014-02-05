#include <I2CSlave.h>

void i2cSlaveReply(uint8_t ack);
void i2cSlaveStop();

void i2CSlaveInit(uint8_t address, I2CSlaveOnReadFunction onReadFunction, I2CSlaveOnWriteFunction onWriteFunction, size_t bufferLength)
{
  I2CSlaveDevice.onReadFunction = onReadFunction;
  I2CSlaveDevice.onWriteFunction = onWriteFunction;

  I2CSlaveDevice.rxBuffer = (uint8_t*)malloc(bufferLength);
  I2CSlaveDevice.txBuffer = (uint8_t*)malloc(bufferLength);
  I2CSlaveDevice.bufferLength = bufferLength;
  I2CSlaveDevice.rxBufferIndex = 0;
  I2CSlaveDevice.txBufferIndex = 0;
  I2CSlaveDevice.txBufferLength = 0;
  
  DDRC = 0;
  PORTC = (1 << 4) | (1 << 5);

  TWAR = (address << 1);
  TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWEA);

  sei();
}

void i2cSlaveReply(uint8_t ack)
{
  uint8_t v = (1 << TWEN) | (1 << TWIE) | (1 << TWINT);
  v |= ((ack ? 1 : 0) << TWEA);
  TWCR = v;
}

void i2cSlaveStop()
{
  TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWEA) | (1 << TWINT) | (1 << TWSTO);

  while(TWCR & (1 << TWSTO))
  {
    continue;
  }
}

void i2cPostOnWrite()
{
  I2CSlaveDevice.rxBufferIndex = 0;
  TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWEA) | (1 << TWINT);
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
      I2CSlaveDevice.rxBufferIndex = 0;
      i2cSlaveReply(1);
      break;
    }
    case TW_SR_DATA_ACK:       // data received, returned ack
    case TW_SR_GCALL_DATA_ACK: // data received generally, returned ack
    {
      // if there is still room in the rx buffer
      if(I2CSlaveDevice.rxBufferIndex < I2CSlaveDevice.bufferLength)
      {
        // put byte in buffer and ack
        I2CSlaveDevice.rxBuffer[I2CSlaveDevice.rxBufferIndex++] = TWDR;
        i2cSlaveReply(1);
      }
      else
      {
        // otherwise nack
        i2cSlaveReply(0);
      }
      break;
    }
    case TW_SR_STOP: // stop or repeated start condition received
    {
       // sends ack and stops interface for clock stretching
      i2cSlaveStop();
      I2CSlaveDevice.onWriteFunction(
	I2CSlaveDevice.rxBuffer[0],
	I2CSlaveDevice.rxBuffer+1,
	I2CSlaveDevice.rxBufferIndex-1
      );

      i2cPostOnWrite(); 
      break;
    }
    case TW_SR_DATA_NACK:       // data received, returned nack
    case TW_SR_GCALL_DATA_NACK: // data received generally, returned nack
    {
      // nack back at master
      i2cSlaveReply(0);
      break;
    }

    // Slave Transmitter
    case TW_ST_SLA_ACK:          // addressed, returned ack
    case TW_ST_ARB_LOST_SLA_ACK: // arbitration lost, returned ack
    {
      I2CSlaveDevice.txBufferIndex = 0;
      I2CSlaveDevice.txBufferLength = I2CSlaveDevice.onReadFunction(
	I2CSlaveDevice.rxBuffer[0],
	I2CSlaveDevice.txBuffer
      );
    }
    case TW_ST_DATA_ACK: // byte sent, ack returned
      // copy data to output register
      TWDR = I2CSlaveDevice.txBuffer[I2CSlaveDevice.txBufferIndex++];
      // if there is more to send, ack, otherwise nack
      if(I2CSlaveDevice.txBufferIndex < I2CSlaveDevice.txBufferLength)
      {
        i2cSlaveReply(1);
      }
      else
      {
        i2cSlaveReply(0);
      }
      break;
    case TW_ST_DATA_NACK: // received nack, we are done 
    case TW_ST_LAST_DATA: // received ack, but we are done already!
      // ack future responses
      i2cSlaveReply(1);
      break;

    // All
    case TW_NO_INFO:   // no state information
      break;
    case TW_BUS_ERROR: // bus error, illegal stop/start
      i2cSlaveStop();
      break;
  }
}

