#ifndef COLLISION_UTILLS_H
#define COLLISION_UTILLS_H

#include "CollisionComponent.h"
#include "TransformComponent.h"

////Shpere BoundingVolume에 대한 함수들...
//void GetWorldCenterRadiusShere(const BoundingSphere &sphere, 
//	TransformComponent *pOutTransform, Vector3 *pOutCenter, float *pOutRadius);
//void SetBoundVolume(BoundingSphere *pOutSphere, const Vector3 &center, const Vector3 &halfSize); 
//
////Box BoundingVolume에 대한 함수들
//void GetWorldBox(const BoundingBox &box, TransformComponent* pOutTransform, Vector3 *pOutBoxPos);
//void GetWorldAABBMinMax(const BoundingBox &box, TransformComponent *pOutTransform, 
//	Vector3 *pOutMin, Vector3 *pOutMax);
//void SetBoundBox(const Vector3 &center, const Vector3 &halfSize, BoundingBox *pOutBoundingBox);
//
//
////struct CollisionResult
////{
////};
//
////레이 바운드에 충돌했는지 확인
//bool32 IsRayHitBoundSphere(
//	const Ray &ray,
//	const CollisionComponent &collision,
//	const TransformComponent &transform,	
//	Vector3 *pOutHitPos, 
//	Vector3 *pOutHitNormal);
//
//
////레이 바운드에 충돌했는지 확인
//bool32 IsRayHitBoundBox( const Ray &ray, const CollisionComponent &pBound, const TransformComponent &transform,	Vector3 *pOutHitPos, Vector3 *pOutHitNormal); 
//
////bool32 IsRayHitStaticMeshObject( const Ray &ray, const CollisionComponent &pObject, Vector3 *pOutHitPos, Vector3 *pOutHitNormal);
//
//bool32 IsRayHitSphere(
//	const Ray &ray,
//	const Vector3 &center,
//	float radius,
//	Vector3 *pOutHitPos,
//	Vector3 *pOutHitNormal);
//
////2개의 바운드에 대한 충돌
//bool32 IsOverlap(cBaseObject* pObjA, cBaseObject* pObjB);
//
//bool32 IsOverlapTwoSphere(const TransformComponent &transformA, const CollisionComponent &collisionA,
//	const TransformComponent &transformB, const CollisionComponent &collisionB);
//
//bool32 IsOverlapTwoBox(const TransformComponent &transformA, const CollisionComponent &collisionA,
//	const TransformComponent &transformB, const CollisionComponent &collisionB);
//
//bool32 IsOverlapSphereBox(const TransformComponent &transformA, const CollisionComponent &collisionA,
//	const TransformComponent &transformB, const CollisionComponent &collisionB);
//
//bool32 IsOverlapBoxSphere(const TransformComponent &transformA, const CollisionComponent &collisionA,
//	const TransformComponent &transformB, const CollisionComponent &collisionB);
//
////bool32 IsBlocking(cBaseObject* pObjA, cBaseObject* pObjB, float moveFactor = 0.0f);
//
////2개의 바운드에 대해 충돌하고 겹치지 않게 한다
////moveFactor가 0일수록 겹칩에 대해 b가 움직인다.
////moveFactor가 0.5이면 겹칩에 대해 A 와 b가 같이량으로 움직인다.
////moveFactor가 1일수록 겹칩에 대해 A가 움직인다.
//
//
//bool32 IsBlockingTwoSphere(const TransformComponent &pTransA, const CollisionComponent &collisionA,
//	const TransformComponent &pTransB, const CollisionComponent &collisionB, float moveFactor = 0.0f);
//
//bool32 IsBlockingTwoBox(const TransformComponent &transformA, const CollisionComponent &collisionA,
//	const TransformComponent &pTransB, const CollisionComponent &collisionB, float moveFactor = 0.0f);

#endif