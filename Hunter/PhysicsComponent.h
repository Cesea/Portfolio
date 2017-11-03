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

class SpherePhysicsComponent : public PhysicsComponent
{
public  :
	SpherePhysicsComponent() {}
	virtual ~SpherePhysicsComponent() {}

	virtual bool PreInit(Entity *pOwner);
	virtual bool Init();
	virtual bool Release();

	virtual void Update(float deltaTime);
private :
};

class BoundingBoxPhysicsComponent : public PhysicsComponent
{
public :
	BoundingBoxPhysicsComponent() {}
	virtual ~BoundingBoxPhysicsComponent() {}

	virtual bool PreInit(Entity *pOwner);
	virtual bool Init();
	virtual bool Release();

	virtual void Update(float deltaTime);
private :
} ;

#endif