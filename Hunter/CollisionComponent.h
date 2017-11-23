#ifndef COLLISION_COMPONENT_H
#define COLLISION_COMPONENT_H

#include "Component.h"

struct CollisionComponent : public Component
{
	//BoudningSphere는 일괄적 스케일만을 지원한다
	struct BoundingSphere
	{
		Vector3 localCenter;
		float radius;
		BoundingSphere & operator=(const BoundingSphere & other)
		{
			this->localCenter = other.localCenter;
			this->radius = other.radius;
		}
	};

	struct BoundingBox
	{
		Vector3 localCenter;
		float xSize;
		float ySize;
		float zSize;
		Vector3 localMinPos;
		Vector3 localMaxPos;
		void initialBox(const Vector3 & min, const Vector3 & max)
		{
			localMinPos = min;
			localMaxPos = max;
			xSize = (max.x - min.x) / 2;
			ySize = (max.y - min.y) / 2;
			zSize = (max.z - min.z) / 2;
			localCenter = (min + max) / 2;
		}

		BoundingBox & operator=(const BoundingBox & other)
		{
			this->localCenter = other.localCenter;
			this->xSize = other.xSize;
			this->ySize = other.ySize;
			this->zSize = other.zSize;
			this->localMinPos = other.localMaxPos;
			this->localMaxPos = other.localMaxPos;
		}
	};

	CollisionComponent();
	virtual ~CollisionComponent();

	enum CollisionType
	{
		Type_Box,
		Type_Sphere
	};

	CollisionType _type;
	BoundingSphere _boundingSphere;
	BoundingBox _boundingBox;

	bool isTrigger;
	bool locked;

	//디버그용 함수들...
	void GetWorldCenterRadius(const TransformComponent &transform, Vector3 *pOutCenter, float *pOutRadius);
	void GetWorldBox(const TransformComponent& transform, Vector3* pOutBoxPos);
	void GetWorldAABBMinMax(const TransformComponent& transform, Vector3* pOutMin, Vector3* pOutMax);

	void RenderSphereGizmo(const TransformComponent &transform);
	void RenderBoxGizmo(const TransformComponent &transform);
};


#endif