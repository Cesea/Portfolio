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
	RegisterEvents();

	video::RenderViewHandle renderViewHandle= VIDEO->CreateRenderView("Main");
	_mainRenderView = VIDEO->GetRenderView(renderViewHandle);
	_mainRenderView->_clearColor = 0xff55330;

	_camera.SetRotationSpeed(0.1f);
	_camera.SetMoveSpeed(10.0f);

	_mainRenderView->_pCamera = &_camera;

	Matrix correctionMat;
	MatrixScaling(&correctionMat, 0.1f, 0.1f, 0.1f);

	_staticMeshHandle = VIDEO->CreateStaticXMesh("../resources/models/Knight/Knight.X", &correctionMat, "Knight");

	_world.AddSystem<RenderSystem>(_renderSystem);
	_world.AddSystem<TransformSystem>(_transformSystem);
	_world.AddSystem<animation::AnimationSystem>(_animationSystem);

	_staticEffect = VIDEO->GetEffect("StaticMesh.fx");
	_skinnedEffect = VIDEO->GetEffect("SkinnedMesh.fx");
	_terrainEffect = VIDEO->GetEffect("TerrainBase.fx");

	_camera.GetTransform().MovePositionSelf(0.0f, 0.0f, -30.0f);

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

	TERRAIN->Create(config, 1);
	//_terrain.Create(config, 1);

	_active = true;
	return result;
}

bool32 BaseScene::Update(float deltaTime)
{
	bool32 result = true;

	_world.Refresh();

	_transformSystem.PreUpdate(deltaTime);

	//Update Camera
	_transformSystem.UpdateTransform(_camera.GetTransform());
	_camera.UpdateMatrix();

	gpDevice->SetTransform(D3DTS_VIEW, &_camera.GetViewMatrix());
	gpDevice->SetTransform(D3DTS_PROJECTION, &_camera.GetProjectionMatrix());

	return result;
}

bool32 BaseScene::Render()
{
	_renderSystem.Render(*_mainRenderView);


	_mainRenderView->PreRender();
	_mainRenderView->ExecCommands();
	_mainRenderView->PostRender();

	return true;
}

void BaseScene::Release()
{
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
	channel.Add<InputManager::KeyPressedEvent, BaseScene>(*this);
}

void BaseScene::Handle(const InputManager::KeyPressedEvent & event)
{
	//switch (event.code)
	//{
	//case '1':
	//{
	//	_pMesh->Play(1, 1.0f);
	//}break;
	//case '2':
	//{
	//	_pMesh->Play(2, 1.0f);
	//}break;
	//case '3':
	//{
	//	_pMesh->Play(3, 1.0f);
	//}break;
	//case '4':
	//{
	//	_pMesh->Play(4, 1.0f);
	//}break;
	//case '5':
	//{
	//	_pMesh->Play(5, 1.0f);
	//}break;
	//case '6':
	//{
	//	_pMesh->Play(6, 1.0f);
	//}break;
	//case '7':
	//{
	//	_pMesh->Play(7, 1.0f);
	//}break;
	//case '8':
	//{
	//	_pMesh->Play(8, 1.0f);
	//}break;
	//case '9':
	//{
	//	_pMesh->Play(9, 1.0f);
	//}break;
	//}
}
