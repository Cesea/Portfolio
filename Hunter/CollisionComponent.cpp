#include "stdafx.h"
#include "CollisionComponent.h"

CollisionComponent::CollisionComponent()
{
}

CollisionComponent::~CollisionComponent()
{
}

void CollisionComponent::GetWorldCenterRadius(const TransformComponent & transform, 
	Vector3 * pOutCenter, float * pOutRadius)
{
	Matrix finalMatrix = transform.GetFinalMatrix();
	//월드 센터
	Vec3TransformCoord(pOutCenter, &_boundingSphere.localCenter, &finalMatrix);

	Vector3 scale = transform.GetScale();
	//구의 길이
	*pOutRadius = _boundingSphere.radius * scale.x;
}

void CollisionComponent::GetWorldBox(const TransformComponent & transform, Vector3 * pOutBoxPos)
{
	//로컬 8개의 정점을 구한다.
	Vector3 vertices[8];
	vertices[0] = Vector3(_boundingBox.localMinPos.x, _boundingBox.localMinPos.y, _boundingBox.localMinPos.z);
	vertices[0] = Vector3(_boundingBox.localMinPos.x, _boundingBox.localMaxPos.y, _boundingBox.localMinPos.z);
	vertices[0] = Vector3(_boundingBox.localMaxPos.x, _boundingBox.localMaxPos.y, _boundingBox.localMinPos.z);
	vertices[0] = Vector3(_boundingBox.localMaxPos.x, _boundingBox.localMinPos.y, _boundingBox.localMinPos.z);

	vertices[0] = Vector3(_boundingBox.localMinPos.x, _boundingBox.localMinPos.y, _boundingBox.localMaxPos.z);
	vertices[0] = Vector3(_boundingBox.localMinPos.x, _boundingBox.localMaxPos.y, _boundingBox.localMaxPos.z);
	vertices[0] = Vector3(_boundingBox.localMaxPos.x, _boundingBox.localMaxPos.y, _boundingBox.localMaxPos.z);
	vertices[0] = Vector3(_boundingBox.localMaxPos.x, _boundingBox.localMinPos.y, _boundingBox.localMaxPos.z);

	Matrix finalMatrix = transform.GetFinalMatrix();
	for (int i = 0; i < 8; i++)
	{
		Vec3TransformCoord(pOutBoxPos + i, vertices + i, &finalMatrix);
	}

}

void CollisionComponent::GetWorldAABBMinMax(const TransformComponent & transform, 
	Vector3 * pOutMin, Vector3 * pOutMax)
{
	//변환된 박스의 월드 위치 8개를 얻는다
	Vector3 worldPos[8];
	this->GetWorldBox(transform, worldPos);
	*pOutMin = worldPos[0];
	*pOutMax = worldPos[0];

	for (int32 i = 0; i < 8; i++)
	{
		if (pOutMin->x > worldPos[i].x)
		{
			pOutMin->x = worldPos[i].x;
		}
		else if (pOutMax->x < worldPos[i].x)
		{
			pOutMax->x = worldPos[i].x;
		}
		
		if (pOutMin->y > worldPos[i].y)
		{
			pOutMin->y = worldPos[i].y;
		}
		else if (pOutMax->y < worldPos[i].y)
		{
			pOutMax->y = worldPos[i].y;
		}

		if (pOutMin->z > worldPos[i].z)
		{
			pOutMin->z = worldPos[i].z;
		}
		else if (pOutMax->z < worldPos[i].z)
		{
			pOutMax->z = worldPos[i].z;
		}
	}
}

void CollisionComponent::RenderSphereGizmo(const TransformComponent & transform)
{
	Vector3 worldCenter;
	float radius;

	this->GetWorldCenterRadius(transform, &worldCenter, &radius);
	GIZMOMANAGER->WireSphere(worldCenter, radius, 0xFFFFFF00);
}

void CollisionComponent::RenderBoxGizmo(const TransformComponent & transform)
{
	Vector3 worldPos[8];
	GetWorldBox(transform, worldPos);

	//OBB를 그리자
	//앞면
	GIZMOMANAGER->Line(worldPos[0], worldPos[1], 0xFF00FF00);
	GIZMOMANAGER->Line(worldPos[1], worldPos[2], 0xFF00FF00);
	GIZMOMANAGER->Line(worldPos[2], worldPos[3], 0xFF00FF00);
	GIZMOMANAGER->Line(worldPos[3], worldPos[0], 0xFF00FF00);
	//뒷면
	GIZMOMANAGER->Line(worldPos[4], worldPos[5], 0xFF00FF00);
	GIZMOMANAGER->Line(worldPos[5], worldPos[6], 0xFF00FF00);
	GIZMOMANAGER->Line(worldPos[6], worldPos[7], 0xFF00FF00);
	GIZMOMANAGER->Line(worldPos[7], worldPos[4], 0xFF00FF00);
	//옆 모서리
	GIZMOMANAGER->Line(worldPos[4], worldPos[0], 0xFF00FF00);
	GIZMOMANAGER->Line(worldPos[5], worldPos[1], 0xFF00FF00);
	GIZMOMANAGER->Line(worldPos[6], worldPos[2], 0xFF00FF00);
	GIZMOMANAGER->Line(worldPos[7], worldPos[3], 0xFF00FF00);
	//AABB를 그리자

	Vector3 min;
	Vector3 max;

	min = worldPos[0];
	max = worldPos[0];

	for (int i = 0; i < 8; i++)
	{
		if (min.x > worldPos[i].x)
		{
			min.x = worldPos[i].x;
		}
		else if (max.x < worldPos[i].x)
		{
			max.x = worldPos[i].x;
		}

		if (min.y > worldPos[i].y)
		{
			min.y = worldPos[i].y;
		}
		else if (max.y < worldPos[i].y)
		{
			max.y = worldPos[i].y;
		}

		if (min.z > worldPos[i].z)
		{
			min.z = worldPos[i].z;
		}
		else if (max.z < worldPos[i].z)
		{
			max.z = worldPos[i].z;
		}
	}
	GIZMOMANAGER->AABBBox(min, max, 0xFF008800);
}
