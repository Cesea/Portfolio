#include "stdafx.h"
#include "GameObjectFactory.h"

//#include "World.h"

void GameObjectFactory::Init()
{
	_channel.Add<GameObjectFactory::CreateObjectOnClickEvent, GameObjectFactory>(*this);
}

void GameObjectFactory::Release()
{
	_channel.Remove<GameObjectFactory::CreateObjectOnClickEvent, GameObjectFactory>(*this);
	_pCurrentScene = nullptr;
}

void GameObjectFactory::CreateObject(ArcheType type, const Vector3 & position)
{
	switch (type)
	{
	case eHero:
	{
	}break;
	case eRock:
	{
		_pCurrentScene->_entities.push_back(_pCurrentScene->_world.CreateEntity());
		Entity &entity = _pCurrentScene->_entities.back();

		TransformComponent &transform = entity.AddComponent<TransformComponent>();
		transform._position = position;
		RenderComponent &render = entity.AddComponent<RenderComponent>();
		render._type = RenderComponent::Type::eStatic;
		render._static = VIDEO->GetStaticXMesh("Rock01");

		entity.Activate();
	}break;
	case eTree:
	{
		_pCurrentScene->_entities.push_back(_pCurrentScene->_world.CreateEntity());
		Entity &entity = _pCurrentScene->_entities.back();

		TransformComponent &transform = entity.AddComponent<TransformComponent>();
		transform._position = position;
		RenderComponent &render = entity.AddComponent<RenderComponent>();
		render._type = RenderComponent::Type::eStatic;
		render._static = VIDEO->GetStaticXMesh("Tree01");

		entity.Activate();
	}break;
	case eGrass:
	{
		_pCurrentScene->_entities.push_back(_pCurrentScene->_world.CreateEntity());
		Entity &entity = _pCurrentScene->_entities.back();

		TransformComponent &transform = entity.AddComponent<TransformComponent>();
		transform._position = position;
		RenderComponent &render = entity.AddComponent<RenderComponent>();
		render._type = RenderComponent::Type::eStatic;
		render._static = VIDEO->GetStaticXMesh("Grass01");

		entity.Activate();
	}break;
	case eDragon:
	{
	}break;
	}
}

//void GameObjectFactory::Handle(const CreateObjectEvent & event)
//{
//}

void GameObjectFactory::Handle(const CreateObjectOnClickEvent & event)
{
	Vector3 position;
	Vector3 terrainHitPos;
	Ray ray;
	_pCurrentScene->_camera.ComputeRay(event._cursorPos, &ray);

	if (_pCurrentScene->_pTerrain->IsIntersectRay(ray, &terrainHitPos))
	{
		CreateObject(event._type, terrainHitPos);
	}

}
