#include "stdafx.h"
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer()
{
	_pBuffer = nullptr;
	_maxCount = 0;
	_stride = 0;					
	_byteSize = 0;
	_lockOffset = 0;
	_lockSize = 0;
}

VertexBuffer::~VertexBuffer()
{
}

LPDIRECT3DVERTEXBUFFER9 VertexBuffer::Buffer() const
{
	return _pBuffer;
}

uint32 VertexBuffer::Stride() const
{
	return _stride;
}

uint16 VertexBuffer::Count() const
{
	return _maxCount;
}

uint32 VertexBuffer::ByteSize() const
{
	return _maxCount * _stride;
}

bool32 VertexBuffer::Create(uint32 maxCount, uint32 stride, bool32 dynamic , const void *pData)
{
	_maxCount = maxCount;
	_stride = stride;
	DWORD usage = 0;
	D3DPOOL pool;

	usage |= (dynamic) ? (D3DUSAGE_DYNAMIC) : (D3DUSAGE_WRITEONLY);
	pool = (dynamic) ? (D3DPOOL_MANAGED) : (D3DPOOL_DEFAULT);

	if (FAILED(gpDevice->CreateVertexBuffer(ByteSize(), usage, 0, pool, &_pBuffer, nullptr)))
	{
		return false;
	}

	if (dynamic == false && pData != nullptr)
	{
		void *datas = Lock(0, 0, 0);
		if (datas != nullptr)
		{
			memcpy(datas, pData, ByteSize());
			Unlock();
			return true;
		}
		else
		{
			return false;
		}
	}
	return true;
}

bool32 VertexBuffer::Destroy()
{
	COM_RELEASE(_pBuffer);
	_handle = INVALID_HANDLE;
	return true;
}

void *VertexBuffer::Lock(DWORD lockType, uint32 lockOffset, uint32 lockSize)
{
	_lockOffset = lockOffset;
	_lockSize = lockSize;
	void *result = nullptr;
	if (FAILED(_pBuffer->Lock(_lockOffset, _lockSize, (void **)&result, lockType)))
	{
		return nullptr;
	}
	return result;
}

void VertexBuffer::Unlock()
{
	_pBuffer->Unlock();
}

VertexBuffer::VertexBuffer(const VertexBuffer & Src)
{
}

VertexBuffer & VertexBuffer::operator=(const VertexBuffer & Src)
{
	// TODO: insert return statement here
	return *this;
}
