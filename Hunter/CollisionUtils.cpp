#include "stdafx.h"
#include "CollisionUtils.h"
//
//bool32 IsRayHitBoundSphere(const Ray & ray, const CollisionComponent & collision, const TransformComponent & transform, Vector3 * pOutHitPos, Vector3 * pOutHitNormal)
//{
//	//��ġ�� 
//	const Matrix &matWorld = transform.GetFinalMatrix();
//
//	//������ �� 
//	const Vector3 &scale = transform.GetScale();
//
//	//�ٿ�� Sphere �� ������ ��´�.
//	Vector3 center;
//	float radius;
//	Vector3 halfSize;
//
//	//��ȯ�� ��ġ
//	Vec3TransformCoord(&center, &collision.->localCenter, &matWorld);
//
//	//������...
//	halfSize.x = pBound->halfSize.x * scale.x;
//	halfSize.y = pBound->halfSize.y * scale.y;
//	halfSize.z = pBound->halfSize.z * scale.z;
//	radius = D3DXVec3Length(&halfSize);
//
//	//������ ���������� ���� ���� ���ͱ����� ���⺤��
//	D3DXVECTOR3 dirToCenter = center - pRay->origin;
//
//	//������ ����
//	float lengthSq = D3DXVec3LengthSq(&dirToCenter);
//
//	//�������� ����
//	float r2 = radius * radius;
//
//	//���� ������ ���ȿ� �ִٸ�..
//	if (r2 > lengthSq)
//	{
//		//������ �ȿ��� ������ ���� üũ �ȵȴ�.
//		return false;
//	}
//
//
//	//����������� �������� ���ۿ� �ִٴ¿���
//	//�����ͱ����� ���⺤�Ϳ� ������ ���⺤�Ͱ� ������ ������ 
//	//�а��̶�� �׾��ٱ���� �浹���Ͼ���
//	float dot = D3DXVec3Dot(&dirToCenter, &pRay->direction);
//	if (dot <= 0.0f)
//	{
//		return false;
//	}
//
//	// ��Ÿ����� ������ �ϱ����� ���� �ﰢ�� ��������
//	// d2 = x2 + y2;
//	// d = sqrt( x2 + y2 );
//
//	float x2 = dot * dot;
//	float d2 = lengthSq;
//	//d2 - x2 = y2;
//	float y2 = d2 - x2;
//
//	//������ ������ �����.
//	if (y2 > r2) {
//		return false;
//	}
//
//	//��������´ٸ� �ϴ��� ��Ʈ
//	//���� ��ư��߾� HitPoint ���ִٸ�..
//	if (pHitPos != NULL)
//	{
//		//d�� raius ����
//		d2 = r2;
//		//d2 = y2 + x2
//		//float x2 = d2 - y2;
//		float x = sqrt(d2 - y2);
//
//		//
//		*pHitPos = pRay->origin + (pRay->direction * (dot - x));
//
//		//Hit �� ��ġ�� �븻�� ��ڴٸ�..
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
