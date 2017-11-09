#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include "TransformComponent.h"
#include "CollisionComponent.h"

class CollitionSystem : public System<Requires<TransformComponent, CollisionComponent>>
{
public :
	CollitionSystem();
	virtual ~CollitionSystem();



};


#endif
