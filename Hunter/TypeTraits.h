#ifndef TYPE_TRAITS_H
#define TYPE_TRAITS_H

#include <type_traits>

//Bool To Type
template <bool b>
struct BoolToType
{};

typedef BoolToType<true> PODType;
typedef BoolToType<false> NonPODType;


//Type And Count
//배열로 넘겼을때 자료형과 갯수를 저장한다
template <class T> 
struct TypeAndCount
{
};

template <class T, size_t N>
struct TypeAndCount<T[N]>
{
	typedef T Type;
	static const size_t Count = N;
};


#endif