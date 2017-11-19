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
	config._tile0FileName = "../resources/Textures/terrain1.jpg";
	config._tile1FileName = "../resources/Textures/terrain2.png";
	config._tile2FileName = "../resources/Textures/terrain3.png";
	config._tile3FileName = "../resources/Textures/terrain4.png";
	config._splatFileName = "../resources/Textures/Splat.png";

	config._cellScale = 1.0f;
	config._heightScale = 80.0f;
	config._textureMult = 50;
	config._sectionResolution;

	//TERRAIN->SetScene(this);
	//TERRAIN->Create(config, 1);
	//_terrainEffect = VIDEO->GetEffect("TerrainBase.fx");


	//메쉬 불러오기..
	Matrix correctionMat;
	MatrixScaling(&correctionMat, 0.1f, 0.1f, 0.1f);
	_skinnedMeshHandle = VIDEO->CreateSkinnedXMesh("../resources/Models/knight/Knight.X", &correctionMat, "Knight");

	//MatrixScaling(&correctionMat, 1.0f, 1.0f, 1.0f);
	//video::StaticXMeshHandle staticMeshHandle = VIDEO->CreateStaticXMesh("../resources/Models/environment/Rock/Rock1_A.X", &correctionMat, "Rock");

	video::StaticXMesh::sDefaultEffectHandle = VIDEO->GetEffect("StaticMesh.fx");
	video::SkinnedAnimation::sDefaultEffectHandle = VIDEO->GetEffect("SkinnedMesh.fx");
	video::DebugBuffer::sDefaultEffectHandle = VIDEO->GetEffect("DebugShader.fx");

	//엔티티 생성
	_world.AddSystem<RenderSystem>(_renderSystem);
	_world.AddSystem<TransformSystem>(_transformSystem);

	for (uint32 z = 0; z < 2; ++z)
	{
		for (uint32 x = 0; x < 2; ++x)
		{
			int32 index = Index2D(x, z, 2);
			_entities.push_back(_world.CreateEntity());
			Entity &entity = _entities.back();

			TransformComponent &transComp = entity.AddComponent<TransformComponent>();
			transComp.MovePositionWorld(x * 10, /*TERRAIN->GetHeight(x * 10, z * 10)*/0 , z * 10);
			RenderComponent &renderComp = entity.AddComponent<RenderComponent>();
			renderComp._type = RenderComponent::Type::eSkinned;
			renderComp._skinned = VIDEO->CreateSkinnedAnimation(_skinnedMeshHandle, "Anim" + std::to_string(index));

			entity.Activate();
		}
	}

	/*_entities.push_back(_world.CreateEntity());
	Entity &entity = _entities.back();*/

	//TransformComponent &transComp = entity.AddComponent<TransformComponent>();
	//transComp.MovePositionWorld(-5.0f, -0.0f, -5.0f);
	//RenderComponent &renderComp = entity.AddComponent<RenderComponent>();
	//renderComp._type = RenderComponent::Type::eStatic;
	//renderComp._static = staticMeshHandle;

	//entity.Activate();

	//에디터 생성

	imguiRenderInit();
	_editor = new Editor;
	_editor->Init();


	_debugBuffer = new DebugBuffer;
	Memory mem;
	mem._data = nullptr;
	mem._size = sizeof(video::DebugVertex) * 1024;
	_debugBuffer->_vHandle = VIDEO->CreateVertexBuffer(&mem, VIDEO->GetVertexDecl(video::DebugVertex::_name));

	_active = true;
	return result;
}

bool32 BaseScene::Update(float deltaTime)
{
	bool32 result = true;

	_editor->Edit(RefVariant());

	_world.Refresh();

	_transformSystem.PreUpdate(deltaTime);

	//Collision Check
	_transformSystem.PostUpdate(deltaTime);
	_renderSystem.UpdateAnimations(deltaTime);
	//Update Camera
	_camera.PreUpdateMatrix();
	_transformSystem.UpdateTransform(_camera.GetTransform());
	_camera.UpdateMatrix();

	//_channel.Update<BaseScene::SpawnEvent>(deltaTime);

	for (int i = 0; i < 10; ++i)
	{
		_debugBuffer->Add(video::DebugVertex(Vector3(-50.0f, 0.0f, i), 0xffffffff));
		_debugBuffer->Add(video::DebugVertex(Vector3(50.0f, 0.0f, i), 0xffffffff));
	}
	//_debugBuffer->Add(video::DebugVertex(Vector3(10.0f, 0.0f, -5.0f), 0xff000000));
	_debugBuffer->Update();

	return result;
}

bool32 BaseScene::Render()
{
	Matrix model;
	MatrixIdentity(&model);

	_renderSystem.Render(*_mainRenderView);

	video::RenderCommand &command =  _mainRenderView->GetCommand();
	command._vHandle = _debugBuffer->_vHandle;
	command._drawType = video::RenderCommand::DrawType::eStatic;
	command._effectHandle = video::DebugBuffer::sDefaultEffectHandle;
	command._primType = video::RenderCommand::eLineList;
	command._numPrim = _debugBuffer->_count / 2;
	command._materialHandle = VIDEO->GetMaterial("Default");
	command._cacheRange = _mainRenderView->_matrixCache.Add(&model);

	_debugBuffer->Reset();

	//TERRAIN->FillRenderCommand(*_mainRenderView);

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
	EventChannel channel;
	channel.Add<BaseScene::SpawnEvent, BaseScene>(*this);
}

void BaseScene::Handle(const SpawnEvent &event)
{
	static int a = 0;
	_animations.push_back(VIDEO->CreateSkinnedAnimation(_skinnedMeshHandle, "aat"));
	video::SkinnedAnimation *pAnimation = VIDEO->GetSkinnedAnimation(_animations.back());
	pAnimation->Play(a++);

}
