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
	renderView.PreRender();
	renderView.Begin();
	for (int32 i = 0; i < entities.size(); ++i)
	{

		Matrix world = entities[i].GetComponent<TransformComponent>().GetFinalMatrix();
		RenderComponent &refRenderComponent = entities[i].GetComponent<RenderComponent>();

		renderView.SetTransform(world);
		//renderView->SetMaterial(refRenderComponent._material);
		renderView.SetEffect(refRenderComponent._effect);
		renderView.Submit(refRenderComponent._vertexBuffer);
		renderView.Submit(refRenderComponent._indexBuffer);
		renderView.Draw();
	}
	renderView.End();
	renderView.PostRender();

	VIDEO->Render(renderView);
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
