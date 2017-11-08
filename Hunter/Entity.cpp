#include "stdafx.h"
#include "Entity.h"

#include "World.h"

Entity::Entity()
	:_world(nullptr)
{
}

Entity::Entity(World & world, ID id)
	:_world(&world), _id(id)
{
}

bool Entity::IsValid() const
{
	if (_world == nullptr)
	{
		return false;
	}

	return _world->IsValid(*this);
}

const Entity::ID & Entity::GetID() const
{
	return _id;
}

World & Entity::GetWorld() const
{
	Assert(_world);

	return *_world;
}

bool32 Entity::IsActivated() const
{
	return _world->IsActivated(*this);
}

void Entity::Activate()
{
	GetWorld().ActivateEntity(*this);
}

void Entity::Deactivate()
{
	GetWorld().DeactivateEntity(*this);
}

void Entity::Kill()
{
	GetWorld().KillEntity(*this);
}

void Entity::RemoveAllComponents()
{
	GetWorld()._entityAttributes.componentStorage.RemoveAllComponents(*this);
}

ComponentArray Entity::GetComponents() const
{
	return GetWorld()._entityAttributes.componentStorage.GetComponents(*this);
}

ComponentTypeList Entity::GetComponentTypeList() const
{
	return GetWorld()._entityAttributes.componentStorage.GetComponentTypeList(*this);
}

bool32 Entity::operator==(const Entity & entity) const
{
	return (_id == entity._id) && (_world == entity._world);
}

void Entity::AddComponent(Component * component, TypeID componentTypeID)
{
	GetWorld()._entityAttributes.componentStorage.AddComponent(*this, component, componentTypeID);
}

void Entity::RemoveComponent(TypeID componentTypeID)
{
	GetWorld()._entityAttributes.componentStorage.RemoveComponent(*this, componentTypeID);
}

Component & Entity::GetComponent(TypeID componentTypeID) const
{
	return GetWorld()._entityAttributes.componentStorage.GetComponent(*this, componentTypeID);
}

bool32 Entity::HasComponent(TypeID componentTypeID) const
{
	return GetWorld()._entityAttributes.componentStorage.HasComponent(*this, componentTypeID);
}
