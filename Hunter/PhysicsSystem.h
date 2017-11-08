#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H


//#include "PhysicsComponent.h"

class PhysicsSystem 
{
public:
	PhysicsSystem();
	virtual ~PhysicsSystem();

	virtual bool Init();
	virtual void ShutDown();

	void Update(float deltaTime);

	EventChannel GetChannel() { return _channel; }

private :
	EventChannel _channel;

};

#endif