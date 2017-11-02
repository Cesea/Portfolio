#ifndef COMMAND_COMPONENT_H
#define COMMAND_COMPONENT_H

#include "Component.h"

class CommandComponent : public Component
{
public :
	CommandComponent() {}
	virtual ~CommandComponent() {}

	virtual bool PreInit(Entity *pOwner);
	virtual bool Init();
	virtual bool Release();

	virtual void Update(float deltaTime);
protected :

};

class PlayerCommandComponent : public CommandComponent
{
public :

private :

};

class AICommandComponent : public CommandComponent
{
public :

private :
};

#endif