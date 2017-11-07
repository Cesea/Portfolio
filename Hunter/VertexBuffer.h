#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include "Resource.h"


class VertexBuffer : public Resource
{
public :
	VertexBuffer();
	virtual ~VertexBuffer();

	LPDIRECT3DVERTEXBUFFER9 Buffer() const;
	uint32		GetStride() const;
	uint16		GetCount() const;
	uint32		GetByteSize() const;

	virtual bool32 Create(uint32 maxCount, uint32 stride, bool32 dynamic = false, const void *pData = nullptr);
	virtual bool32 Destroy();

	void *Lock(DWORD lockType, uint32 lockOffset, uint32 lockSize);
	void Unlock();

private :
	// Private Data...
	LPDIRECT3DVERTEXBUFFER9	 _pBuffer;	// pointer to the actual DirectX Buffer
	uint16 _maxCount;
	uint32 _stride;					// size of a single member in bytes
	uint32 _byteSize;
	uint32 _lockOffset;
	uint32 _lockSize;
	bool32 _isDynamic;


	// Invalid Functions...
    VertexBuffer(const VertexBuffer& Src);
    VertexBuffer& operator=(const VertexBuffer& Src);
};

#endif