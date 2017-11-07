#include "stdafx.h"
#include "IndexBuffer.h"

IndexBuffer::IndexBuffer()
{
	_pBuffer = nullptr;
	_maxCount = 0;
	_stride = 0;
	_byteSize = 0;
	_lockOffset = 0;
	_lockSize = 0;
	_isDynamic = false;
}

IndexBuffer::~IndexBuffer()
{
}

LPDIRECT3DINDEXBUFFER9 IndexBuffer::Buffer() const
{
	return _pBuffer;
}

uint32 IndexBuffer::GetStride() const
{
	return _stride;
}

uint16 IndexBuffer::GetCount() const
{
	return _maxCount;
}

uint32 IndexBuffer::GetByteSize() const
{
	return _maxCount * _stride;
}

bool32 IndexBuffer::Create(uint32 maxCount, IndexFormat format, bool32 dynamic, const void * pData)
{
	_maxCount = maxCount;
	D3DFORMAT usingFormat;
	if (format == IndexFormat::Uint16)
	{
		_stride = sizeof(uint16);
		usingFormat = D3DFMT_INDEX16;
	}
	else if(format == IndexFormat::Uint32)
	{
		_stride = sizeof(uint32);
		usingFormat = D3DFMT_INDEX32;
	}

	DWORD usage = 0;
	D3DPOOL pool;

	if (dynamic)
	{
		usage = D3DUSAGE_DYNAMIC;
		_isDynamic = true;
		pool = D3DPOOL_MANAGED;
	}
	else
	{
		usage = D3DUSAGE_WRITEONLY;
		_isDynamic = true;
		pool = D3DPOOL_DEFAULT;
	}

	if (FAILED(gpDevice->CreateIndexBuffer(GetByteSize(), usage, usingFormat, pool, &_pBuffer, nullptr)))
	{
		return false;
	}

	if (pData != nullptr)
	{
		void *pIndexData = nullptr;
		pIndexData = Lock(0, 0, 0);

		if (pIndexData == nullptr)
		{
			return false;
		}

		memcpy(pIndexData, pData, GetByteSize());
		Unlock();
	}

	return true;
}

bool32 IndexBuffer::Destroy()
{
	COM_RELEASE(_pBuffer);
	_handle = INVALID_HANDLE;
	return true;
}

void * IndexBuffer::Lock(DWORD lockType, uint32 lockOffset, uint32 lockSize)
{
	void *result = nullptr;
	if (FAILED(_pBuffer->Lock(lockOffset, lockSize, (void **)&result, lockType)))
	{
		return nullptr;
	}
	return result;
}

void IndexBuffer::Unlock()
{
	_pBuffer->Unlock();
}

IndexBuffer::IndexBuffer(const IndexBuffer & Src)
{
}

IndexBuffer & IndexBuffer::operator=(const IndexBuffer & Src)
{
	// TODO: insert return statement here
	return *this;
}
