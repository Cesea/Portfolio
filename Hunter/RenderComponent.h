#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H

#include "Component.h"


class RenderComponent : public Component
{
public :
	RenderComponent() {}
	virtual ~RenderComponent() {}

	virtual bool PreInit(Entity *pOwner);
	virtual bool Init();
	virtual bool Release();

	virtual void Update(float deltaTime);
};

class BufferRenderComponent : public RenderComponent
{
public :

private :

};

class MeshRenderComponent : public RenderComponent
{
public :

private :

};


#endif