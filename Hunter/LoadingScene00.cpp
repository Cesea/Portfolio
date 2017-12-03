#include "stdafx.h"
#include "LoadingScene00.h"

bool LoadingScene00::SceneInit()
{
	//시스템 생성
	_world.AddSystem<RenderSystem>(_renderSystem);
	_world.AddSystem<TransformSystem>(_transformSystem);
	_world.AddSystem<ActionSystem>(_actionSystem);
	_world.AddSystem<ScriptSystem>(_scriptSystem);
	_world.AddSystem<CollisionSystem>(_collisionSystem);

	//카메라 생성
	_camera.CreateFromWorld(_world);
	_camera.SetRotationSpeed(1.0f);
	_camera.SetMoveSpeed(2.0f);
	_camera.GetEntity().GetComponent<TransformComponent>().MovePositionWorld(Vector3(0.0f, 4.0f, -6.0f));

	//라이트 생성
	_pMainLight = new DirectionalLight();
	_pMainLight->CreateFromWorld(_world);
	_pMainLight->SetWorldPosition(Vector3(0.0f, 5.0f, 5.0f));
	_pMainLight->SetTarget(Vector3(0.0f, 0.0f, 0.0f));

	_pEnvironmentSphere = new EnvironmentSphere;

	return true;
}

bool LoadingScene00::SceneUpdate(float deltaTime, const InputManager & input)
{
	bool result = true;

	_world.Refresh();

	_scriptSystem.Update(deltaTime);

	_camera.MoveAndRotate(deltaTime, input);
	_transformSystem.PreUpdate(deltaTime);

	//Collision Check
	_collisionSystem.Update(deltaTime, 4.0f);
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

bool LoadingScene00::SceneRelease()
{
	for (auto object : _gameObjects)
	{
		SAFE_DELETE(object);
	}
	_gameObjects.clear();

	_world.Clear();

	return true;
}

bool LoadingScene00::SceneRender0()
{
	GIZMOMANAGER->WorldGrid(1.0f, 20);

	return true;
}

const char * LoadingScene00::GetSceneName()
{
	return "LoadingScene00";
}
