#ifndef COLLISION_COMPONENT_H
#define COLLISION_COMPONENT_H

struct CollisionComponent : public Component
{
	CollisionComponent();
	virtual ~CollisionComponent();
	enum BoundType
	{
		eBoundingBox = 0,
		eBoundingSphere
	};

	BoundType _type;

	union
	{
		BoundingBox boundingBox;
		BoundingSphere boundingSphere;
	};
};


#endif