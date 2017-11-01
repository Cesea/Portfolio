#ifndef PHYSCIS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "System.h"

//#include "PhysicsComponent.h"

class PhysicsSystem : public System
{
public:
	PhysicsSystem();
	virtual ~PhysicsSystem();

	virtual bool Init(const std::string &name, SystemSetting &setting);
	virtual void ShutDown();

	virtual void Update(float deltaTime);

	virtual EventChannel GetChannel() { return _channel; }

};

#endif