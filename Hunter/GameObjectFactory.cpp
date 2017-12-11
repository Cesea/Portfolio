#include "stdafx.h"
#include "GameObjectFactory.h"

#include "GameObjects.h"
//#include "World.h"

void GameObjectFactory::Init()
{
	_channel.Add<GameObjectFactory::CreateObjectOnClickEvent, GameObjectFactory>(*this);
	_channel.Add<GameObjectFactory::CreateObjectOnLocationEvent, GameObjectFactory>(*this);
	_channel.Add<GameObjectFactory::CreateObjectFromSaveInfoEvent, GameObjectFactory>(*this);
	_channel.Add<GameObjectFactory::DamageBoxEvent, GameObjectFactory>(*this);
	_channel.Add<GameObjectFactory::CreateBlood, GameObjectFactory>(*this);
	_channel.Add<GameObjectFactory::CreateBreath, GameObjectFactory>(*this);
	_channel.Add<GameObjectFactory::CreateNFireBall, GameObjectFactory>(*this);
	_channel.Add<GameObjectFactory::CreateNFireBall2, GameObjectFactory>(*this);
	_channel.Add<GameObjectFactory::CreateCharge, GameObjectFactory>(*this);
	_channel.Add<GameObjectFactory::CreateDragonBreath, GameObjectFactory>(*this);
}

void GameObjectFactory::Release()
{
	_channel.Remove<GameObjectFactory::CreateObjectOnClickEvent, GameObjectFactory>(*this);
	_channel.Remove<GameObjectFactory::CreateObjectOnLocationEvent, GameObjectFactory>(*this);
	_channel.Remove<GameObjectFactory::CreateObjectFromSaveInfoEvent, GameObjectFactory>(*this);
	_channel.Remove<GameObjectFactory::DamageBoxEvent, GameObjectFactory>(*this);
	_channel.Remove<GameObjectFactory::CreateBlood, GameObjectFactory>(*this);
	_channel.Remove<GameObjectFactory::CreateBreath, GameObjectFactory>(*this);
	_channel.Remove<GameObjectFactory::CreateNFireBall, GameObjectFactory>(*this);
	_channel.Remove<GameObjectFactory::CreateNFireBall2, GameObjectFactory>(*this);
	_channel.Remove<GameObjectFactory::CreateCharge, GameObjectFactory>(*this);
	_channel.Remove<GameObjectFactory::CreateDragonBreath, GameObjectFactory>(*this);
	_pCurrentScene = nullptr;
	_pPlayer = nullptr;
}

