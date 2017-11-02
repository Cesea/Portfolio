#ifndef COMPONENT_H
#define COMPONENT_H

class Entity;

class Component
{
public:
	Component() {}
	virtual ~Component() {}

	virtual bool PreInit(Entity *pOwner) = 0;
	virtual bool Init() = 0;
	virtual bool Release() = 0;

	virtual void Update(float deltaTime) = 0;

protected :
	Entity *_pOwner{};

};

#endif