#include "stdafx.h"
#include "TerrainEditorScene.h"

TerrainEditorScene::TerrainEditorScene()
	:_world(4096)
{
}

TerrainEditorScene::~TerrainEditorScene()
{
}

bool TerrainEditorScene::Init()
{
	bool result = true;

	_camera.SetRotationSpeed(10.0f);
	_camera.SetMoveSpeed(20.0f);
	_camera.GetTransform().MovePositionSelf(0.0f, 0.0f, -30.0f);

	_world.AddSystem<RenderSystem>(_renderSystem);
	_world.AddSystem<TransformSystem>(_transformSystem);
	_world.AddSystem<ActionSystem>(_actionSystem);
	_world.AddSystem<ScriptSystem>(_scriptSystem);

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

	//TERRAIN->SetScene(this);
	//TERRAIN->Create(config, 1, true);

	imguiRenderInit();
	_editor = new Editor;
	_editor->Init();

	return result;
}

bool TerrainEditorScene::Update(float deltaTime, const InputManager &input)
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
	_camera.PreUpdateMatrix();
	_transformSystem.UpdateTransform(_camera.GetTransform());
	_camera.UpdateMatrix();
	return result;
}

bool TerrainEditorScene::Render()
{
	bool result = true;
	//DEBUG_DRAWER->DrawWorldGrid(1, 80);
	//DEBUG_DRAWER->FillRenderCommand(*_mainRenderView);

	//TERRAIN->FillRenderCommand(*_mainRenderView);

	_mainRenderView->PreRender();
	_mainRenderView->ExecCommands();
	imguiRenderDraw();
	_mainRenderView->PostRender();


	return result;
}

void TerrainEditorScene::Release()
{
	

}

const char * TerrainEditorScene::GetSceneName()
{
	return "TerrainEditorScene";
}

void TerrainEditorScene::RegisterEvents()
{
}
