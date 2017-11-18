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

	_camera.SetRotationSpeed(10.0f);
	_camera.SetMoveSpeed(10.0f);

	_mainRenderView->_pCamera = &_camera;

	Matrix correctionMat;
	MatrixScaling(&correctionMat, 0.1f, 0.1f, 0.1f);

	//_staticMeshHandle = VIDEO->CreateStaticXMesh("../resources/models/knight/Knight.X", &correctionMat, "aa");
	_skinnedMeshHandle = VIDEO->CreateSkinnedXMesh("../resources/models/knight/Knight.X", &correctionMat, "Knight");

	_animations.reserve(16);
	for (int32 i = 0; i < 16; ++i)
	{
		_animations.push_back(VIDEO->CreateSkinnedAnimation(_skinnedMeshHandle, "Anim" + std::to_string(i)));
	}
	for (int32 i = 0; i < 16; ++i)
	{
		video::SkinnedAnimation *pAnimation = VIDEO->GetSkinnedAnimation(_animations[i]);
		pAnimation->Play(i);
	}


	_world.AddSystem<RenderSystem>(_renderSystem);
	_world.AddSystem<TransformSystem>(_transformSystem);

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

	_active = true;
	return result;
}

bool32 BaseScene::Update(float deltaTime)
{
	bool32 result = true;

	_world.Refresh();

	_transformSystem.PreUpdate(deltaTime);

	int32 count = 0;
	Matrix world;
	for (auto &animHandle : _animations)
	{
		video::SkinnedAnimation *pAnimation = VIDEO->GetSkinnedAnimation(animHandle);
		MatrixTranslation(&world, count * 5, 0, 0);
		pAnimation->UpdateAnimation(deltaTime, world);
		count++;
	}

	//Update Camera
	_camera.PreUpdateMatrix();
	_transformSystem.UpdateTransform(_camera.GetTransform());
	_camera.UpdateMatrix();

	return result;
}

bool32 BaseScene::Render()
{
	//video::StaticXMesh *pMesh = VIDEO->GetStaticXMesh(_staticMeshHandle);

	for (uint32 i = 0; i < _animations.size(); ++i)
	{
		video::SkinnedAnimation *pAnimation = VIDEO->GetSkinnedAnimation(_animations[i]);
		pAnimation->UpdateMesh();
		pAnimation->FillRenderCommand(*_mainRenderView, _skinnedEffect, _staticEffect);
	}

	TERRAIN->FillRenderCommand(*_mainRenderView);

	

	//Matrix matrix;
	//for (int32 y = 0; y < 8; ++y)
	//{
	//	for (int32 x = 0; x < 8; ++x)
	//	{
	//		MatrixTranslation(&matrix, x * 5, 0, y * 5);
	//		pMesh->FillRenderCommand(*_mainRenderView, _staticEffect, &matrix);
	//	}
	//}

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
}