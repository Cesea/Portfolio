#include "stdafx.h"
#include "RenderSystem.h"

RenderSystem::RenderSystem()
{
}

RenderSystem::~RenderSystem()
{
}

void RenderSystem::Render(video::RenderView &renderView, const Camera &camera)
{
	auto &entities = GetEntities();
	renderView.SetViewProjection(camera.GetViewMatrix(), camera.GetProjectionMatrix());

	renderView.PreRender();
	renderView.Begin();
	for (int32 i = 0; i < entities.size(); ++i)
	{
		TransformComponent &transformComponent = entities[i].GetComponent<TransformComponent>();
		RenderComponent &refRenderComponent = entities[i].GetComponent<RenderComponent>();

		//if (camera.GetFrustum().IsSphereInFrustum(transformComponent.GetWorldPosition(), 50.0f))
		{
			renderView.SetEffect(refRenderComponent.model._effect);
			//변환 행렬을 설정한다
			for (uint32 i = 0; i < refRenderComponent.model._skeleton._numhierachy; ++i)
			{
				renderView.SetTransform(refRenderComponent.model._skeleton._globalPoses[i]);
				
				for (uint32 j = 0; j < refRenderComponent.model._groups.size(); ++j)
				{
					renderView.SubmitGroup(refRenderComponent.model._groups[j]);
					renderView.Draw();
				}
			}
			//renderView.SetTransform(transformComponent.GetFinalMatrix());

			//이펙트를 설정한다

			//RenderGroup을 돌면서 각각의 그룹을 제출한다
			
			//for (uint32 i = 0; i < refRenderComponent.model._groups.size(); ++i)
			//{
			//	renderView.Submit(refRenderComponent.model._groups[i]._vertexBuffer);
			//	renderView.Submit(refRenderComponent.model._groups[i]._indexBuffer);
			//	renderView.Draw();
			//}
			//renderView.SetMaterial(refRenderComponent._material);
			//renderView.SetEffect(refRenderComponent._effect);
			//renderView.Submit(refRenderComponent._vertexBuffer);
			//renderView.Submit(refRenderComponent._indexBuffer);
		}
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

//void RenderSystem::SubmitMesh(Mesh & mesh)
//{
//
//}
