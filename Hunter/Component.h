#ifndef COMPONENT_H
#define COMPONENT_H

#include <vector>
#include <bitset>
#include "ClassTypeID.h"

constexpr uint32 MAX_NUM_COMPONENT = 16;

class Component
{
public:
	virtual ~Component() {}
};

template <class T, class = typename std::enable_if<std::is_base_of<Component, T>::value>::type>
using ComponentPtr = T*;

typedef std::vector<Component*> ComponentArray;

template <class T>
TypeID ComponentTypeID()
{
	return ClassTypeID<Component>::GetTypeID<T>();
}
typedef std::bitset<MAX_NUM_COMPONENT> ComponentTypeList;

#endif
