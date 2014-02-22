#include <I2CDeviceBootLoader.h>

I2CDeviceBootLoader::I2CDeviceBootLoader(I2CBus* bus, uint8_t address) : I2CDeviceBootable(bus, address)
{
	if(!inBootLoaderMode())
	{
		enterBootLoaderMode();
	}
}

void I2CDeviceBootLoader::checkInBootLoaderMode()
{
	if(!inBootLoaderMode())
	{
		throw ios_base::failure("Not in Boot Loader mode");
	}
}

uint8_t I2CDeviceBootLoader::getRegisterBootVersion()
{
	uint8_t version;
	i2cRead(
		I2C_DEVICE_BOOT_LOADER_REGISTER_VERSION,
		&version, 1
	);
	return version;
}

uint8_t I2CDeviceBootLoader::getRegisterPageSize()
{
	uint8_t pageSize;
	i2cRead(
		I2C_DEVICE_BOOT_LOADER_REGISTER_PAGE_SIZE,
		&pageSize, 1
	);
	return pageSize;
}

uint8_t I2CDeviceBootLoader::getMajorVersionNumber()
{
	uint8_t status = getRegisterBootVersion();

	return (status >> 4) & 0xF;
}

uint8_t I2CDeviceBootLoader::getMinorVersionNumber()
{
	uint8_t status = getRegisterBootVersion();

	return (status) & 0xF;
}

uint8_t I2CDeviceBootLoader::getPageSize()
{
	return getRegisterPageSize();
}
