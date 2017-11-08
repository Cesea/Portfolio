#ifndef BASE_SYSTEM_H
#define BASE_SYSTEM_H

#include <vector>
#include "Entity.h"

#include "Filter.h"

class World;

class BaseSystem
{
public:

	BaseSystem(const Filter& filter);

	virtual ~BaseSystem() {}

	const Filter& GetFilter() const;

	World& GetWorld() const;

	const std::vector<Entity>& GetEntities() const;

protected :

	virtual void Initialize() {}

	virtual void OnEntityAdded(Entity& entity) {}

	virtual void OnEntityRemoved(Entity& entity) {}

	void Add(Entity& entity);

	void Remove(Entity& entity);

	void SetWorld(World& world);
	World* _world;

	Filter _filter;

	std::vector<Entity> _entities;
	friend World;
};

#endif