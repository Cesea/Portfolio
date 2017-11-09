#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "TransformComponent.h"
#include "RenderComponent.h"

class RenderSystem : public System<Requires<TransformComponent, RenderComponent>>
{
public:
	RenderSystem();
	virtual ~RenderSystem() override;

	void Update(float deltaTime);

	virtual void Initialize();

	virtual void OnEntityAdded(Entity& entity);

	virtual void OnEntityRemoved(Entity& entity);

};

#endif