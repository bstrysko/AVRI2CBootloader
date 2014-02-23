#include <ApplicationProgram.h>

ApplicationProgram::ApplicationProgram(string filename)
{
	this->filename = filename;
}

uint32_t ApplicationProgram::asciiHexToNumber(char* s, uint8_t n)
{
	char* p = s;
	uint32_t result = 0;

	for(int i = 0; i < n; i++)
	{
		char c = *p;

		if(c >= 'a')
		{
			c += 10 - 'a';
		}
		else if(c >= 'A')
		{
			c += 10 - 'A';
		}
		else
		{
			c -= '0';
		}

		if(c >= 16)
		{
			stringstream o;
			o << s << " is invalid and should not be in the .hex file";
			throw ios_base::failure(o.str());
		}

		result = 16 * result + c;
	}

	return result;
}

uint8_t ApplicationProgram::asciiHexToByte(char* s)
{
	return asciiHexToNumber(s, 2);
}

uint16_t ApplicationProgram::asciiHexToShort(char* s)
{
	return asciiHexToNumber(s, 4);
}

void ApplicationProgram::loadPages(size_t pageSize)
{
	ifstream file(filename.c_str());

	if(!file.is_open())
	{
		stringstream o;
		o << "Cannot load file ";
		o << filename;
		throw ios_base::failure(o.str());
	}

	char buffer[APPLICATION_PROGRAM_HEX_ENTRY_SIZE];

	while(true)
	{
		file.getline(buffer, APPLICATION_PROGRAM_HEX_ENTRY_SIZE);

		/*
		 * End of file
		 */
		if(!file)
		{
			break;
		}

		uint16_t address;
		vector<uint8_t> data = parseHexEntry(buffer, &address);

		//TODO: add data to pages

		//size_t page = address % pageSize;
		//ApplicationPage page = pages[page];
	}
}

vector<uint8_t> ApplicationProgram::parseHexEntry(char* buffer, uint16_t* address)
{
	vector<uint8_t> data;

	if(buffer[0] != ':')
	{
		stringstream o;
		o << "Invalid hex entry ";
		o << buffer;
		throw ios_base::failure(o.str());
	}

	uint8_t size = asciiHexToByte(&buffer[1]);
	(*address) = asciiHexToShort(&buffer[3]);
	uint8_t byte = asciiHexToByte(&buffer[7]);

	/*
	 * End or Segment Record
	 */
	if(byte != 0)
	{
		return data;
	}

	for(uint8_t i = 0; i < size; i++)
	{
		data.push_back(asciiHexToByte(&buffer[9+i]));
	}

	return data;
}

vector<ApplicationPage> ApplicationProgram::getPages()
{
	return pages;
}
