#include "stdafx.h"
#include "EntityComponentStorage.h"

EntityComponentStorage::EntityComponentStorage(uint32 entityAmount)
	:_componentEntries(entityAmount)
{
}

void EntityComponentStorage::AddComponent(Entity & entity, Component * component, TypeID componentTypeId)
{
	auto index = entity.GetID().index;
	auto &componentData = _componentEntries[index];

	componentData.components[componentTypeId].reset(component);
	componentData.componentTypeList[componentTypeId] = true;
}

void EntityComponentStorage::RemoveComponent(Entity & entity, TypeID componentTypeId)
{
	auto index = entity.GetID().index;
	auto &componentData = _componentEntries[index];

	componentData.components[componentTypeId].reset();
	componentData.componentTypeList[componentTypeId] = false;
}

void EntityComponentStorage::RemoveAllComponents(Entity & entity)
{
	auto index = entity.GetID().index;
	auto& componentData = _componentEntries[index];

	for (auto &c : componentData.components)
	{
		c.reset();
	}
	componentData.componentTypeList.reset();
}

Component & EntityComponentStorage::GetComponent(const Entity & entity, TypeID componentTypeID) const
{
	//ANAX_ASSERT(entity.isValid() && hasComponent(entity, componentTypeId), "Entity is not valid or does not contain component");
	return *GetComponentsImpl(entity)[componentTypeID];
}

ComponentTypeList EntityComponentStorage::GetComponentTypeList(const Entity & entity) const
{
	return _componentEntries[entity.GetID().index].componentTypeList;
}

ComponentArray EntityComponentStorage::GetComponents(const Entity & entity) const
{
	//ANAX_ASSERT(entity.isValid(), "invalid entity cannot retrieve components, as it has none");

	auto &componentsToConvert = GetComponentsImpl(entity);

	ComponentArray temp;
	temp.reserve(componentsToConvert.size());

	for (auto &component : componentsToConvert)
	{
		temp.emplace_back(component.get());
	}

	return temp;
}

bool32 EntityComponentStorage::HasComponent(const Entity & entity, TypeID componentTypeID) const
{
	auto &components = GetComponentsImpl(entity);
	return components.size() > componentTypeID && components[componentTypeID] != nullptr;
}

EntityComponentStorage::ImplComponentArray &EntityComponentStorage::GetComponentsImpl(const Entity & e)
{
	return _componentEntries[e.GetID().index].components;
}

const EntityComponentStorage::ImplComponentArray &EntityComponentStorage::GetComponentsImpl(const Entity & e) const
{
	return _componentEntries[e.GetID().index].components;
}

void EntityComponentStorage::Resize(uint32 entityAmount)
{
	_componentEntries.resize(entityAmount);
}

void EntityComponentStorage::Clear()
{
	_componentEntries.clear();
}
