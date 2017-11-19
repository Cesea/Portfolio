#ifndef SCRIPT_SYSTEM_H
#define SCRIPT_SYSTEM_H

#include "System.h"
#include "ScriptComponent.h"

class ScriptSystem : public System<Requires<ScriptComponent>>
{
public:
	ScriptSystem();
	virtual ~ScriptSystem() override;

	void Update(float deltaTime);

	virtual void Initialize();

	virtual void OnEntityAdded(Entity& entity);

	virtual void OnEntityRemoved(Entity& entity);

private:

};

#endif