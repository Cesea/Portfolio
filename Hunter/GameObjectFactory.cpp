#include "stdafx.h"
#include "GameObjectFactory.h"

#include "GameObjects.h"
//#include "World.h"

void GameObjectFactory::Init()
{
	_channel.Add<GameObjectFactory::CreateObjectOnClickEvent, GameObjectFactory>(*this);
	_channel.Add<GameObjectFactory::CreateObjectOnLocationEvent, GameObjectFactory>(*this);
}

void GameObjectFactory::Release()
{
	_channel.Remove<GameObjectFactory::CreateObjectOnClickEvent, GameObjectFactory>(*this);
	_channel.Remove<GameObjectFactory::CreateObjectOnLocationEvent, GameObjectFactory>(*this);
	_pCurrentScene = nullptr;
	_pPlayer = nullptr;
}

void GameObjectFactory::CreateObject(ARCHE_TYPE type, ResourceHandle handle, const Vector3 & position)
{
	switch (type)
	{
	case ARCHE_ROCK:
	{
		Entity &entity = _pCurrentScene->_world.CreateEntity();

		TransformComponent &transform = entity.AddComponent<TransformComponent>();
		transform._position = position;
		RenderComponent &render = entity.AddComponent<RenderComponent>();
		render._type = RenderComponent::Type::eStatic;
		render._arche = ARCHE_ROCK;

		TERRAIN->AddEntityToSection(entity, position);

		video::StaticXMeshHandle meshHandle;
		meshHandle.count = handle.count;
		meshHandle.index = handle.index;
		Assert(meshHandle.IsValid());
		render._static = meshHandle;

		video::StaticXMesh *pMesh = VIDEO->GetStaticXMesh(meshHandle);

		CollisionComponent &collision = entity.AddComponent<CollisionComponent>();
		collision._boundingBox.Init(pMesh->_meshBoundInfo._min, pMesh->_meshBoundInfo._max);
		collision._boundingSphere._localCenter = pMesh->_meshBoundInfo._center;
		collision._boundingSphere._radius = pMesh->_meshBoundInfo._radius;
		collision._locked = true;

		entity.Activate();

	}break;
	case ARCHE_TREE:
	{
		Entity &entity = _pCurrentScene->_world.CreateEntity();

		TransformComponent &transform = entity.AddComponent<TransformComponent>();
		transform._position = position;

		TERRAIN->AddEntityToSection(entity, position);

		RenderComponent &render = entity.AddComponent<RenderComponent>();
		render._type = RenderComponent::Type::eStatic;
		render._arche = ARCHE_TREE;

		video::StaticXMeshHandle meshHandle;
		meshHandle.count = handle.count;
		meshHandle.index = handle.index;
		Assert(meshHandle.IsValid());
		render._static = meshHandle;

		video::StaticXMesh *pMesh = VIDEO->GetStaticXMesh(render._static);

		CollisionComponent &collision = entity.AddComponent<CollisionComponent>();
		collision._boundingBox.Init(pMesh->_meshBoundInfo._min, pMesh->_meshBoundInfo._max);
		collision._boundingSphere._localCenter = pMesh->_meshBoundInfo._center;
		collision._boundingSphere._radius = pMesh->_meshBoundInfo._radius;
		collision._locked = true;

		entity.Activate();
	}break;
	case ARCHE_GRASS:
	{
		Entity &entity = _pCurrentScene->_world.CreateEntity();

		TransformComponent &transform = entity.AddComponent<TransformComponent>();
		transform._position = position;

		TERRAIN->AddEntityToSection(entity, position);

		RenderComponent &render = entity.AddComponent<RenderComponent>();
		render._type = RenderComponent::Type::eStatic;
		render._arche = ARCHE_GRASS;

		video::StaticXMeshHandle meshHandle;
		meshHandle.count = handle.count;
		meshHandle.index = handle.index;
		Assert(meshHandle.IsValid());
		render._static = meshHandle;

		video::StaticXMesh *pMesh = VIDEO->GetStaticXMesh(render._static);

		CollisionComponent &collision = entity.AddComponent<CollisionComponent>();
		collision._boundingBox.Init(pMesh->_meshBoundInfo._min, pMesh->_meshBoundInfo._max);
		collision._boundingSphere._localCenter = pMesh->_meshBoundInfo._center;
		collision._boundingSphere._radius = pMesh->_meshBoundInfo._radius;
		collision._locked = true;

		entity.Activate();
	}break;

	case ARCHE_HERO :
	{
		_pCurrentScene->_gameObjects.push_back(new Player());
		BaseGameObject *pBack = _pCurrentScene->_gameObjects.back();
		pBack->CreateFromWorld(_pCurrentScene->_world);
		//NOTE : 여기서 플레이어의 포인터를 저장하고는 있지만, 나중에는 키값으로 찾을 수 있게끔 바꿔야 한다...
		_pPlayer = pBack;
	}break;

	case ARCHE_BAT :
	{
		_pCurrentScene->_gameObjects.push_back(new Bat());
		BaseGameObject *pBack = _pCurrentScene->_gameObjects.back();
		pBack->CreateFromWorld(_pCurrentScene->_world);
		TransformComponent &refTransform = pBack->_entity.GetComponent<TransformComponent>();
		refTransform._position = position;
	}break;

	case ARCHE_CAT :
	{
		_pCurrentScene->_gameObjects.push_back(new Cat());
		BaseGameObject *pBack = _pCurrentScene->_gameObjects.back();
		pBack->CreateFromWorld(_pCurrentScene->_world);
		TransformComponent &refTransform = pBack->_entity.GetComponent<TransformComponent>();
		refTransform._position = position;
	}break;

	case ARCHE_LIZARD:
	{
		_pCurrentScene->_gameObjects.push_back(new Lizard());
		BaseGameObject *pBack = _pCurrentScene->_gameObjects.back();
		pBack->CreateFromWorld(_pCurrentScene->_world);
		TransformComponent &refTransform = pBack->_entity.GetComponent<TransformComponent>();
		refTransform._position = position;
	}break;

	case ARCHE_SNAKE:
	{
		_pCurrentScene->_gameObjects.push_back(new Snake());
		BaseGameObject *pBack = _pCurrentScene->_gameObjects.back();
		pBack->CreateFromWorld(_pCurrentScene->_world);
		TransformComponent &refTransform = pBack->_entity.GetComponent<TransformComponent>();
		refTransform._position = position;
	}break;

	case ARCHE_TURTLE :
	{
		_pCurrentScene->_gameObjects.push_back(new Turtle());
		BaseGameObject *pBack = _pCurrentScene->_gameObjects.back();
		pBack->CreateFromWorld(_pCurrentScene->_world);
		TransformComponent &refTransform = pBack->_entity.GetComponent<TransformComponent>();
		refTransform._position = position;
	}break;

	case ARCHE_HYDRA :
	{
		_pCurrentScene->_gameObjects.push_back(new Hydra());
		BaseGameObject *pBack = _pCurrentScene->_gameObjects.back();
		pBack->CreateFromWorld(_pCurrentScene->_world);
		TransformComponent &refTransform = pBack->_entity.GetComponent<TransformComponent>();
		refTransform._position = position;
	}break;
	}
}

void GameObjectFactory::CreateObject(ARCHE_TYPE type, char * name, const Vector3 & position)
{
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

	if (TERRAIN->IsIntersectRay(ray, &terrainHitPos))
	{
		CreateObject(event._type, event._handle, terrainHitPos);
	}

}

void GameObjectFactory::Handle(const CreateObjectOnLocationEvent & event)
{
	CreateObject(event._type, event._handle, event._position);
}
