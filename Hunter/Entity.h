#ifndef ENTITY_H
#define ENTITY_H

#include <map>

#include "Component.h"

#include "Transform.h"

constexpr EntityID INVALID_ENTITY = 0;

class Entity
{
	typedef std::map<ComponentFamilyID, Component *> ComponentTable;
	typedef std::map<ComponentFamilyID, Component *>::iterator ComponentTableIter;

public:
	explicit Entity(EntityID id);
	~Entity();

	HRESULT PrevInit();
	HRESULT Init();
	void Destroy();

	EntityID GetId() const { return _id; }

	void AddComponent(Component *pComponent);
	void RemoveComponent(Component *pComponent);

	bool HasComponent(ComponentID id);

	template<typename T>
	T *GetComponent(ComponentFamilyID id)
	{
		T *result = nullptr;

		ComponentTableIter& iter = _components.find(T::ID);
		if (iter != _components.end())
		{
			result = static_cast<T *>(iter->second);
			return result;
		}
		return result;
	}
private:
	EntityID _id;
	ComponentTable _components;
};
#endif
