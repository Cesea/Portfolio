#include "stdafx.h"
#include "TestScene.h"

bool TestScene::SceneInit()
{
	bool result = true;
	_channel.Add<Editor::GetObjectFromSceneEvent, TestScene>(*this);

	GAMEOBJECTFACTORY->SetCurrentScene(this);

	video::StaticXMesh::_sEffectHandle = VIDEO->GetEffect("StaticMesh.fx");
	video::SkinnedXMesh::_sEffectHandle = VIDEO->GetEffect("SkinnedMesh.fx");

	InitAnimations();

	//터레인 로드
	Terrain::TerrainConfig config;

	//DataPackage terrainData;
	//uint32 terrainFileSize = 0;
	//terrainData.OpenFile("../resources/TestScene/Terrain01.tr", &terrainFileSize);
	//Assert(terrainFileSize > 0);
	//terrainData.ReadAs<Terrain::TerrainConfig>(&config);

	//config._xChunkCount = 2;
	//config._zChunkCount = 2;
	//strncpy(config._tile0FileName, "../resources/Terrain/TerrainTexture01.jpg", MAX_FILE_NAME);
	//strncpy(config._tile1FileName, "../resources/Terrain/TerrainTexture02.jpg", MAX_FILE_NAME);
	//strncpy(config._tile2FileName, "../resources/Terrain/TerrainTexture03.png", MAX_FILE_NAME);
	//strncpy(config._tile3FileName, "../resources/Terrain/TerrainTexture04.png", MAX_FILE_NAME);
	//config._textureMult = 200;

	TERRAIN->SetScene(this);
	TERRAIN->Create(config, true);
	TERRAIN->LoadTerrain("../resources/TestScene/Terrain01.tr");

	//메쉬 불러오기..
	Matrix correctionMat;
	MatrixScaling(&correctionMat, 0.01f, 0.01f, 0.01f);
	video::SkinnedXMeshHandle knight  = VIDEO->CreateSkinnedXMesh(
		"../resources/Models/Knight/Knight.X", &correctionMat, "Knight");

	MatrixScaling(&correctionMat, 1.0f, 1.0f, 1.0f);
	video::SkinnedXMeshHandle snakeMesh = VIDEO->CreateSkinnedXMesh(
		"../resources/Models/Snake/Snake_Red.X", &correctionMat, "Snake1");

	MatrixScaling(&correctionMat, 1.0f, 1.0f, 1.0f);
	video::SkinnedXMeshHandle snakeMesh2 = VIDEO->CreateSkinnedXMesh(
		"../resources/Models/Snake/Snake_Black.X", &correctionMat, "Snake2");

	MatrixScaling(&correctionMat, 1.0f, 1.0f, 1.0f);
	video::SkinnedXMeshHandle snakeMesh3 = VIDEO->CreateSkinnedXMesh(
		"../resources/Models/Snake/Snake_Cyan.X", &correctionMat, "Snake3");

	MatrixScaling(&correctionMat, 1.0f, 1.0f, 1.0f);
	video::SkinnedXMeshHandle turtleMesh = VIDEO->CreateSkinnedXMesh(
		"../resources/Models/DragonTurtle/DragonTurtle_Black.X", &correctionMat, "Turtle");

	MatrixScaling(&correctionMat, 1.0f, 1.0f, 1.0f);
	video::SkinnedXMeshHandle batMesh1 = VIDEO->CreateSkinnedXMesh(
		"../resources/Models/DragonBat/DragonBat_Red.X", &correctionMat, "Bat1");


	MatrixScaling(&correctionMat, 1.0f, 1.0f, 1.0f);
	video::SkinnedXMeshHandle batMesh2 = VIDEO->CreateSkinnedXMesh(
		"../resources/Models/DragonBat/DragonBat_Black.X", &correctionMat, "Bat2");


	MatrixScaling(&correctionMat, 1.0f, 1.0f, 1.0f);
	video::SkinnedXMeshHandle batMesh3 = VIDEO->CreateSkinnedXMesh(
		"../resources/Models/DragonBat/DragonBat_Gold.X", &correctionMat, "Bat3");


	MatrixScaling(&correctionMat, 1.0f, 1.0f, 1.0f);
	video::SkinnedXMeshHandle catMesh = VIDEO->CreateSkinnedXMesh(
		"../resources/Models/DevilCat/DevilCat.X", &correctionMat, "Cat");

	MatrixScaling(&correctionMat, 2.0f, 2.0f, 2.0f);
	video::SkinnedXMeshHandle HydraMesh1 = VIDEO->CreateSkinnedXMesh(
		"../resources/Models/Hydra/Hydra_Green.X", &correctionMat, "Hydra1");

	MatrixScaling(&correctionMat, 2.0f, 2.0f, 2.0f);
	video::SkinnedXMeshHandle HydraMesh2 = VIDEO->CreateSkinnedXMesh(
		"../resources/Models/Hydra/Hydra_Red.X", &correctionMat, "Hydra2");

	MatrixScaling(&correctionMat, 2.0f, 2.0f, 2.0f);
	video::SkinnedXMeshHandle HydraMesh3 = VIDEO->CreateSkinnedXMesh(
		"../resources/Models/Hydra/Hydra_Black.X", &correctionMat, "Hydra3");

	MatrixScaling(&correctionMat, 2.0f, 2.0f, 2.0f);
	video::SkinnedXMeshHandle HydraMesh4 = VIDEO->CreateSkinnedXMesh(
		"../resources/Models/Hydra/Hydra_Gold.X", &correctionMat, "Hydra4");

	MatrixScaling(&correctionMat, 1.0f, 1.0f, 1.0f);
	video::SkinnedXMeshHandle LizardMesh1 = VIDEO->CreateSkinnedXMesh(
		"../resources/Models/Lizard/Lizard.X", &correctionMat, "Lizard1");

	MatrixScaling(&correctionMat, 1.0f, 1.0f, 1.0f);
	video::SkinnedXMeshHandle LizardMesh2 = VIDEO->CreateSkinnedXMesh(
		"../resources/Models/Lizard/Lizard_Black.X", &correctionMat, "Lizard2");

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

	//시스템 생성
	_world.AddSystem<RenderSystem>(_renderSystem);
	_world.AddSystem<TransformSystem>(_transformSystem);
	_world.AddSystem<ActionSystem>(_actionSystem);
	_world.AddSystem<ScriptSystem>(_scriptSystem);
	_world.AddSystem<CollisionSystem>(_collisionSystem);

	//라이트 생성
	_pMainLight = new DirectionalLight();
	_pMainLight->CreateFromWorld(_world);
	_pMainLight->SetWorldPosition(Vector3(0.0f, 5.0f, 5.0f));
	_pMainLight->SetTarget(Vector3(0.0f, 0.0f, 0.0f));

	_pEnvironmentSphere = new EnvironmentSphere;
	_pEnvironmentSphere->Create("../resources/Textures/grassenvmap1024.dds");

	DataPackage dataPackage;
	uint32 fileSize{};
	dataPackage.OpenFile("../resources/TestScene/Test.ed", &fileSize);
	int32 numEntityToCreate;
	dataPackage.ReadAs<int32>(&numEntityToCreate);

	EntitySaveInfo entitySaveInfo;
	ZeroMemory(&entitySaveInfo, sizeof(EntitySaveInfo));
	for (int32 i = 0; i < numEntityToCreate; ++i)
	{
		dataPackage.ReadAs<EntitySaveInfo>(&entitySaveInfo);

		_channel.Broadcast<GameObjectFactory::CreateObjectFromSaveInfoEvent>(
			GameObjectFactory::CreateObjectFromSaveInfoEvent(entitySaveInfo._archeType, 
				entitySaveInfo._resourceName, entitySaveInfo._position));
	}


	//카메라 생성
	_camera.CreateFromWorld(_world);
	_camera.SetRotationSpeed(10.0f);
	_camera.SetMoveSpeed(20.0f);
	_camera.GetEntity().GetComponent<TransformComponent>().MovePositionWorld(Vector3(0.0f, 4.0f, -6.0f));
	_camera.SetTargetObject(GAMEOBJECTFACTORY->GetPlayerObject());

	//_channel.Broadcast<GameObjectFactory::CreateObjectOnLocationEvent>(
	//	GameObjectFactory::CreateObjectOnLocationEvent(ARCHE_SNAKE, ResourceHandle(), Vector3(0.0f, 5.0f, 0.0f)));
	//_channel.Broadcast<GameObjectFactory::CreateObjectOnLocationEvent>(
	//	GameObjectFactory::CreateObjectOnLocationEvent(ARCHE_CAT, ResourceHandle(), Vector3(0.0f, 7.0f, 0.0f)));
	//_channel.Broadcast<GameObjectFactory::CreateObjectOnLocationEvent>(
	//	GameObjectFactory::CreateObjectOnLocationEvent(ARCHE_HYDRA, ResourceHandle(), Vector3(0.0f, 9.0f, 0.0f)));
	//_channel.Broadcast<GameObjectFactory::CreateObjectOnLocationEvent>(
	//	GameObjectFactory::CreateObjectOnLocationEvent(ARCHE_HERO, ResourceHandle(), Vector3(0.0f, 0.0f, 0.0f)));

	//에디터 생성
	imguiRenderInit();
	_editor = new Editor;
	_editor->Init(this);

	return result; 
}

