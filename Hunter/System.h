#ifndef SYSTEM_H
#define SYSTEM_H

#include "BaseSystem.h"
#include "ClassTypeID.h"

template <class RequireList, class ExcludeList = Excludes<>>
class System : public BaseSystem
{
public:
	/// Default constructor
	System() :
		BaseSystem{ MakeFilter<RequireList, ExcludeList>() }
	{
	}
	virtual ~System() {}
};

template<class T>
TypeID SystemTypeID()
{
	return ClassTypeID<BaseSystem>::GetTypeID<T>();
}

#endif
