#include "stdafx.h"
#include "BaseScene.h"

using namespace video;


bool BaseScene::Init()
{

	bool result = true;

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

	Terrain::TerrainConfig config;
	config._xChunkCount = 2;
	config._zChunkCount = 2;
	strncpy(config._tile0FileName, "../resources/Terrain/TerrainTexture01.jpg", MAX_FILE_NAME);
	strncpy(config._tile1FileName, "../resources/Terrain/TerrainTexture02.jpg", MAX_FILE_NAME);
	strncpy(config._tile2FileName, "../resources/Terrain/TerrainTexture03.png", MAX_FILE_NAME);
	strncpy(config._tile3FileName, "../resources/Terrain/TerrainTexture04.png", MAX_FILE_NAME);

	config._textureMult = 300;

	//_pTerrain = new Terrain();
	TERRAIN->SetScene(this);
	TERRAIN->Create(config, true);

	Matrix correctionMat;
	MatrixScaling(&correctionMat, 0.01f, 0.01f, 0.01f);
	video::SkinnedXMeshHandle knight  = VIDEO->CreateSkinnedXMesh(
		"../resources/Models/Knight/Knight.X", &correctionMat, "Knight");
	video::AnimationInstanceHandle ainmHandle = VIDEO->CreateAnimationInstance(knight, "Knight0");

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

	_world.AddSystem<RenderSystem>(_renderSystem);
	_world.AddSystem<TransformSystem>(_transformSystem);
	_world.AddSystem<ActionSystem>(_actionSystem);
	_world.AddSystem<ScriptSystem>(_scriptSystem);
	_world.AddSystem<CollisionSystem>(_collisionSystem);

	_camera.CreateFromWorld(_world);
	_camera.SetRotationSpeed(10.0f);
	_camera.SetMoveSpeed(20.0f);

	_pMainLight = new DirectionalLight();
	_pMainLight->CreateFromWorld(_world);
	_pMainLight->SetWorldPosition(Vector3(0.0f, 5.0f, 5.0f));
	_pMainLight->SetTarget(Vector3(0.0f, 0.0f, 0.0f));

	_pEnvironmentSphere = new EnvironmentSphere;
	_pEnvironmentSphere->Create("../resources/Textures/grassenvmap1024.dds");

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

	_pEnvironmentSphere->Render(_camera);
	
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
