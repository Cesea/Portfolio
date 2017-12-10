#include "stdafx.h"
#include "TestScene.h"


bool TestScene::SceneInit()
{
	bool result = true;
	GAMEOBJECTFACTORY->SetCurrentScene(this);

	video::StaticXMesh::_sEffectHandle = VIDEO->GetEffect("StaticMesh.fx");
	video::SkinnedXMesh::_sEffectHandle = VIDEO->GetEffect("SkinnedMesh.fx");

	//터레인 로드
	Terrain::TerrainConfig config;

	TERRAIN->SetScene(this);
	TERRAIN->Create(config, false);
	TERRAIN->LoadTerrain("../resources/TestScene/Terrain01.tr", false);

	//라이트 생성
	_pMainLight->SetWorldPosition(Vector3(0.0f, 5.0f, 5.0f));
	_pMainLight->SetTarget(Vector3(0.0f, 0.0f, 0.0f));

	_pEnvironmentSphere->Create("../resources/Textures/grassenvmap1024.dds");
	this->CreateObjectFromFile("../resources/TestScene/Terrain01.ed");

	//카메라 생성
	_camera.SetMoveSpeed(6.0f);
	_camera.SetRotationSpeed(1.0f);
	_camera.GetEntity().GetComponent<TransformComponent>().MovePositionWorld(Vector3(0.0f, 4.0f, -6.0f));
	_camera.SetTargetObject((Player *)GAMEOBJECTFACTORY->GetPlayerObject());

	_pInGameUI = new UI;
	_pInGameUI->Init(this);

	_camera.SetCameraState(CAMERASTATE_INCOMBAT);

	SOUNDMANAGER->Play("in_game_bgm_01");

	return result; 
}

bool TestScene::SceneUpdate(float deltaTime, const InputManager & input)
{
	bool result = true;

	_world.Refresh();
	_scriptSystem.Update(deltaTime);
	_transformSystem.PreUpdate(deltaTime);
	_collisionSystem.Update(deltaTime, 4.0f);
	_actionSystem.Update(deltaTime);

	_particleSystem.setCamera(&_camera, _camera.GetEntity().GetComponent<TransformComponent>().GetWorldPosition());
	_particleSystem.update(deltaTime);

	_pInGameUI->Update(deltaTime, input);

	ReadyShadowMap(TERRAIN);

	return result;
}

bool TestScene::SceneRelease()
{
	_pInGameUI->Release();
	SAFE_DELETE(_pInGameUI);
	ReleaseAllGameObjects();
	_world.Clear();

	return true;
}

bool TestScene::SceneRender0()
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

	return true;
}

bool TestScene::SceneRenderSprite()
{
	SPRITEMANAGER->BeginSpriteRender();
	_pInGameUI->RenderUI();


	SPRITEMANAGER->EndSpriteRender();

	return true;
}

const char * TestScene::GetSceneName()
{
	return "TestScene";
}
