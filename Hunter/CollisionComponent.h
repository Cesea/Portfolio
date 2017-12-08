#ifndef COLLISION_COMPONENT_H
#define COLLISION_COMPONENT_H

#include "Component.h"

struct CollisionComponent : public Component
{
	//BoudningSphere는 일괄적 스케일만을 지원한다
	struct BoundingSphere
	{
		Vector3 _localCenter;
		float _radius;
		BoundingSphere & operator=(const BoundingSphere & other)
		{
			this->_localCenter = other._localCenter;
			this->_radius = other._radius;
		}
	};

	struct BoundingBox
	{
		Vector3 _localCenter;
		float _xSize;
		float _ySize;
		float _zSize;
		Vector3 _localMinPos;
		Vector3 _localMaxPos;
		void Init(const Vector3 & min, const Vector3 & max)
		{
			_localMinPos = min;
			_localMaxPos = max;
			_xSize = (max.x - min.x) / 2;
			_ySize = (max.y - min.y) / 2;
			_zSize = (max.z - min.z) / 2;
			_localCenter = (min + max) / 2;
		}

		BoundingBox & operator=(const BoundingBox & other)
		{
			this->_localCenter = other._localCenter;
			this->_xSize = other._xSize;
			this->_ySize = other._ySize;
			this->_zSize = other._zSize;
			this->_localMinPos = other._localMaxPos;
			this->_localMaxPos = other._localMaxPos;
		}
	};

	CollisionComponent();
	virtual ~CollisionComponent();

	enum COLLISION_TYPE
	{
		COLLISION_TYPE_BOX,
		COLLISION_TYPE_SPHERE,
		COLLISION_TYPE_OBB
	};

	COLLISION_TYPE _type{};
	BoundingSphere _boundingSphere;
	BoundingBox _boundingBox;

	enum TRIGGER_TYPE
	{
		TRIGGER_TYPE_PLAYER,
		TRIGGER_TYPE_ENEMY,
		TRIGGER_TYPE_OBJECT,
		TRIGGER_TYPE_PLAYER_DMGBOX,
		TRIGGER_TYPE_ENEMY_DMGBOX
	};
	bool _isTrigger{};
	TRIGGER_TYPE _triggerType;
	bool _locked{};
	//DMG 박스들을 위한 변수들..
	float _dmg;
	float _velocity;
	float _accel;
	float _duration;
	bool _valid;
	//디버그용 함수들...
	void GetWorldCenterRadius(const TransformComponent &transform, Vector3 *pOutCenter, float *pOutRadius);
	void GetWorldBox(const TransformComponent& transform, Vector3* pOutBoxPos);
	void GetWorldAABBMinMax(const TransformComponent& transform, Vector3* pOutMin, Vector3* pOutMax);

	void RenderSphereGizmo(const TransformComponent &transform);
	void RenderBoxGizmo(const TransformComponent &transform);
};


#endif