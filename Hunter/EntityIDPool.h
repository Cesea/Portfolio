#ifndef ENTITY_ID_POOL_H
#define ENTITY_ID_POOL_H

#include "Entity.h"

class EntityIDPool
{
public:
	explicit EntityIDPool(uint32 poolSize);

	EntityIDPool(const EntityIDPool&) = delete;
	EntityIDPool(EntityIDPool&&) = delete;
	EntityIDPool& operator=(const EntityIDPool&) = delete;
	EntityIDPool& operator=(EntityIDPool&&) = delete;

	Entity::ID Create();
	void Remove(Entity::ID id);
	Entity::ID Get(uint32 index) const;
	bool32 IsValid(Entity::ID id) const;
	uint32 GetSize() const;
	void Resize(uint32 amount);
	void Clear();

	uint32 GetNextID() const;

	const Entity::ID BuildHandle(uint32 index, uint32 count) const;

private:
	uint32 _defaultPoolSize;

	uint32 _nextID;

	std::vector<Entity::ID> _freeList;

	std::vector<uint32> _counts;
};


#endif