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

void * DataPackage::Create(uint32 size)
{
	_data = new uint8[size];
	_pCurrent = _data;
	_size = size;
	return _data;
}

bool DataPackage::ReadInt8(int8 * pOut)
{
	Assert(_pCurrent - _data < _size);
	*pOut = *((int8 *)_pCurrent);

	_pCurrent += 1;

	return true;
}

//TODO : Assert Logic 점검하라
bool DataPackage::ReadInt16(int16 * pOut)
{
	Assert(_pCurrent - _data < _size);
	*pOut = *((int16 *)_pCurrent);

	_pCurrent += 2;

	return true;
}

bool DataPackage::ReadInt32(int32 * pOut)
{
	Assert(_pCurrent - _data < _size);
	*pOut = *((int32 *)_pCurrent);

	_pCurrent += 4;

	return true;
}

bool DataPackage::ReadInt64(int64 * pOut)
{
	Assert(_pCurrent - _data < _size);
	*pOut = *((int64 *)_pCurrent);

	_pCurrent += 8;

	return true;
}

bool DataPackage::ReadUint8(uint8 * pOut)
{
	Assert(_pCurrent - _data < _size);
	*pOut = *((uint8 *)_pCurrent);

	_pCurrent += 1;

	return true;
}

bool DataPackage::ReadUint16(uint16 * pOut)
{
	Assert(_pCurrent - _data < _size);
	*pOut = *((uint16 *)_pCurrent);

	_pCurrent += 2;

	return true;
}

bool DataPackage::ReadUint32(uint32 * pOut)
{
	Assert(_pCurrent - _data < _size);
	*pOut = *((uint32 *)_pCurrent);

	_pCurrent += 4;

	return true;
}

bool DataPackage::ReadUint64(uint64 * pOut)
{
	Assert(_pCurrent - _data < _size);
	*pOut = *((uint64 *)_pCurrent);

	_pCurrent += 8;

	return true;
}

bool DataPackage::ReadFloat(float * pOut)
{
	Assert(_pCurrent - _data < _size);
	*pOut = *((float *)_pCurrent);

	_pCurrent += 4;

	return true;
}

bool DataPackage::ReadDouble(double * pOut)
{
	Assert(_pCurrent - _data < _size);
	*pOut = *((double *)_pCurrent);

	_pCurrent += 8;

	return true;
}

bool DataPackage::WriteInt8(int8 value)
{
	Assert(_pCurrent - _data < _size);
	*((int8 *)_pCurrent) = value;

	_pCurrent += 1;

	return true;
}

bool DataPackage::WriteInt16(int16 value)
{
	Assert(_pCurrent - _data < _size);
	*((int16 *)_pCurrent) = value;

	_pCurrent += 2;

	return true;
}

bool DataPackage::WriteInt32(int32 value)
{
	Assert(_pCurrent - _data < _size);
	*((int32 *)_pCurrent) = value;

	_pCurrent += 4;

	return true;
}

bool DataPackage::WriteInt64(int64 value)
{
	Assert(_pCurrent - _data < _size);
	*((int64 *)_pCurrent) = value;

	_pCurrent += 8;
	return true;
}

bool DataPackage::WriteUint8(uint8 value)
{
	Assert(_pCurrent - _data < _size);
	*((uint8 *)_pCurrent) = value;

	_pCurrent += 1;

	return true;
}

bool DataPackage::WriteUint16(uint16  value)
{
	Assert(_pCurrent - _data < _size);
	*((uint16 *)_pCurrent) = value;

	_pCurrent += 2;

	return true;
}

bool DataPackage::WriteUint32(uint32 value)
{
	Assert(_pCurrent - _data < _size);
	*((uint32 *)_pCurrent) = value;

	_pCurrent += 4;

	return true;
}

bool DataPackage::WriteUint64(uint64 value)
{
	Assert(_pCurrent - _data < _size);
	*((uint64 *)_pCurrent) = value;

	_pCurrent += 8;

	return true;
}

bool DataPackage::WriteFloat(float value)
{
	Assert(_pCurrent - _data < _size);
	*((float *)_pCurrent) = value;

	_pCurrent += 4;

	return true;
}

bool DataPackage::WriteDouble(double value)
{
	Assert(_pCurrent - _data < _size);
	*((float *)_pCurrent) = value;

	_pCurrent += 8;

	return true;
}

void DataPackage::Free()
{
	SAFE_DELETE_ARRAY(_data);
	_pCurrent = nullptr;
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

void *DataPackage::OpenFile(const char *fileName, uint32 * pOutSize)
{
	Free();
	FILE *pFile;
	fopen_s(&pFile, fileName, "rb");
	if (pFile)
	{
		fseek(pFile, 0, SEEK_END);
	
		int32 fileSize = ftell(pFile);
		fseek(pFile, 0, SEEK_SET);
		_data = new uint8[fileSize];
		_size = fileSize;

		if (pOutSize != nullptr)
		{
			*pOutSize = _size;
		}

		fread(_data, _size, 1, pFile);
		fclose(pFile);

		_pCurrent = _data;

	}
	return _data;
}