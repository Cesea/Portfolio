#ifndef ENTITY_COMPONENT_STORAGE_H
#define ENTITY_COMPONENT_STORAGE_H

#include "ClassTypeId.h"

#include "Entity.h"
#include "Component.h"

class EntityComponentStorage
{
public:
	typedef std::array<std::unique_ptr<Component>, MAX_NUM_COMPONENT> ImplComponentArray;

	explicit EntityComponentStorage(uint32 entityAmount);

	EntityComponentStorage(const EntityComponentStorage&) = delete;
	EntityComponentStorage(EntityComponentStorage&&) = delete;
	EntityComponentStorage& operator=(const EntityComponentStorage&) = delete;
	EntityComponentStorage& operator=(EntityComponentStorage&&) = delete;


	void AddComponent(Entity& entity, Component* component, TypeID componentTypeId);

	void RemoveComponent(Entity& entity, TypeID componentTypeId);

	void RemoveAllComponents(Entity& entity);

	Component &GetComponent(const Entity& entity, TypeID componentTypeId) const;

	ComponentTypeList GetComponentTypeList(const Entity& entity) const;

	ComponentArray GetComponents(const Entity& entity) const;

	bool32 HasComponent(const Entity& entity, TypeID componentTypeId) const;

	void Resize(uint32 entityAmount);

	void Clear();

private:
	struct EntityComponents
	{
		EntityComponents() = default;

		EntityComponents(EntityComponents&& e) :
			components(std::move(e.components)),
			componentTypeList(std::move(e.componentTypeList))
		{
		}

		/// The components of an entity. The
		/// index of this array is the same as the TypeId
		/// of the component.
		ImplComponentArray components;

		/// A list of component types, which resembles
		/// what components an entity has
		ComponentTypeList componentTypeList;
	};

	/// All the components for every entity, which has
	/// an entity. The indices of this array is the same
	/// as the index component of an entity's ID.
	std::vector<EntityComponents> _componentEntries;

	ImplComponentArray &GetComponentsImpl(const Entity& e);
	const ImplComponentArray &GetComponentsImpl(const Entity& e) const;
};

#endif