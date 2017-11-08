#ifndef DATA_PACKAGE_H
#define DATA_PACKAGE_H

#include <string>

class DataPackage
{
public :
	DataPackage();
	~DataPackage();

	void *Create(uint32 size);

	void Free();
	void Save(const char *fileName);
	void *Read(const char *fileName, uint32 *pOutSize);

	void *Data()
	{
		Assert(_data); //�����Ͱ� �ȿ� �־������ ���� �� �� �ִ�
		return _data;
	}

	uint32 Size()
	{
		return _size;
	}

protected:
	void *_data;
	uint32 _size;
};

#endif
