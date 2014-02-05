#ifndef _AVR_I2C_BOOTLOADER_AVR_SHARED_H_
#define _AVR_I2C_BOOTLOADER_AVR_SHARED_H_

#include <inttypes.h>
#include <stddef.h>

typedef size_t (*I2CSlaveOnReadFunction)(uint8_t, uint8_t*);
typedef void (*I2CSlaveOnWriteFunction)(uint8_t, uint8_t*, size_t);

typedef struct {
  I2CSlaveOnReadFunction onReadFunction;
  I2CSlaveOnWriteFunction onWriteFunction;
} I2CCallbacks_t;

#define I2CCallbacks (*(I2CCallbacks_t*)0x130)

#endif
