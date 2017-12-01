#include "stdafx.h"
#include "MapToolScene.h"

void LoadEveryStaticResources()
{
	Matrix correctionMat;
	Matrix rotationCorrection;
//Load Rocks
	MatrixScaling(&correctionMat, 1.0f, 1.0f, 1.0f);
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Rock/Rock1_A.X", &correctionMat, "Rock01");
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Rock/Rock2_A.X", &correctionMat, "Rock02");
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Rock/Rock3_A.X", &correctionMat, "Rock03");
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Rock/Rock4_A.X", &correctionMat, "Rock04");
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Rock/Rock5_A.X", &correctionMat, "Rock05");
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Rock/Rock6_A.X", &correctionMat, "Rock06");
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Rock/Rock7_A.X", &correctionMat, "Rock07");
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Rock/Rock8_A.X", &correctionMat, "Rock08");
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Rock/Rock9_A.X", &correctionMat, "Rock09");
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Rock/Rock10_A.X", &correctionMat, "Rock10");

	//Load Grass
	MatrixScaling(&correctionMat, 0.01f, 0.01f, 0.01f);
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Grass/Grass1.X", &correctionMat, "Grass01");
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Grass/Grass2.X", &correctionMat, "Grass02");
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Grass/Grass3.X", &correctionMat, "Grass03");
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Grass/Grass4.X", &correctionMat, "Grass04");
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Grass/Grass5.X", &correctionMat, "Grass05");
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Grass/Grass6.X", &correctionMat, "Grass06");
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Grass/Grass7.X", &correctionMat, "Grass07");
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Grass/Grass8.X", &correctionMat, "Grass08");
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Grass/Grass9.X", &correctionMat, "Grass09");
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Grass/Grass10.X", &correctionMat, "Grass10");

	//Load Tree
	MatrixScaling(&correctionMat, 0.01f, 0.01f, 0.01f);
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Tree/Tree1.X", &correctionMat, "Tree01");
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Tree/Tree2.X", &correctionMat, "Tree02");
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Tree/Tree3.X", &correctionMat, "Tree03");
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Tree/Tree4.X", &correctionMat, "Tree04");
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Tree/Tree5.X", &correctionMat, "Tree05");
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Tree/Tree6.X", &correctionMat, "Tree06");
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Tree/Tree7.X", &correctionMat, "Tree07");
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Tree/Tree8.X", &correctionMat, "Tree08");
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Tree/Tree9.X", &correctionMat, "Tree09");
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Tree/Tree10.X", &correctionMat, "Tree10");

	//Load TreeTrunk
	MatrixScaling(&correctionMat, 0.01f, 0.01f, 0.01f);
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/TreeTrunk/TreeTrunk1.X", &correctionMat, "TreeTrunk01");
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/TreeTrunk/TreeTrunk2.X", &correctionMat, "TreeTrunk02");
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/TreeTrunk/TreeTrunk3.X", &correctionMat, "TreeTrunk03");

	//Load Mushroom
	MatrixScaling(&correctionMat, 0.01f, 0.01f, 0.01f);
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Mushroom/Mushroom1.X", &correctionMat, "Mushroom01");
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Mushroom/Mushroom2.X", &correctionMat, "Mushroom02");
	VIDEO->CreateStaticXMesh("../resources/Models/Environment/Mushroom/Mushroom3.X", &correctionMat, "Mushroom03");
}

void LoadEverySkinnedResources()
{
//메쉬 불러오기..
	Matrix correctionMat;
	Matrix rotationCorrection;
	MatrixRotationY(&rotationCorrection, D3DX_PI);
	MatrixScaling(&correctionMat, 0.01f, 0.01f, 0.01f);
	MatrixMultiply(&correctionMat, &correctionMat, &rotationCorrection);
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

}

