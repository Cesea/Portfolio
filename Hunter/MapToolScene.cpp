#include "stdafx.h"
#include "MapToolScene.h"


void LoadEverySkinnedResources()
{

}

bool MapToolScene::SceneInit()
{
	bool result = true;
	_channel.Add<Editor::GetObjectFromSceneEvent, MapToolScene>(*this);

	GAMEOBJECTFACTORY->SetCurrentScene(this);

	video::StaticXMesh::_sEffectHandle = VIDEO->GetEffect("StaticMesh.fx");
	video::SkinnedXMesh::_sEffectHandle  = VIDEO->GetEffect("SkinnedMesh.fx");


	//터레인 로드
	Terrain::TerrainConfig config;
	config._xChunkCount = 4;
	config._zChunkCount = 4;
	strncpy(config._tile0FileName, "../resources/Terrain/TerrainTexture01.jpg", MAX_FILE_NAME);
	strncpy(config._tile1FileName, "../resources/Terrain/TerrainTexture02.jpg", MAX_FILE_NAME);
	strncpy(config._tile2FileName, "../resources/Terrain/TerrainTexture03.png", MAX_FILE_NAME);

	//TextureMult는 하나의 TerrainChun에 대한 Multiplier값이다.
	config._textureMult = 25;

	TERRAIN->SetScene(this);
	TERRAIN->Create(config, true);
	
	_camera.SetMoveSpeed(6.0f);
	_camera.SetRotationSpeed(1.0f);

	_pMainLight->SetWorldPosition(Vector3(4.0f, 7.0f, 3.0f));
	_pMainLight->SetTarget(Vector3(0.0f, 0.0f, 0.0f));

	_pEnvironmentSphere->Create("../resources/Textures/grassenvmap1024.dds");

	_channel.Broadcast<GameObjectFactory::CreateObjectOnLocationEvent>(
		GameObjectFactory::CreateObjectOnLocationEvent(ARCHE_HERO, ResourceHandle(), 
			Vector3(0.0f, 0.0f, 0.0f)));

	//NOTE : GameObjectFactory의 GetPlayerObject는 생성에 의존성을 가진다
	_camera.SetTargetObject((Player *)GAMEOBJECTFACTORY->GetPlayerObject());

	//에디터 생성
	imguiRenderInit();
	_editor = new Editor;
	_editor->Init(this);
	_editor->_pSelectedObject = GAMEOBJECTFACTORY->GetPlayerObject();

	_scriptSystem.SetRunning(false);
	_actionSystem.SetRunning(false);

	Player * pPlayer = (Player *)GAMEOBJECTFACTORY->GetPlayerObject();
	if (!_actionSystem.GetRunning() || 
		!_scriptSystem.GetRunning())
	{
		pPlayer->UnRegisterEvents();
	}
	_channel.Broadcast<GameObjectFactory::CreateObjectOnLocationEvent>(
		GameObjectFactory::CreateObjectOnLocationEvent(ARCHE_DRAGON, ResourceHandle(), 
			Vector3(0.0f, 0.0f, 0.0f)));


	return result;
}

bool MapToolScene::SceneUpdate(float deltaTime, const InputManager & input)
{
	bool result = true;
	{
		static float x = 0;
		static float y = 0;

		if (input.keyboard.IsDown(VK_DOWN))
		{
			x -= 0.02f;
		}
		if (input.keyboard.IsDown(VK_UP))
		{
			x += 0.02f;
		}
		if (input.keyboard.IsDown(VK_LEFT))
		{
			y -= 0.02f;
		}
		if (input.keyboard.IsDown(VK_RIGHT))
		{
			y += 0.02f;
		}
	}

	if (input.keyboard.IsPressed('1'))
	{
		_camera.SetCameraState(CAMERASTATE_CREATE);
	}
	else if(input.keyboard.IsPressed('2'))
	{
		_camera.SetCameraState(CAMERASTATE_INCOMBAT);
	}
	else if(input.keyboard.IsPressed('3'))
	{
		_camera.SetCameraState(CAMERASTATE_UI);
	}

	if (input.keyboard.IsReleased(VK_ESCAPE))
	{
		ReleaseAllGameObjects();
		_world.Clear();

		gEngine->GetScene()->ChangeScene("MainMenuScene");
		return true;
	}

	_editor->Edit(RefVariant(), input);

	_world.Refresh();
	_scriptSystem.Update(deltaTime);
	_transformSystem.PreUpdate(deltaTime);
	_collisionSystem.Update(deltaTime, 4.0f);
	_actionSystem.Update(deltaTime);
	_particleSystem.setCamera(&_camera, _camera.GetEntity().GetComponent<TransformComponent>().GetWorldPosition());
	_particleSystem.update(deltaTime);

	ReadyShadowMap(TERRAIN);

	//_ui->Update(deltaTime, input);


	return result;
}

bool MapToolScene::SceneRelease()
{
	SAFE_DELETE(_editor);

	for (auto object : _gameObjects)
	{
		SAFE_DELETE(object);
	}
	_gameObjects.clear();

	_world.Clear();

	//VIDEO->DestroyEveryVertexBuffers();
	//VIDEO->DestroyEveryndexBuffers();
	//VIDEO->DestroyEveryAnimationInstances();
	//VIDEO->DestroyEverySkinnedMesh();
	//VIDEO->DestroyEveryStaticMesh();
	//VIDEO->DestroyEveryTextures();

	return true;
}

bool MapToolScene::SceneRender0()
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
	_editor->Render();

	return true;
}

bool MapToolScene::SceneRenderSprite()
{
	//SPRITEMANAGER->BeginSpriteRender();
	//SPRITEMANAGER->DrawTexture(_shadowCamera.GetRenderTexture(), nullptr, 600, 0, 1.0f, 1.0f, 0.0f);
	
	//SPRITEMANAGER->EndSpriteRender();
	return true;
}

const char * MapToolScene::GetSceneName()
{
	return "MapToolScene";
}


void MapToolScene::Handle(const Editor::GetObjectFromSceneEvent & event)
{
	Vector3 position;
	Vector3 terrainHitPos;
	Ray ray;
	_camera.ComputeRay(event._cursorPos, &ray);
	std::vector<Entity> collidingEntity{};
	std::vector<float> collidingDistance;

	_collisionSystem.QueryRayEntityHit(ray, &collidingEntity, &collidingDistance);
	if (collidingEntity.size() > 0)
	{
		float minDistance = 9999.0f;
		int32 minIndex = 0;
		for (int32 i = 0; i < collidingDistance.size(); ++i)
		{
			if (collidingDistance[i] < minDistance)
			{
				minDistance = collidingDistance[i];
				minIndex = i;
			}
		}
		_editor->SetEdittingEntity(collidingEntity[minIndex]);
	}
}
