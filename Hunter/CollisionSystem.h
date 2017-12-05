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

	void render();
	//Event
	
	struct ActorTriggerEvent
	{
		ActorTriggerEvent(Entity entity1,Entity entity2) : _entity1(entity1),_entity2(entity2){}
		Entity _entity1;
		Entity _entity2;
	};
	struct ObjectTriggerEvent
	{
		ObjectTriggerEvent(Entity entity1, Entity entity2) : _entity1(entity1), _entity2(entity2) {}
		Entity _entity1;
		Entity _entity2;
	};
	EventChannel _channel;
};


#endif
