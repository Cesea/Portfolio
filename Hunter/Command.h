#ifndef COMMAND_H
#define COMMAND_H

#include "Typedefs.h"

#include "BackendDispatch.h"

namespace commands
{
	struct Draw
	{
		static const BackendDispatchFunction DISPATCH_FUNCTION;

		VertexBufferHandle vertexBufferHandle;

		uint32 primitiveCount;
		uint32 startVertex;

		uint16 worldMatrix;
	};	

	struct DrawIndexed
	{
		static const BackendDispatchFunction DISPATCH_FUNCTION;

		VertexBufferHandle vertexBufferHandle;
		IndexBufferHandle indexBufferHandle;

		uint32 numVertex;
		uint32 startIndex;
		uint32 primitiveCount;

		uint16 worldMatrix;
	};
}

#endif