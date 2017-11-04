#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include "ResourcePoolItem.h"

template <typename T>
class ResourcePool;

class VertexBuffer : public ResourcePoolItem
{
public:

	// Data Types & Constants...
	enum BufferFlagBits
	{
		DynamicOverwriteBit = 0,	// each lock completely replaces thew buffer
		DynamicUpdateBit,			// each lock may update *some* data
		ReadableBit,				// the buffer may be read
		RamBackupBit,				// a backup of the buffer is stored in RAM
		SoftwareBit,				// this buffer is not for hardware rendering
		BUFFER_FALG_BIT_COUNT,
	};

	enum BufferStateBits
	{
		LockedReadBit,				// the buffer is currently locked for reading
		LockedWriteBit,			// the buffer is currently locked for writing
		DataInitialized,			// data has been placed in the buffer
		Volatile,					// this buffer must be restored by hand
		BUFFER_STATE_COUNT,
	};

	enum eConstants
	{
		MaxElements = 16,			// maximum number of vertex elements stored
	};


	VertexBuffer();
	virtual ~VertexBuffer();

	// Mutators...
	bool Create(uint16 count, uint32 stride, Uint16Flags flags, void* pData);
	void Destroy();

	void* Lock(LockType lockType, uint16 index, uint16 count);
	void Unlock();

	void PreLoad();
	void Activate(int stream, uint16 index, bool setDeclaration = true);

	bool Clone(VertexBuffer* src);

	// Base Class Function Overrides
	bool CreateResource();					// innitialize the resource (called once)
	bool DestroyResource();					// destroy the resource
	bool DisableResource();					// purge the resource from volatile memory
	bool RestoreResource();					// prepare the resource for use (create any volatile memory objects needed)
	bool LoadResource(const char* filename = 0); // load the resource from a file (or NULL to use the resource name)
	bool SaveResource(const char* filename = 0);	// save the resource to a file (or NULL to use the resource name)

	bool SetVertexDescription(int elementCount, const D3DVERTEXELEMENT9* elementList);

	// Accessors...
	bool IsValid() const;
	bool IsReady() const;
	bool IsDynamic() const;
	bool IsDynamicOverwrite() const;
	bool IsDynamicUpdate() const;
	bool IsReadable() const;
	bool IsRAMBufferPresent() const;
	bool IsSoftware() const;
	bool IsLocked() const;

	LPDIRECT3DVERTEXBUFFER9 Buffer() const;
	uint32		Stride() const;
	Uint16Flags	TypeFlags() const;
	Uint16Flags	StateFlags() const;
	uint16		Count() const;
	uint32		ByteSize() const;
	uint32		VertexElementCount() const;
	const D3DVERTEXELEMENT9*	VertexElements() const;

private:

	// Private Data...
	LPDIRECT3DVERTEXBUFFER9	 _d3dBuffer;	// pointer to the actual DirectX Buffer
	uint8*		_backupCopy;
	Uint16Flags	_typeFlags;				// flags describing the buffer's type
	Uint16Flags	_stateFlags;				// flags describing the buffer's state
	uint16		_memberCount;
	uint32		_stride;					// size of a single member in bytes
	uint32		_byteSize;
	uint32		_lockOffset;
	uint32		_lockSize;
	uint32		_usageFlags;

	uint32		_vertexElementCount;
	D3DVERTEXELEMENT9 _vertexElement[MaxElements];
	LPDIRECT3DVERTEXDECLARATION9 _pVertexDeclaration;

	// Private Functions...
	bool CreateD3DBuffer();
	uint8* LockD3DBuffer(LockType lockType, uint32 byteOffset, uint32 byteCount);
	void UnlockD3DBuffer();

	// Invalid Functions...
	VertexBuffer(const VertexBuffer& Src) {}
	VertexBuffer& operator=(const VertexBuffer& Src) {}

};

typedef ResourcePool<VertexBuffer> VertexBufferManager;

//- Inline Functions --------------------------------------------

//. Creators ....................................................

/*	VertexBuffer
-----------------------------------------------------------------

Default Constructor

-----------------------------------------------------------------
*/
inline VertexBuffer::VertexBuffer()
	:_d3dBuffer(0)
	, _backupCopy(0)
	, _typeFlags(0)
	, _stateFlags(0)
	, _vertexElementCount(0)
	, _pVertexDeclaration(0)
{
}

/*	~VertexBuffer
-----------------------------------------------------------------

Default Destructor

-----------------------------------------------------------------
*/
inline VertexBuffer::~VertexBuffer()
{
	//debug_assert(_d3dBuffer == 0, "d3d buffer left allocated. Call destroy()");
	//debug_assert(_backupCopy == 0, "backup buffer left allocated. Call destroy()");
}

inline bool VertexBuffer::IsValid() const
{
	// if at least one buffer is allocated, this object is valid
	return(_d3dBuffer ? true : false);
}

inline bool VertexBuffer::IsReady() const
{
	// if at least one buffer is allocated, this object is valid
	return(TEST_BIT(_stateFlags, DataInitialized));
}

inline bool VertexBuffer::IsDynamic() const
{
	return (TEST_BIT(_typeFlags, DynamicOverwriteBit) || TEST_BIT(_typeFlags, DynamicUpdateBit));
}

inline bool VertexBuffer::IsDynamicOverwrite() const
{
	return (TEST_BIT(_typeFlags, DynamicOverwriteBit));
}

inline bool VertexBuffer::IsDynamicUpdate() const
{
	return (TEST_BIT(_typeFlags, DynamicUpdateBit));
}

inline bool VertexBuffer::IsReadable() const
{
	return (TEST_BIT(_typeFlags, ReadableBit));
}

inline bool VertexBuffer::IsRAMBufferPresent() const
{
	return (_backupCopy ? true : false);
}

inline bool VertexBuffer::IsSoftware() const
{
	return (TEST_BIT(_typeFlags, SoftwareBit));
}

inline bool VertexBuffer::IsLocked() const
{
	return (TEST_BIT(_stateFlags, LockedReadBit) || TEST_BIT(_stateFlags, LockedWriteBit));
}

inline LPDIRECT3DVERTEXBUFFER9 VertexBuffer::Buffer()const
{
	return _d3dBuffer;
}

inline Uint16Flags VertexBuffer::TypeFlags() const
{
	return _typeFlags;
}

inline Uint16Flags VertexBuffer::StateFlags() const
{
	return _stateFlags;
}

inline uint16 VertexBuffer::Count()const
{
	return _memberCount;
}

inline uint32 VertexBuffer::Stride()const
{
	return _stride;
}

inline uint32 VertexBuffer::ByteSize()const
{
	return _byteSize;
}

inline uint32 VertexBuffer::VertexElementCount()const
{
	return _vertexElementCount;
}

inline const D3DVERTEXELEMENT9* VertexBuffer::VertexElements()const
{
	return _vertexElement;
}

inline bool VertexBuffer::CreateD3DBuffer()
{
	return false;
}

inline uint8 * VertexBuffer::LockD3DBuffer(LockType lockType, uint32 byteOffset, uint32 byteCount)
{
	return NULL;
}

inline void VertexBuffer::UnlockD3DBuffer()
{
}


#endif