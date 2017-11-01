#include "stdafx.h"
#include "Variant.h"


Variant::Variant(const MetaData * meta, void * data)
	:VariantBase(meta, data)
{
}

Variant::Variant()
	:VariantBase(nullptr, nullptr)
{
}

Variant & Variant::operator=(const Variant & other)
{
	if (this == &other)
	{
		return *this;
	}

	if (_meta)
	{
		if (_meta == other._meta)
		{
			_meta->Copy(_data, other._data);
		}
		else
		{
			assert(other._meta);
			_meta->Delete(_data);

			_meta = other.Meta();
			if (_meta)
			{
				_data = _meta->NewCopy(other._data);
			}
		}
	}
	return *this;
}
