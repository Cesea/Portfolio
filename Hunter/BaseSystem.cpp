#include "stdafx.h"
#include "BaseSystem.h"

#include "World.h"

BaseSystem::BaseSystem(const Filter & filter)
	:_filter(filter), _world(nullptr)
{
}
const Filter & BaseSystem::GetFilter() const
{
	return _filter;
}

World & BaseSystem::GetWorld() const
{
	return *_world;
}

const std::vector<Entity>& BaseSystem::GetEntities() const
{
	return _entities;
}

void BaseSystem::Add(Entity & entity)
{
	_entities.push_back(entity);
	OnEntityAdded(entity);
}

void BaseSystem::Remove(Entity & entity)
{
	_entities.erase(std::remove(_entities.begin(), _entities.end(), entity), _entities.end());
	OnEntityRemoved(entity);
}

void BaseSystem::SetWorld(World & world)
{
	_world = &world;
	Initialize();
}
