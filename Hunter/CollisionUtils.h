#ifndef COLLISION_UTILLS_H
#define COLLISION_UTILLS_H

#include "CollisionComponent.h"
#include "TransformComponent.h"

////Shpere BoundingVolume�� ���� �Լ���...
//void GetWorldCenterRadiusShere(const BoundingSphere &sphere, 
//	TransformComponent *pOutTransform, Vector3 *pOutCenter, float *pOutRadius);
//void SetBoundVolume(BoundingSphere *pOutSphere, const Vector3 &center, const Vector3 &halfSize); 
//
////Box BoundingVolume�� ���� �Լ���
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
////���� �ٿ�忡 �浹�ߴ��� Ȯ��
//bool32 IsRayHitBoundSphere(
//	const Ray &ray,
//	const CollisionComponent &collision,
//	const TransformComponent &transform,	
//	Vector3 *pOutHitPos, 
//	Vector3 *pOutHitNormal);
//
//
////���� �ٿ�忡 �浹�ߴ��� Ȯ��
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
////2���� �ٿ�忡 ���� �浹
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
////2���� �ٿ�忡 ���� �浹�ϰ� ��ġ�� �ʰ� �Ѵ�
////moveFactor�� 0�ϼ��� ��Ĩ�� ���� b�� �����δ�.
////moveFactor�� 0.5�̸� ��Ĩ�� ���� A �� b�� ���̷����� �����δ�.
////moveFactor�� 1�ϼ��� ��Ĩ�� ���� A�� �����δ�.
//
//
//bool32 IsBlockingTwoSphere(const TransformComponent &pTransA, const CollisionComponent &collisionA,
//	const TransformComponent &pTransB, const CollisionComponent &collisionB, float moveFactor = 0.0f);
//
//bool32 IsBlockingTwoBox(const TransformComponent &transformA, const CollisionComponent &collisionA,
//	const TransformComponent &pTransB, const CollisionComponent &collisionB, float moveFactor = 0.0f);

#endif