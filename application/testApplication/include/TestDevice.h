#ifndef _AVR_I2C_BOOT_LOADER_TEST_DEVICE_H_
#define _AVR_I2C_BOOT_LOADER_TEST_DEVICE_H_

#include <AVRI2CBootloader/I2CDeviceBootable.h>

#include "TestDeviceRegisters.h"

class TestDevice : public I2CDeviceBootable
{
  public:
    TestDevice(I2CBus* bus, uint8_t address);

    uint8_t getRegisterA();
    uint8_t getRegisterB();
  
    void writeRegisterA(uint8_t v);
    void writeRegisterB(uint8_t v);
};

#endif
