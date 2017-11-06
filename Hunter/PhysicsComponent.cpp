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

bool RigidPhysicsComponent::PreInit(Entity * pOwner)
{
	return false;
}

bool RigidPhysicsComponent::Init()
{
	return false;
}

bool RigidPhysicsComponent::Release()
{
	return false;
}

void RigidPhysicsComponent::Update(float deltaTime)
{
}