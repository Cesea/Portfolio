#include "stdafx.h"
#include "RenderSystem.h"

RenderSystem::RenderSystem()
{
}

RenderSystem::~RenderSystem()
{
}

void RenderSystem::UpdateAnimations(float deltaTime)
{
	//video::SkinnedAnimation *pFirstPointer = VIDEO->GetSkinnedAnimation(video::SkinnedAnimationHandle());

	//auto &entities = GetEntities();
	//for (uint32 i = 0; i < entities.size(); ++i)
	//{
	//	RenderComponent &refRenderComponent = entities[i].GetComponent<RenderComponent>();
	//	if (refRenderComponent._type == RenderComponent::Type::eSkinned)
	//	{
	//		video::SkinnedAnimation *pAnimation = VIDEO->GetSkinnedAnimation(refRenderComponent._skinned);
	//		pAnimation->UpdateAnimation(deltaTime);
	//	}
	//}
}

void RenderSystem::Render(const Camera &camera)
{
	//video::StaticXMesh *pFirstStatic = VIDEO->GetStaticXMesh(video::StaticXMeshHandle());
	//video::SkinnedAnimation *pFirstAnimation = VIDEO->GetSkinnedAnimation(video::SkinnedAnimationHandle());

	auto &entities = GetEntities();
	//renderView.SetViewProjection(camera.GetViewMatrix(), camera.GetProjectionMatrix());
	Matrix worldMatrix;

	for (uint32 i = 0; i < entities.size(); ++i)
	{
		TransformComponent &refTransformComponent = entities[i].GetComponent<TransformComponent>();
		RenderComponent &refRenderComponent = entities[i].GetComponent<RenderComponent>();

		if (refRenderComponent._type == RenderComponent::Type::eBuffer)
		{
			//video::RenderCommand &refCommand = renderView.GetCommand();
			//refCommand._vHandle = refRenderComponent._vHandle;
			//refCommand._iHandle = refRenderComponent._iHandle;
			//refCommand._materialHandle = refRenderComponent._material;
			//refCommand._effectHandle = refRenderComponent._effect;

			//worldMatrix = refTransformComponent.GetFinalMatrix();
			//video::MatrixCache::CacheRange range = renderView._matrixCache.Add(&refTransformComponent.GetFinalMatrix());

			//refCommand._cacheRange = range;
		}
		else if (refRenderComponent._type == RenderComponent::Type::eStatic)
		{
			video::StaticXMesh *pMesh = VIDEO->GetStaticXMesh(refRenderComponent._static);
			pMesh->Render(refTransformComponent);
			
#if defined (DEBUG) || defined (_DEBUG)
			CollisionComponent &refCollisionComp = entities[i].GetComponent<CollisionComponent>();
			refCollisionComp.RenderBoxGizmo(refTransformComponent);
#endif
		}
		else if (refRenderComponent._type == RenderComponent::Type::eSkinned)
		{
			video::AnimationInstance *pAnimation = VIDEO->GetAnimationInstance(refRenderComponent._skinned);
			ActionComponent &actionComp = entities[i].GetComponent<ActionComponent>();
			actionComp._pAnimationController->AdvanceTime(actionComp._animDelta, actionComp._pCallbackHandler);
			pAnimation->_pSkinnedMesh->Update(&refTransformComponent.GetFinalMatrix());
			pAnimation->_pSkinnedMesh->Render(refTransformComponent);

#if defined (DEBUG) || defined (_DEBUG)
			CollisionComponent &refCollisionComp = entities[i].GetComponent<CollisionComponent>();
			refCollisionComp.RenderBoxGizmo(refTransformComponent);
#endif

		}
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

void RenderSystem::FillRenderView(const RenderComponent & component, video::RenderView & renderView)
{

}