#ifndef _AVR_I2C_BOOTLOADER_APPLICATION_PAGE_H_
#define _AVR_I2C_BOOTLOADER_APPLICATION_PAGE_H_

#include <sstream>

#include <stdint.h>
#include <cstring>

using namespace std;

class ApplicationPage
{
  private:
    size_t pageSize;
    uint8_t* data;
  public:
    ApplicationPage(size_t pageSize);
    ~ApplicationPage();

    size_t getPageSize();

    void write(size_t offset, uint8_t v);

    uint8_t* getData();
};

#endif
