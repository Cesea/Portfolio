#ifndef BIT_FLAGS_H
#define BIT_FLAGS_H

#include "Typedefs.h"

#define FLAG(x) (1<<(x))
#define TEST_BIT(flag, bit) (((flag.value) & FLAG((bit))) !=0)
#define TEST_ALL(flag, set) (((flag.value) & (set)) == (set))
#define TEST_ANY(flag, set) (((flag.value) & (set)) !=0)
#define SET_BIT(flag, bit) ((flag.value) |= FLAG((bit)))
#define CLEAR_BIT(flag, bit) ((flag.value) &= ~FLAG((bit)))
#define SET_ALL(flag, set) ((flag.value) |= (set))
#define CLEAR_ALL(flag, set) ((flag.value) &= ~(set))

template <class T>
class BitFlags
{
public :
	T value;

	BitFlags();
	BitFlags(T data);
	BitFlags(const BitFlags &other);
	~BitFlags();

	BitFlags &operator= (const BitFlags &other);
	BitFlags &operator= (T t);

	operator T() const { return (value); }

	bool operator== (const BitFlags &other) const;
	bool operator!= (const BitFlags &other) const;

	void Set(T settings);
	void Clear();
	void SetFlags(T settings);
	void ClearFlags(T settings);
	void SetBit(int32 bit);
	void ClearBit(int32 bit);
	void SetBit(int32 bit, bool setting);

	bool IsEmpty() const;
	bool TestBit(int32 bit) const;
	bool TestFlags(T test) const;
	bool TestAny(T test) const;

	int32 TotalBits() const;
	int32 TotalSet() const;

private :
};

typedef BitFlags<uint8> Uint8Flags;
typedef BitFlags<uint16> Uint16Flags;
typedef BitFlags<uint32> Uint32Flags;

template<class T>
inline BitFlags<T>::BitFlags()
	:value(0)
{
}

template<class T>
inline BitFlags<T>::BitFlags(T data)
	:value(data)
{
}

template<class T>
inline BitFlags<T>::BitFlags(const BitFlags & other)
	:value(other.value)
{
}

template<class T>
inline BitFlags<T>::~BitFlags()
{
}

template<class T>
inline BitFlags<T> & BitFlags<T>::operator=(const BitFlags & other)
{
	value = other.value;
	return *this;
}

template<class T>
inline BitFlags<T> & BitFlags<T>::operator=(T t)
{
	value = t;
	return *this;
}

template<class T>
inline bool BitFlags<T>::operator==(const BitFlags & other) const
{
	return value == other.value;
}

template<class T>
inline bool BitFlags<T>::operator!=(const BitFlags & other) const
{
	return value != other.value;
}

template<class T>
inline void BitFlags<T>::Set(T settings)
{
	value = settings;
}

template<class T>
inline void BitFlags<T>::Clear()
{
	value = 0;
}

template<class T>
inline void BitFlags<T>::SetFlags(T settings)
{
	value |= settings;
}

template<class T>
inline void BitFlags<T>::ClearFlags(T settings)
{
	value &= ~settings;
}

template<class T>
inline void BitFlags<T>::SetBit(int32 bit)
{
	value |= (1 << bit);
}

template<class T>
inline void BitFlags<T>::ClearBit(int32 bit)
{
	value &= (~(1 << bit));
}

template<class T>
inline void BitFlags<T>::SetBit(int32 bit, bool setting)
{
	if (setting)
	{
		value |= (1 << bit);
	}
	else
	{
		ClearBit(bit);
	}
}

template<class T>
inline bool BitFlags<T>::IsEmpty() const
{
	return (value == static_cast<T>(0));
}

template<class T>
inline bool BitFlags<T>::TestBit(int32 bit) const
{
	return (value & (1 << bit));
}

template<class T>
inline bool BitFlags<T>::TestFlags(T test) const
{
	return ((value & test) == test);
}

template<class T>
inline bool BitFlags<T>::TestAny(T test) const
{
	return (value & test);
}

template<class T>
inline int32 BitFlags<T>::TotalBits() const
{
	return (sizeof(T) * 3);
}

template<class T>
inline int32 BitFlags<T>::TotalSet() const
{
	int32 count = 0;
	int32 total = TotalBits();

	T testValue = value;

	for (int32 i = total; i; --i)
	{
		count += (testValue & 1);
		testValue >>= 1;
	}
	return count;
}

#endif
