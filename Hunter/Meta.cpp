#include "stdafx.h"
#include "Meta.h"

#include "RefVariant.h"


void MetaData::Copy(void * dest, const void * src) const
{
	memcpy(dest, src, _size);
}

void MetaData::Delete(void * data) const
{
	delete[] reinterpret_cast<char *>(data);
	data = nullptr;
}

void * MetaData::NewCopy(const void * src) const
{
	void *result = new char[_size];
	memcpy(result, src, _size);
	return result;
}

void * MetaData::New()
{
	return new char[_size];
}

void MetaData::AddMember(const Member * member)
{
	if (!_members)
	{
		_members = const_cast<Member *>(member);
	}
	else
	{
		_lastMember->Next() = const_cast<Member *>(member);
	}
	_lastMember = const_cast<Member *>(member);
}

bool32 MetaData::HasMembers() const
{
	return (_members) ? true : false;
}

const Member * MetaData::Members() const
{
	return _members;
}

void MetaData::PrintMembers(std::ostream & os) const
{
	const Member *mem = _members;
}

void MetaData::SetSerializer(SerializerFunction func)
{
	_serializer = func;
}

void MetaData::Serialize(std::ostream & os, RefVariant ref) const
{
	if (_serializer)
	{
		_serializer(os, ref);
	}
	else
	{
		TextSerialize(os, ref);
	}
}

uint32 MetaData::TotalNumElement() const
{
	return uint32();
}

Member *& Member::Next()
{
	return _next;
}

Member * const & Member::Next() const
{
	return _next;
}
