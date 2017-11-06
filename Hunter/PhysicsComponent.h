#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

#include "Component.h"

class PhysicsComponent : public Component
{
public :
	PhysicsComponent() {}
	virtual ~PhysicsComponent() {}

	virtual bool PreInit(Entity *pOwner);
	virtual bool Init();
	virtual bool Release();

	virtual void Update(float deltaTime);
protected :
};

class RigidPhysicsComponent : public PhysicsComponent
{
public  :
	RigidPhysicsComponent() {}
	virtual ~RigidPhysicsComponent() {}

	virtual bool PreInit(Entity *pOwner);
	virtual bool Init();
	virtual bool Release();

	virtual void Update(float deltaTime);
private :
	Vector3 _accel;
	Vector3 _velocity;;
	Vector3 _friction;

};


#endif