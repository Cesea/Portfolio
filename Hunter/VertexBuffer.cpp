#include "stdafx.h"
#include "VertexBuffer.h"


bool VertexBuffer::Create(uint16 count, uint32 stride, Uint16Flags flags, void * pData)
{
	return false;
}

void VertexBuffer::Destroy()
{
}

void* VertexBuffer::Lock(LockType lockType, uint16 index, uint16 count)
{
}
void VertexBuffer::Unlock()
{
}

void VertexBuffer::PreLoad()
{
}
void VertexBuffer::Activate(int stream, uint16 index, bool setDeclaration = true)
{
}

bool VertexBuffer::Clone(VertexBuffer* src)
{
}

// Base Class Function Overrides
bool VertexBuffer::CreateResource()				
{
}
bool VertexBuffer::DestroyResource()			
{
}
bool VertexBuffer::DisableResource()		
{
}
bool VertexBuffer::RestoreResource()	
{
}
bool VertexBuffer::LoadResource(const char* filename = 0) 
{
}
bool VertexBuffer::SaveResource(const char* filename = 0)	
{
}

bool VertexBuffer::SetVertexDescription(int elementCount, const D3DVERTEXELEMENT9* elementList)
{
}