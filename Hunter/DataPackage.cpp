#include "stdafx.h"
#include "DataPackage.h"

DataPackage::DataPackage()
{
	_data = nullptr;
	_size = 0;
}

DataPackage::~DataPackage()
{
	Free();
}

void * DataPackage::Create(unsigned int size)
{
	_data = new char[size];
	_size = size;
	return _data;
}

void DataPackage::Free()
{
	if (_data)
	{
		delete[] _data;
	}
	_size = 0;
}

void DataPackage::Save(const char *fileName)
{
	FILE *pFile;
	fopen_s(&pFile, fileName, "wb");
	if (pFile)
	{
		fwrite(_data, _size, 1, pFile);
		fclose(pFile);
	}
}

void *DataPackage::Read(const char *fileName, unsigned int * size)
{
	Free();
	FILE *pFile;
	fopen_s(&pFile, fileName, "rb");
	if (pFile)
	{
		fseek(pFile, 0, SEEK_END);
	
		int fileSize = ftell(pFile);
		fseek(pFile, 0, SEEK_SET);
		_data = new char[fileSize];
		_size = fileSize;

		if (size != nullptr)
		{
			*size = _size;
		}

		fread(_data, _size, 1, pFile);
		fclose(pFile);
	}
	return _data;
}
