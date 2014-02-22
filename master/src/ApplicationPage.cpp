#include <ApplicationPage.h>

ApplicationPage::ApplicationPage(size_t pageSize)
{
	this->pageSize = pageSize;
	this->data = new uint8_t[pageSize];
}

ApplicationPage::~ApplicationPage()
{
	delete data;
}

size_t ApplicationPage::getPageSize()
{
	return pageSize;
}

void ApplicationPage::write(size_t offset, uint8_t v)
{
	if(offset >= pageSize)
	{
		stringstream o;
		o << "Offset " << offset;
		o << " exceeds pageSize " << pageSize;
		throw ios_base::failure(o.str());
	}

	data[offset] = v;
}

uint8_t* ApplicationPage::getData()
{
	return data;
}
