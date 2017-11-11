#include "stdafx.h"
#include "CollisionUtils.h"
//
//bool32 IsRayHitBoundSphere(const Ray & ray, const CollisionComponent & collision, const TransformComponent & transform, Vector3 * pOutHitPos, Vector3 * pOutHitNormal)
//{
//	//위치값 
//	const Matrix &matWorld = transform.GetFinalMatrix();
//
//	//스케일 값 
//	const Vector3 &scale = transform.GetScale();
//
//	//바운드 Sphere 의 정보를 얻는다.
//	Vector3 center;
//	float radius;
//	Vector3 halfSize;
//
//	//변환된 위치
//	Vec3TransformCoord(&center, &collision.->localCenter, &matWorld);
//
//	//반지름...
//	halfSize.x = pBound->halfSize.x * scale.x;
//	halfSize.y = pBound->halfSize.y * scale.y;
//	halfSize.z = pBound->halfSize.z * scale.z;
//	radius = D3DXVec3Length(&halfSize);
//
//	//레이의 오리진에서 부터 구의 센터까지의 방향벡터
//	D3DXVECTOR3 dirToCenter = center - pRay->origin;
//
//	//길이의 제곱
//	float lengthSq = D3DXVec3LengthSq(&dirToCenter);
//
//	//반지름의 제곱
//	float r2 = radius * radius;
//
//	//만약 광선이 구안에 있다면..
//	if (r2 > lengthSq)
//	{
//		//광선이 안에서 나가는 것은 체크 안된다.
//		return false;
//	}
//
//
//	//여기까지오면 오리진은 구밖에 있다는예기
//	//구센터까지의 방향벡터와 레이의 방향벡터가 직각을 포함한 
//	//둔각이라면 죽었다깨어나도 충돌될일없다
//	float dot = D3DXVec3Dot(&dirToCenter, &pRay->direction);
//	if (dot <= 0.0f)
//	{
//		return false;
//	}
//
//	// 피타고라스의 정리를 하기위해 직각 삼각형 공식유도
//	// d2 = x2 + y2;
//	// d = sqrt( x2 + y2 );
//
//	float x2 = dot * dot;
//	float d2 = lengthSq;
//	//d2 - x2 = y2;
//	float y2 = d2 - x2;
//
//	//광선이 원밖을 벗어났다.
//	if (y2 > r2) {
//		return false;
//	}
//
//	//여기까지온다면 일단은 히트
//	//만약 얻아가야알 HitPoint 가있다면..
//	if (pHitPos != NULL)
//	{
//		//d를 raius 제곱
//		d2 = r2;
//		//d2 = y2 + x2
//		//float x2 = d2 - y2;
//		float x = sqrt(d2 - y2);
//
//		//
//		*pHitPos = pRay->origin + (pRay->direction * (dot - x));
//
//		//Hit 된 위치의 노말을 얻겠다면..
//		if (pHitNormal)
//		{
//			*pHitNormal = *pHitPos - center;
//			D3DXVec3Normalize(pHitNormal, pHitNormal);
//		}
//	}
//
//	return true;
//}
//
//bool32 IsRayHitBoundBox(const Ray & ray, const CollisionComponent & pBound, const TransformComponent & transform, Vector3 * pOutHitPos, Vector3 * pOutHitNormal)
//{
//	return bool32();
//}
//
//bool32 IsRayHitSphere(const Ray & ray, const Vector3 & center, float radius, Vector3 * pOutHitPos, Vector3 * pOutHitNormal)
//{
//	return bool32();
//}
//
//bool32 IsOverlap(cBaseObject * pObjA, cBaseObject * pObjB)
//{
//	return bool32();
//}
//
//bool32 IsOverlapTwoSphere(const TransformComponent & transformA, const CollisionComponent & collisionA, const TransformComponent & transformB, const CollisionComponent & collisionB)
//{
//	return bool32();
//}
//
//bool32 IsOverlapTwoBox(const TransformComponent & transformA, const CollisionComponent & collisionA, const TransformComponent & transformB, const CollisionComponent & collisionB)
//{
//	return bool32();
//}
//
//bool32 IsOverlapSphereBox(const TransformComponent & transformA, const CollisionComponent & collisionA, const TransformComponent & transformB, const CollisionComponent & collisionB)
//{
//	return bool32();
//}
//
//bool32 IsOverlapBoxSphere(const TransformComponent & transformA, const CollisionComponent & collisionA, const TransformComponent & transformB, const CollisionComponent & collisionB)
//{
//	return bool32();
//}
//
//bool32 IsBlockingTwoSphere(const TransformComponent & pTransA, const CollisionComponent & collisionA, const TransformComponent & pTransB, const CollisionComponent & collisionB, float moveFactor)
//{
//	return bool32();
//}
//
//bool32 IsBlockingTwoBox(const TransformComponent & transformA, const CollisionComponent & collisionA, const TransformComponent & pTransB, const CollisionComponent & collisionB, float moveFactor)
//{
//	return bool32();
//}
