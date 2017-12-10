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

	InitAnimations();

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
	
	//LoadEverySkinnedResources();
	//LoadEveryStaticResources();
	//LoadEverySoundResources();
	LoadStaticModelResources();
	LoadSkinnedModelResources();
	LoadSoundResources();

	float loaded{};
	while (loaded < 1.0f)
	{
		loaded = _loading.LoadNext();
	}

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


	bool  inTerrain = TERRAIN->IsWorldPositionInTerrain(Vector3(-200, -200, -200));

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

void MapToolScene::LoadStaticModelResources()
{
	Matrix correctionMat;
	Matrix rotationCorrection;
#pragma region Rock
	MatrixScaling(&correctionMat, 1.0f, 1.0f, 1.0f);
	_loading.LoadModelResources("../resources/Models/Environment/Rock/Rock1_A.X", "Rock01", true, 1.0f, 0.0f);
	_loading.LoadModelResources("../resources/Models/Environment/Rock/Rock4_A.X", "Rock02", true, 1.0f, 0.0f);
	_loading.LoadModelResources("../resources/Models/Environment/Rock/Rock6_A.X", "Rock03", true, 1.0f, 0.0f);
	_loading.LoadModelResources("../resources/Models/Environment/Rock/Rock8_A.X", "Rock04", true, 1.0f, 0.0f);
	_loading.LoadModelResources("../resources/Models/Environment/Rock/Rock9_A.X", "Rock05", true, 1.0f, 0.0f);

	_loading.LoadModelResources("../resources/Models/Environment/DruidRock/LargeStone1.X",
		"LargeStone1", true, 0.018f, 0.0f);
	_loading.LoadModelResources("../resources/Models/Environment/DruidRock/LargeStone2.X",
		"LargeStone2", true, 0.018f, 0.0f);
	_loading.LoadModelResources("../resources/Models/Environment/DruidRock/MediumStone1.X",
		"MediumStone1", true, 0.018f, 0.0f);
#pragma endregion

	_loading.LoadModelResources("../resources/Models/Environment/Grass/Grass1.X", "Grass01", true, 0.01f, 0.0f);
	_loading.LoadModelResources("../resources/Models/Environment/Grass/Grass2.X", "Grass02", true, 0.01f, 0.0f);
	_loading.LoadModelResources("../resources/Models/Environment/Grass/Grass3.X", "Grass03", true, 0.01f, 0.0f);
	_loading.LoadModelResources("../resources/Models/Environment/Grass/Grass4.X", "Grass04", true, 0.01f, 0.0f);
	_loading.LoadModelResources("../resources/Models/Environment/Grass/Grass5.X", "Grass05", true, 0.01f, 0.0f);
	_loading.LoadModelResources("../resources/Models/Environment/Grass/Grass6.X", "Grass06", true, 0.01f, 0.0f);
	_loading.LoadModelResources("../resources/Models/Environment/Grass/Grass7.X", "Grass07", true, 0.01f, 0.0f);
	_loading.LoadModelResources("../resources/Models/Environment/Grass/Grass8.X", "Grass08", true, 0.01f, 0.0f);
	_loading.LoadModelResources("../resources/Models/Environment/Grass/Grass9.X", "Grass09", true, 0.01f, 0.0f);
	_loading.LoadModelResources("../resources/Models/Environment/Grass/Grass10.X", "Grass10", true, 0.01f, 0.0f);

	//Load Tree
	_loading.LoadModelResources("../resources/Models/Environment/Tree/Tree1.X", "Tree01", true, 0.01f, 0.0f);
	_loading.LoadModelResources("../resources/Models/Environment/Tree/Tree2.X", "Tree02", true, 0.01f, 0.0f);
	_loading.LoadModelResources("../resources/Models/Environment/Tree/Tree3.X", "Tree03", true, 0.01f, 0.0f);
	_loading.LoadModelResources("../resources/Models/Environment/Tree/Tree4.X", "Tree04", true, 0.01f, 0.0f);
	_loading.LoadModelResources("../resources/Models/Environment/Tree/Tree5.X", "Tree05", true, 0.01f, 0.0f);
	_loading.LoadModelResources("../resources/Models/Environment/Tree/Tree6.X", "Tree06", true, 0.01f, 0.0f);
	_loading.LoadModelResources("../resources/Models/Environment/Tree/Tree7.X", "Tree07", true, 0.01f, 0.0f);
	_loading.LoadModelResources("../resources/Models/Environment/Tree/Tree8.X", "Tree08", true, 0.01f, 0.0f);
	_loading.LoadModelResources("../resources/Models/Environment/Tree/Tree9.X", "Tree09", true, 0.01f, 0.0f);
	_loading.LoadModelResources("../resources/Models/Environment/Tree/Tree10.X", "Tree10", true, 0.01f, 0.0f);

	//Load TreeTrunk
	_loading.LoadModelResources("../resources/Models/Environment/TreeTrunk/TreeTrunk1.X",
		"TreeTrunk01", true, 0.01f, 0.0f);
	_loading.LoadModelResources("../resources/Models/Environment/TreeTrunk/TreeTrunk2.X",
		"TreeTrunk02", true, 0.01f, 0.0f);
	_loading.LoadModelResources("../resources/Models/Environment/TreeTrunk/TreeTrunk3.X",
		"TreeTrunk03", true, 0.01f, 0.0f);

	_loading.LoadModelResources("../resources/Models/Environment/Mushroom/Mushroom1.X",
		"Mushroom01", true, 0.01f, 0.0f);
	_loading.LoadModelResources("../resources/Models/Environment/Mushroom/Mushroom2.X",
		"Mushroom02", true, 0.01f, 0.0f);
	_loading.LoadModelResources("../resources/Models/Environment/Mushroom/Mushroom3.X",
		"Mushroom03", true, 0.01f, 0.0f);
}

