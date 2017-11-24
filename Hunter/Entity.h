#ifndef ENTITY_H
#define ENTITY_H

#include "ClassTypeId.h"

#include "Component.h"

constexpr uint32 ENTITY_ID_INDEX_BIT_COUNT = 16;
constexpr uint32 ENTITY_ID_COUNTER_BIT_COUNT = 16;

constexpr uint32 DEFAULT_ENTITY_SIZE = 1024;

class World;

class Entity
{
public:
	struct ID
	{
		ID() :
			index(0),
			counter(0)
		{
		}

		ID(uint32 index, uint32 counter)
			: index(index), counter(counter)
		{
			this->index = index;
			this->counter = counter;
		}

		inline uint32 Value() const
		{
			return (counter << ENTITY_ID_COUNTER_BIT_COUNT) | index;
		}

		void Clear() { index = counter = 0; }

		bool IsNull() const { return Value() == 0; }

		uint32 index : ENTITY_ID_INDEX_BIT_COUNT;
		uint32 counter : ENTITY_ID_COUNTER_BIT_COUNT;

		bool32 operator== (const ID &other) const
		{
			return Value() == other.Value();
		}


	};

	Entity();

	Entity(World& world, ID id);

	Entity(const Entity &other)
	{
		_id = other._id;;
		_world = other._world;
	}
	Entity(Entity&&) = default;
	Entity& operator=(const Entity &other)
	{
		_id = other._id;;
		_world = other._world;
		return *this;
	}
	Entity& operator=(Entity&&) = default;

	bool IsValid() const;

	const ID& GetID() const;
	ID &GetID();

	World& GetWorld() const;

	bool32 IsActivated() const;

	void Activate();

	void Deactivate();

	void Kill();

	template <typename T, typename... Args>
	T& AddComponent(Args&&... args);

	template <typename T>
	void RemoveComponent();

	void RemoveAllComponents();

	template <typename T>
	T& GetComponent() const;

	template <typename T>
	bool32 HasComponent() const;

	/// \return All the components the Entity has
	ComponentArray GetComponents() const;

	/// \return A component type list, which resembles the components
	/// this entity has attached to it
	ComponentTypeList GetComponentTypeList() const;

	/// Comparison operator
	bool32 operator==(const Entity& entity) const;
	bool32 operator!=(const Entity& entity) const { return !operator == (entity); }

private:
	void AddComponent(Component* component, TypeID componentTypeID);
	void RemoveComponent(TypeID componentTypeID);
	Component& GetComponent(TypeID componentTypeID) const;
	bool32 HasComponent(TypeID componentTypeID) const;

	ID _id;

	World* _world;
};

template <typename T, typename... Args>
T& Entity::AddComponent(Args&&... args)
{
	static_assert(std::is_base_of<Component, T>(), "T is not a component, cannot add T to entity");
	// TODO: align components by type
	auto component = new T{ std::forward<Args>(args)... };
	AddComponent(component, ComponentTypeID<T>());
	return *component;
}

template <typename T>
void Entity::RemoveComponent()
{
	static_assert(std::is_base_of<Component, T>(), "T is not a component, cannot remove T from entity");
	RemoveComponent(ComponentTypeID<T>());
}

template <typename T>
T& Entity::GetComponent() const
{
	static_assert(std::is_base_of<Component, T>(), "T is not a component, cannot retrieve T from entity");
	return static_cast<T&>(GetComponent(ComponentTypeID<T>()));
}

template <typename T>
bool32 Entity::HasComponent() const
{
	static_assert(std::is_base_of<Component, T>(), "T is not a component, cannot determine if entity has T");
	return HasComponent(ComponentTypeID<T>());
}

#endif // ANAX_ENTITY_HPP
