#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H


#include "Resource.h"

class IndexBuffer : public Resource
{
public:
	enum IndexFormat
	{
		Uint16,
		Uint32
	};

	IndexBuffer();
	virtual ~IndexBuffer();

	LPDIRECT3DINDEXBUFFER9 Buffer() const;
	uint32		GetStride() const;
	uint16		GetCount() const;
	uint32		GetByteSize() const;

	virtual bool32 Create(uint32 maxCount, IndexFormat format, bool32 dynamic = false, const void *pData = nullptr);
	virtual bool32 Destroy();

	void *Lock(DWORD lockType, uint32 lockOffset, uint32 lockSize);
	void Unlock();

private:
	// Private Data...
	LPDIRECT3DINDEXBUFFER9 _pBuffer;	// pointer to the actual DirectX Buffer
	uint16 _maxCount;
	uint32 _stride;					// size of a single member in bytes
	uint32 _byteSize;
	uint32 _lockOffset;
	uint32 _lockSize;
	bool32 _isDynamic;


	// Invalid Functions...
	IndexBuffer(const IndexBuffer& Src);
	IndexBuffer& operator=(const IndexBuffer& Src);
};

#endif