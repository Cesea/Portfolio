#ifndef COLLISION_COMPONENT_H
#define COLLISION_COMPONENT_H

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

	enum BoundingType
	{
		eBoundingBox = 0,
		eBoundingSphere
	};

	BoundingType _type;
	union
	{
		BoundingBox boundingBox;
		BoundingSphere boundingSphere;
	};
};


#endif