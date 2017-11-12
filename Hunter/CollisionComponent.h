#ifndef COLLISION_COMPONENT_H
#define COLLISION_COMPONENT_H

#include "Component.h"

struct BoundingSphere
{
	Vector3 _localCenter;
	float _radius;
	Vector3 _halfSize;
};

struct BoundingBox
{
	Vector3 _localCenter;
	float _radius;
	Vector3 _halfSize;
	Vector3 localMinPos;
	Vector3 localMaxPos;
};

struct CollisionComponent : public Component
{
	CollisionComponent();
	virtual ~CollisionComponent();

	Vector3 _localCenter;
	float _radius;

	Vector3 _hafSize;

	Vector3 _locaMinPos;
	Vector3 _locaMaxPos;
};


#endif