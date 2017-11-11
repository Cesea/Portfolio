#include "stdafx.h"
#include "TransformSystem.h"

TransformSystem::TransformSystem()
{
}

TransformSystem::~TransformSystem()
{
}

void TransformSystem::PreUpdate(float deltaTime)
{
	auto &entities = GetEntities();
	for (int32 i = 0; i < entities.size(); ++i)
	{
		TransformComponent &refTransform = entities[i].GetComponent<TransformComponent>();
		UpdateTransform(refTransform);
	}
}

void TransformSystem::Update(float deltaTime)
{
}

void TransformSystem::PostUpdate(float deltaTime)
{
}

void TransformSystem::Initialize()
{
	std::cout << "Transform System Initialized" << std::endl;
}

void TransformSystem::OnEntityAdded(Entity & entity)
{
}

void TransformSystem::OnEntityRemoved(Entity & entity)
{
}

void TransformSystem::UpdateTransform(TransformComponent &component)
{
	//자신의 정보로 matLocal 행렬을 갱신한다.
	MatrixIdentity(&component._matLocal);

	//스케일 먹은 축정보
	const Vector3 &scaledRight = component._right * component._scale.x;
	const Vector3 &scaledUp = component._up * component._scale.y;
	const Vector3 &scaledForward = component._forward * component._scale.z;

	memcpy(&component._matLocal._11, &scaledRight, sizeof(Vector3));
	memcpy(&component._matLocal._21, &scaledUp, sizeof(Vector3));
	memcpy(&component._matLocal._31, &scaledForward, sizeof(Vector3));
	memcpy(&component._matLocal._41, &component._position, sizeof(Vector3));

	//나의 최종 행렬
	if (nullptr == component._pParent)
	{
		component._matFinal = component._matLocal;
	}
	//내가 부모가 있다면...
	else 
	{
		component._matFinal = component._matLocal * component._pParent->_matFinal;
	}

	//자식 업데이트
	TransformComponent* pChild = component._pFirstChild;
	//자식들아 너희들도 업데이트
	while (pChild != NULL)
	{
		UpdateTransform(*pChild);
		pChild = pChild->_pNextSibling;
	}

}
