#include <ApplicationProgram.h>

ApplicationProgram::ApplicationProgram(string filename)
{
	this->filename = filename;
}

bool ApplicationProgram::loadPages(size_t pageSize)
{

}

vector<ApplicationPage> ApplicationProgram::getPages()
{
	return pages;
}
