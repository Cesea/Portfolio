#ifndef DATA_PACKAGE_H
#define DATA_PACKAGE_H

#include <string>

class DataPackage
{
public :
	DataPackage();
	~DataPackage();

	void *Create(uint32 size);

	bool ReadInt8(int8 *pOut);
	bool ReadInt16(int16 *pOut);
	bool ReadInt32(int32 *pOut);
	bool ReadInt64(int64 *pOut);

	bool ReadUint8(uint8 *pOut);
	bool ReadUint16(uint16 *pOut);
	bool ReadUint32(uint32 *pOut);
	bool ReadUint64(uint64 *pOut);

	bool ReadFloat(float *pOut);
	bool ReadDouble(double *pOut);

	bool WriteInt8(int8 value);
	bool WriteInt16(int16 value);
	bool WriteInt32(int32 value);
	bool WriteInt64(int64 value);

	bool WriteUint8(uint8 value);
	bool WriteUint16(uint16 value);
	bool WriteUint32(uint32 value);
	bool WriteUint64(uint64 value);

	bool WriteFloat(float value);
	bool WriteDouble(double value);


	template <typename T>
	bool ReadAs(T *pOut);

	template <typename T>
	bool WriteAs(T &value);

	void Free();
	void Save(const char *fileName);
	void *OpenFile(const char *fileName, uint32 *pOutSize);

public :
	uint8 *_data{};
	uint8 *_pCurrent{};

	uint32 _size;
};


template<typename T>
inline bool DataPackage::ReadAs(T * pOut)
{
	Assert(_pCurrent - _data < _size);
	*pOut = *((T *)_pCurrent);

	_pCurrent += sizeof(T);

	return true;
}

template<typename T>
inline bool DataPackage::WriteAs(T & value)
{
	Assert(_pCurrent - _data < _size);
	*((T *)_pCurrent) = value;

	_pCurrent += sizeof(T);

	return true;
}


#endif
