#include "stdafx.h"
#include "BaseScene.h"

using namespace video;

bool32 BaseScene::Load()
{
	bool32 result = true;
	return result;
}

bool32 BaseScene::Unload()
{
	bool32 result = true;
	return result;
}

bool32 BaseScene::Init()
{
	bool32 result = true;
	//RegisterEvents();

	_channel.Add<BaseScene::SpawnEvent, BaseScene>(*this);

	InitPlayerAnimation();

	//렌더 타켓 설정
	video::RenderViewHandle renderViewHandle= VIDEO->CreateRenderView("Main");
	_mainRenderView = VIDEO->GetRenderView(renderViewHandle);
	_mainRenderView->_clearColor = 0xff55330;

	_camera.SetRotationSpeed(10.0f);
	_camera.SetMoveSpeed(20.0f);
	_camera.GetTransform().MovePositionSelf(0.0f, 0.0f, -30.0f);

	_mainRenderView->_pCamera = &_camera;

	//터레인 로드
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
	config._sectionResolution = 64;

	TERRAIN->SetScene(this);
	TERRAIN->Create(config, 1, false);
	_terrainEffect = VIDEO->GetEffect("TerrainBase.fx");

	//메쉬 불러오기..
	Matrix correctionMat;
	MatrixScaling(&correctionMat, 0.012f, 0.012f, 0.012f);
	_skinnedMeshHandle = VIDEO->CreateSkinnedXMesh("../resources/Models/Knight/Knight.X", &correctionMat, "Knight");

	MatrixScaling(&correctionMat, 1.0f, 1.0f, 1.0f);
	video::StaticXMeshHandle staticMeshHandle = VIDEO->CreateStaticXMesh("../resources/Models/Environment/Rock/Rock1_A.X", &correctionMat, "Rock1_A");

	video::StaticXMeshHandle gizmoTranslation = VIDEO->CreateStaticXMesh("../resources/GizmoTranslation.X", &correctionMat, "GizmoTranslation");
	video::StaticXMeshHandle gizmoScaling = VIDEO->CreateStaticXMesh("../resources/GizmoScale.X", &correctionMat, "GizmoScaling");
	video::StaticXMeshHandle gizmoRotation = VIDEO->CreateStaticXMesh("../resources/GizmoRotation.X", &correctionMat, "GizmoRotation");

	video::StaticXMesh::sDefaultEffectHandle = VIDEO->GetEffect("StaticMesh.fx");
	video::AnimationInstance::sDefaultEffectHandle = VIDEO->GetEffect("SkinnedMesh.fx");
	video::DebugBuffer::sDefaultEffectHandle = VIDEO->GetEffect("DebugShader.fx");

	//엔티티 생성
	_world.AddSystem<RenderSystem>(_renderSystem);
	_world.AddSystem<TransformSystem>(_transformSystem);
	_world.AddSystem<ActionSystem>(_actionSystem);
	_world.AddSystem<ScriptSystem>(_scriptSystem);



	//for (uint32 z = 0; z < 2; ++z)
	//{
	//	for (uint32 x = 0; x < 2; ++x)
	//	{
	//		int32 index = Index2D(x, z, 2);
	//		_entities.push_back(_world.CreateEntity());
	//		Entity &entity = _entities.back();
	//		TransformComponent &transComp = entity.AddComponent<TransformComponent>();
	//		transComp.MovePositionWorld(x * 10, /*TERRAIN->GetHeight(x * 10, z * 10)*/0 , z * 10);
	//		RenderComponent &renderComp = entity.AddComponent<RenderComponent>();
	//		renderComp._type = RenderComponent::Type::eSkinned;
	//		renderComp._skinned = VIDEO->CreateSkinnedAnimation(_skinnedMeshHandle, "Anim" + std::to_string(index));
	//		ActionComponent &actionComp = entity.AddComponent<ActionComponent>();
	//		actionComp.CreateFrom(renderComp._skinned);
	//		entity.Activate();
	//	}
	//}

	_player.CreateFromWorld(_world);

	{
		_entities.push_back(_world.CreateEntity());
		Entity &entity = _entities.back();

		TransformComponent &transComp = entity.AddComponent<TransformComponent>();
		transComp.MovePositionWorld(0.0f, -0.0f, 0.0f);
		RenderComponent &renderComp = entity.AddComponent<RenderComponent>();
		renderComp._type = RenderComponent::Type::eStatic;
		renderComp._static = gizmoTranslation;
		renderComp._effect = _staticEffect;

		entity.Activate();
	}

	{
		_entities.push_back(_world.CreateEntity());
		Entity &entity = _entities.back();

		TransformComponent &transComp = entity.AddComponent<TransformComponent>();
		transComp.MovePositionWorld(0.0f, -0.0f, 0.0f);
		RenderComponent &renderComp = entity.AddComponent<RenderComponent>();
		renderComp._type = RenderComponent::Type::eStatic;
		renderComp._static = gizmoRotation;
		renderComp._effect = _staticEffect;

		entity.Activate();
	}

	{
		_entities.push_back(_world.CreateEntity());
		Entity &entity = _entities.back();

		TransformComponent &transComp = entity.AddComponent<TransformComponent>();
		transComp.MovePositionWorld(0.0f, -0.0f, 0.0f);
		RenderComponent &renderComp = entity.AddComponent<RenderComponent>();
		renderComp._type = RenderComponent::Type::eStatic;
		renderComp._static = gizmoScaling;
		renderComp._effect = _staticEffect;

		entity.Activate();
	}

	//에디터 생성
	imguiRenderInit();
	_editor = new Editor;
	_editor->Init();

	_active = true;
	return result;
}

bool32 BaseScene::Update(float deltaTime)
{
	bool32 result = true;

	_editor->Edit(RefVariant());

	_world.Refresh();

	_scriptSystem.Update(deltaTime);
	_transformSystem.PreUpdate(deltaTime);

	//Collision Check
	_transformSystem.PostUpdate(deltaTime);
	_actionSystem.Update(deltaTime);

	//Update Camera
	_camera.PreUpdateMatrix();
	_transformSystem.UpdateTransform(_camera.GetTransform());
	_camera.UpdateMatrix();

	//_channel.Update<BaseScene::SpawnEvent>(deltaTime);

	return result;
}

bool32 BaseScene::Render()
{
	DEBUG_DRAWER->DrawWorldGrid(1, 80);
	DEBUG_DRAWER->FillRenderCommand(*_mainRenderView);

	_renderSystem.Render(*_mainRenderView);
	TERRAIN->FillRenderCommand(*_mainRenderView);

	_mainRenderView->PreRender();
	_mainRenderView->ExecCommands();
	imguiRenderDraw();
	_mainRenderView->PostRender();

	return true;
}

void BaseScene::Release()
{
	_editor->Shutdown();
	SAFE_DELETE(_editor);

	imguiRenderDestroy();
}

const char * BaseScene::GetSceneName()
{
	return "BaseScene";
}

bool32 BaseScene::IsActive()
{
	return _active;
}

void BaseScene::RegisterEvents()
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

