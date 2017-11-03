#include "stdafx.h"
#include "PhysicsComponent.h"

bool PhysicsComponent::PreInit(Entity * pOwner)
{
	return false;
}

bool PhysicsComponent::Init()
{
	return false;
}

bool PhysicsComponent::Release()
{
	return false;
}

void PhysicsComponent::Update(float deltaTime)
{
}

bool SpherePhysicsComponent::PreInit(Entity * pOwner)
{
	return false;
}

bool SpherePhysicsComponent::Init()
{
	return false;
}

bool SpherePhysicsComponent::Release()
{
	return false;
}

void SpherePhysicsComponent::Update(float deltaTime)
{
}

bool BoundingBoxPhysicsComponent::PreInit(Entity * pOwner)
{
	return false;
}

bool BoundingBoxPhysicsComponent::Init()
{
	return false;
}

bool BoundingBoxPhysicsComponent::Release()
{
	return false;
}

void BoundingBoxPhysicsComponent::Update(float deltaTime)
{
}
