#ifndef WORLD_H
#define WORLD_H

#include "EntityIDPool.h"
#include "EntityComponentStorage.h"

#include "Component.h"
#include "Entity.h"
#include "System.h"

#include <unordered_map>

class World
{
private:

	struct SystemDeleter
	{
		void operator() (BaseSystem* system) const;
	};

	/// Describes an array of Systems for storage within the world
	/// The index is the type ID of the system,
	/// thus systems of the same type can not be stored
	/// in the same World object.
	typedef  std::unordered_map<TypeID, std::unique_ptr<BaseSystem, SystemDeleter>>  SystemArray;

public:
	typedef std::vector<Entity> EntityArray;
	/// Default Constructor
	World();

	/// Constructs the world with a custom entity pool size
	/// \param entityPoolSize The amount of entities you wish to have pooled ready to use by default
	explicit World(std::size_t entityPoolSize);

	World(const World& world) = delete;
	World(World&& world) = delete;
	World& operator=(const World&) = delete;
	World& operator=(World&&) = delete;

	template <typename TSystem>
	void AddSystem(TSystem& system);

	template <typename TSystem>
	void RemoveSystem();

	template <typename TSystem>
	bool32 DoesSystemExist() const;

	template <typename TSystem>
	bool32 DoesSystemExist(const TSystem& system) const;

	void RemoveAllSystems();

	Entity CreateEntity();

	template <typename EntityTemplateFn, typename... Args>
	Entity CreateEntity(EntityTemplateFn function, Args&&... args)
	{
		Entity e = CreateEntity();
		function(e, std::forward<Args>(args)...);
		return e;
	}

	EntityArray CreateEntities(uint32 amount);

	void KillEntity(Entity& entity);

	void KillEntities(std::vector<Entity>& entities);

	void ActivateEntity(Entity& entity);

	void DeactivateEntity(Entity& entity);

	bool IsActivated(const Entity& entity) const;

	bool IsValid(const Entity& entity) const;

	void Refresh();

	void Clear();

	uint32 GetEntityCount() const;

	const EntityArray& GetEntities() const;

	Entity GetEntity(uint32 index);

private:
	SystemArray _systems;

	struct EntityAttributes
	{
		// todo: possibly move component storage to single attribute?
		// compare performance.
		struct Attribute
		{
			/// determines if the entity is activated
			bool activated;

			/// a bitset that resembles if the entity
			/// exists in a specific system.
			/// The index specifies what system, 0 resembles
			/// it is in the system, 1 is out of the system
			std::vector<bool32> systems;
		};

		explicit EntityAttributes(uint32 amountOfEntities) :
			componentStorage(amountOfEntities),
			attributes(amountOfEntities)
		{
		}

		/// A storage of all components that an entity has
		EntityComponentStorage componentStorage;

		/// the attributes of each entity
		std::vector<Attribute> attributes;

		/// Used on resize to allow room
		/// for more entities that require to be allocated
		/// \param amountOfEntities The amount of entities to resize for
		void Resize(std::size_t amountOfEntities)
		{
			componentStorage.Resize(amountOfEntities);
			attributes.resize(amountOfEntities);
		}

		void Clear()
		{
			componentStorage.Clear();
			attributes.clear();
		}
	};

	EntityAttributes _entityAttributes;

	struct EntityCache
	{
		EntityArray alive;

		EntityArray killed;

		EntityArray activated;

		EntityArray deactivated;

		void ClearTemp()
		{
			killed.clear();
			activated.clear();
			deactivated.clear();
		}
		void Clear()
		{
			alive.clear();
			ClearTemp();
		}
	};
	EntityCache _entityCache;
	EntityIDPool _entityIDPool;

	void CheckForResize(uint32 amountOfEntitiesToBeAllocated);
	void Resize(uint32 amount);

	void AddSystem(BaseSystem& system, TypeID systemTypeID);
	void RemoveSystem(TypeID systemTypeID);
	bool DoesSystemExist(TypeID systemTypeID) const;

	// to access components
	friend class Entity;
};

template <class TSystem>
void World::AddSystem(TSystem& system)
{
	static_assert(std::is_base_of<BaseSystem, TSystem>(), "Template argument does not inherit from BaseSystem");
	AddSystem(system, SystemTypeID<TSystem>());
}

template <class TSystem>
void World::RemoveSystem()
{
	static_assert(std::is_base_of<BaseSystem, TSystem>(), "Template argument does not inherit from BaseSystem");
	RemoveSystem(SystemTypeID<TSystem>());
}

template <class TSystem>
bool32 World::DoesSystemExist() const
{
	static_assert(std::is_base_of<BaseSystem, TSystem>(), "Template argument does not inherit from BaseSystem");
	return DoesSystemExist(SystemTypeID<TSystem>());
}

template <class TSystem>
bool32 World::DoesSystemExist(const TSystem& system) const
{
	static_assert(std::is_base_of<BaseSystem, TSystem>(), "Template argument does not inherit from BaseSystem");
	return system._world == this && DoesSystemExist<TSystem>();
}

#endif 
