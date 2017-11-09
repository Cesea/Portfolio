#include "stdafx.h"
#include "RenderSystem.h"

RenderSystem::RenderSystem()
{
}

RenderSystem::~RenderSystem()
{
}

void RenderSystem::Update(float deltaTime)
{
	auto &entities = GetEntities();
	for (int32 i = 0; i < entities.size(); ++i)
	{
		Vector3 &refPosition = entities[i].GetComponent<TransformComponent>()._position;

		Matrix world;
		MatrixTranslation(&world, 0.0f, 0.0f, 0.0f);

		RenderComponent &refRenderComponent = entities[i].GetComponent<RenderComponent>();

		std::cout << "Rendering " << std::endl;
	}
}

void RenderSystem::Initialize()
{
	std::cout << "Reder system online" << std::endl;
}

void RenderSystem::OnEntityAdded(Entity & entity)
{
}

void RenderSystem::OnEntityRemoved(Entity & entity)
{
}
