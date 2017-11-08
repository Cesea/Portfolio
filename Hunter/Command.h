#ifndef COMMAND_H
#define COMMAND_H

#include "Typedefs.h"

#include "BackendDispatch.h"

constexpr uint32 MAX_COMMAND_BUFFER_SIZE = 1024 * 20;

//class CommandBuffer
//{
//public:
//	CommandBuffer()
//		: _pos(0)
//		, _size(MAX_COMMAND_BUFFER_SIZE)
//	{
//		Finish();
//	}
//
//	enum Enum
//	{
//		RendererInit,
//		RendererShutdownBegin,
//		CreateVertexDecl,
//		CreateIndexBuffer,
//		CreateVertexBuffer,
//		CreateDynamicIndexBuffer,
//		UpdateDynamicIndexBuffer,
//		CreateDynamicVertexBuffer,
//		UpdateDynamicVertexBuffer,
//		CreateShader,
//		CreateProgram,
//		CreateTexture,
//		UpdateTexture,
//		ResizeTexture,
//		CreateFrameBuffer,
//		CreateUniform,
//		UpdateViewName,
//		InvalidateOcclusionQuery,
//		SetName,
//		End,
//		RendererShutdownEnd,
//		DestroyVertexDecl,
//		DestroyIndexBuffer,
//		DestroyVertexBuffer,
//		DestroyDynamicIndexBuffer,
//		DestroyDynamicVertexBuffer,
//		DestroyShader,
//		DestroyProgram,
//		DestroyTexture,
//		DestroyFrameBuffer,
//		DestroyUniform,
//		ReadTexture,
//		RequestScreenShot,
//	};
//
//	void Write(const void* data, uint32_t size)
//	{
//		Assert(_size == MAX_COMMAND_BUFFER_SIZE);
//		Assert(_pos < _size);
//		memcpy(&_buffer[_pos], data, size);
//		_pos += size;
//	}
//
//	template<typename Type>
//	void write(const Type& in)
//	{
//		align(BX_ALIGNOF(Type));
//		write(reinterpret_cast<const uint8*>(&in), sizeof(Type));
//	}
//
//	void Read(void* data, uint32_t size)
//	{
//		Assert(_pos < _size);
//		memcpy(data, &_buffer[_pos], _size);
//		_pos += _size;
//	}
//
//	template<typename T>
//	void Read(T& in)
//	{
//		align(BX_ALIGNOF(T));
//		Read(reinterpret_cast<uint8*>(&in), sizeof(T));
//	}
//
//	const uint8* Skip(uint32 size)
//	{
//		Assert(_pos < _size);
//		const uint8* result = &_buffer[_pos];
//		_pos += size;
//		return result;
//	}
//
//	template<typename T>
//	void Skip()
//	{
//		align(BX_ALIGNOF(T));
//		Skip(sizeof(T));
//	}
//
//	void Align(uint32 _alignment)
//	{
//		const uint32 mask = _alignment - 1;
//		const uint32 pos = (_pos + mask) & (~mask);
//		_pos = pos;
//	}
//
//	void Reset()
//	{
//		_pos = 0;
//	}
//
//	void Start()
//	{
//		_pos = 0;
//		_size = MAX_COMMAND_BUFFER_SIZE;
//	}
//
//	void Finish()
//	{
//		uint8 cmd = End;
//		write(cmd);
//		_size = _pos;
//		_pos = 0;
//	}
//
//	uint32 _pos;
//	uint32 _size;
//	uint8 _buffer[MAX_COMMAND_BUFFER_SIZE];
//};

//namespace commands
//{
//	struct Draw
//	{
//		static const BackendDispatchFunction DISPATCH_FUNCTION;
//
//		VertexBufferHandle vertexBufferHandle;
//
//		uint32 primitiveCount;
//		uint32 startVertex;
//
//		uint16 worldMatrix;
//	};	
//
//	struct DrawIndexed
//	{
//		static const BackendDispatchFunction DISPATCH_FUNCTION;
//
//		VertexBufferHandle vertexBufferHandle;
//		IndexBufferHandle indexBufferHandle;
//
//		uint32 numVertex;
//		uint32 startIndex;
//		uint32 primitiveCount;
//
//		uint16 worldMatrix;
//	};
//}

#endif