#include "stdafx.h"
#include "RenderSystem.h"

#define RENDER_TILE_TEST

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
	static int32 count = 0;
	static int32 renderCount = 0;

#if defined (RENDER_TILE_TEST)

	std::vector<Terrain::TerrainTile *> &visibleTiles = TERRAIN->GetVisibleTerrainTiles();
	for (auto &tile : visibleTiles)
	{
		for (uint32 i = 0; i < tile->_entities.size(); ++i)
		{
			TransformComponent &refTransformComponent = tile->_entities[i].GetComponent<TransformComponent>();
			RenderComponent &refRenderComponent = tile->_entities[i].GetComponent<RenderComponent>();
			if (refRenderComponent._type == RenderComponent::Type::eBuffer)
			{
			}
			else if (refRenderComponent._type == RenderComponent::Type::eStatic)
			{
				video::StaticXMesh *pMesh = VIDEO->GetStaticXMesh(refRenderComponent._static);
				pMesh->Render(refRenderComponent._arche, refTransformComponent);

#if defined (DEBUG) || defined (_DEBUG)
				/*CollisionComponent &refCollisionComp = entities[i].GetComponent<CollisionComponent>();
				refCollisionComp.RenderBoxGizmo(refTransformComponent);*/
#endif
				renderCount++;
			}
			else if (refRenderComponent._type == RenderComponent::Type::eSkinned)
			{
				video::AnimationInstance *pAnimation = VIDEO->GetAnimationInstance(refRenderComponent._skinned);
				ActionComponent &actionComp = tile->_entities[i].GetComponent<ActionComponent>();
				actionComp._pAnimationController->AdvanceTime(actionComp._animDelta, actionComp._pCallbackHandler);
				pAnimation->_pSkinnedMesh->Update(&refTransformComponent.GetFinalMatrix());
				pAnimation->_pSkinnedMesh->Render(refTransformComponent);

#if defined (DEBUG) || defined (_DEBUG)
				/*CollisionComponent &refCollisionComp = entities[i].GetComponent<CollisionComponent>();
				refCollisionComp.RenderBoxGizmo(refTransformComponent);*/
#endif
				renderCount++;
			}
		}
	}

#else
	auto &entities = GetEntities();

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
			pMesh->Render(refRenderComponent._arche, refTransformComponent);
			
#if defined (DEBUG) || defined (_DEBUG)
			/*CollisionComponent &refCollisionComp = entities[i].GetComponent<CollisionComponent>();
			refCollisionComp.RenderBoxGizmo(refTransformComponent);*/
#endif
			renderCount++;
		}
		else if (refRenderComponent._type == RenderComponent::Type::eSkinned)
		{
			video::AnimationInstance *pAnimation = VIDEO->GetAnimationInstance(refRenderComponent._skinned);
			ActionComponent &actionComp = entities[i].GetComponent<ActionComponent>();
			actionComp._pAnimationController->AdvanceTime(actionComp._animDelta, actionComp._pCallbackHandler);
			pAnimation->_pSkinnedMesh->Update(&refTransformComponent.GetFinalMatrix());
			pAnimation->_pSkinnedMesh->Render(refTransformComponent);

#if defined (DEBUG) || defined (_DEBUG)
			/*CollisionComponent &refCollisionComp = entities[i].GetComponent<CollisionComponent>();
			refCollisionComp.RenderBoxGizmo(refTransformComponent);*/
#endif
			renderCount;
		}
	}
#endif
	
	if (count % 60 == 0)
	{
		Console::Log("Render Count : %d\n", renderCount);
		renderCount = 0;
	}
	count++;
}

void RenderSystem::RenderShadow(const Camera & camera)
{
	std::vector<Terrain::TerrainTile *> &visibleTiles = TERRAIN->GetVisibleTerrainTiles();
	for (auto &tile : visibleTiles)
	{
		for (uint32 i = 0; i < tile->_entities.size(); ++i)
		{
			TransformComponent &refTransformComponent = tile->_entities[i].GetComponent<TransformComponent>();
			RenderComponent &refRenderComponent = tile->_entities[i].GetComponent<RenderComponent>();
			if (refRenderComponent._type == RenderComponent::Type::eBuffer)
			{
			}
			else if (refRenderComponent._type == RenderComponent::Type::eStatic)
			{
				video::StaticXMesh *pMesh = VIDEO->GetStaticXMesh(refRenderComponent._static);
				pMesh->RenderShadow(refTransformComponent);
			}
			else if (refRenderComponent._type == RenderComponent::Type::eSkinned)
			{
				video::AnimationInstance *pAnimation = VIDEO->GetAnimationInstance(refRenderComponent._skinned);
				pAnimation->_pSkinnedMesh->RenderShadow();

			}
		}
	}


//	auto &entities = GetEntities();
//	//renderView.SetViewProjection(camera.GetViewMatrix(), camera.GetProjectionMatrix());
//
//	for (uint32 i = 0; i < entities.size(); ++i)
//	{
//		TransformComponent &refTransformComponent = entities[i].GetComponent<TransformComponent>();
//		RenderComponent &refRenderComponent = entities[i].GetComponent<RenderComponent>();
//
//		if (refRenderComponent._type == RenderComponent::Type::eBuffer)
//		{
//		}
//		else if (refRenderComponent._type == RenderComponent::Type::eStatic)
//		{
//			video::StaticXMesh *pMesh = VIDEO->GetStaticXMesh(refRenderComponent._static);
//			pMesh->RenderShadow(refTransformComponent);
//			
//		}
//		else if (refRenderComponent._type == RenderComponent::Type::eSkinned)
//		{
//			video::AnimationInstance *pAnimation = VIDEO->GetAnimationInstance(refRenderComponent._skinned);
//			pAnimation->_pSkinnedMesh->RenderShadow();
//		}
//	}
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