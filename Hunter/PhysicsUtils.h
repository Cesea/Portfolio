#ifndef PHYSICS_UTILS_H
#define PHYSICS_UTILS_H


#include "PhysicsComponent.h"

//���� �ٿ�忡 �浹�ߴ��� Ȯ��
bool IsRayHitBound(
	Ray *pRay,
	PhysicsComponent *pComponent,
	Transform* pBoundTrans,	//�ٿ���� Ʈ����
	D3DXVECTOR3* pHitPos, //hit ��ġ (NULL)
	D3DXVECTOR3* pHitNormal);


//���� �ٿ�忡 �浹�ߴ��� Ȯ��
bool IsRayHitBound(
	Ray *pRay,
	PhysicsComponent *pBound,
	Transform* pBoundTrans,	//�ٿ���� Ʈ����
	D3DXVECTOR3* pHitPos, //hit ��ġ (NULL)
	D3DXVECTOR3* pHitNormal);

//���̰� ������Ʈ�� �浹
bool IsRayHitStaticMeshObject(
	Ray *pRay,
	PhysicsComponent *pComponent,
	D3DXVECTOR3* pHitPos,
	D3DXVECTOR3* pHitNormal
);

bool IsRayHitSphere(
	Ray *pRay,
	const D3DXVECTOR3* pCenter,
	float radius,
	D3DXVECTOR3* pHitPos,
	D3DXVECTOR3* pHitNormal);

//2���� �ٿ�忡 ���� �浹
bool IsOverlap(cBaseObject* pObjA, cBaseObject* pObjB);

bool IsOverlap(Transform* pTransA, PhysicsComponent* pBoundA,
	Transform* pTransB, cBound_Sphere* pBoundB);

bool IsOverlap(Transform* pTransA, cBound_Box* pBoundA,
	Transform* pTransB, cBound_Box* pBoundB);

bool IsOverlap(Transform* pTransA, cBound_Sphere* pBoundA,
	Transform* pTransB, cBound_Box* pBoundB);

bool IsOverlap(Transform* pTransA, cBound_Box* pBoundA,
	Transform* pTransB, cBound_Sphere* pBoundB);

bool IsBlocking(cBaseObject* pObjA, cBaseObject* pObjB, float moveFactor = 0.0f);

//2���� �ٿ�忡 ���� �浹�ϰ� ��ġ�� �ʰ� �Ѵ�
//moveFactor�� 0�ϼ��� ��Ĩ�� ���� b�� �����δ�.
//moveFactor�� 0.5�̸� ��Ĩ�� ���� A �� b�� ���̷����� �����δ�.
//moveFactor�� 1�ϼ��� ��Ĩ�� ���� A�� �����δ�.


bool IsBlocking(Transform* pTransA, cBound_Sphere* pBoundA,
	Transform* pTransB, cBound_Sphere* pBoundB, float moveFactor = 0.0f);

bool IsBlocking(Transform* pTransA, cBound_Box* pBoundA,
	Transform* pTransB, cBound_Box* pBoundB, float moveFactor = 0.0f);

//�� 3���� ���� ����� �����
void CteatePlane(LPD3DXPLANE pOutPlane, const D3DXVECTOR3* p0,
	const D3DXVECTOR3* p1, const D3DXVECTOR3* p2);

//������ �������� �������� �ִܰŸ�
float PlaneDot(const LPD3DXPLANE pPlane, const D3DXVECTOR3* point);

//�������� ����� �浹 ��ġ
bool IntersectRayToPlane(D3DXVECTOR3* pOut, const Ray *, const LPD3DXPLANE pPlane,
	bool bCheck2Side = false);


#endif