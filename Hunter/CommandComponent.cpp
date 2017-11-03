#include "stdafx.h"
#include "CommandComponent.h"

bool CommandComponent::PreInit(Entity * pOwner)
{
	return false;
}

bool CommandComponent::Init()
{
	return false;
}

bool CommandComponent::Release()
{
	return false;
}

void CommandComponent::Update(float deltaTime)
{
}

bool AICommandComponent::PreInit(Entity * pOwner)
{
	return false;
}

bool AICommandComponent::Init()
{
	return false;
}

bool AICommandComponent::Release()
{
	return false;
}

void AICommandComponent::Update(float deltaTime)
{
}

bool PlayerCommandComponent::PreInit(Entity * pOwner)
{
	return false;
}

bool PlayerCommandComponent::Init()
{
	return false;
}

bool PlayerCommandComponent::Release()
{
	return false;
}

void PlayerCommandComponent::Update(float deltaTime)
{
}
