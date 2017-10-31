#ifndef DATA_PACKAGE_H
#define DATA_PACKAGE_H

#include <string>

class DataPackage
{
public :
	DataPackage();
	~DataPackage();

	void *Create(unsigned int size);

	void Free();
	void Save(const char *fileName);
	void *Read(const char *fileName, unsigned int *size);

protected:
	void *_data;
	unsigned int _size;
};


#endif
