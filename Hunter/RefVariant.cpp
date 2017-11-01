#include "stdafx.h"
#include "RefVariant.h"

RefVariant::RefVariant(const RefVariant & other)
	:VariantBase(other.Meta(), other.Data())
{
}

RefVariant::RefVariant(const Variant & other)
	:VariantBase(other.Meta(), other.Data())
{
}

RefVariant::RefVariant(const MetaData * meta, void * data)
	: VariantBase(meta, data)
{
}

RefVariant::RefVariant()
	:VariantBase(nullptr, nullptr)
{
}

RefVariant & RefVariant::operator=(const RefVariant & other)
{
	_meta = other._meta;
	_data = other._data;
	return *this;
}

RefVariant & RefVariant::operator=(const Variant & other)
{
	_meta = other.Meta();
	_data = other.Data();
	return *this;
}
