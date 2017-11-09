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
//�迭�� �Ѱ����� �ڷ����� ������ �����Ѵ�
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