void MapToolScene::LoadSkinnedModelResources()
{
	_loading.LoadModelResources("../resources/Models/Knight/Knight.X",
		"Knight", false, 0.01f, D3DX_PI);

	_loading.LoadModelResources("../resources/Models/Snake/Snake_Red.X",
		"Snake1", false, 1.4f, 0.0f);
	_loading.LoadModelResources("../resources/Models/Snake/Snake_Black.X",
		"Snake2", false, 1.4f, 0.0f);
	_loading.LoadModelResources("../resources/Models/Snake/Snake_Cyan.X",
		"Snake3", false, 1.4f, 0.0f);


	_loading.LoadModelResources("../resources/Models/DragonTurtle/DragonTurtle_Black.X",
		"Turtle", false, 1.4f, 0.0f);

	_loading.LoadModelResources("../resources/Models/DragonBat/DragonBat_Red.X",
		"Bat1", false, 1.0f, 0.0f);
	_loading.LoadModelResources("../resources/Models/DragonBat/DragonBat_Black.X",
		"Bat2", false, 1.0f, 0.0f);
	_loading.LoadModelResources("../resources/Models/DragonBat/DragonBat_Gold.X",
		"Bat3", false, 1.0f, 0.0f);

	_loading.LoadModelResources("../resources/Models/DevilCat/DevilCat.X",
		"Cat", false, 1.4f, 0.0f);

	_loading.LoadModelResources("../resources/Models/Hydra/Hydra_Green.X",
		"Hydra1", false, 2.0f, 0.0f);
	_loading.LoadModelResources("../resources/Models/Hydra/Hydra_Red.X",
		"Hydra2", false, 2.0f, 0.0f);
	_loading.LoadModelResources("../resources/Models/Hydra/Hydra_Black.X",
		"Hydra3", false, 2.0f, 0.0f);
	_loading.LoadModelResources("../resources/Models/Hydra/Hydra_Gold.X",
		"Hydra4", false, 2.0f, 0.0f);

	_loading.LoadModelResources("../resources/Models/Lizard/Lizard.X",
		"Lizard1", false, 1.0f, 0.0f);
	_loading.LoadModelResources("../resources/Models/Lizard/Lizard_Black.X",
		"Lizard2", false, 1.0f, 0.0f);

	_loading.LoadModelResources("../resources/Models/DragonBoss/DragonBoss.X",
		"Dragon", false, 1.0f, 0.0f);
}

