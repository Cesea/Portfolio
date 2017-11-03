#ifndef ENTITY_H
#define ENTITY_H

#include <map>

#include "Component.h"

constexpr EntityID INVALID_ENTITY = 0;

class Entity
{
	typedef std::map<ComponentID, Component *> ComponentMap;
	typedef std::map<ComponentID, Component *>::iterator ComponentMapIter;

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
	T *GetComponent(ComponentID id)
	{
		T *result = nullptr;

		ComponentMapIter& iter = _components.find(T::ID);
		if (iter != _components.end())
		{
			result = static_cast<T *>(iter->second);
			return result;
		}
		return result;
	}
private:
	EntityID _id;
	ComponentMap _components;
};
#endif
