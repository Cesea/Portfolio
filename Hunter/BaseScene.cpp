#include "stdafx.h"
#include "BaseScene.h"

using namespace video;


bool BaseScene::Init()
{
	return false;
}

bool BaseScene::Update(float deltaTime, const InputManager &input)
{
	
	return false;
}

bool BaseScene::Render()
{
	return false;
	
}

void BaseScene::Release()
{
}

//void BaseScene::Handle(const BaseScene::SpawnEvent & event)
//{
//	_entities.push_back(_world.CreateEntity());
//	Entity &entity = _entities.back();
//	TransformComponent &refTransform = entity.AddComponent<TransformComponent>();
//
//	float terrainHeight = _pTerrain->GetHeight(event._position.x, event._position.z);
//	refTransform.MovePositionWorld(Vector3(event._position.x, terrainHeight, event._position.z));
//
//	RenderComponent &refRender = entity.AddComponent<RenderComponent>();
//
//	if (event._isStatic)
//	{
//		refRender._type = RenderComponent::Type::eStatic;
//		refRender._static = VIDEO->GetStaticXMesh(event._name);
//		Assert(refRender._static.IsValid());
//	}
//	else
//	{
//		refRender._type = RenderComponent::Type::eSkinned;
//		video::SkinnedXMeshHandle meshHandle = VIDEO->GetSkinnedXMesh(event._name);
//		refRender._skinned = VIDEO->CreateAnimationInstance(meshHandle, event._name + std::to_string(entity.GetID().index));
//		Assert(refRender._skinned.IsValid());
//	}
//	entity.Activate();
//}