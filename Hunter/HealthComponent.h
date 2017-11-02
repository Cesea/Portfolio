#ifndef HEALTH_COMPONENT_H
#define HEALTH_COMPONENT_H

#include "Component.h"

class HealthComponent : public Component
{
public :
	HealthComponent() {}
	virtual ~HealthComponent() {}

	virtual bool PreInit(Entity *pOwner);
	virtual bool Init();
	virtual bool Release();

	virtual void Update(float deltaTime);

private :
	int32 _max;
	int32 _current;
};

#endif
