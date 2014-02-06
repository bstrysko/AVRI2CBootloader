#include <iostream>

#include <TestDevice.h>

#include <sstream>
#include <algorithm>

using namespace std;

void usage(string name);
uint8_t stringToUInt8(char* s);

int main(int argc, char* argv[])
{
	if(argc < 3)
	{
		usage(argv[0]);
		return 1;
	}

	uint8_t i2cBusNumber = stringToUInt8(argv[1]);
	uint8_t i2cDeviceAddress = stringToUInt8(argv[2]);

	I2CBus bus(i2cBusNumber);
	TestDevice device(&bus, i2cDeviceAddress);

	cout << "BootLoader Version #: " << device.getVersionNumber() << endl;
	cout << "Page Size: " << (size_t)device.getPageSize() << endl;

	if(!device.inApplicationMode())
	{
		cout << "Device NOT in application mode" << endl;
		cout << "Switching to application mode..." << endl;
		device.enterApplicationMode();
	}
	else
	{
		cout << "Device in application mode..." << endl;
	}

	device.writeRegisterA(10);
	device.writeRegisterB(30);

	cout << "Register A: " << (size_t)device.getRegisterA() << endl;
	cout << "Register B: " << (size_t)device.getRegisterB() << endl;
	
	return 0;
}

void usage(string name)
{
	cout << "usage: " << name << " [ I2C Bus Number ] [I2C Device Address]" << endl;
}

uint8_t stringToUInt8(char* s)
{
	uint8_t result;
	
	result = atoi(s);
	
	if(result == 0 && s[0] != '0')
	{
		stringstream o;
		o << s << " is not a number";		
		throw ios_base::failure(o.str());
	}

	return result;
}
