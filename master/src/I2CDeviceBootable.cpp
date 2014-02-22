#include <I2CDeviceBootable.h>

I2CDeviceBootable::I2CDeviceBootable(I2CBus* bus, uint8_t address) : I2CDevice(bus, address)
{
}

void I2CDeviceBootable::checkRegisterMagic()
{
	uint8_t magic;
	i2cRead(
		I2C_DEVICE_BOOTABLE_REGISTER_BOOT_MAGIC,
		&magic, 1
	);

	if(magic != I2C_DEVICE_BOOTABLE_BOOT_MAGIC)
	{
		stringstream o;
		o << "Incorrect BOOTLOADER I2C DEVICE MAGIC NUMBER ";
		o << (int)magic;
		o << " ( expecting ";
		o << I2C_DEVICE_BOOTABLE_BOOT_MAGIC;
		o << " )";
		throw ios_base::failure(o.str());
	}
}

uint8_t I2CDeviceBootable::getRegisterBootStatus()
{
	uint8_t status;
	i2cRead(
		I2C_DEVICE_BOOTABLE_REGISTER_BOOT_STATUS,
		&status, 1
	);
	return status;
}

void I2CDeviceBootable::writeBootStatus(uint8_t status)
{
	i2cWrite(
		I2C_DEVICE_BOOTABLE_REGISTER_BOOT_STATUS,
		&status, 1
	);
	
}
 
bool I2CDeviceBootable::inBootLoaderMode()
{
	checkRegisterMagic();

	return (getRegisterBootStatus() ==
		I2C_DEVICE_BOOTABLE_STATUS_BOOT_LOADER);
}

bool I2CDeviceBootable::inApplicationMode()
{
	checkRegisterMagic();

	return (getRegisterBootStatus() ==
		I2C_DEVICE_BOOTABLE_STATUS_APPLICATION);
}

void I2CDeviceBootable::enterBootLoaderMode()
{
	checkRegisterMagic();

	writeBootStatus(
		I2C_DEVICE_BOOTABLE_STATUS_BOOT_LOADER
	);

	usleep(5);

	if(!inBootLoaderMode())
	{
		throw ios_base::failure(
			"Could not enter Boot Loader Mode"
		);
	}
}

void I2CDeviceBootable::enterApplicationMode()
{
	checkRegisterMagic();

	writeBootStatus(
		I2C_DEVICE_BOOTABLE_STATUS_APPLICATION
	);

	usleep(5);

	if(!inApplicationMode())
	{
		throw ios_base::failure(
			"Could not enter Application Mode"
		);
	}
}
