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
	//�ڽ��� ������ matLocal ����� �����Ѵ�.
	MatrixIdentity(&component._matLocal);

	//������ ���� ������
	const Vector3 &scaledRight = component._right * component._scale.x;
	const Vector3 &scaledUp = component._up * component._scale.y;
	const Vector3 &scaledForward = component._forward * component._scale.z;

	memcpy(&component._matLocal._11, &scaledRight, sizeof(Vector3));
	memcpy(&component._matLocal._21, &scaledUp, sizeof(Vector3));
	memcpy(&component._matLocal._31, &scaledForward, sizeof(Vector3));
	memcpy(&component._matLocal._41, &component._position, sizeof(Vector3));

	//���� ���� ���
	if (nullptr == component._pParent)
	{
		component._matFinal = component._matLocal;
	}
	//���� �θ� �ִٸ�...
	else 
	{
		component._matFinal = component._matLocal * component._pParent->_matFinal;
	}

	//�ڽ� ������Ʈ
	TransformComponent* pChild = component._pFirstChild;
	//�ڽĵ�� ����鵵 ������Ʈ
	while (pChild != NULL)
	{
		UpdateTransform(*pChild);
		pChild = pChild->_pNextSibling;
	}

}