void MapToolScene::LoadSoundResources()
{
#pragma region Player 

	_loading.LoadSoundResources("player_walk_left", 
		"../resources/sfx/Player/Walk/player_dirt_walk_armorlight_01.wav", false, false, 4.0f, 13.0f);
	_loading.LoadSoundResources("player_walk_right", 
		"../resources/sfx/Player/Walk/player_dirt_walk_armorlight_04.wav", false, false, 4.0f, 13.0f);

	_loading.LoadSoundResources("player_run_left", 
		"../resources/sfx/Player/Run/player_dirt_run_armorlight_01.wav", false, false, 4.0f, 13.0f);
	_loading.LoadSoundResources("player_run_right", 
		"../resources/sfx/Player/Run/player_dirt_run_armorlight_04.wav", false, false, 4.0f, 13.0f);

	_loading.LoadSoundResources("player_swing_01", 
		"../resources/sfx/Player/Attack/player_swing_blade_medium_01.wav", false, false, 4.0f, 13.0f);
	_loading.LoadSoundResources("player_swing_02", 
		"../resources/sfx/Player/Attack/player_swing_blade_medium_02.wav", false, false, 4.0f, 13.0f);
	_loading.LoadSoundResources("player_swing_03", 
		"../resources/sfx/Player/Attack/player_swing_blade_medium_03.wav", false, false, 4.0f, 13.0f);

	_loading.LoadSoundResources("player_shield_block", 
		"../resources/sfx/Player/Block/player_shield_block.wav", false, false, 4.0f, 13.0f);
	_loading.LoadSoundResources("player_shield_draw", 
		"../resources/sfx/Player/Block/player_shield_draw.wav", false, false, 4.0f, 13.0f);

	_loading.LoadSoundResources("player_to_combat", 
		"../resources/sfx/Player/ToCombat/player_1hand_draw.wav", false, false, 4.0f, 13.0f);
	_loading.LoadSoundResources("player_to_stance", 
		"../resources/sfx/Player/ToStance/player_1hand_sheathe.wav", false, false, 4.0f, 13.0f);

	_loading.LoadSoundResources("player_impact_01",
		"../resources/sfx/Player/Attack/player_impact_01.wav", false, false, 2.0, 5.0f);
	_loading.LoadSoundResources("player_impact_02",
		"../resources/sfx/Player/Attack/player_impact_02.wav", false, false, 2.0, 5.0f);
	_loading.LoadSoundResources("player_impact_03",
		"../resources/sfx/Player/Attack/player_impact_03.wav", false, false, 2.0, 5.0f);

	_loading.LoadSoundResources("player_hit_01",
		"../resources/sfx/Player/player_hit_01.wav", false, false, 2.0f, 5.0f);
	_loading.LoadSoundResources("player_hit_02",
		"../resources/sfx/Player/player_hit_02.wav", false, false, 2.0f, 5.0f);
	_loading.LoadSoundResources("player_hit_03",
		"../resources/sfx/Player/player_hit_03.wav", false, false, 2.0f, 5.0f);

	_loading.LoadSoundResources("player_hit_talk_01",
		"../resources/sfx/Player/player_hit_talk_01.wav", false, false, 2.0f, 5.0f);

#pragma endregion

#pragma region Cat
	_loading.LoadSoundResources("cat_attack_01", 
		"../resources/sfx/Cat/cat_attack_01.wav", false, false, 4.0f, 13.0f);
	_loading.LoadSoundResources("cat_attack_02", 
		"../resources/sfx/Cat/cat_attack_02.wav", false, false, 4.0f, 13.0f);
	_loading.LoadSoundResources("cat_attack_04", 
		"../resources/sfx/Cat/cat_attack_04.wav", false, false, 4.0f, 13.0f);
	_loading.LoadSoundResources("cat_roar_01", 
		"../resources/sfx/Cat/cat_roar_01.wav", false, false, 4.0f, 13.0f);
	_loading.LoadSoundResources("cat_run_01", 
		"../resources/sfx/Cat/cat_run_01.wav", false, false, 4.0f, 13.0f);
	_loading.LoadSoundResources("cat_hit_01", 
		"../resources/sfx/Cat/cat_hit_01.wav", false, false, 4.0f, 13.0f);
	_loading.LoadSoundResources("cat_dead_01", 
		"../resources/sfx/Cat/cat_dead_01.wav", false, false, 4.0f, 13.0f);
#pragma endregion

#pragma region Bat
	_loading.LoadSoundResources("bat_forward_01", 
		"../resources/sfx/Bat/bat_forward_01.wav", false, false, 4.0f, 13.0f);
	_loading.LoadSoundResources("bat_roar_01", 
		"../resources/sfx/Bat/bat_roar_01.wav", false, false, 4.0f, 13.0f);
	_loading.LoadSoundResources("bat_attack_01", 
		"../resources/sfx/Bat/bat_attack_01.wav", false, false, 4.0f, 13.0f);
	_loading.LoadSoundResources("bat_attack_02", 
		"../resources/sfx/Bat/bat_attack_02.wav", false, false, 4.0f, 13.0f);
	_loading.LoadSoundResources("bat_spit_01", 
		"../resources/sfx/Bat/bat_spit_01.wav", false, false, 4.0f, 13.0f);
	_loading.LoadSoundResources("bat_dead_01", 
		"../resources/sfx/Bat/bat_dead_01.wav", false, false, 4.0f, 13.0f);
	_loading.LoadSoundResources("bat_hit_01", 
		"../resources/sfx/Bat/bat_hit_01.wav", false, false, 4.0f, 13.0f);
#pragma endregion

#pragma region Hydra
	_loading.LoadSoundResources("hydra_walk_left", 
		"../resources/sfx/Hydra/hydra_walk_left.wav", false, false, 4.0f, 13.0f);
	_loading.LoadSoundResources("hydra_walk_right", 
		"../resources/sfx/Hydra/hydra_walk_right.wav", false, false, 4.0f, 13.0f);

	_loading.LoadSoundResources("hydra_attack_01", 
		"../resources/sfx/Hydra/hydra_attack_01.wav", false, false, 4.0f, 13.0f);
	_loading.LoadSoundResources("hydra_attack_02", 
		"../resources/sfx/Hydra/hydra_attack_02.wav", false, false, 4.0f, 13.0f);
	_loading.LoadSoundResources("hydra_attack_03", 
		"../resources/sfx/Hydra/hydra_attack_03.wav", false, false, 4.0f, 13.0f);

	_loading.LoadSoundResources("hydra_breath_01", 
		"../resources/sfx/Hydra/hydra_breath_01.wav", false, false, 4.0f, 13.0f);

	_loading.LoadSoundResources("hydra_death_01", 
		"../resources/sfx/Hydra/hydra_death_01.wav", false, false, 4.0f, 13.0f);
	_loading.LoadSoundResources("hydra_hit_01", 
		"../resources/sfx/Hydra/hydra_hit_01.wav", false, false, 4.0f, 13.0f);

	_loading.LoadSoundResources("hydra_idle_01", 
		"../resources/sfx/Hydra/hydra_idle_01.wav", false, false, 4.0f, 13.0f);

	_loading.LoadSoundResources("hydra_whip_01", 
		"../resources/sfx/Hydra/hydra_whip_01.wav", false, false, 4.0f, 13.0f);

#pragma endregion

#pragma region Lizard
	_loading.LoadSoundResources("lizard_attack_01", 
		"../resources/sfx/Lizard/lizard_attack_01.wav", false, false, 4.0f, 13.0f);
	_loading.LoadSoundResources("lizard_attack_02", 
		"../resources/sfx/Lizard/lizard_attack_02.wav", false, false, 4.0f, 13.0f);
	_loading.LoadSoundResources("lizard_bite_01", 
		"../resources/sfx/Lizard/lizard_bite_01.wav", false, false, 4.0f, 13.0f);
	_loading.LoadSoundResources("lizard_dead_01", 
		"../resources/sfx/Lizard/lizard_dead_01.wav", false, false, 4.0f, 13.0f);
	_loading.LoadSoundResources("lizard_hit_01", 
		"../resources/sfx/Lizard/lizard_hit_01.wav", false, false, 4.0f, 13.0f);
	_loading.LoadSoundResources("lizard_idle_01", 
		"../resources/sfx/Lizard/lizard_idle_01.wav", false, false, 4.0f, 13.0f);
	_loading.LoadSoundResources("lizard_spit_01", 
		"../resources/sfx/Lizard/lizard_spit_01.wav", false, false, 4.0f, 13.0f);
	_loading.LoadSoundResources("lizard_walk_left", 
		"../resources/sfx/Lizard/lizard_walk_left.wav", false, false, 4.0f, 13.0f);
	_loading.LoadSoundResources("lizard_walk_right", 
		"../resources/sfx/Lizard/lizard_walk_right.wav", false, false, 4.0f, 13.0f);
	_loading.LoadSoundResources("lizard_fall_01", 
		"../resources/sfx/Lizard/lizard_fall_01.wav", false, false, 4.0f, 13.0f);
#pragma endregion

#pragma region Snake
	_loading.LoadSoundResources("snake_attack_01", 
		"../resources/sfx/Snake/snake_attack_01.wav", false, false, 4.0f, 13.0f);
	_loading.LoadSoundResources("snake_spit_01", 
		"../resources/sfx/Snake/snake_spit_01.wav", false, false, 4.0f, 13.0f);
	_loading.LoadSoundResources("snake_attack_03", 
		"../resources/sfx/Snake/snake_attack_03.wav", false, false, 4.0f, 13.0f);
	_loading.LoadSoundResources("snake_roar_01", 
		"../resources/sfx/Snake/snake_roar_01.wav", false, false, 4.0f, 13.0f);
	_loading.LoadSoundResources("snake_dead_01", 
		"../resources/sfx/Snake/snake_dead_01.wav", false, false, 4.0f, 13.0f);
#pragma endregion

#pragma region Turtle
	_loading.LoadSoundResources("turtle_attack_01", 
		"../resources/sfx/Turtle/turtle_attack_01.wav", false, false, 4.0f, 13.0f);
	_loading.LoadSoundResources("turtle_attack_02", 
		"../resources/sfx/Turtle/turtle_attack_02.wav", false, false, 4.0f, 13.0f);

	_loading.LoadSoundResources("turtle_dead_01", 
		"../resources/sfx/Turtle/turtle_dead_01.wav", false, false, 4.0f, 13.0f);
	_loading.LoadSoundResources("turtle_hit_01", 
		"../resources/sfx/Turtle/turtle_hit_01.wav", false, false, 4.0f, 13.0f);

	_loading.LoadSoundResources("turtle_roar_01", 
		"../resources/sfx/Turtle/turtle_roar_01.wav", false, false, 4.0f, 13.0f);

	_loading.LoadSoundResources("turtle_walk_left", 
		"../resources/sfx/Turtle/turtle_walk_left.wav", false, false, 4.0f, 13.0f);

	_loading.LoadSoundResources("turtle_walk_right", 
		"../resources/sfx/Turtle/turtle_walk_right.wav", false, false, 4.0f, 13.0f);
#pragma endregion

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
