#ifndef _AVR_I2C_BOOT_LOADER_I2C_DEVICE_BOOTABLE_H_
#define _AVR_I2C_BOOT_LOADER_I2C_DEVICE_BOOTABLE_H_

#include <I2CMaster/I2CBus.h>
#include <I2CMaster/I2CDevice.h>

/*
 * This 1 byte register is R
 * Read:
 *  [7:0] - should always be 0xAE if the device
 *  adheres to the communication protocol defined here.
 *  If the register does not return the value that satisfies,
 *  the invariant, the device is of unknown type. 
 */
#define I2C_DEVICE_BOOTABLE_REGISTER_BOOT_MAGIC 0x0

/*
 * This 1 byte register is R/W.  Can only assume
 * valid is REGISTER_BOOT_MAGIC is 0xAE
 * Read:
 *  [1:0] - 1 if the bootloader is in bootloader mode,
 *  2 if the bootloader is in application mode, else
 *  undefined behavior
 *  [7:2] - reserved
 * Write:
 *  [1:0] - 1 place the device in bootloader mode,
 *  2 place the device in application mode, else
 *  undefined behavior
 *  [7:2] - reserved, write 0
 */
#define I2C_DEVICE_BOOTABLE_REGISTER_BOOT_STATUS 0x1

#define I2C_DEVICE_BOOTABLE_BOOT_MAGIC 0xAE

#define I2C_DEVICE_BOOTABLE_STATUS_BOOT_LOADER 0x1
#define I2C_DEVICE_BOOTABLE_STATUS_APPLICATION 0x2

class I2CDeviceBootable : public I2CDevice
{
  private:
    void checkRegisterMagic();
    uint8_t getRegisterBootStatus();

    void writeBootStatus(uint8_t status);
  protected:
    I2CDeviceBootable(I2CBus* bus, uint8_t address);
  public:
    bool inBootLoaderMode();
    bool inApplicationMode();

    void enterBootLoaderMode();
    void enterApplicationMode();
};

#endif
