#ifndef PHYSICS_UTILS_H
#define PHYSICS_UTILS_H


#include "PhysicsComponent.h"

//레이 바운드에 충돌했는지 확인
bool IsRayHitBound(
	Ray *pRay,
	PhysicsComponent *pComponent,
	Transform* pBoundTrans,	//바운드의 트랜스
	D3DXVECTOR3* pHitPos, //hit 위치 (NULL)
	D3DXVECTOR3* pHitNormal);


//레이 바운드에 충돌했는지 확인
bool IsRayHitBound(
	Ray *pRay,
	PhysicsComponent *pBound,
	Transform* pBoundTrans,	//바운드의 트랜스
	D3DXVECTOR3* pHitPos, //hit 위치 (NULL)
	D3DXVECTOR3* pHitNormal);

//레이가 오브젝트와 충돌
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

//2개의 바운드에 대한 충돌
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

//2개의 바운드에 대해 충돌하고 겹치지 않게 한다
//moveFactor가 0일수록 겹칩에 대해 b가 움직인다.
//moveFactor가 0.5이면 겹칩에 대해 A 와 b가 같이량으로 움직인다.
//moveFactor가 1일수록 겹칩에 대해 A가 움직인다.


bool IsBlocking(Transform* pTransA, cBound_Sphere* pBoundA,
	Transform* pTransB, cBound_Sphere* pBoundB, float moveFactor = 0.0f);

bool IsBlocking(Transform* pTransA, cBound_Box* pBoundA,
	Transform* pTransB, cBound_Box* pBoundB, float moveFactor = 0.0f);

//점 3개로 무한 평면을 만든다
void CteatePlane(LPD3DXPLANE pOutPlane, const D3DXVECTOR3* p0,
	const D3DXVECTOR3* p1, const D3DXVECTOR3* p2);

//임의의 한점에서 평면까지의 최단거리
float PlaneDot(const LPD3DXPLANE pPlane, const D3DXVECTOR3* point);

//반직선과 평면의 충돌 위치
bool IntersectRayToPlane(D3DXVECTOR3* pOut, const Ray *, const LPD3DXPLANE pPlane,
	bool bCheck2Side = false);


#endif