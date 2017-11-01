#include "stdafx.h"
#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem()
{
}

PhysicsSystem::~PhysicsSystem()
{
}

bool PhysicsSystem::Init(const std::string & name, SystemSetting & setting)
{
	_name = name;
	return true;
}

void PhysicsSystem::ShutDown()
{
}

void PhysicsSystem::Update(float deltaTime)
{
}
