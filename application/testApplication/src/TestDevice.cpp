#include <TestDevice.h>

TestDevice::TestDevice(I2CBus* bus, uint8_t address) : I2CDeviceBootable(bus, address)
{

}

uint8_t TestDevice::getRegisterA()
{
	uint8_t v;
	i2cRead(
		REGISTER_A,
		&v, 1
	);
	return v;
}

uint8_t TestDevice::getRegisterB()
{
	uint8_t v;
	i2cRead(
		REGISTER_B,
		&v, 1
	);
	return v;
}

void TestDevice::writeRegisterA(uint8_t v)
{
	i2cWrite
	(
		REGISTER_A,
		&v, 1
	);
}

void TestDevice::writeRegisterB(uint8_t v)
{
	i2cWrite
	(
		REGISTER_B,
		&v, 1
	);
}
