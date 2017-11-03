#ifndef COMPONENT_H
#define COMPONENT_H

constexpr ComponentFamilyID COMMAND_FAMILY_ID = 1;
constexpr ComponentFamilyID HEALTH_FAMILY_ID= 2;
constexpr ComponentFamilyID PHYSICS_FAMILY_ID= 3;
constexpr ComponentFamilyID VISUAL_FAMILY_ID= 4;

constexpr ComponentID INVALID_COMPONENT = 0;

class Entity;

//enum ComponentType
//{
//};

class Component
{
public:
	Component() {}
	virtual ~Component() {}

	virtual bool Init(Entity *pOwner);
	virtual bool Release() {};

	virtual void Update(float deltaTime) {};

	virtual ComponentFamilyID GetFamilyID() const = 0;
	virtual ComponentID GetComponentID() const = 0;

protected :
	Entity *_pOwner{};
};

#endif