#include "stdafx.h"
#include "World.h"

#include "ContainerCheck.h"

World::World()
	:World(DEFAULT_ENTITY_SIZE)
{
}

World::World(std::size_t entityPoolSize)
	: _entityIDPool(entityPoolSize),
		_entityAttributes(entityPoolSize)
{
}


void World::RemoveAllSystems()
{
	_systems.clear();
}

Entity World::CreateEntity()
{
	CheckForResize(1);

	_entityCache.alive.emplace_back(*this, _entityIDPool.Create());
	return _entityCache.alive.back();
}

World::EntityArray World::CreateEntities(uint32 amount)
{
	std::vector<Entity> temp;
	temp.reserve(amount);

	CheckForResize(amount);

	for (uint32 i = 0; i < amount; ++i)
	{
		Entity e{ *this, _entityIDPool.Create() };
		_entityCache.alive.push_back(e);
		temp.push_back(e);
	}

	return temp;
}

void World::KillEntity(Entity & entity)
{
	DeactivateEntity(entity);

	// now kill the entity (add it to the killed cache)
	_entityCache.killed.push_back(entity);
}

void World::KillEntities(std::vector<Entity>& entities)
{
	for (auto &entity : entities)
	{
		KillEntity(entity);
	}
}

void World::ActivateEntity(Entity & entity)
{
	//Assert(isValid(entity), "invalid entity tried to be activated");

	IsValid(entity);
	_entityCache.activated.push_back(entity);
}

void World::DeactivateEntity(Entity & entity)
{
	//ANAX_ASSERT(isValid(entity), "invalid entity tried to be deactivated");

	IsValid(entity);
	_entityCache.deactivated.push_back(entity);
}

bool World::IsActivated(const Entity & entity) const
{
	if (IsValid(entity))
		return _entityAttributes.attributes[entity.GetID().index].activated;
	else
		return false;
}

bool World::IsValid(const Entity & entity) const
{
	return _entityIDPool.IsValid(entity.GetID());
}

void World::Refresh()
{
	// go through all the activated entities from last call to refresh
	for (auto& entity : _entityCache.activated)
	{
		auto& attribute = _entityAttributes.attributes[entity.GetID().index];
		attribute.activated = true;

		// loop through all the systems within the world
		for (auto &system : _systems)
		{
			auto systemIndex = system.first;

			// if the entity passes the filter the system has and is not already part of the system
			if (system.second->GetFilter().DoesPassFilter(
				_entityAttributes.componentStorage.GetComponentTypeList(entity)))
			{
				if (attribute.systems.size() <= systemIndex || !attribute.systems[systemIndex])
				{
					system.second->Add(entity); // add it to the system

					EnsureCapacity(attribute.systems, systemIndex);
					attribute.systems[systemIndex] = true;
				}
			}
			// otherwise if the entity is within the system 
			// and is not relevant to the system anymore...
			// note: the entity has already failed the filter
			else if (attribute.systems.size() > systemIndex && attribute.systems[systemIndex])
			{
				// duplicate code (1)
				system.second->Remove(entity);
				attribute.systems[systemIndex] = false;
			}
		}
	}


	// go through all the deactivated entities from last call to refresh
	for (auto& entity : _entityCache.deactivated)
	{
		auto& attribute = _entityAttributes.attributes[entity.GetID().index];
		attribute.activated = false;

		// loop through all the systems within the world
		for (auto& system : _systems)
		{
			auto systemIndex =  system.first;
			if (attribute.systems.size() <= systemIndex) continue;

			if (attribute.systems[systemIndex])
			{
				// duplicate code ...(1)
				 system.second->Remove(entity);
				attribute.systems[systemIndex] = false;
			}
		}
	}

	// go through all the killed entities from last call to refresh
	for (auto& entity : _entityCache.killed)
	{
		// remove the entity from the alive array
		_entityCache.alive.erase(
			std::remove(_entityCache.alive.begin(), _entityCache.alive.end(), entity), _entityCache.alive.end());

		// destroy all the components it has
		_entityAttributes.componentStorage.RemoveAllComponents(entity);

		// remove it from the id pool
		_entityIDPool.Remove(entity.GetID());
	}

	// clear the temp cache
	_entityCache.ClearTemp();
}

