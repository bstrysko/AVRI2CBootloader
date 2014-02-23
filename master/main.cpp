#include <iostream>

#include <I2CDeviceBootable.h>
#include <I2CDeviceBootLoader.h>
#include <ApplicationProgram.h>

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

	uint8_t i2cBusNumber = stringToUInt8(argv[1]);
	uint8_t i2cDeviceAddress = stringToUInt8(argv[2]);
	string filename(argv[3]);

	ApplicationProgram program(filename);
	program.loadPages(260);
	
	I2CBus bus(1);
	I2CDeviceBootLoader d(&bus, 0x40);


	cout << "In BootLoader Mode: " << d.inBootLoaderMode() << endl;
	cout << "In Application Mode: " << d.inApplicationMode() << endl;

	return 0;
}

void usage(string name)
{
	cout << "usage: " << name << " [ I2C Bus Number ] [I2C Device Address] [ .hex file ]" << endl;
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
