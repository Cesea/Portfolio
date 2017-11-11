#include "stdafx.h"

#include "MoveSystem.h"


MoveSystem::MoveSystem()
	:System::System()
{
}

MoveSystem::~MoveSystem()
{
}

void MoveSystem::Update(float deltaTime)
{
	auto &entities = GetEntities();
	for (int32 i = 0; i < entities.size(); ++i)
	{
		Vector3 &refPosition = entities[i].GetComponent<TransformComponent>()._position;
		MoveComponent &refMoveComponent = entities[i].GetComponent<MoveComponent>();

		refMoveComponent.velocity += refMoveComponent.accel;

		refPosition += refMoveComponent.velocity;

		//std::cout << refPosition.x << " " <<refPosition.y << " " <<  refPosition.z << std::endl;
	}
}

void MoveSystem::Initialize()
{
	std::cout << "Init" << std::endl;
}

void MoveSystem::OnEntityAdded(Entity & entity)
{
	std::cout << "Entity " << entity.GetID().index << " Added" << std::endl;
}

void MoveSystem::OnEntityRemoved(Entity & entity)
{
	std::cout << "Entity " << entity.GetID().index << " Removed" << std::endl;
}