void World::Clear()
{
	RemoveAllSystems(); // remove the systems

	   // clear the attributes for all the entities
	_entityAttributes.Clear();

	// clear the entity cache
	_entityCache.Clear();

	// clear the id pool
	_entityIDPool.Clear();
}

uint32 World::GetEntityCount() const
{
	return _entityCache.alive.size();
}

const World::EntityArray & World::GetEntities() const
{
	return _entityCache.alive;
}

Entity World::GetEntity(uint32 index)
{
	return Entity{ *this, _entityIDPool.Get(index) };
}

void World::SystemDeleter::operator()(BaseSystem * system) const
{
	system->_world = nullptr;
	system->_entities.clear();
}

void World::CheckForResize(uint32 amountOfEntitiesToBeAllocated)
{
	uint32 newSize = GetEntityCount() + amountOfEntitiesToBeAllocated;
	if (newSize > _entityIDPool.GetSize())
	{
		Resize(newSize);
	}
}

void World::Resize(uint32 amount)
{
	_entityIDPool.Resize(amount);
	_entityAttributes.Resize(amount);
}

void World::AddSystem(BaseSystem & system, TypeID systemTypeID)
{
	_systems[systemTypeID].reset(&system);

	system._world = this;
	system.Initialize();
}

void World::RemoveSystem(TypeID systemTypeID)
{
	_systems.erase(systemTypeID);
}

bool World::DoesSystemExist(TypeID systemTypeID) const
{
	return _systems.find(systemTypeID) != _systems.end();
}


//Scene Entity Description file Format
//int32 : numEntitySaved
//for (numEntitySaved)
//{
//		EntitySaveInfo : saveInfo
//}
bool World::SaveEntitiesInWorld(const std::string & fileName)
{
	DataPackage package;

	int32 numEntityToSave = 0;
	for (uint32 i = 0; i < _entityCache.alive.size(); ++i)
	{
		Entity entity = _entityCache.alive[i];
		if (entity.HasComponent<RenderComponent>())
		{
			numEntityToSave++;
		}
	}
	package.Create(sizeof(int32) + (sizeof(EntitySaveInfo) * numEntityToSave) );
	package.WriteInt32(numEntityToSave);

	EntitySaveInfo saveInfo;
	ZeroMemory(&saveInfo, sizeof(EntitySaveInfo));

	std::string resourceName;

	for (uint32 i = 0; i < _entityCache.alive.size(); ++i)
	{
		Entity entity = _entityCache.alive[i];

		if (entity.HasComponent<RenderComponent>())
		{
			RenderComponent &refRender = entity.GetComponent<RenderComponent>();
			saveInfo._archeType = refRender._arche;
			switch (refRender._type)
			{
			case RenderComponent::Type::eBuffer:
			{
				//버퍼로 저장을 하면 안돌아 갈거다.......
				Assert(false);
			}break;
			case RenderComponent::Type::eStatic:
			{
				resourceName = VIDEO->GetStaticXMeshName(refRender._static);
				Assert(resourceName.length() < MAX_FILE_NAME);
				strncpy(saveInfo._resourceName, resourceName.c_str(), resourceName.length());
			}break;
			case RenderComponent::Type::eSkinned:
			{
				video::AnimationInstance *pAnimation = VIDEO->GetAnimationInstance(refRender._skinned);
				resourceName = VIDEO->GetSkinnedXMeshName(pAnimation->_skinnedMeshHandle);
				Assert(resourceName.length() < MAX_FILE_NAME);
				strncpy(saveInfo._resourceName, resourceName.c_str(), resourceName.length());
			}break;
			}
			saveInfo._resourceName;
			saveInfo._position = entity.GetComponent<TransformComponent>()._position;

			package.WriteAs<EntitySaveInfo>(saveInfo);

			ZeroMemory(&saveInfo, sizeof(EntitySaveInfo));
		}
	}

	package.Save(fileName.c_str());
	return true;
}
