#ifndef INDEX_BUFFER_MANAGER_H
#define INDEX_BUFFER_MANAGER_H

#include "ResourceManager.h"
#include "IndexBuffer.h"

#include <map>

class IndexBufferManager : public ResourceManager<IndexBuffer>
{
public:
	IndexBufferManager();
	~IndexBufferManager();
	IndexBufferHandle CreateStatic(uint32 numVertex, IndexBuffer::IndexFormat format, const void * pData);
	IndexBufferHandle CreateDynamic(uint32 numVertex, uint32 stride);

	virtual bool32 Destroy(IndexBufferHandle handle);
private:
};


#endif