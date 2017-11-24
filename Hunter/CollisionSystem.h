#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include "TransformComponent.h"
#include "CollisionComponent.h"

#include "CollisionUtils.h"

class CollisionSystem : public System<Requires<TransformComponent, CollisionComponent>>
{
public :
	CollisionSystem();
	virtual ~CollisionSystem();

	void Update(float deltaTime, float checkRange);

	void QueryRayEntityHit(const Ray &ray, std::vector<Entity> *pOutCollidingEntity, 
		std::vector<float> *pOutDistance);

	virtual void Initialize();

	virtual void OnEntityAdded(Entity& entity);

	virtual void OnEntityRemoved(Entity& entity);
};


#endif
