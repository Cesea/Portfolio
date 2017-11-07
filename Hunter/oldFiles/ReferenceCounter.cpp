#include "stdafx.h"
#include "ReferenceCounter.h"

ReferenceCounter::ReferenceCounter()
{
	_count = 0;
}

ReferenceCounter::ReferenceCounter(const ReferenceCounter & other)
{
	_count = 0;
}

ReferenceCounter & ReferenceCounter::operator=(const ReferenceCounter & other)
{
	return *this;
}

ReferenceCounter::~ReferenceCounter()
{
}

int32 ReferenceCounter::AddRef()
{
	_count++;
	return _count;
}

int32 ReferenceCounter::Release()
{
	_count--;
	return _count;
}

int32 ReferenceCounter::GetCount()
{
	return _count;
}
