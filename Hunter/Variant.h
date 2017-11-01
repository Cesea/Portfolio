#ifndef VARIANT_H
#define VARIANT_H

#include "VariantBase.h"

class Variant : public VariantBase
{
public :
	template <typename T>
	Variant(const T &value);
	Variant(const MetaData *meta, void *data);
	Variant();

	Variant &operator= (const Variant &other);
	template <typename T>
	Variant &operator = (const T &rhs);
};



template<typename T>
inline Variant::Variant(const T & value)
	:VariantBase(META_TYPE(T), nullptr)
{
	_data = _meta->NewCopy(&value);
}

template<typename T>
inline Variant & Variant::operator=(const T & rhs)
{
	if (_meta != META_TYPE(T))
	{
		assert(META_TYPE(T));

		_meta->Delete(_data);
		_meta = META_TYPE(T);
		_data = _meta->NewCopy(&rhs);
	}
	else
	{
		_data = _meta->Copy(&rhs);
	}
	return *this;
}

#endif
