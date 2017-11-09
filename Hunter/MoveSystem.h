#ifndef MOVE_SYSTEM_H
#define MOVE_SYSTEM_H

#include "TransformComponent.h"
#include "MoveComponent.h"

class MoveSystem : public System<Requires<TransformComponent, MoveComponent>>
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