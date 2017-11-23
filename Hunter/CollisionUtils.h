#ifndef COLLISION_UTILLS_H
#define COLLISION_UTILLS_H

#include "CollisionComponent.h"
#include "TransformComponent.h"

struct AABB
{
	D3DXVECTOR3 min;
	D3DXVECTOR3 max;
	D3DXVECTOR3 center;
	float xSize;
	float ySize;
	float zSize;
	void setAABB(D3DXVECTOR3 _min, D3DXVECTOR3 _max)
	{
		min = _min;
		max = _max;
		center = (min + max) / 2;
		xSize = (max.x - min.x);
		ySize = (max.y - min.y);
		zSize = (max.z - min.z);

	}
};
struct Sphere
{
	D3DXVECTOR3 center;
	float radius;
};

#include "stdafx.h"
#include "CollisionUtils.h"


#define EPSILON 0.001f

//������ ����Ʈ������ ���ϰ���� ��ǥ�� �������ش�.
Vector3 getSphereClosetVector(Vector3 SpherePos, Vector3 point, float radius);
//aabb�ڽ����� ����Ʈ������ ���ϰ���� ��ǥ�� �������ش�.
Vector3 getAABBClosetVector(Vector3 MinPoint, Vector3 MaxPoint, Vector3 point);
//OBB�ڽ����� ����Ʈ������ ���ϰ���� ��ǥ�� �������ش�.
Vector3 getOBBClosetVector(Vector3 OBBPos, Vector3 xVec, Vector3 yVec, Vector3 zVec, float xVecSize, float yVecSize, float zVecSize, Vector3 point);
//��鿡�� ����Ʈ������ ���� ����� ��ǥ�� �������ش�.
Vector3 getPlaneClosetVector(Vector3 planeNormal, float planeDistance, Vector3 point);
//������ ����Ʈ������ ���� ����� ��ǥ�� �������ش�.
Vector3 getLineClosetVector(Vector3 startPoint, Vector3 endPoint, Vector3 point);
//�������� ����Ʈ������ ���� ����� ��ǥ�� �������ش�.
Vector3 getRayClosetVector(Vector3 rayOrigin, Vector3 rayDirection, Vector3 point);
//���� ������ �浹üũ
bool Collision_SphereToSphere(Vector3 spherePos1, Vector3 spherePos2, float radius1, float radius2);
//���� AABB�ڽ��� �浹üũ
bool Collision_SphereToAABB(Vector3 MinPoint, Vector3 MaxPoint, Vector3 spherePos, float radius);
//���� OBB�ڽ��� �浹üũ
bool Collision_SphereToOBB(Vector3 OBBPos, Vector3 xVec, Vector3 yVec, Vector3 zVec, float xVecSize, float yVecSize, float zVecSize,
	Vector3 spherePos, float radius);
//���� ����� �浹üũ
bool Collision_SphereToPlane(Vector3 planeNormal, float planeDistance, Vector3 spherePos, float radius);
//AABB�ڽ��� AABB�ڽ��� �浹üũ
bool Collision_AABBToAABB(Vector3 minPoint1, Vector3 maxPoint1, Vector3 minPoint2, Vector3 maxPoint2);
//AABB�ڽ��� OBB�ڽ��� �浹üũ
bool Collision_AABBToOBB(Vector3 minPoint, Vector3 maxPoint, Vector3 OBBPos,
	Vector3 xVec, Vector3 yVec, Vector3 zVec, float xVecSize, float yVecSize, float zVecSize);
//AABB�ڽ��� ����� �浹üũ
bool Collision_AABBtoPlane(Vector3 minPoint, Vector3 maxPoint, Vector3 planeNormal, float planeDistance);
//OBB�ڽ��� OBB�ڽ��� �浹üũ
bool Collision_OBBToOBB(Vector3 OBBPos1, Vector3 xVec1, Vector3 yVec1, Vector3 zVec1, float xVecSize1, float yVecSize1, float zVecSize1
	, Vector3 OBBPos2, Vector3 xVec2, Vector3 yVec2, Vector3 zVec2, float xVecSize2, float yVecSize2, float zVecSize2);
//OBB�ڽ��� ����� �浹üũ
bool Collision_OBBToPlane(Vector3 OBBPos, Vector3 xVec, Vector3 yVec, Vector3 zVec, float xVecSize, float yVecSize, float zVecSize
	, Vector3 planeNormal, float planeDistance);
//���� ����� �浹üũ
bool Collision_PlaneToPlane(Vector3 planeNormal1, float planeDistance1, Vector3 planeNormal2, float planeDistance2);
//������ ������ �浹üũ
bool Collision_RayToSphere(Vector3 rayPos, Vector3 rayDir, Vector3 spherePos, float radius);
//������ ������ �浹���� ������ ������ �����ϴ� �Ÿ��� �����Ѵ�. �ð��� �������̸� �浹��������
float Time_RayToSphere(Vector3 rayPos, Vector3 rayDir, Vector3 spherePos, float radius);
//������ AABB���� �浹üũ
bool Collision_RayToAABB(Vector3 rayPos, Vector3 rayDir, Vector3 minPos, Vector3 maxPos);
//������ AABB���� �浹���� ������ AABB���� �����ϴ� �Ÿ��� �����Ѵ�. �ð��� �������̸� �浹���� ����
float Time_RayToAABB(Vector3 rayPos, Vector3 rayDir, Vector3 minPos, Vector3 maxPos);
//������ OBB���� �浹üũ
bool Collision_RayToOBB(Vector3 rayPos, Vector3 rayDir, Vector3 OBBPos, Vector3 xVec, Vector3 yVec, Vector3 zVec,
	float xVecSize, float yVecSize, float zVecSize);
//������ OBB�ڽ����� �浹���� ������ OBB�ڽ����� �����ϴ� �Ÿ��� �����Ѵ�. �ð��� �������̸� �浹���� ����.
bool Time_RayToOBB(Vector3 rayPos, Vector3 rayDir, Vector3 OBBPos, Vector3 xVec, Vector3 yVec, Vector3 zVec,
	float xVecSize, float yVecSize, float zVecSize);
bool IsBlocking(TransformComponent * pTransA, CollisionComponent::BoundingBox * pBoundA, TransformComponent * pTransB, CollisionComponent::BoundingBox * pBoundB, float moveFactor);
#endif