void GameObjectFactory::CreateObject(ARCHE_TYPE type, 
	ResourceHandle handle, 
	const Vector3 & position, 
	const Vector3 & scale, 
	const Vector3 & orientation)
{
	Quaternion quat(orientation.x, orientation.y, orientation.z, 1.0f);
	QuaternionNormalize(&quat, &quat);
	switch (type)
	{
	case ARCHE_ROCK:
	{
		Entity &entity = _pCurrentScene->_world.CreateEntity();

		TransformComponent &transform = entity.AddComponent<TransformComponent>();
		transform._position = position;
		transform.SetScale(scale);
		transform.SetRotateWorld(quat);

		RenderComponent &render = entity.AddComponent<RenderComponent>();
		render._type = RenderComponent::Type::eStatic;
		render._arche = ARCHE_ROCK;

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
		collision._triggerType = CollisionComponent::TRIGGER_TYPE_OBJECT;

		//entity.Activate();
		_channel.Broadcast<GameObjectFactory::ObjectCreatedEvent>(
			ObjectCreatedEvent(ARCHE_ROCK, entity, transform.GetWorldPosition()));

	}break;
	case ARCHE_TREE:
	{
		Entity &entity = _pCurrentScene->_world.CreateEntity();

		TransformComponent &transform = entity.AddComponent<TransformComponent>();
		transform._position = position;
		transform.SetScale(scale);
		transform.SetRotateWorld(quat);

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

		//entity.Activate();

		_channel.Broadcast<GameObjectFactory::ObjectCreatedEvent>(
			ObjectCreatedEvent(ARCHE_TREE, entity, transform.GetWorldPosition()));
	}break;

	case ARCHE_TREETRUNK :
	{
		Entity &entity = _pCurrentScene->_world.CreateEntity();

		TransformComponent &transform = entity.AddComponent<TransformComponent>();
		transform._position = position;
		transform.SetScale(scale);
		transform.SetRotateWorld(quat);

		RenderComponent &render = entity.AddComponent<RenderComponent>();
		render._type = RenderComponent::Type::eStatic;
		render._arche = ARCHE_TREETRUNK;

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

		_channel.Broadcast<GameObjectFactory::ObjectCreatedEvent>(
			ObjectCreatedEvent(ARCHE_TREETRUNK, entity, transform.GetWorldPosition()));
	}break;

	case ARCHE_GRASS:
	{
		Entity &entity = _pCurrentScene->_world.CreateEntity();

		TransformComponent &transform = entity.AddComponent<TransformComponent>();
		transform._position = position;
		transform.SetScale(scale);
		transform.SetRotateWorld(quat);

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
		collision._locked = true;;

		_channel.Broadcast<GameObjectFactory::ObjectCreatedEvent>(
			ObjectCreatedEvent(ARCHE_GRASS, entity, transform.GetWorldPosition()));
	}break;

	case ARCHE_MUSHROOM :
	{
		Entity &entity = _pCurrentScene->_world.CreateEntity();

		TransformComponent &transform = entity.AddComponent<TransformComponent>();
		transform._position = position;
		transform.SetScale(scale);
		transform.SetRotateWorld(quat);

		RenderComponent &render = entity.AddComponent<RenderComponent>();
		render._type = RenderComponent::Type::eStatic;
		render._arche = ARCHE_MUSHROOM;

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
		collision._type = CollisionComponent::COLLISION_TYPE_BOX;
		collision._locked = true;
		collision._triggerType = CollisionComponent::TRIGGER_TYPE_OBJECT;
		collision._isTrigger = true;

		//entity.Activate();

		_channel.Broadcast<GameObjectFactory::ObjectCreatedEvent>(
			ObjectCreatedEvent(ARCHE_MUSHROOM, entity, transform.GetWorldPosition()));
	}break;

	case ARCHE_HERO :
	{
		//NOTE : SetLinkCamera를 이렇게 할때 MultiThreadedLoading을 할때 터질 수 있다
		Player* _player = new Player();
		_player->SetLinkCamera(&_pCurrentScene->_camera);
		_pCurrentScene->_gameObjects.push_back(_player);
		BaseGameObject *pBack = _pCurrentScene->_gameObjects.back();
		pBack->CreateFromWorld(_pCurrentScene->_world, position);
		//NOTE : 여기서 플레이어의 포인터를 저장하고는 있지만, 나중에는 키값으로 찾을 수 있게끔 바꿔야 한다...
		_pPlayer = pBack;

		TerrainTilePos tilePos;
		TERRAIN->ConvertWorldPostoTilePos(position, &tilePos);
		std::vector<int32> &activeChunks = TERRAIN->GetActiveTerrainChunkIndices();
		int32 minX = tilePos._chunkX - 1;
		int32 maxX = tilePos._chunkX + 1;
		int32 minZ = tilePos._chunkZ - 1;
		int32 maxZ = tilePos._chunkZ + 1;

		ClampInt(minX, 0, TERRAIN->GetXChunkCount() - 1);
		ClampInt(maxX, 0, TERRAIN->GetXChunkCount() - 1);
		ClampInt(minZ, 0, TERRAIN->GetZChunkCount() - 1);
		ClampInt(maxZ, 0, TERRAIN->GetZChunkCount() - 1);

		for (int32 z = minZ; z <= maxZ; ++z)
		{
			for (int32 x = minX; x <= maxX; ++x)
			{
				activeChunks.push_back(Index2D(x, z, TERRAIN->GetXChunkCount()));
			}
		}
		_pCurrentScene->_camera.SetTargetObject((Player *)_pPlayer);
	}break;

	case ARCHE_BAT :
	{
		_pCurrentScene->_gameObjects.push_back(new Bat());
		BaseGameObject *pBack = _pCurrentScene->_gameObjects.back();
		pBack->CreateFromWorld(_pCurrentScene->_world, position);

	}break;
	case ARCHE_CAT :
	{
		_pCurrentScene->_gameObjects.push_back(new Cat());
		BaseGameObject *pBack = _pCurrentScene->_gameObjects.back();
		pBack->CreateFromWorld(_pCurrentScene->_world, position);

	}break;
	case ARCHE_LIZARD:
	{
		_pCurrentScene->_gameObjects.push_back(new Lizard());
		BaseGameObject *pBack = _pCurrentScene->_gameObjects.back();
		pBack->CreateFromWorld(_pCurrentScene->_world, position);

	}break;
	case ARCHE_SNAKE:
	{
		_pCurrentScene->_gameObjects.push_back(new Snake());
		BaseGameObject *pBack = _pCurrentScene->_gameObjects.back();
		pBack->CreateFromWorld(_pCurrentScene->_world, position);

	}break;
	case ARCHE_TURTLE :
	{
		_pCurrentScene->_gameObjects.push_back(new Turtle());
		BaseGameObject *pBack = _pCurrentScene->_gameObjects.back();
		pBack->CreateFromWorld(_pCurrentScene->_world, position);

	}break;
	case ARCHE_HYDRA :
	{
		_pCurrentScene->_gameObjects.push_back(new Hydra());
		BaseGameObject *pBack = _pCurrentScene->_gameObjects.back();
		pBack->CreateFromWorld(_pCurrentScene->_world, position);

	}break;
	case ARCHE_DRAGON:
	{
		_pCurrentScene->_gameObjects.push_back(new Dragon());
		BaseGameObject *pBack = _pCurrentScene->_gameObjects.back();
		pBack->CreateFromWorld(_pCurrentScene->_world, position);
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

	Vector3 randomScale(event._scale, event._scale, event._scale);
	Vector3 randomYOrientation(0.0f, event._yRotation, 0.0f);

	if (TERRAIN->IsIntersectRay(ray, &terrainHitPos))
	{
		position = terrainHitPos;
		position.y = TERRAIN->GetHeight(position.x, position.z) - 0.1f;

		CreateObject(event._type, event._handle, position, 
			randomScale, randomYOrientation);
	}

}

void GameObjectFactory::Handle(const CreateObjectOnLocationEvent & event)
{
	Vector3 position = event._position;
	position.y = TERRAIN->GetHeight(position.x, position.z);
	CreateObject(event._type, event._handle, position);
}

void GameObjectFactory::Handle(const CreateObjectFromSaveInfoEvent & event)
{
	switch (event._type)
	{
	case ARCHE_HERO  :
	{
		CreateObject(ARCHE_HERO, ResourceHandle(), event._position);
	}break;
	case ARCHE_ROCK :
	{
		video::StaticXMeshHandle handle = VIDEO->GetStaticXMesh(event._key);
		CreateObject(ARCHE_ROCK, handle, event._position);
	}break;
	case ARCHE_GRASS :
	{
		video::StaticXMeshHandle handle = VIDEO->GetStaticXMesh(event._key);
		CreateObject(ARCHE_GRASS, handle, event._position);
	}break;
	case ARCHE_TREE:
	{
		video::StaticXMeshHandle handle = VIDEO->GetStaticXMesh(event._key);
		CreateObject(ARCHE_TREE, handle, event._position);
	}break;
	case ARCHE_MUSHROOM:
	{
		video::StaticXMeshHandle handle = VIDEO->GetStaticXMesh(event._key);
		CreateObject(ARCHE_MUSHROOM, handle, event._position);
	}break;
	case ARCHE_BAT:
	{
		CreateObject(ARCHE_BAT, ResourceHandle(), event._position);
	}break;
	case ARCHE_CAT:
	{
		CreateObject(ARCHE_CAT, ResourceHandle(), event._position);
	}break;
	case ARCHE_LIZARD:
	{
		CreateObject(ARCHE_LIZARD, ResourceHandle(), event._position);
	}break;
	case ARCHE_SNAKE:
	{
		CreateObject(ARCHE_SNAKE, ResourceHandle(), event._position);
	}break;
	case ARCHE_TURTLE:
	{
		CreateObject(ARCHE_TURTLE, ResourceHandle(), event._position);
	}break;
	case ARCHE_HYDRA:
	{
		CreateObject(ARCHE_HYDRA, ResourceHandle(), event._position);
	}break;
	case ARCHE_DRAGON:
	{
		CreateObject(ARCHE_DRAGON, ResourceHandle(), event._position);
	}break;
	}
}

void GameObjectFactory::Handle(const DamageBoxEvent & event)
{
	_pCurrentScene->_gameObjects.push_back(new DamageBox());
	BaseGameObject *pBack = _pCurrentScene->_gameObjects.back();

	pBack->CreateFromWorld(_pCurrentScene->_world, event._position);

	//이벤트설정
	Entity _entity = pBack->GetEntity();
	CollisionComponent & collision = _entity.GetComponent<CollisionComponent>();
	collision._boundingBox.Init(-event._size, event._size);
	collision._accel = event._accel;
	collision._dmg = event._dmg;
	collision._velocity = event._velocity;
	collision._triggerType = event._type;
	collision._duration = event._duration;
}

void GameObjectFactory::Handle(const CreateBlood & event)
{
	Entity &entity = _pCurrentScene->_world.CreateEntity();

	TransformComponent &transform = entity.AddComponent<TransformComponent>();
	transform._position = event._pos;

	ParticleComponent &particle = entity.AddComponent<ParticleComponent>();
	particle.min = Vector3(-0.1f, -0.1f, -0.1f);
	particle.max = Vector3(0.1f, 0.1f, 0.1f);
	particle.isEmission = true;
	particle.duration = 0.5f;
	particle.delay = 0.0f;
	particle.init(ParticleComponent::PARTICLE_TYPE_BLOOD_FOG, 500, 0.025f, Vector3(1, 0, 0), event._pos);

	Entity &entity2 = _pCurrentScene->_world.CreateEntity();

	TransformComponent &transform2 = entity2.AddComponent<TransformComponent>();
	transform2._position = event._pos;


	ParticleComponent &particle2 = entity2.AddComponent<ParticleComponent>();
	particle2.min = Vector3(-0.1f, -0.1f, -0.1f);
	particle2.max = Vector3(0.1f, 0.1f, 0.1f);
	particle.isEmission = true;
	particle2.duration = 0.5f;
	particle2.delay = 0.0f;
	particle2.init(ParticleComponent::PARTICLE_TYPE_BLOOD_PARTICLE, 50, 0.005f, Vector3(1, 0, 0), event._pos);

	entity.Activate();
	entity2.Activate();
}

void GameObjectFactory::Handle(const CreateBreath & event)
{
	Entity &entity = _pCurrentScene->_world.CreateEntity();
	TransformComponent &transform = entity.AddComponent<TransformComponent>();
	transform._position = event._pos;
	ParticleComponent &particle = entity.AddComponent<ParticleComponent>();
	particle.min = Vector3(-0.1f, -0.1f, -0.1f);
	particle.max = Vector3(0.1f, 0.1f, 0.1f);
	particle.isEmission = true;
	particle.duration = 1.0f;
	particle.delay = 0.0f;
	particle.init(ParticleComponent::PARTICLE_TYPE_NORMALBREATH, 450, 0.025f, event._direction/3, event._pos);

	entity.Activate();
}

void GameObjectFactory::Handle(const CreateNFireBall & event)
{
	Entity &entity = _pCurrentScene->_world.CreateEntity();
	TransformComponent &transform = entity.AddComponent<TransformComponent>();
	transform._position = event._pos;
	ParticleComponent &particle = entity.AddComponent<ParticleComponent>();
	particle.min = Vector3(-0.1f, -0.1f, -0.1f);
	particle.max = Vector3(0.1f, 0.1f, 0.1f);
	particle.isEmission = true;
	particle.duration = 5.0f;
	particle.delay = 0.0f;
	particle.init(ParticleComponent::PARTICLE_TYPE_NORMALFIREBALL, 450, 0.025f, Vector3(0, 0, 0), event._pos);
	particle.canMove = true;
	particle.velocity = event._velocity;
	CollisionComponent &collision = entity.AddComponent<CollisionComponent>();
	collision._boundingBox.Init(Vector3(-1, -1, -1), Vector3(1, 1, 1));
	collision._dmg = event._dmg;
	collision._triggerType = CollisionComponent::TRIGGER_TYPE_ENEMY_DMGBOX;
	collision._type = CollisionComponent::COLLISION_TYPE_BOX;
	collision._isTrigger = true;
	collision._locked = false;

	entity.Activate();
}

void GameObjectFactory::Handle(const CreateNFireBall2 & event)
{
	Entity &entity = _pCurrentScene->_world.CreateEntity();
	TransformComponent &transform = entity.AddComponent<TransformComponent>();
	transform._position = event._pos;
	ParticleComponent &particle = entity.AddComponent<ParticleComponent>();
	particle.min = Vector3(-0.1f, -0.1f, -0.1f);
	particle.max = Vector3(0.1f, 0.1f, 0.1f);
	particle.isEmission = true;
	particle.duration = 5.0f;
	particle.delay = 0.0f;
	particle.init(ParticleComponent::PARTICLE_TYPE_NORMALFIREBALL2, 450, 0.025f, Vector3(0, 0, 0), event._pos);
	particle.canMove = true;
	particle.velocity = event._velocity;
	CollisionComponent &collision = entity.AddComponent<CollisionComponent>();
	collision._boundingBox.Init(Vector3(-1, -1, -1), Vector3(1, 1, 1));
	collision._dmg = event._dmg;
	collision._triggerType = CollisionComponent::TRIGGER_TYPE_ENEMY_DMGBOX;
	collision._type = CollisionComponent::COLLISION_TYPE_BOX;
	collision._isTrigger = true;
	collision._locked = false;

	entity.Activate();
}

void GameObjectFactory::Handle(const CreateCharge & event)
{
	Entity &entity = _pCurrentScene->_world.CreateEntity();
	TransformComponent &transform = entity.AddComponent<TransformComponent>();
	transform._position = event._pos;
	ParticleComponent &particle = entity.AddComponent<ParticleComponent>();
	particle.min = Vector3(-0.1f, -0.1f, -0.1f);
	particle.max = Vector3(0.1f, 0.1f, 0.1f);
	particle.isEmission = true;
	particle.duration = 3.0f;
	particle.delay = 0.0f;
	particle.init(ParticleComponent::PARTICLE_TYPE_CHARGE, 1500, 0.01f, Vector3(0, 0, 0), event._pos);

	entity.Activate();

	Entity &entity2 = _pCurrentScene->_world.CreateEntity();
	TransformComponent &transform2 = entity2.AddComponent<TransformComponent>();
	transform2._position = event._pos;
	ParticleComponent &particle2 = entity2.AddComponent<ParticleComponent>();
	particle2.min = Vector3(-0.1f, -0.1f, -0.1f);
	particle2.max = Vector3(0.1f, 0.1f, 0.1f);
	particle2.isEmission = true;
	particle2.duration = 3.0f;
	particle2.delay = 0.0f;
	particle2.init(ParticleComponent::PARTICLE_TYPE_FIREBALL, 1500, 0.01f, Vector3(0, 0, 0), event._pos);

	entity2.Activate();
}

void GameObjectFactory::Handle(const CreateDragonBreath & event)
{
	Entity &entity = _pCurrentScene->_world.CreateEntity();
	TransformComponent &transform = entity.AddComponent<TransformComponent>();
	transform._position = event._pos;
	ParticleComponent &particle = entity.AddComponent<ParticleComponent>();
	particle.min = Vector3(-0.1f, -0.1f, -0.1f);
	particle.max = Vector3(0.1f, 0.1f, 0.1f);
	particle.isEmission = true;
	particle.duration = 1.5f;
	particle.delay = 0.0f;
	particle.init(ParticleComponent::PARTICLE_TYPE_FIREBREATH, 1500, 0.0025f, event._direction, event._pos);

	entity.Activate();

	Entity &entity2 = _pCurrentScene->_world.CreateEntity();
	TransformComponent &transform2 = entity2.AddComponent<TransformComponent>();
	transform2._position = event._pos;
	ParticleComponent &particle2 = entity2.AddComponent<ParticleComponent>();
	particle2.min = Vector3(-0.1f, -0.1f, -0.1f);
	particle2.max = Vector3(0.1f, 0.1f, 0.1f);
	particle2.isEmission = true;
	particle2.duration = 1.5f;
	particle2.delay = 0.0f;
	particle2.init(ParticleComponent::PARTICLE_TYPE_SMOKE, 500, 0.025f,Vector3(0,1,0), event._pos);

	entity2.Activate();
}

const char * ArcheToString(ARCHE_TYPE type)
{
	switch (type)
	{
	case ARCHE_HERO:
	{
		return "Player";
	}
	break;
	case ARCHE_ROCK:
	{
		return "Rock";
	}
	break;
	case ARCHE_GRASS:
	{
		return "Grass";
	}
	break;
	case ARCHE_TREE:
	{
		return "Tree";
	}
	case ARCHE_TREETRUNK :
	{
		return "TreeTrunk";
	}break;
	break;
	case ARCHE_MUSHROOM:
	{
		return "MushRoom";
	}
	break;
	case ARCHE_BAT:
	{
		return "Bat";
	}
	break;
	case ARCHE_CAT:
	{
		return "Cat";
	}
	break;
	case ARCHE_LIZARD:
	{
		return "Lizard";
	}
	break;
	case ARCHE_SNAKE:
	{
		return "Snake";
	}
	break;
	case ARCHE_TURTLE:
	{
		return "Turtle";
	}
	break;
	case ARCHE_HYDRA:
	{
		return "Hydra";
	}
	break;
	}
}