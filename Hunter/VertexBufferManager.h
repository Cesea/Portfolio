#ifndef VERTEX_BUFFER_MANAGER_H
#define VERTEX_BUFFER_MANAGER_H

#include "ResourceManager.h"
#include "VertexBuffer.h"

#include <map>

class VertexBufferManager : public ResourceManager<VertexBuffer>
{
public :
	VertexBufferManager();
	~VertexBufferManager();
	VertexBufferHandle CreateStatic(uint32 numVertex, uint32 stride, const void * pData);
	VertexBufferHandle CreateDynamic(uint32 numVertex, uint32 stride);

	virtual bool32 Destroy(ResourceHandle handle);
private :
};


#endif