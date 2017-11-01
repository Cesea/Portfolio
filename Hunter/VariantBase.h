#ifndef VARIANT_BASE_H
#define VARIANT_BASE_H

class MetaData;

class VariantBase
{
public :

	const MetaData *Meta() const;

	template <typename T>
	T &GetValue();

	template <typename T> 
	const T &GetValue() const;

	void *Data() const;

	void Serialize(std::ostream &os) const;

protected :
	VariantBase();
	VariantBase(const MetaData *meta, void *data);
	const MetaData *_meta;
	void *_data;
};

template<typename T>
inline T & VariantBase::GetValue()
{
	return *reinterpret_cast<T *>(_data);
}

template<typename T>
inline const T & VariantBase::GetValue() const
{
	return *reinterpret_cast<T *>(_data);
}

#endif
