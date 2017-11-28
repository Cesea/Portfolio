#include "stdafx.h"
#include "BaseScene.h"

using namespace video;


bool BaseScene::Init()
{
	bool result = true;
	_channel.Add<Editor::GetObjectFromSceneEvent, BaseScene>(*this);
	//RegisterEvents();

	GAMEOBJECTFACTORY->SetCurrentScene(this);

	video::StaticXMesh::_sEffectHandle = VIDEO->GetEffect("StaticMesh.fx");
	video::SkinnedXMesh::_sStaticEffectHandle = VIDEO->GetEffect("StaticMesh.fx");
	video::SkinnedXMesh::_sSkinnedEffectHandle = VIDEO->GetEffect("SkinnedMesh.fx");

	InitPlayerAnimation();
	InitSnakeAnimation();
	InitTurtleAnimation();
	InitBatAnimation();
	InitCatAnimation();
	InitHydraAnimation();
	//터레인 로드
	Terrain::TerrainConfig config;
	config._xChunkCount = 4;
	config._zChunkCount = 4;
	config._tile0FileName = "../resources/Textures/TerrainTexture01.jpg";
	config._tile1FileName = "../resources/Textures/TerrainTexture02.jpg";
	config._tile2FileName = "../resources/Textures/TerrainTexture03.png";
	config._tile3FileName = "../resources/Textures/TerrainTexture04.png";
	config._splatFileName = "../resources/Textures/Splat.png";

	//config._cellScale = 1.0f;
	//config._heightScale = 20.0f;
	config._textureMult = 300;
	//config._lodRatio = 0.1f;
	//config._sectionResolution = TERRAIN_CHUNK_DIM;

	//_pTerrain = new Terrain();
	TERRAIN->SetScene(this);
	TERRAIN->Create(config, true);

	//메쉬 불러오기..
	Matrix correctionMat;
	MatrixScaling(&correctionMat, 0.01f, 0.01f, 0.01f);
	video::SkinnedXMeshHandle knight  = VIDEO->CreateSkinnedXMesh(
		"../resources/Models/Knight/Knight.X", &correctionMat, "Knight");
	video::AnimationInstanceHandle ainmHandle = VIDEO->CreateAnimationInstance(knight, "Knight0");

	MatrixScaling(&correctionMat, 1.15f, 1.15f, 1.15f);
	video::SkinnedXMeshHandle snakeMesh = VIDEO->CreateSkinnedXMesh(
		"../resources/Models/Snake/Snake_Red.X", &correctionMat, "Snake");

	MatrixScaling(&correctionMat, 1.15f, 1.15f, 1.15f);
	video::SkinnedXMeshHandle turtleMesh = VIDEO->CreateSkinnedXMesh(
		"../resources/Models/DragonTurtle/DragonTurtle_Black.X", &correctionMat, "Turtle");

	MatrixScaling(&correctionMat, 1.0f, 1.0f, 1.0f);
	video::SkinnedXMeshHandle batMesh = VIDEO->CreateSkinnedXMesh(
		"../resources/Models/DragonBat/DragonBat_Black.X", &correctionMat, "Bat");


	MatrixScaling(&correctionMat, 1.15f, 1.15f, 1.15f);
	video::SkinnedXMeshHandle catMesh = VIDEO->CreateSkinnedXMesh(
		"../resources/Models/DevilCat/DevilCat.X", &correctionMat, "Cat");

	MatrixScaling(&correctionMat, 1.15f, 1.15f, 1.15f);
	video::SkinnedXMeshHandle HydraMesh = VIDEO->CreateSkinnedXMesh(
		"../resources/Models/Hydra/Hydra_Gold.X", &correctionMat, "Hydra");

	MatrixScaling(&correctionMat, 1.0f, 1.0f, 1.0f);
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Rock/Rock1_A.X", &correctionMat, "Rock01");
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Rock/Rock2_A.X", &correctionMat, "Rock02");
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Rock/Rock3_A.X", &correctionMat, "Rock03");
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Rock/Rock4_A.X", &correctionMat, "Rock04");
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Rock/Rock5_A.X", &correctionMat, "Rock05");

	MatrixScaling(&correctionMat, 0.01f, 0.01f, 0.01f);
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Grass/Grass1.X", &correctionMat, "Grass01");
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Grass/Grass2.X", &correctionMat, "Grass02");
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Grass/Grass3.X", &correctionMat, "Grass03");
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Grass/Grass4.X", &correctionMat, "Grass04");
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Grass/Grass5.X", &correctionMat, "Grass05");

	MatrixScaling(&correctionMat, 1.0f, 1.0f, 1.0f);
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Tree/Tree1.X", &correctionMat, "Tree01");
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Tree/Tree2.X", &correctionMat, "Tree02");
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Tree/Tree3.X", &correctionMat, "Tree03");
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Tree/Tree4.X", &correctionMat, "Tree04");
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Tree/Tree5.X", &correctionMat, "Tree05");

	//엔티티 생성
	_world.AddSystem<RenderSystem>(_renderSystem);
	_world.AddSystem<TransformSystem>(_transformSystem);
	_world.AddSystem<ActionSystem>(_actionSystem);
	_world.AddSystem<ScriptSystem>(_scriptSystem);
	_world.AddSystem<CollisionSystem>(_collisionSystem);

	//카메라 생성
	_camera.CreateFromWorld(_world);
	_camera.SetRotationSpeed(10.0f);
	_camera.SetMoveSpeed(20.0f);

	//라이트 생성
	_pMainLight = new DirectionalLight();
	_pMainLight->CreateFromWorld(_world);
	_pMainLight->SetWorldPosition(Vector3(0.0f, 5.0f, 5.0f));
	_pMainLight->SetTarget(Vector3(0.0f, 0.0f, 0.0f));

	//_channel.Broadcast<GameObjectFactory::CreateObjectOnLocationEvent>(
	//	GameObjectFactory::CreateObjectOnLocationEvent(ARCHE_HERO, ResourceHandle(), Vector3(0.0f, 0.0f, 0.0f)));

	_channel.Broadcast<GameObjectFactory::CreateObjectOnLocationEvent>(
		GameObjectFactory::CreateObjectOnLocationEvent(ARCHE_SNAKE, ResourceHandle(), Vector3(0.0f, 5.0f, 0.0f)));

	_channel.Broadcast<GameObjectFactory::CreateObjectOnLocationEvent>(
		GameObjectFactory::CreateObjectOnLocationEvent(ARCHE_HERO, ResourceHandle(), Vector3(0.0f, 0.0f, 0.0f)));

	//_player.CreateFromWorld(_world);
	//_snake.CreateFromWorld(_world);
	_turtle.CreateFromWorld(_world);
	//_bat.CreateFromWorld(_world);
	_cat.CreateFromWorld(_world);
	//_hydra.CreateFromWorld(_world);

	//에디터 생성
	imguiRenderInit();
	_editor = new Editor;
	_editor->Init();

	return result;
}

