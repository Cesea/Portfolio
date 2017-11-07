#ifndef COMPONENT_H
#define COMPONENT_H

constexpr ComponentFamilyID INVALID_FAMILY_ID = 0;
constexpr ComponentFamilyID COMMAND_FAMILY_ID = 1;
constexpr ComponentFamilyID HEALTH_FAMILY_ID= 2;
constexpr ComponentFamilyID PHYSICS_FAMILY_ID= 3;
constexpr ComponentFamilyID VISUAL_FAMILY_ID= 4;

constexpr ComponentID INVALID_COMPONENT_ID = 0;

class Entity;

struct Component
{
public:
	Component() {}
	//virtual ~Component() {}

	//virtual bool Init(Entity *pOwner);
	//virtual bool Release() {};

	//virtual void Update(float deltaTime) {};

	//virtual ComponentFamilyID GetFamilyID() const = 0;
	//virtual ComponentID GetComponentID() const = 0;

//protected :
	Entity *pOwner{};
	static ComponentID componentID;
	static ComponentFamilyID componentFamilyID;
};

#endif