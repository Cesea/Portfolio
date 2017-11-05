#ifndef COMMAND_H
#define COMMAND_H

#include "Typedefs.h"

#include "BackendDispatch.h"

namespace commands
{
	struct Draw
	{
		static const BackendDispatchFunction DISPATCH_FUNCTION;
		uint32 primitiveCount;
		uint32 startVertex;

		IDirect3DVertexBuffer9 *vertexBuffer;
		//VertexDeclHandle vertexDecl;
		//VertexBufferHandle vertexBuffer;
		//IndexBufferHandle indexBuffer;
	};	

	struct DrawIndexed
	{
		static const BackendDispatchFunction DISPATCH_FUNCTION;
		uint32 indexCount;
		uint32 startIndex;
		uint32 baseVertex;

		//VertexDeclHandle vertexDecl;
		//VertexBufferHandle vertexBuffer;
		//IndexBufferHandle indexBuffer;
	};
}

#endif