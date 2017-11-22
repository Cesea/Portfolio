#include "stdafx.h"
#include "BaseScene.h"

using namespace video;

//
//bool32 BaseScene::Init()
//{
//	bool32 result = true;
//	//RegisterEvents();
//
//
//	video::StaticXMesh::_sEffectHandle = VIDEO->GetEffect("StaticMesh.fx");
//	video::SkinnedXMesh::_sStaticEffectHandle = VIDEO->GetEffect("StaticMesh.fx");
//	video::SkinnedXMesh::_sSkinnedEffectHandle = VIDEO->GetEffect("SkinnedMesh.fx");
//
//	_channel.Add<BaseScene::SpawnEvent, BaseScene>(*this);
//
//	InitPlayerAnimation();
//
//	_camera.SetRotationSpeed(10.0f);
//	_camera.SetMoveSpeed(20.0f);
//	_camera.GetTransform().MovePositionSelf(0.0f, 0.0f, -30.0f);
//
//
//	//터레인 로드
//	Terrain::TerrainConfig config;
//	config._heightFileName = "../resources/Textures/Height_map1024.jpg";
//	config._tile0FileName = "../resources/Textures/TerrainTexture01.jpg";
//	config._tile1FileName = "../resources/Textures/TerrainTexture02.jpg";
//	config._tile2FileName = "../resources/Textures/TerrainTexture03.png";
//	config._tile3FileName = "../resources/Textures/TerrainTexture04.png";
//	config._splatFileName = "../resources/Textures/Splat.png";
//
//	config._cellScale = 1.0f;
//	config._heightScale = 20.0f;
//	config._textureMult = 300;
//	config._lodRatio = 0.1f;
//	config._sectionResolution = 64;
//
//	//TERRAIN->SetScene(this);
//	//TERRAIN->Create(config, 1, false);
//	//_terrainEffect = VIDEO->GetEffect("TerrainBase.fx");
//
//	//메쉬 불러오기..
//	Matrix correctionMat;
//	MatrixScaling(&correctionMat, 0.15f, 0.15f, 0.15f);
//	video::SkinnedXMeshHandle skinned  = VIDEO->CreateSkinnedXMesh("../resources/Models/Knight/Knight.X", &correctionMat, "Knight");
//	video::AnimationInstanceHandle ainmHandle = VIDEO->CreateAnimationInstance(skinned, "Knight0");
//
//	MatrixScaling(&correctionMat, 1.0f, 1.0f, 1.0f);
//	video::StaticXMeshHandle staticMeshHandle = VIDEO->CreateStaticXMesh("../resources/Models/Environment/Rock/Rock1_A.X", &correctionMat, "Rock1_A");
//
//	//엔티티 생성
//	_world.AddSystem<RenderSystem>(_renderSystem);
//	_world.AddSystem<TransformSystem>(_transformSystem);
//	_world.AddSystem<ActionSystem>(_actionSystem);
//	_world.AddSystem<ScriptSystem>(_scriptSystem);
//
//
//	_player.CreateFromWorld(_world);
//
//	_entities.push_back(_world.CreateEntity());
//	Entity &entity = _entities.back();
//
//	TransformComponent &transComp = entity.AddComponent<TransformComponent>();
//	transComp.MovePositionWorld(0.0f, -0.0f, 0.0f);
//	RenderComponent &renderComp = entity.AddComponent<RenderComponent>();
//	renderComp._type = RenderComponent::Type::eStatic;
//	renderComp._static = staticMeshHandle;
//	entity.Activate();
//
//
//	//{
//	//	_entities.push_back(_world.CreateEntity());
//	//	entity = _entities.back();
//
//	//	TransformComponent &transComp = entity.AddComponent<TransformComponent>();
//	//	transComp.MovePositionWorld(0.0f, -0.0f, 0.0f);
//	//	RenderComponent &renderComp = entity.AddComponent<RenderComponent>();
//	//	renderComp._type = RenderComponent::Type::eSkinned;
//	//	renderComp._skinned = ainmHandle;
//	//	ActionComponent &actionComp = entity.AddComponent<ActionComponent>();
//	//	actionComp._
//	//	entity.Activate();
//	//}
//
//	_pDirectional = new DirectionalLight;
//	_pDirectional->CreateFromWorld(_world);
//	_pDirectional->SetWorldPosition(Vector3( 0, 20, 0 ));
//	_pDirectional->SetRotateWorld( 90.0f * ONE_RAD, 0, 0 );
//
//	//에디터 생성
//	imguiRenderInit();
//	_editor = new Editor;
//	_editor->Init();
//
//	_active = true;
//	return result;
//}
//
//bool32 BaseScene::Update(float deltaTime)
//{
//	bool32 result = true;
//
//	_editor->Edit(RefVariant());
//
//	_world.Refresh();
//
//	_scriptSystem.Update(deltaTime);
//	_transformSystem.PreUpdate(deltaTime);
//
//	//Collision Check
//	_transformSystem.PostUpdate(deltaTime);
//	_actionSystem.Update(deltaTime);
//
//	//Update Camera
//	_camera.PreUpdateMatrix();
//	_transformSystem.UpdateTransform(_camera.GetTransform());
//	_camera.UpdateMatrix();
//	_camera.UpdateCamToDevice();
//
//	//_channel.Update<BaseScene::SpawnEvent>(deltaTime);
//
//	return result;
//}
//
//bool32 BaseScene::Render()
//{
//	video::StaticXMesh::SetCamera(_camera);
//	video::StaticXMesh::SetBaseLight(_pDirectional);
//
//	video::SkinnedXMesh::SetCamera(_camera);
//	video::SkinnedXMesh::SetBaseLight(_pDirectional);
//
//	gpDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, 0xff303040, 1.0f, 0);
//	gpDevice->BeginScene();
//	
//	GIZMOMANAGER->WorldGrid(1.0f, 20);
//
//	_renderSystem.Render(_camera);
//
//	gpDevice->EndScene();
//	gpDevice->Present(nullptr, nullptr, NULL, nullptr);
//	//TERRAIN->FillRenderCommand(*_mainRenderView);
//
//	//_mainRenderView->PreRender();
//	//_mainRenderView->ExecCommands();
//	//imguiRenderDraw();
//	//_mainRenderView->PostRender();
//
//	return true;
//}

bool BaseScene::SceneInit()
{
	//라이트 위치
	_pBaseDirectionalLight->SetWorldPosition(Vector3(0, 20, 0));
	_pBaseDirectionalLight->SetRotateWorld(90.0f * ONE_RAD, 0, 0);

	return true;
}

void BaseScene::SceneRelease()
{
}

bool BaseScene::SceneUpdate(float timDelta)
{
	return false;
}

void BaseScene::SceneRender1()
{
}

void BaseScene::SceneRenderSprite()
{
}

void BaseScene::Handle(const BaseScene::SpawnEvent & event)
{
	_entities.push_back(_world.CreateEntity());
	Entity &entity = _entities.back();
	TransformComponent &refTransform = entity.AddComponent<TransformComponent>();
	refTransform.MovePositionWorld(event._position);

	RenderComponent &refRender = entity.AddComponent<RenderComponent>();
	refRender._type = RenderComponent::Type::eStatic;
	refRender._static = VIDEO->GetStaticXMesh("Rock1_A");

	entity.Activate();
}

