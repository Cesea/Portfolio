#ifndef FILTER_H
#define FILTER_H

#include <type_traits>

#include "Component.h"

#include "ClassTypeId.h"

template <class... Args>
struct TypeList { };

struct BaseRequires { };
struct BaseExcludes { };

struct Filter
{
public:

	Filter(ComponentTypeList requires, ComponentTypeList excludes) :
		_requires(requires), _excludes(excludes)
	{ }

	bool32 DoesPassFilter(const ComponentTypeList& typeList) const;

private:

	ComponentTypeList _requires;
	ComponentTypeList _excludes;
};

template <class... Args>
static ComponentTypeList Types(TypeList<Args...> typeList) { return ComponentTypeList(); }

template <class T, class... Args>
static ComponentTypeList Types(TypeList<T, Args...> typeList)
{
	static_assert(std::is_base_of<Component, T>::value, "Invalid component");
	return ComponentTypeList().set(ComponentTypeID<T>()) | Types(TypeList<Args...>());
}

template <class RequireList, class ExcludeList>
Filter MakeFilter()
{
	static_assert(std::is_base_of<BaseRequires, RequireList>::value, "RequireList is not a requirement list");
	static_assert(std::is_base_of<BaseExcludes, ExcludeList>::value, "ExcludeList is not an excludes list");
	return Filter{ Types(RequireList{}), Types(ExcludeList{}) };
}

/// Requires a set of components
template <class... Args>
struct Requires : public TypeList<Args...>, public BaseRequires {};

/// Excludes a set of components
template <class... Args>
struct Excludes : public TypeList<Args...>, public BaseExcludes {};

#endif 
