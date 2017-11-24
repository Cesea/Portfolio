#include "stdafx.h"
#include "BaseScene.h"

using namespace video;


bool BaseScene::Init()
{
	bool result = true;
	//RegisterEvents();

	GAMEOBJECTFACTORY->SetCurrentScene(this);

	video::StaticXMesh::_sEffectHandle = VIDEO->GetEffect("StaticMesh.fx");
	video::SkinnedXMesh::_sStaticEffectHandle = VIDEO->GetEffect("StaticMesh.fx");
	video::SkinnedXMesh::_sSkinnedEffectHandle = VIDEO->GetEffect("SkinnedMesh.fx");


	InitPlayerAnimation();
	InitSnakeAnimation();

	_camera.SetRotationSpeed(10.0f);
	_camera.SetMoveSpeed(20.0f);
	_camera.GetTransform().MovePositionSelf(0.0f, 0.0f, -30.0f);

	//ÅÍ·¹ÀÎ ·Îµå
	Terrain::TerrainConfig config;
	config._heightFileName = "../resources/Textures/Height_map1024.jpg";
	config._tile0FileName = "../resources/Textures/TerrainTexture01.jpg";
	config._tile1FileName = "../resources/Textures/TerrainTexture02.jpg";
	config._tile2FileName = "../resources/Textures/TerrainTexture03.png";
	config._tile3FileName = "../resources/Textures/TerrainTexture04.png";
	config._splatFileName = "../resources/Textures/Splat.png";

	config._cellScale = 1.0f;
	config._heightScale = 20.0f;
	config._textureMult = 300;
	config._lodRatio = 0.1f;
	config._sectionResolution = TERRAIN_CHUNK_DIM;

	_pTerrain = new Terrain();
	_pTerrain->SetScene(this);
	_pTerrain->Create(config, 1, false);

	//¸Þ½¬ ºÒ·¯¿À±â..
	Matrix correctionMat;
	MatrixScaling(&correctionMat, 0.15f, 0.15f, 0.15f);
	video::SkinnedXMeshHandle knight  = VIDEO->CreateSkinnedXMesh(
		"../resources/Models/Knight/Knight.X", &correctionMat, "Knight");
	video::AnimationInstanceHandle ainmHandle = VIDEO->CreateAnimationInstance(knight, "Knight0");

	MatrixScaling(&correctionMat, 1.15f, 1.15f, 1.15f);
	video::SkinnedXMeshHandle snakeMesh = VIDEO->CreateSkinnedXMesh(
		"../resources/Models/Snake/Snake_Red.X", &correctionMat, "Snake");

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

	//¿£Æ¼Æ¼ »ý¼º
	_world.AddSystem<RenderSystem>(_renderSystem);
	_world.AddSystem<TransformSystem>(_transformSystem);
	_world.AddSystem<ActionSystem>(_actionSystem);
	_world.AddSystem<ScriptSystem>(_scriptSystem);

	//¶óÀÌÆ® »ý¼º
	_pMainLight = new DirectionalLight();
	_pMainLight->CreateFromWorld(_world);
	_pMainLight->SetWorldPosition(Vector3(0.0f, -5.0f, 5.0f));
	_pMainLight->SetTarget(Vector3(0.0f, 0.0f, 0.0f));

	//_player.CreateFromWorld(_world);
	_snake.CreateFromWorld(_world);

	//¿¡µðÅÍ »ý¼º
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
	_transformSystem.PreUpdate(deltaTime);

	//Collision Check
	_transformSystem.PostUpdate(deltaTime);
	_actionSystem.Update(deltaTime);

	//Update Camera
	{
		_camera.PreUpdateMatrix();
		_transformSystem.UpdateTransform(_camera.GetTransform());
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

	_pTerrain->Render(_camera);
	_renderSystem.Render(_camera);
	//TERRAIN->FillRenderCommand(*_mainRenderView);

	imguiRenderDraw();
<<<<<<< HEAD
<<<<<<< HEAD
	_snake.render();
=======

>>>>>>> parent of 4902ea6... Gizmoì¶”ê°€
=======

>>>>>>> parent of 4902ea6... Gizmoì¶”ê°€
	gpDevice->EndScene();
	gpDevice->Present(nullptr, nullptr, NULL, nullptr);

	return true;
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