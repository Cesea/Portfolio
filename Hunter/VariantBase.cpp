#include "stdafx.h"
#include "VariantBase.h"

const MetaData * VariantBase::Meta() const
{
	return _meta;
}

void * VariantBase::Data() const
{
	return _data;
}

void VariantBase::Serialize(std::ostream & os) const
{
	_meta->Serialize(os, RefVariant(_meta, _data));
}

VariantBase::VariantBase()
	:_meta(nullptr), _data(nullptr)
{
}

VariantBase::VariantBase(const MetaData * meta, void * data)
	:_meta(meta), _data(data)
{
}
