#ifndef _AVR_I2C_BOOT_LOADER_AVR_I2C_SLAVE_H_
#define _AVR_I2C_BOOT_LOADER_AVR_I2C_SLAVE_H_

#include <inttypes.h>
#include <stddef.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/twi.h>

#include "../../slaveLib/include/shared.h"

#define AVR_I2C_SLAVE_FREQ 100000L

#define I2CSlaveDevice (*(I2CSlave_t*)0x100) 

typedef struct {
  I2CSlaveOnReadFunction onReadFunction;
  I2CSlaveOnWriteFunction onWriteFunction;

  uint8_t* rxBuffer;
  uint8_t* txBuffer;
  size_t bufferLength; 
  size_t rxBufferIndex;
  size_t txBufferIndex;
  size_t txBufferLength;
} I2CSlave_t;

void i2CSlaveInit
(
  uint8_t address,
  I2CSlaveOnReadFunction onReadFunction,
  I2CSlaveOnWriteFunction onWriteFunction,
  size_t bufferLength
);

void i2cPostOnWrite();

#endif
