#ifndef COLLISION_UTILLS_H
#define COLLISION_UTILLS_H

#include "CollisionComponent.h"
#include "TransformComponent.h"

struct AABB
{
	Vector3 _min;
	Vector3 _max;
	Vector3 _center;
	float _xSize;
	float _ySize;
	float _zSize;
	void setAABB(const Vector3 &min, const Vector3 &max)
	{
		_min = min;
		_max = max;
		_center = (_min + _max) / 2;
		_xSize = (_max.x - _min.x);
		_ySize = (_max.y - _min.y);
		_zSize = (_max.z - _min.z);

	}
};
struct Sphere
{
	Vector3 _center;
	float _radius;
};

#include "stdafx.h"
#include "CollisionUtils.h"


#define EPSILON 0.001f

bool IsRayHitBound(
	const Ray &ray,
	const CollisionComponent::BoundingSphere &boundingSphere,
	const TransformComponent &transform,	
	Vector3* pOutHitPos, 
	Vector3* pOutHitNormal);

bool IsRayHitBound(
		const Ray &ray,
		const CollisionComponent::BoundingBox &boundingBox,
		const TransformComponent &transform,
		Vector3 *pOutHitPos, 
		Vector3 *pOutHitNormal);



//������ ����Ʈ������ ���ϰ���� ��ǥ�� �������ش�.
Vector3 GetSphereClosetVector(const Vector3 &SpherePos, const Vector3 &point, float radius);

//aabb�ڽ����� ����Ʈ������ ���ϰ���� ��ǥ�� �������ش�.
Vector3 GetAABBClosetVector(const Vector3 &MinPoint, const Vector3 &MaxPoint, const Vector3 &point);

//OBB�ڽ����� ����Ʈ������ ���ϰ���� ��ǥ�� �������ش�.
Vector3 GetOBBClosetVector(const Vector3 &OBBPos, const Vector3 &xVec, const Vector3 &yVec, const Vector3 &zVec, 
	float xVecSize, float yVecSize, float zVecSize, const Vector3 &point);

//��鿡�� ����Ʈ������ ���� ����� ��ǥ�� �������ش�.
Vector3 GetPlaneClosetVector(const Vector3 &planeNormal, float planeDistance, const Vector3 &point);

//������ ����Ʈ������ ���� ����� ��ǥ�� �������ش�.
Vector3 GetLineClosetVector(const Vector3 &startPoint, const Vector3 &endPoint, const Vector3 &point);

//�������� ����Ʈ������ ���� ����� ��ǥ�� �������ش�.
Vector3 GetRayClosetVector(const Vector3 &rayOrigin, const Vector3 &rayDirection, const Vector3 &point);

//���� ������ �浹üũ
bool Collision_SphereToSphere(const Vector3 &spherePos1, const Vector3 &spherePos2, float radius1, float radius2);

//���� AABB�ڽ��� �浹üũ
bool Collision_SphereToAABB(const Vector3 &MinPoint, const Vector3 &MaxPoint, const Vector3 &spherePos, float radius);

//���� OBB�ڽ��� �浹üũ
bool Collision_SphereToOBB(const Vector3 &OBBPos, const Vector3 &xVec, const Vector3 &yVec, const Vector3 &zVec, 
	float xVecSize, float yVecSize, float zVecSize, const Vector3 &spherePos, float radius);

//���� ����� �浹üũ
bool Collision_SphereToPlane(const Vector3 &planeNormal, float planeDistance, 
	const Vector3 &spherePos, float radius);

//AABB�ڽ��� AABB�ڽ��� �浹üũ
bool Collision_AABBToAABB(const Vector3 &minPoint1, const Vector3 &maxPoint1, 
	const Vector3 &minPoint2, const Vector3 &maxPoint2);

//AABB�ڽ��� OBB�ڽ��� �浹üũ
bool Collision_AABBToOBB(const Vector3 &minPoint, const Vector3 &maxPoint, const Vector3 &OBBPos,
	const Vector3 &xVec, const Vector3 &yVec, const Vector3 &zVec, 
	float xVecSize, float yVecSize, float zVecSize);

//AABB�ڽ��� ����� �浹üũ
bool Collision_AABBtoPlane(const Vector3 &minPoint, const Vector3 &maxPoint, 
	const Vector3 &planeNormal, float planeDistance);

//OBB�ڽ��� OBB�ڽ��� �浹üũ
bool Collision_OBBToOBB(const Vector3 &OBBPos1, const Vector3 &xVec1, const Vector3 &yVec1, const Vector3 &zVec1, 
	float xVecSize1, float yVecSize1, float zVecSize1,
	const Vector3 &OBBPos2, const Vector3 &xVec2, const Vector3 &yVec2, const Vector3 &zVec2, float xVecSize2, 
	float yVecSize2, float zVecSize2);

//OBB�ڽ��� ����� �浹üũ
bool Collision_OBBToPlane(const Vector3 &OBBPos, const Vector3 &xVec, 
	const Vector3 &yVec, const Vector3 &zVec, 
	float xVecSize, float yVecSize, float zVecSize , 
	const Vector3 &planeNormal, float planeDistance);

//���� ����� �浹üũ
bool Collision_PlaneToPlane(const Vector3 &planeNormal1, float planeDistance1, 
	const Vector3 planeNormal2, float planeDistance2);

//������ ������ �浹üũ
bool Collision_RayToSphere(const Vector3 &rayPos, const Vector3 &rayDir, const Vector3 &spherePos, float radius);

//������ ������ �浹���� ������ ������ �����ϴ� �Ÿ��� �����Ѵ�. �ð��� �������̸� �浹��������
float Time_RayToSphere(const Vector3 &rayPos, const Vector3 &rayDir, const Vector3 &spherePos, float radius);

//������ AABB���� �浹üũ
bool Collision_RayToAABB(const Vector3 &rayPos, const Vector3 &rayDir, const Vector3 &minPos, const Vector3 &maxPos);

//������ AABB���� �浹���� ������ AABB���� �����ϴ� �Ÿ��� �����Ѵ�. �ð��� �������̸� �浹���� ����
float Time_RayToAABB(const Vector3 &rayPos, const Vector3 &rayDir, const Vector3 &minPos, const Vector3 &maxPos);

//������ OBB���� �浹üũ
bool Collision_RayToOBB(const Vector3 &rayPos, const Vector3 &rayDir, const Vector3 &OBBPos, 
	const Vector3 &xVec, const Vector3 &yVec, const Vector3 &zVec, float xVecSize, float yVecSize, float zVecSize);

//������ OBB�ڽ����� �浹���� ������ OBB�ڽ����� �����ϴ� �Ÿ��� �����Ѵ�. �ð��� �������̸� �浹���� ����.
bool Time_RayToOBB(const Vector3 &rayPos, Vector3 &rayDir, 
	Vector3 &OBBPos, Vector3 &xVec, Vector3 &yVec, Vector3 &zVec,
	float xVecSize, float yVecSize, float zVecSize);

bool IsBlocking(TransformComponent * pTransA, CollisionComponent::BoundingBox * pBoundA, TransformComponent * pTransB, CollisionComponent::BoundingBox * pBoundB, float moveFactor);
#endif