bool BaseScene::Update(float deltaTime, const InputManager &input)
{
	bool result = true;

	_editor->Edit(RefVariant(), input);

	_world.Refresh();

	_scriptSystem.Update(deltaTime);

	_camera.PreUpdateMatrix();
	_transformSystem.PreUpdate(deltaTime);

	//Collision Check
	//_transformSystem.PostUpdate(deltaTime);
	_actionSystem.Update(deltaTime);

	//Update Camera
	{
		_camera.UpdateMatrix();
		_camera.UpdateCamToDevice();
		_camera.UpdateFrustum();
	}

	//_channel.Update<BaseScene::SpawnEvent>(deltaTime);

	return result;
}

bool BaseScene::Render()
{
	video::StaticXMesh::SetCamera(_camera);
	video::StaticXMesh::SetBaseLight(_pMainLight);

	video::SkinnedXMesh::SetCamera(_camera);
	video::SkinnedXMesh::SetBaseLight(_pMainLight);

	gpDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, 0xff303040, 1.0f, 0);
	gpDevice->BeginScene();
	
	GIZMOMANAGER->WorldGrid(1.0f, 20);

	TERRAIN->Render(_camera);
	_renderSystem.Render(_camera);

	imguiRenderDraw();


	gpDevice->EndScene();
	gpDevice->Present(nullptr, nullptr, NULL, nullptr);

	return true;
}

void BaseScene::Release()
{
}

void BaseScene::Handle(const Editor::GetObjectFromSceneEvent & event)
{
	Vector3 position;
	Vector3 terrainHitPos;
	Ray ray;
	_camera.ComputeRay(event._cursorPos, &ray);
	std::vector<Entity> collidingEntity{};
	std::vector<float> collidingDistance;

	_collisionSystem.QueryRayEntityHit(ray, &collidingEntity, &collidingDistance);
	if (collidingEntity.size() > 0)
	{
		float minDistance = 9999.0f;
		int32 minIndex = 0;
		for (int32 i = 0; i < collidingDistance.size(); ++i)
		{
			if (collidingDistance[i] < minDistance)
			{
				minDistance = collidingDistance[i];
				minIndex = i;
			}
		}
		_editor->SetEdittingEntity(collidingEntity[minIndex]);
	}
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