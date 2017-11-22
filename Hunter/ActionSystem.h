#ifndef ACTION_SYSTEM_H
#define ACTION_SYSTEM_H

#include "ActionComponent.h"
#include "RenderComponent.h"

class ActionSystem : public System<Requires<ActionComponent, RenderComponent>>
{
public:
	ActionSystem();
	virtual ~ActionSystem() override;

	void Update(float deltaTime);

	virtual void Initialize();

	virtual void OnEntityAdded(Entity& entity);

	virtual void OnEntityRemoved(Entity& entity);

private :
};

#endif