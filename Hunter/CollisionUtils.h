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



//구에서 포인트까지의 제일가까운 좌표를 리턴해준다.
Vector3 GetSphereClosetVector(const Vector3 &SpherePos, const Vector3 &point, float radius);

//aabb박스에서 포인트까지의 제일가까운 좌표를 리턴해준다.
Vector3 GetAABBClosetVector(const Vector3 &MinPoint, const Vector3 &MaxPoint, const Vector3 &point);

//OBB박스에서 포인트까지의 제일가까운 좌표를 리턴해준다.
Vector3 GetOBBClosetVector(const Vector3 &OBBPos, const Vector3 &xVec, const Vector3 &yVec, const Vector3 &zVec, 
	float xVecSize, float yVecSize, float zVecSize, const Vector3 &point);

//평면에서 포인트까지의 제일 가까운 좌표를 리턴해준다.
Vector3 GetPlaneClosetVector(const Vector3 &planeNormal, float planeDistance, const Vector3 &point);

//선에서 포인트까지의 제일 가까운 좌표를 리턴해준다.
Vector3 GetLineClosetVector(const Vector3 &startPoint, const Vector3 &endPoint, const Vector3 &point);

//광선에서 포인트까지의 제일 가까운 좌표를 리턴해준다.
Vector3 GetRayClosetVector(const Vector3 &rayOrigin, const Vector3 &rayDirection, const Vector3 &point);

//구와 구간의 충돌체크
bool Collision_SphereToSphere(const Vector3 &spherePos1, const Vector3 &spherePos2, float radius1, float radius2);

//구와 AABB박스의 충돌체크
bool Collision_SphereToAABB(const Vector3 &MinPoint, const Vector3 &MaxPoint, const Vector3 &spherePos, float radius);

//구와 OBB박스의 충돌체크
bool Collision_SphereToOBB(const Vector3 &OBBPos, const Vector3 &xVec, const Vector3 &yVec, const Vector3 &zVec, 
	float xVecSize, float yVecSize, float zVecSize, const Vector3 &spherePos, float radius);

//구와 평면의 충돌체크
bool Collision_SphereToPlane(const Vector3 &planeNormal, float planeDistance, 
	const Vector3 &spherePos, float radius);

//AABB박스와 AABB박스의 충돌체크
bool Collision_AABBToAABB(const Vector3 &minPoint1, const Vector3 &maxPoint1, 
	const Vector3 &minPoint2, const Vector3 &maxPoint2);

//AABB박스와 OBB박스의 충돌체크
bool Collision_AABBToOBB(const Vector3 &minPoint, const Vector3 &maxPoint, const Vector3 &OBBPos,
	const Vector3 &xVec, const Vector3 &yVec, const Vector3 &zVec, 
	float xVecSize, float yVecSize, float zVecSize);

//AABB박스와 평면의 충돌체크
bool Collision_AABBtoPlane(const Vector3 &minPoint, const Vector3 &maxPoint, 
	const Vector3 &planeNormal, float planeDistance);

//OBB박스와 OBB박스의 충돌체크
bool Collision_OBBToOBB(const Vector3 &OBBPos1, const Vector3 &xVec1, const Vector3 &yVec1, const Vector3 &zVec1, 
	float xVecSize1, float yVecSize1, float zVecSize1,
	const Vector3 &OBBPos2, const Vector3 &xVec2, const Vector3 &yVec2, const Vector3 &zVec2, float xVecSize2, 
	float yVecSize2, float zVecSize2);

//OBB박스와 평면의 충돌체크
bool Collision_OBBToPlane(const Vector3 &OBBPos, const Vector3 &xVec, 
	const Vector3 &yVec, const Vector3 &zVec, 
	float xVecSize, float yVecSize, float zVecSize , 
	const Vector3 &planeNormal, float planeDistance);

//평면과 평면의 충돌체크
bool Collision_PlaneToPlane(const Vector3 &planeNormal1, float planeDistance1, 
	const Vector3 planeNormal2, float planeDistance2);

//광선과 구와의 충돌체크
bool Collision_RayToSphere(const Vector3 &rayPos, const Vector3 &rayDir, const Vector3 &spherePos, float radius);

//광선과 구와의 충돌에서 광선이 구까지 도달하는 거리를 리턴한다. 시간이 음수값이면 충돌하지않음
float Time_RayToSphere(const Vector3 &rayPos, const Vector3 &rayDir, const Vector3 &spherePos, float radius);

//광선과 AABB와의 충돌체크
bool Collision_RayToAABB(const Vector3 &rayPos, const Vector3 &rayDir, const Vector3 &minPos, const Vector3 &maxPos);

//광선과 AABB와의 충돌에서 광선이 AABB까지 도달하는 거리를 리턴한다. 시간이 음수값이면 충돌하지 않음
float Time_RayToAABB(const Vector3 &rayPos, const Vector3 &rayDir, const Vector3 &minPos, const Vector3 &maxPos);

//광선과 OBB와의 충돌체크
bool Collision_RayToOBB(const Vector3 &rayPos, const Vector3 &rayDir, const Vector3 &OBBPos, 
	const Vector3 &xVec, const Vector3 &yVec, const Vector3 &zVec, float xVecSize, float yVecSize, float zVecSize);

//광선과 OBB박스와의 충돌에서 광선이 OBB박스까지 도달하는 거리를 리턴한다. 시간이 음수값이면 충돌하지 않음.
bool Time_RayToOBB(const Vector3 &rayPos, Vector3 &rayDir, 
	Vector3 &OBBPos, Vector3 &xVec, Vector3 &yVec, Vector3 &zVec,
	float xVecSize, float yVecSize, float zVecSize);

bool IsBlocking(TransformComponent * pTransA, CollisionComponent::BoundingBox * pBoundA, TransformComponent * pTransB, CollisionComponent::BoundingBox * pBoundB, float moveFactor);
#endif