bool MapToolScene::SceneInit()
{
	bool result = true;
	_channel.Add<Editor::GetObjectFromSceneEvent, MapToolScene>(*this);

	GAMEOBJECTFACTORY->SetCurrentScene(this);

	video::StaticXMesh::_sEffectHandle = VIDEO->GetEffect("StaticMesh.fx");
	video::SkinnedXMesh::_sStaticEffectHandle = VIDEO->GetEffect("StaticMesh.fx");
	video::SkinnedXMesh::_sSkinnedEffectHandle = VIDEO->GetEffect("SkinnedMesh.fx");

	InitAnimations();

	//터레인 로드
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
	
	LoadEverySkinnedResources();
	LoadEveryStaticResources();

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


	_channel.Broadcast<GameObjectFactory::CreateObjectOnLocationEvent>(
		GameObjectFactory::CreateObjectOnLocationEvent(ARCHE_HERO, ResourceHandle(), Vector3(0.0f, 2.0f, 0.0f)));

	//_channel.Broadcast<GameObjectFactory::CreateObjectOnLocationEvent>(
	//	GameObjectFactory::CreateObjectOnLocationEvent(ARCHE_SNAKE, ResourceHandle(), Vector3(3.0f, 2.0f, 0.0f)));
	//_channel.Broadcast<GameObjectFactory::CreateObjectOnLocationEvent>(
	//	GameObjectFactory::CreateObjectOnLocationEvent(ARCHE_CAT, ResourceHandle(), Vector3(6.0f, 2.0f, 0.0f)));
	//_channel.Broadcast<GameObjectFactory::CreateObjectOnLocationEvent>(
	//	GameObjectFactory::CreateObjectOnLocationEvent(ARCHE_HYDRA, ResourceHandle(), Vector3(9.0f, 2.0f, 0.0f)));
	//_channel.Broadcast<GameObjectFactory::CreateObjectOnLocationEvent>(
	//	GameObjectFactory::CreateObjectOnLocationEvent(ARCHE_TURTLE, ResourceHandle(), Vector3(12.0f, 2.0f, 0.0f)));
	//_channel.Broadcast<GameObjectFactory::CreateObjectOnLocationEvent>(
	//	GameObjectFactory::CreateObjectOnLocationEvent(ARCHE_LIZARD, ResourceHandle(), Vector3(15.0f, 2.0f, 0.0f)));
	//_channel.Broadcast<GameObjectFactory::CreateObjectOnLocationEvent>(
	//	GameObjectFactory::CreateObjectOnLocationEvent(ARCHE_BAT, ResourceHandle(), Vector3(18.0f, 2.0f, 0.0f)));

	//카메라 생성
	_camera.CreateFromWorld(_world);
	_camera.SetRotationSpeed(2.0f);
	_camera.SetMoveSpeed(3.0f);
	_camera.GetEntity().GetComponent<TransformComponent>().MovePositionWorld(Vector3(0.0f, 4.0f, -6.0f));
	//NOTE : GameObjectFactory의 GetPlayerObject는 생성에 의존성을 가진다
	_camera.SetTargetObject(GAMEOBJECTFACTORY->GetPlayerObject());

	//에디터 생성
	imguiRenderInit();
	_editor = new Editor;
	_editor->Init(this);

	return result;
}

bool MapToolScene::SceneUpdate(float deltaTime, const InputManager & input)
{
	bool result = true;


	_editor->Edit(RefVariant(), input);

	_world.Refresh();

	_scriptSystem.Update(deltaTime);

	_camera.MoveAndRotate(deltaTime,input);
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

	//if (input.keyboard.IsPressed(VK_SPACE))
	//{
	//	gEngine->GetScene()->ChangeSceneWithLoading("BaseScene", "LoadingScene00", 1, 1);
	//}
	return result;
}

bool MapToolScene::SceneRelease()
{
	for (auto object : _gameObjects)
	{
		SAFE_DELETE(object);
	}
	_gameObjects.clear();

	_world.Clear();

	//VIDEO->DestroyEveryVertexBuffers();
	//VIDEO->DestroyEveryndexBuffers();

	//VIDEO->DestroyEveryAnimationInstances();
	//VIDEO->DestroyEverySkinnedMesh();

	//VIDEO->DestroyEveryStaticMesh();

	//VIDEO->DestroyEveryTextures();

	return true;
}

bool MapToolScene::SceneRender0()
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

const char * MapToolScene::GetSceneName()
{
	return "MapToolScene";
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
