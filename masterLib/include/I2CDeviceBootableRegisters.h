#ifndef _AVR_I2C_BOOT_LOADER_I2C_DEVICE_BOOTABLE_REGISTERS_H_
#define _AVR_I2C_BOOT_LOADER_I2C_DEVICE_BOOTABLE_REGISTERS_H_

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
 * valid is REGISTER_BOOT_MAGIC is valid
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

/*
 * This 1 byte register is R.
 * Read:
 *  [3:0] - minor bootloader version number
 *  [7:4] - major bootloader version number
 */
#define I2C_DEVICE_BOOTABLE_REGISTER_VERSION 0x2

/*
 * This 1 byte register is R.
 * Read:
 *  [7:0] - page size of bootloader
 */
#define I2C_DEVICE_BOOTABLE_REGISTER_PAGE_SIZE 0x3

/*
 * This 2 byte register is R/W.
 * Read:
    [15:0] - current page number
 * Write:
    [15:0] - current page number
 */
#define I2C_DEVICE_BOOTABLE_REGISTER_PROGRAM_PAGE_NUMBER 0x4

/*
 * This register is R/W.
 * size PAGESIZE on W / size 1 on R.
 * Read:
 *  [7:0] - 1 if last write to this register was successfully written
    to flash, 0 otherwise
 * Write:
    Bytes [PAGE_SIZE-1:0] - data to be written to current
    page as pointed to by the register PROGRAM_PAGE_NUMBER
 */
#define I2C_DEVICE_BOOTABLE_REGISTER_PROGRAM_PAGE_DATA 0x5


#define I2C_DEVICE_BOOTABLE_BOOT_MAGIC 0xAE
#define I2C_DEVICE_BOOTABLE_STATUS_BOOT_LOADER 0x1
#define I2C_DEVICE_BOOTABLE_STATUS_APPLICATION 0x2

#endif
