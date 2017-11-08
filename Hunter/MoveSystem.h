#ifndef POSITION_SYSTEM_H
#define POSITION_SYSTEM_H

#include "PositionComponent.h"
#include "MoveComponent.h"

class MoveSystem : public System<Requires<PositionComponent, MoveComponent>>
{
public :
	MoveSystem();
	virtual ~MoveSystem() override;

	void Update(float deltaTime);

	virtual void Initialize();

	virtual void OnEntityAdded(Entity& entity);

	virtual void OnEntityRemoved(Entity& entity);

};

#endif