#ifndef _AVR_I2C_BOOT_LOADER_I2C_DEVICE_BOOT_LOADER_H_
#define _AVR_I2C_BOOT_LOADER_I2C_DEVICE_BOOT_LOADER_H_

#include "I2CDeviceBootable.h"
#include "ApplicationProgram.h"

/*
 * This 1 byte register is R.
 * Read:
 *  [3:0] - minor bootloader version number
 *  [7:4] - major bootloader version number
 */
#define I2C_DEVICE_BOOT_LOADER_REGISTER_VERSION 0x2

/*
 * This 1 byte register is R.
 * Read:
 *  [6:0] - page size of bootloader
 */
#define I2C_DEVICE_BOOT_LOADER_REGISTER_PAGE_SIZE 0x3

class I2CDeviceBootLoader : public I2CDeviceBootable
{
  private:
    void checkInBootLoaderMode();
    uint8_t getRegisterBootVersion();
    uint8_t getRegisterPageSize();
  public:
    I2CDeviceBootLoader(I2CBus* bus, uint8_t address);
    
    uint8_t getMajorVersionNumber();
    uint8_t getMinorVersionNumber();

    uint8_t getPageSize();
    void flash(ApplicationProgram* program);
};

#endif
