#ifndef CLASS_TYPE_ID_H
#define CLASS_TYPE_ID_H

#include <atomic>

#include "Typedefs.h"

typedef uint32 TypeID;

template <typename T>
class ClassTypeID
{
public:
	template <typename T>
	static TypeID GetTypeID()
	{
		static const TypeID id = _nextTypeID++;
		return id;
	}
private:
	static std::atomic<TypeID> _nextTypeID;
};

template <typename T>
std::atomic<TypeID> ClassTypeID<T>::_nextTypeID( 0 );

#endif