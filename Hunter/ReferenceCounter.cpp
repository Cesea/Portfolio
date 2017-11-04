#include "stdafx.h"
#include "ReferenceCounter.h"

ReferenceCounter::ReferenceCounter()
{
	_referenceCount = 0;
}

ReferenceCounter::ReferenceCounter(const ReferenceCounter & other)
{
	_referenceCount = other._referenceCount;
}

ReferenceCounter::~ReferenceCounter()
{
}

ReferenceCounter &ReferenceCounter::operator=(const ReferenceCounter & other)
{
	return *this;
}

int32 ReferenceCounter::AddRef()
{
	_referenceCount++;
	return _referenceCount;
}

int32 ReferenceCounter::Release()
{
	assert(_referenceCount > 0);
	_referenceCount--;
	return _referenceCount;
}

int32 ReferenceCounter::GetCount()
{
	return _referenceCount;
}
