#ifndef _AVR_I2C_BOOT_LOADER_I2C_DEVICE_BOOTABLE_H_
#define _AVR_I2C_BOOT_LOADER_I2C_DEVICE_BOOTABLE_H_

#include "I2CDeviceBootableRegisters.h"

#include <I2CMaster/I2CBus.h>
#include <I2CMaster/I2CDevice.h>
#include <IntelHexParser/Program.h>

class I2CDeviceBootable : public I2CDevice
{
  private:
    void checkRegisterMagic();

    uint8_t getRegisterBootStatus();
    uint8_t getRegisterBootVersion();
    uint8_t getRegisterPageSize();
    uint16_t getRegisterPageNumber();
    uint8_t getRegisterPageData();

    void writeBootStatus(uint8_t status);
    void writePageNumber(uint16_t pageNumber);
    void writePageData(vector<uint8_t> pageData);
  public:
    I2CDeviceBootable(I2CBus* bus, uint8_t address);
    
    bool inBootLoaderMode();
    bool inApplicationMode();

    float getVersionNumber();
    uint8_t getPageSize();

    void enterBootLoaderMode();
    void enterApplicationMode();

    void flash(Program program);
};

#endif