bool TestScene::SceneUpdate(float deltaTime, const InputManager & input)
{
	bool result = true;

	_editor->Edit(RefVariant(), input);

	_world.Refresh();

	_scriptSystem.Update(deltaTime);

	_camera.MoveAndRotate(deltaTime, input);
	_transformSystem.PreUpdate(deltaTime);

	//Collision Check
	_collisionSystem.Update(deltaTime, 4.0f);
	//_transformSystem.PostUpdate(deltaTime);
	_actionSystem.Update(deltaTime);

	//Update Camera
	{
		_camera.UpdateMatrix();
		_camera.UpdateCamToDevice();
		_camera.UpdateFrustum();
	}
	//_channel.Update<BaseScene::SpawnEvent>(deltaTime);
	return result;}

bool TestScene::SceneRelease()
{
	for (auto object : _gameObjects)
	{
		SAFE_DELETE(object);
	}
	_gameObjects.clear();

	_world.Clear();

	return true;
}

bool TestScene::SceneRender0()
{
	video::StaticXMesh::SetCamera(_camera);
	video::StaticXMesh::SetBaseLight(_pMainLight);

	video::SkinnedXMesh::SetCamera(_camera);
	video::SkinnedXMesh::SetBaseLight(_pMainLight);

	GIZMOMANAGER->WorldGrid(1.0f, 20);

	TERRAIN->Render(_camera, *_pMainLight, _camera);
	_renderSystem.Render(_camera);
	_editor->Render();


	return true;
}

const char * TestScene::GetSceneName()
{
	return "TestScene";
}

void TestScene::Handle(const Editor::GetObjectFromSceneEvent & event)
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
