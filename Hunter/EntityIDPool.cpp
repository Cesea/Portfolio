#include "stdafx.h"
#include "EntityIDPool.h"

EntityIDPool::EntityIDPool(uint32 poolSize)
	:_defaultPoolSize(poolSize), _counts(poolSize), _nextID(0)
{
}

Entity::ID EntityIDPool::Create()
{
	Entity::ID id;
	if (!_freeList.empty())
	{
		id = _freeList.back();
		_freeList.pop_back();
	}
	else
	{
		id.index = _nextID++;
		// an ID given out cannot have a counter of 0. 
		// 0 is an "invalid" counter, thus we must update
		// the counter within the pool for the corresponding entity
		_counts[id.index] = id.counter = 1;
	}
	return id;
}

void EntityIDPool::Remove(Entity::ID id)
{
	auto &counter = _counts[id.index];
	++counter; // increment the counter in the cache
	_freeList.push_back(BuildHandle(id.index, counter)); // add the ID to the freeList
}

Entity::ID EntityIDPool::Get(uint32 index) const
{
	if (index < _counts.size())
	{
		return BuildHandle(index, _counts[index]);
	}
	else
	{
		return BuildHandle(index, 0);
	}
}

bool32 EntityIDPool::IsValid(Entity::ID id) const
{
	if (id.index > _counts.size())
	{
		return false;
	}
	else
	{
		return (id.counter == _counts[id.index]) && (id.counter > 0);
	}
}

uint32 EntityIDPool::GetSize() const
{
	return _counts.size();
}

void EntityIDPool::Resize(uint32 amount)
{
	_counts.resize(amount);
}

void EntityIDPool::Clear()
{
	_counts.clear();
	_freeList.clear();
	_nextID = 0;
}

uint32 EntityIDPool::GetNextID() const
{
	return _nextID;
}

const Entity::ID EntityIDPool::BuildHandle(uint32 index, uint32 count) const
{
	Entity::ID result(index, count);
	return result;
}
