#include <iostream>

#include <IntelHexParser/Program.h>

#include <I2CDeviceBootable.h>

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

	I2CBus bus(1);
	I2CDeviceBootable d(&bus, 0x40);

//	cout << "In BootLoader Mode: " << d.inBootLoaderMode() << endl;
//	cout << "In Application Mode: " << d.inApplicationMode() << endl;

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
