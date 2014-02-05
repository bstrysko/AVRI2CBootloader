#include <iostream>

#include <IntelHexParser/IntelHexFile.h>
#include <IntelHexParser/Program.h>

#include <AVRI2CBootloader/I2CDeviceBootable.h>

#include <sstream>
#include <algorithm>

using namespace std;

void usage(string name);
uint8_t stringToUInt8(char* s);

int main(int argc, char* argv[])
{
	if(argc < 4)
	{
		usage(argv[0]);
		return 1;
	}

	string filename(argv[1]);
	uint8_t i2cBusNumber = stringToUInt8(argv[2]);
	uint8_t i2cDeviceAddress = stringToUInt8(argv[3]);

	IntelHexFile file(filename);
	Program program = file.getProgram();

	I2CBus bus(i2cBusNumber);
	I2CDeviceBootable device(&bus, i2cDeviceAddress);

	cout << "BootLoader Version #: " << device.getVersionNumber() << endl;
	cout << "Page Size: " << (size_t)device.getPageSize() << endl;

	if(!device.inBootLoaderMode())
	{
		cout << "Device NOT in bootloader mode" << endl;
		cout << "Switching to bootloader mode..." << endl;
		device.enterBootLoaderMode();
	}
	else
	{
		cout << "Device in bootloader mode..." << endl;
	}

	cout << "Attempting to flash program..." << endl;
    device.flash(program);
	cout << "FLASH SUCCESSFULL" << endl;


	cout << "Testing new application..." << endl;
	device.enterApplicationMode();
	cout << "Switching back to bootloader..." << endl;
	device.enterBootLoaderMode();

	cout << "SUCCESS" << endl;

	return 0;
}

void usage(string name)
{
	cout << "usage: " << name << " [ .hex file] [ I2C Bus Number ] [I2C Device Address]" << endl;
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
