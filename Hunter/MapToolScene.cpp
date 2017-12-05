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
}

bool MapToolScene::SceneInit()
{
	bool result = true;
	_channel.Add<Editor::GetObjectFromSceneEvent, MapToolScene>(*this);

	GAMEOBJECTFACTORY->SetCurrentScene(this);

	video::StaticXMesh::_sEffectHandle = VIDEO->GetEffect("StaticMesh.fx");
	video::SkinnedXMesh::_sEffectHandle  = VIDEO->GetEffect("SkinnedMesh.fx");

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

	_pMainLight->SetWorldPosition(Vector3(4.0f, 10.0f, 3.0f));
	_pMainLight->SetTarget(Vector3(0.0f, 0.0f, 0.0f));

	_pEnvironmentSphere->Create("../resources/Textures/grassenvmap1024.dds");

	_channel.Broadcast<GameObjectFactory::CreateObjectOnLocationEvent>(
		GameObjectFactory::CreateObjectOnLocationEvent(ARCHE_HERO, ResourceHandle(), Vector3(0.0f, 2.0f, 0.0f)));

	//NOTE : GameObjectFactory의 GetPlayerObject는 생성에 의존성을 가진다
	_camera.SetTargetObject(GAMEOBJECTFACTORY->GetPlayerObject());

	//에디터 생성
	imguiRenderInit();
	_editor = new Editor;
	_editor->Init(this);


	//실험
	//trash = _world.CreateEntity();
	//TransformComponent & trans = trash.AddComponent<TransformComponent>();
	//trans.SetWorldPosition(Vector3(0, 5.0f, 0));
	//ParticleComponent & par = trash.AddComponent<ParticleComponent>();
	//par.EmissionType = ParticleComponent::PARTICLE_EMISSION_TYPE::BOX;
	//VEC_COLOR colors;
	//VEC_SCALE scales;

	//colors.push_back(D3DXCOLOR(0.1f, 1.0f, 1.0f, 1.0f));
	//colors.push_back(D3DXCOLOR(0.1f, 1.0f, 1.0f, 1.0f));
	//colors.push_back(D3DXCOLOR(0.1f, 1.0f, 1.0f, 1.0f));

	//scales.push_back(1.0f);
	//scales.push_back(1.0f);
	//scales.push_back(1.0f);

	//LPDIRECT3DTEXTURE9 _tex;
	//D3DXCreateTextureFromFile(gpDevice, "../fireball.JPG", &_tex);
	//par.Init(1000, 0.025f, 2.0f, 3.0f, Vector3(0, 0, 0), Vector3(1.0f, 1.0f, 1.0f), Vector3(0, 0, 0), Vector3(1.0f, 1.0f, 1.0f), colors, scales, 2.0f, 3.0f, _tex, false);


	//trash.Activate();

	_particle = new fireRing("../fireRing.fx", "FireRingTech", "../fire.png",
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), 6000, 0.0025f, D3DXVECTOR3(0, 0, 0));

	return result;
}

bool MapToolScene::SceneUpdate(float deltaTime, const InputManager & input)
{
	bool result = true;
	{
		static float x = 0;
		static float y = 0;

		if (input.keyboard.IsDown(VK_DOWN))
		{
			x -= 0.02f;
		}
		if (input.keyboard.IsDown(VK_UP))
		{
			x += 0.02f;
		}
		if (input.keyboard.IsDown(VK_LEFT))
		{
			y -= 0.02f;
		}
		if (input.keyboard.IsDown(VK_RIGHT))
		{
			y += 0.02f;
		}
	}

	_editor->Edit(RefVariant(), input);
	_world.Refresh();
	_scriptSystem.Update(deltaTime);
	_transformSystem.PreUpdate(deltaTime);
	_collisionSystem.Update(deltaTime, 4.0f);
	_actionSystem.Update(deltaTime);
	_particleSystem.update(deltaTime);
	_particle->setCamera(&_camera, _camera.GetEntity().GetComponent<TransformComponent>().GetWorldPosition());
	_particle->update(deltaTime);
	ReadyShadowMap(TERRAIN);

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
	_particleSystem.render();
	_collisionSystem.render();
	_particle->draw();
	_editor->Render();

	return true;
}

bool MapToolScene::SceneRenderSprite()
{

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
