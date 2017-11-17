#include "stdafx.h"
#include "RenderSystem.h"

RenderSystem::RenderSystem()
{
}

RenderSystem::~RenderSystem()
{
}

void RenderSystem::Render(video::RenderView &renderView)
{
	auto &entities = GetEntities();
	//renderView.SetViewProjection(camera.GetViewMatrix(), camera.GetProjectionMatrix());

	renderView.PreRender();
	for (int32 i = 0; i < entities.size(); ++i)
	{
		TransformComponent &transformComponent = entities[i].GetComponent<TransformComponent>();
		RenderComponent &refRenderComponent = entities[i].GetComponent<RenderComponent>();

		//if (camera.GetFrustum().IsSphereInFrustum(transformComponent.GetWorldPosition(), 50.0f))
		{

			//renderView.SetFillMode(video::RenderState::FillMode::eFillPoint);
			//변환 행렬을 설정한다

		}
	}
	renderView.PostRender();
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