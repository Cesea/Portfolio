#ifndef POOL_INDEX_H
#define POOL_INDEX_H

#include "SparseArray.h"
#include "Handle.h"

class PoolIndex
{
public :
	PoolIndex(uint32 size);
	Handle CreateHandle();
	Handle CreateHandle(void *pData);
	bool IsValid(Handle handle);

	void SetInnerIndexValue(Handle handle void *pData);
	void *Get(Handle handle);

private :
	struct InnerHandle
	{
		uint32 group;
		void *pData;
	};

	SparseArray<InnerHandle> _indices;
};

#endif