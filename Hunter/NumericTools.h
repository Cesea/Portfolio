#ifndef NUMERIC_TOOLS_H
#define NUMERIC_TOOLS_H

#include "Typedefs.h"

// bit testing
template<class T> int HighestBitSet(T input);
template<class T> int LowestBitSet(T input);

// power of two functions
template<class T> bool IsPowerOfTwo(T input);
template<class T> T NearestPowerOfTwo(T input);
template<class T> T CeilingPowerOfTwo(T input);
template<class T> T FloorPowerOfTwo(T input);

template <class T> T raiseToPower(T value, T power);


template<class T>
inline int HighestBitSet(T input)
{
	register int result;
	assert(input); // zero is invalid input!
	assert(sizeof(T) == 4); // 32bit data only!
	_asm bsr eax, input
	_asm mov result, eax
	return result;
}

template <>
inline int HighestBitSet(uint8 input)
{
	register int result;
	_asm mov dl, input // copy into a 32bit reg
	_asm and edx, 0xff // keep only the bits we want
	_asm bsr eax, edx // perform the scan
	_asm mov result, eax
	return result;
}
template <>
inline int HighestBitSet(int8 input)
{
	register int result;
	_asm mov dl, input // copy into a 32bit reg
	_asm and edx, 0xff // keep only the bits we want
	_asm bsr eax, edx // perform the scan
	_asm mov result, eax
	return result;
}
template <>
inline int HighestBitSet(uint16 input)
{
	register int result;
	_asm mov dx, input // copy into a 32bit reg
	_asm and edx, 0xffff // keep only the bits we want
	_asm bsr eax, edx // perform the scan
	_asm mov result, eax
	return result;
}
template <>
inline int HighestBitSet(int16 input)
{
	register int result;
	_asm mov dx, input // copy into a 32bit reg
	_asm and edx, 0xffff // keep only the bits we want
	_asm bsr eax, edx // perform the scan
	_asm mov result, eax
	return result;
}
template<class T>
inline int LowestBitSet(T input)
{
	register int result;
	_asm bsf eax, input
	_asm mov result, eax
	return result;
}
template<>
inline int LowestBitSet(uint8 input)
{
	register int result;
	_asm mov dl, input // copy into a 32bit reg
	_asm and edx, 0xff // keep only the bits we want
	_asm bsf eax, edx // perform the scan
	_asm mov result, eax
	return result;
}
template<>
inline int LowestBitSet(int8 input)
{
	register int result;
	_asm mov dl, input // copy into a 32bit reg
	_asm and edx, 0xff // keep only the bits we want
	_asm bsf eax, edx // perform the scan
	_asm mov result, eax
	return result;
}

template<>
inline int LowestBitSet(uint16 input)
{
	register int result;
	_asm mov dx, input // copy into a 32bit reg
	_asm and edx, 0xffff // keep only the bits we want
	_asm bsf eax, edx // perform the scan
	_asm mov result, eax
	return result;
}
template<>
inline int LowestBitSet(int16 input)
{
	register int result;
	_asm mov dx, input // copy into a 32bit reg
	_asm and edx, 0xffff // keep only the bits we want
	_asm bsf eax, edx // perform the scan
	_asm mov result, eax
	return result;
}

template<class T>
inline bool IsPowerOfTwo(T input)
{
	return (input > 0 && HighestBitSet(input) == LowestBitSet(input));
}

template<class T>
inline T NearestPowerOfTwo(T input)
{
	// the least possible power-of-two value is 1
	if (input <= 1) return 1;

	int highestBit = HighestBitSet(input);
	int roundingTest = input & (1 << (highestBit - 1));
	if (roundingTest) ++highestBit;
	return static_cast<T>(1 << highestBit);
}

template<class T>
inline T CeilingPowerOfTwo(T input)
{
	// the least possible power-of-two value is 1
	if (input <= (T)1) return 1;

	int highestBit = HighestBitSet(index);
	int mask = input & ((1 << highestBit) - 1);
	highestBit += mask & 1;
	return static_cast<T>(1 << highestBit);
}

template<class T>
inline T FloorPowerOfTwo(T input)
{
	if (input <= (T)1) return 1;

	int highestBit = HighestBitSet(input);
	return static_cast<T>(1 << highestBit);
}

template<class T>
inline T raiseToPower(T value, T power)
{
	return (value^power);
}

#endif
