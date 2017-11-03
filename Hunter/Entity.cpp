#include "stdafx.h"
#include "Entity.h"

Entity::Entity(EntityID id)
{
}

Entity::~Entity()
{
}

HRESULT Entity::PrevInit()
{
	return E_NOTIMPL;
}

HRESULT Entity::Init()
{
	return E_NOTIMPL;
}

void Entity::Destroy()
{
}

void Entity::AddComponent(Component * pComponent)
{
}

void Entity::RemoveComponent(Component * pComponent)
{
}

bool Entity::HasComponent(ComponentID id)
{
	return false;
}
