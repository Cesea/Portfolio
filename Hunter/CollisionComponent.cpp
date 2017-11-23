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
	//���� ����
	Vec3TransformCoord(pOutCenter, &_boundingSphere._localCenter, &finalMatrix);

	Vector3 scale = transform.GetScale();
	//���� ����
	*pOutRadius = _boundingSphere._radius * scale.x;
}

void CollisionComponent::GetWorldBox(const TransformComponent & transform, Vector3 * pOutBoxPos)
{
	//���� 8���� ������ ���Ѵ�.
	Vector3 vertices[8];
	vertices[0] = Vector3(_boundingBox._localMinPos.x, _boundingBox._localMinPos.y, _boundingBox._localMinPos.z);
	vertices[1] = Vector3(_boundingBox._localMinPos.x, _boundingBox._localMaxPos.y, _boundingBox._localMinPos.z);
	vertices[2] = Vector3(_boundingBox._localMaxPos.x, _boundingBox._localMaxPos.y, _boundingBox._localMinPos.z);
	vertices[3] = Vector3(_boundingBox._localMaxPos.x, _boundingBox._localMinPos.y, _boundingBox._localMinPos.z);

	vertices[4] = Vector3(_boundingBox._localMinPos.x, _boundingBox._localMinPos.y, _boundingBox._localMaxPos.z);
	vertices[5] = Vector3(_boundingBox._localMinPos.x, _boundingBox._localMaxPos.y, _boundingBox._localMaxPos.z);
	vertices[6] = Vector3(_boundingBox._localMaxPos.x, _boundingBox._localMaxPos.y, _boundingBox._localMaxPos.z);
	vertices[7] = Vector3(_boundingBox._localMaxPos.x, _boundingBox._localMinPos.y, _boundingBox._localMaxPos.z);

	Matrix finalMatrix = transform.GetFinalMatrix();
	for (int i = 0; i < 8; i++)
	{
		Vec3TransformCoord(pOutBoxPos + i, vertices + i, &finalMatrix);
	}

}

void CollisionComponent::GetWorldAABBMinMax(const TransformComponent & transform, 
	Vector3 * pOutMin, Vector3 * pOutMax)
{
	//��ȯ�� �ڽ��� ���� ��ġ 8���� ��´�
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

	//OBB�� �׸���
	//�ո�
	GIZMOMANAGER->Line(worldPos[0], worldPos[1], 0xFF00FF00);
	GIZMOMANAGER->Line(worldPos[1], worldPos[2], 0xFF00FF00);
	GIZMOMANAGER->Line(worldPos[2], worldPos[3], 0xFF00FF00);
	GIZMOMANAGER->Line(worldPos[3], worldPos[0], 0xFF00FF00);
	//�޸�
	GIZMOMANAGER->Line(worldPos[4], worldPos[5], 0xFF00FF00);
	GIZMOMANAGER->Line(worldPos[5], worldPos[6], 0xFF00FF00);
	GIZMOMANAGER->Line(worldPos[6], worldPos[7], 0xFF00FF00);
	GIZMOMANAGER->Line(worldPos[7], worldPos[4], 0xFF00FF00);
	//�� �𼭸�
	GIZMOMANAGER->Line(worldPos[4], worldPos[0], 0xFF00FF00);
	GIZMOMANAGER->Line(worldPos[5], worldPos[1], 0xFF00FF00);
	GIZMOMANAGER->Line(worldPos[6], worldPos[2], 0xFF00FF00);
	GIZMOMANAGER->Line(worldPos[7], worldPos[3], 0xFF00FF00);
	//AABB�� �׸���

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
