#ifndef _AVR_I2C_BOOT_LOADER_APPLICATION_PROGRAM_H_
#define _AVR_I2C_BOOT_LOADER_APPLICATION_PROGRAM_H_

#include "ApplicationPage.h"

#include <string>
#include <vector>
#include <fstream>

using namespace std;

#define APPLICATION_PROGRAM_HEX_ENTRY_SIZE 524

class ApplicationProgram
{
  private:
    string filename;
    vector<ApplicationPage> pages;

    uint32_t asciiHexToNumber(char* s, uint8_t n);
    uint8_t asciiHexToByte(char* s);
    uint16_t asciiHexToShort(char* s);

    vector<uint8_t> parseHexEntry(char* buffer, uint16_t* address);
  public:
    ApplicationProgram(string filename);

    void loadPages(size_t pageSize);

    vector<ApplicationPage> getPages();    
};

#endif
