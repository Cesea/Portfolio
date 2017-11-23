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

//구에서 포인트까지의 제일가까운 좌표를 리턴해준다.
Vector3 getSphereClosetVector(Vector3 SpherePos, Vector3 point, float radius);
//aabb박스에서 포인트까지의 제일가까운 좌표를 리턴해준다.
Vector3 getAABBClosetVector(Vector3 MinPoint, Vector3 MaxPoint, Vector3 point);
//OBB박스에서 포인트까지의 제일가까운 좌표를 리턴해준다.
Vector3 getOBBClosetVector(Vector3 OBBPos, Vector3 xVec, Vector3 yVec, Vector3 zVec, float xVecSize, float yVecSize, float zVecSize, Vector3 point);
//평면에서 포인트까지의 제일 가까운 좌표를 리턴해준다.
Vector3 getPlaneClosetVector(Vector3 planeNormal, float planeDistance, Vector3 point);
//선에서 포인트까지의 제일 가까운 좌표를 리턴해준다.
Vector3 getLineClosetVector(Vector3 startPoint, Vector3 endPoint, Vector3 point);
//광선에서 포인트까지의 제일 가까운 좌표를 리턴해준다.
Vector3 getRayClosetVector(Vector3 rayOrigin, Vector3 rayDirection, Vector3 point);
//구와 구간의 충돌체크
bool Collision_SphereToSphere(Vector3 spherePos1, Vector3 spherePos2, float radius1, float radius2);
//구와 AABB박스의 충돌체크
bool Collision_SphereToAABB(Vector3 MinPoint, Vector3 MaxPoint, Vector3 spherePos, float radius);
//구와 OBB박스의 충돌체크
bool Collision_SphereToOBB(Vector3 OBBPos, Vector3 xVec, Vector3 yVec, Vector3 zVec, float xVecSize, float yVecSize, float zVecSize,
	Vector3 spherePos, float radius);
//구와 평면의 충돌체크
bool Collision_SphereToPlane(Vector3 planeNormal, float planeDistance, Vector3 spherePos, float radius);
//AABB박스와 AABB박스의 충돌체크
bool Collision_AABBToAABB(Vector3 minPoint1, Vector3 maxPoint1, Vector3 minPoint2, Vector3 maxPoint2);
//AABB박스와 OBB박스의 충돌체크
bool Collision_AABBToOBB(Vector3 minPoint, Vector3 maxPoint, Vector3 OBBPos,
	Vector3 xVec, Vector3 yVec, Vector3 zVec, float xVecSize, float yVecSize, float zVecSize);
//AABB박스와 평면의 충돌체크
bool Collision_AABBtoPlane(Vector3 minPoint, Vector3 maxPoint, Vector3 planeNormal, float planeDistance);
//OBB박스와 OBB박스의 충돌체크
bool Collision_OBBToOBB(Vector3 OBBPos1, Vector3 xVec1, Vector3 yVec1, Vector3 zVec1, float xVecSize1, float yVecSize1, float zVecSize1
	, Vector3 OBBPos2, Vector3 xVec2, Vector3 yVec2, Vector3 zVec2, float xVecSize2, float yVecSize2, float zVecSize2);
//OBB박스와 평면의 충돌체크
bool Collision_OBBToPlane(Vector3 OBBPos, Vector3 xVec, Vector3 yVec, Vector3 zVec, float xVecSize, float yVecSize, float zVecSize
	, Vector3 planeNormal, float planeDistance);
//평면과 평면의 충돌체크
bool Collision_PlaneToPlane(Vector3 planeNormal1, float planeDistance1, Vector3 planeNormal2, float planeDistance2);
//광선과 구와의 충돌체크
bool Collision_RayToSphere(Vector3 rayPos, Vector3 rayDir, Vector3 spherePos, float radius);
//광선과 구와의 충돌에서 광선이 구까지 도달하는 거리를 리턴한다. 시간이 음수값이면 충돌하지않음
float Time_RayToSphere(Vector3 rayPos, Vector3 rayDir, Vector3 spherePos, float radius);
//광선과 AABB와의 충돌체크
bool Collision_RayToAABB(Vector3 rayPos, Vector3 rayDir, Vector3 minPos, Vector3 maxPos);
//광선과 AABB와의 충돌에서 광선이 AABB까지 도달하는 거리를 리턴한다. 시간이 음수값이면 충돌하지 않음
float Time_RayToAABB(Vector3 rayPos, Vector3 rayDir, Vector3 minPos, Vector3 maxPos);
//광선과 OBB와의 충돌체크
bool Collision_RayToOBB(Vector3 rayPos, Vector3 rayDir, Vector3 OBBPos, Vector3 xVec, Vector3 yVec, Vector3 zVec,
	float xVecSize, float yVecSize, float zVecSize);
//광선과 OBB박스와의 충돌에서 광선이 OBB박스까지 도달하는 거리를 리턴한다. 시간이 음수값이면 충돌하지 않음.
bool Time_RayToOBB(Vector3 rayPos, Vector3 rayDir, Vector3 OBBPos, Vector3 xVec, Vector3 yVec, Vector3 zVec,
	float xVecSize, float yVecSize, float zVecSize);
bool IsBlocking(TransformComponent * pTransA, CollisionComponent::BoundingBox * pBoundA, TransformComponent * pTransB, CollisionComponent::BoundingBox * pBoundB, float moveFactor);
#endif