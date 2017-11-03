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
	BufferRenderComponent() {}
	virtual ~BufferRenderComponent() {}

	virtual bool PreInit(Entity *pOwner);
	virtual bool Init();
	virtual bool Release();

	virtual void Update(float deltaTime);
private :

};

class MeshRenderComponent : public RenderComponent
{
public :
	MeshRenderComponent() {}
	virtual ~MeshRenderComponent() {}

	virtual bool PreInit(Entity *pOwner);
	virtual bool Init();
	virtual bool Release();

	virtual void Update(float deltaTime);
private :

};


#endif