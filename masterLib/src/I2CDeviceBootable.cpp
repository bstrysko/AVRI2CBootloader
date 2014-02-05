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

uint8_t I2CDeviceBootable::getRegisterBootVersion()
{
	uint8_t version;
	i2cRead(
		I2C_DEVICE_BOOTABLE_REGISTER_VERSION,
		&version, 1
	);
	return version;
}

uint8_t I2CDeviceBootable::getRegisterPageSize()
{
	uint8_t pageSize;
	i2cRead(
		I2C_DEVICE_BOOTABLE_REGISTER_PAGE_SIZE,
		&pageSize, 1
	);
	return pageSize;
}

uint16_t I2CDeviceBootable::getRegisterPageNumber()
{
	uint16_t pageNumber;
	i2cRead(
		I2C_DEVICE_BOOTABLE_REGISTER_PROGRAM_PAGE_NUMBER,
		(uint8_t*)&pageNumber, 2
	);
	return pageNumber;
}

uint8_t I2CDeviceBootable::getRegisterPageData()
{
	uint8_t pageData;
	i2cRead(
		I2C_DEVICE_BOOTABLE_REGISTER_PROGRAM_PAGE_DATA,
		(uint8_t*)&pageData, 1
	);
	return pageData;
}

void I2CDeviceBootable::writeBootStatus(uint8_t status)
{
	i2cWrite(
		I2C_DEVICE_BOOTABLE_REGISTER_BOOT_STATUS,
		&status, 1
	);
}
 
void I2CDeviceBootable::writePageNumber(uint16_t pageNumber)
{
	i2cWrite(
		I2C_DEVICE_BOOTABLE_REGISTER_PROGRAM_PAGE_NUMBER,
		(uint8_t*)&pageNumber, 2
	);
}

void I2CDeviceBootable::writePageData(vector<uint8_t> pageData)
{
	i2cWrite(
		I2C_DEVICE_BOOTABLE_REGISTER_PROGRAM_PAGE_DATA,
		pageData.data(), pageData.size()
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

float I2CDeviceBootable::getVersionNumber()
{
	uint8_t status = getRegisterBootVersion();

	uint8_t hStatus = (status >> 4) & 0xF;
	uint8_t lStatus = status & 0xF;	

	float version = hStatus;
	version += (0.1 * lStatus);

	return version;
}

uint8_t I2CDeviceBootable::getPageSize()
{
	return getRegisterPageSize();
}

void I2CDeviceBootable::flash(Program program)
{
	uint8_t pageSize = getPageSize();
	vector<ProgramPage> pages = program.getPages(pageSize);

	for(size_t i = 0; i < pages.size(); i++)
	{
		ProgramPage page = pages[i];

		writePageNumber((uint16_t)i);

		size_t r = getRegisterPageNumber();
		if(r != i)
		{
			stringstream o;
			o << "Page Number " << r;
			o << " expecting " << i;
			throw ios_base::failure(o.str());
		}

		vector<uint8_t> pageData = page.getData();
		pageData.resize(pageSize, 0xFF);
		writePageData(pageData);

		r = getRegisterPageData();
		if(!r)
		{
			stringstream o;
			o << "Page write failed, Result: ";
			o << r;
			throw ios_base::failure(o.str());
		}
	}
}

