#include "stdafx.h"
#include "MapToolScene.h"

bool MapToolScene::Init()
{
	bool result = true;
	_channel.Add<Editor::GetObjectFromSceneEvent, MapToolScene>(*this);

	DataPackage dataPackage;
	uint32 fileSize{};
	dataPackage.OpenFile("../resources/Test.ed", &fileSize);
	int32 numEntityToCreate;
	dataPackage.ReadAs<int32>(&numEntityToCreate);

	EntitySaveInfo entitySaveInfo;
	ZeroMemory(&entitySaveInfo, sizeof(EntitySaveInfo));
	for (int32 i = 0; i < numEntityToCreate; ++i)
	{
		dataPackage.ReadAs<EntitySaveInfo>(&entitySaveInfo);

	}

	GAMEOBJECTFACTORY->SetCurrentScene(this);

	video::StaticXMesh::_sEffectHandle = VIDEO->GetEffect("StaticMesh.fx");
	video::SkinnedXMesh::_sStaticEffectHandle = VIDEO->GetEffect("StaticMesh.fx");
	video::SkinnedXMesh::_sSkinnedEffectHandle = VIDEO->GetEffect("SkinnedMesh.fx");

	InitAnimations();

	//�ͷ��� �ε�
	Terrain::TerrainConfig config;
	config._xChunkCount = 2;
	config._zChunkCount = 2;
	strncpy(config._tile0FileName, "../resources/Terrain/TerrainTexture01.jpg", MAX_FILE_NAME);
	strncpy(config._tile1FileName, "../resources/Terrain/TerrainTexture02.jpg", MAX_FILE_NAME);
	strncpy(config._tile2FileName, "../resources/Terrain/TerrainTexture03.png", MAX_FILE_NAME);
	strncpy(config._tile3FileName, "../resources/Terrain/TerrainTexture04.png", MAX_FILE_NAME);

	config._textureMult = 200;

	TERRAIN->SetScene(this);
	TERRAIN->Create(config, true);

	//�޽� �ҷ�����..
	Matrix correctionMat;
	MatrixScaling(&correctionMat, 0.01f, 0.01f, 0.01f);
	video::SkinnedXMeshHandle knight  = VIDEO->CreateSkinnedXMesh(
		"../resources/Models/Knight/Knight.X", &correctionMat, "Knight");

	MatrixScaling(&correctionMat, 1.0f, 1.0f, 1.0f);
	video::SkinnedXMeshHandle snakeMesh = VIDEO->CreateSkinnedXMesh(
		"../resources/Models/Snake/Snake_Red.X", &correctionMat, "Snake");

	MatrixScaling(&correctionMat, 1.0f, 1.0f, 1.0f);
	video::SkinnedXMeshHandle turtleMesh = VIDEO->CreateSkinnedXMesh(
		"../resources/Models/DragonTurtle/DragonTurtle_Black.X", &correctionMat, "Turtle");

	MatrixScaling(&correctionMat, 1.0f, 1.0f, 1.0f);
	video::SkinnedXMeshHandle batMesh = VIDEO->CreateSkinnedXMesh(
		"../resources/Models/DragonBat/DragonBat_Black.X", &correctionMat, "Bat");

	MatrixScaling(&correctionMat, 1.0f, 1.0f, 1.0f);
	video::SkinnedXMeshHandle catMesh = VIDEO->CreateSkinnedXMesh(
		"../resources/Models/DevilCat/DevilCat.X", &correctionMat, "Cat");

	MatrixScaling(&correctionMat, 1.0f, 1.0f, 1.0f);
	video::SkinnedXMeshHandle HydraMesh = VIDEO->CreateSkinnedXMesh(
		"../resources/Models/Hydra/Hydra_Red.X", &correctionMat, "Hydra");

	MatrixScaling(&correctionMat, 1.0f, 1.0f, 1.0f);
	video::SkinnedXMeshHandle LizardMesh = VIDEO->CreateSkinnedXMesh(
		"../resources/Models/Lizard/Lizard.X", &correctionMat, "Lizard");

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

	//��ƼƼ ����
	_world.AddSystem<RenderSystem>(_renderSystem);
	_world.AddSystem<TransformSystem>(_transformSystem);
	_world.AddSystem<ActionSystem>(_actionSystem);
	_world.AddSystem<ScriptSystem>(_scriptSystem);
	_world.AddSystem<CollisionSystem>(_collisionSystem);

	//ī�޶� ����
	_camera.CreateFromWorld(_world);
	_camera.SetRotationSpeed(10.0f);
	_camera.SetMoveSpeed(20.0f);

	//����Ʈ ����
	_pMainLight = new DirectionalLight();
	_pMainLight->CreateFromWorld(_world);
	_pMainLight->SetWorldPosition(Vector3(0.0f, 5.0f, 5.0f));
	_pMainLight->SetTarget(Vector3(0.0f, 0.0f, 0.0f));

	_pEnvironmentSphere = new EnvironmentSphere;
	_pEnvironmentSphere->Create("../resources/Textures/grassenvmap1024.dds");

	_channel.Broadcast<GameObjectFactory::CreateObjectOnLocationEvent>(
		GameObjectFactory::CreateObjectOnLocationEvent(ARCHE_SNAKE, ResourceHandle(), Vector3(0.0f, 5.0f, 0.0f)));

	_channel.Broadcast<GameObjectFactory::CreateObjectOnLocationEvent>(
		GameObjectFactory::CreateObjectOnLocationEvent(ARCHE_CAT, ResourceHandle(), Vector3(0.0f, 7.0f, 0.0f)));

	_channel.Broadcast<GameObjectFactory::CreateObjectOnLocationEvent>(
		GameObjectFactory::CreateObjectOnLocationEvent(ARCHE_HYDRA, ResourceHandle(), Vector3(0.0f, 9.0f, 0.0f)));

	_channel.Broadcast<GameObjectFactory::CreateObjectOnLocationEvent>(
		GameObjectFactory::CreateObjectOnLocationEvent(ARCHE_HERO, ResourceHandle(), Vector3(0.0f, 0.0f, 0.0f)));

	//������ ����
	imguiRenderInit();
	_editor = new Editor;
	_editor->Init();

	return result;
}

bool MapToolScene::Update(float deltaTime, const InputManager & input)
{
	bool result = true;

	_editor->Edit(RefVariant(), input);

	_world.Refresh();

	_scriptSystem.Update(deltaTime);

	_camera.PreUpdateMatrix();
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

	return result;
}

bool MapToolScene::Render()
{
	video::StaticXMesh::SetCamera(_camera);
	video::StaticXMesh::SetBaseLight(_pMainLight);

	video::SkinnedXMesh::SetCamera(_camera);
	video::SkinnedXMesh::SetBaseLight(_pMainLight);

	gpDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, 0xff303040, 1.0f, 0);
	gpDevice->BeginScene();

	_pEnvironmentSphere->Render(_camera);
	
	GIZMOMANAGER->WorldGrid(1.0f, 20);

	TERRAIN->Render(_camera);
	_renderSystem.Render(_camera);
	_editor->Render();

	imguiRenderDraw();

	gpDevice->EndScene();
	gpDevice->Present(nullptr, nullptr, NULL, nullptr);

	return true;
}

void MapToolScene::Release()
{
	for (auto object : _gameObjects)
	{
		SAFE_DELETE(object);
	}
	_gameObjects.clear();

	_world.Clear();
}

void MapToolScene::Handle(const Editor::GetObjectFromSceneEvent & event)
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
