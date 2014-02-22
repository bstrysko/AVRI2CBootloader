#ifndef _AVR_I2C_BOOT_LOADER_APPLICATION_PROGRAM_H_
#define _AVR_I2C_BOOT_LOADER_APPLICATION_PROGRAM_H_

#include "ApplicationPage.h"

#include <string>
#include <vector>

using namespace std;

class ApplicationProgram
{
  private:
    string filename;
    vector<ApplicationPage> pages;
  public:
    ApplicationProgram(string filename);

    bool loadPages(size_t pageSize);

    vector<ApplicationPage> getPages();    
};

#endif
