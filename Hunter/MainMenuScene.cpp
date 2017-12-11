#include "stdafx.h"
#include "MainMenuScene.h"

bool MainMenuScene::SceneInit()
{
	HRESULT re = S_OK;
	re = D3DXCreateTextureFromFile(gpDevice, "../resources/Title/title_background.png", &_pBackground);
	re = D3DXCreateTextureFromFile(gpDevice, "../resources/Title/title_characters_01.png", &_pCharacters);
	re = D3DXCreateTextureFromFile(gpDevice, "../resources/Title/title_letters.png", &_pLetters);

	GAMEOBJECTFACTORY->SetCurrentScene(this);

	SOUNDMANAGER->AddSound("title_bgm", "../resources/sfx/title_bgm_01.mp3", true, true);

	LoadStaticModelResources();
	LoadSkinnedModelResources();
	LoadSoundResources();

	_camera.SetCameraState(CAMERASTATE_UI);
	_camera.SetTargetObject((Player *)GAMEOBJECTFACTORY->GetPlayerObject());

	InitAnimations();

	return true;
}

bool MainMenuScene::SceneUpdate(float deltaTime, const InputManager & input)
{
	RECT gameStartRect;
	gameStartRect.left = 230;
	gameStartRect.top = 300;
	gameStartRect.right = gameStartRect.left + 265;
	gameStartRect.bottom = gameStartRect.top + 86;

	RECT mapToolRect;
	mapToolRect.left = 230;
	mapToolRect.top = 458;
	mapToolRect.right = mapToolRect.left + 190;
	mapToolRect.bottom = mapToolRect.top + 82;

	RECT gameEndRect;
	gameEndRect.left = 230;
	gameEndRect.top = 618;
	gameEndRect.right = gameEndRect.left + 81;
	gameEndRect.bottom = gameEndRect.top + 78;
	if (input.mouse.IsReleased(MOUSE_BUTTON_LEFT))
	{
		POINT mousePoint;
		mousePoint = input.mouse.GetCurrentPoint();
		EventChannel channel;
		if (PtInRect(&gameStartRect, mousePoint))
		{
			gEngine->GetScene()->ChangeScene("TestScene");
			//channel.Broadcast<SceneManager::SceneChangeEvent>(SceneManager::SceneChangeEvent("TestScene"));
			SOUNDMANAGER->Stop("title_bgm");
		}
		else if (PtInRect(&mapToolRect, mousePoint))
		{
			gEngine->GetScene()->ChangeScene("MapToolScene");
			//channel.Broadcast<SceneManager::SceneChangeEvent>(SceneManager::SceneChangeEvent("MapToolScene"));
			SOUNDMANAGER->Stop("title_bgm");
			return true;;
		}
		else if (PtInRect(&gameEndRect, mousePoint))
		{
			SOUNDMANAGER->Stop("title_bgm");
		}
	}

	_loadPercent = _loading.LoadNext();
	if (_loadPercent >= 1.0f)
	{
		_loaded = true;
		StartShowTitle();
		if (!_titlePlaying)
		{
			_titlePlaying = true;
			SOUNDMANAGER->Play("title_bgm");
		}
	}

	if (_titleGoing)
	{
		_titlePercent += 0.016f;
	}
	if (_titlePercent >= 1.0f)
	{
		_titlePercent = 1.0f;
		_titleGoing = false;
		_titleShown = true;
	}

	return true;
}

bool MainMenuScene::SceneRelease()
{
	COM_RELEASE(_pBackground);
	COM_RELEASE(_pCharacters);
	COM_RELEASE(_pLetters);

	return true;
}

bool MainMenuScene::SceneRender0()
{
	return true;
}

bool MainMenuScene::SceneRenderSprite()
{
	SPRITEMANAGER->BeginSpriteRender();
	if (_titleGoing)
	{
		uint8 alpha = (uint8)(_titlePercent * 255.0f);
		SPRITEMANAGER->DrawTexture(_pLetters, nullptr, 0, 0, D3DCOLOR_ARGB(alpha, 255, 255, 255));
	}
	else if (_titleShown)
	{
		SPRITEMANAGER->DrawTexture(_pLetters, nullptr, 0, 0);
	}
	uint8 a = (uint8)(_loadPercent * 255.0f);
	SPRITEMANAGER->DrawTexture(_pCharacters, nullptr, 0, 0, D3DCOLOR_ARGB(a, 255, 255, 255));

	SPRITEMANAGER->DrawTexture(_pBackground, nullptr, 0, 0);
	SPRITEMANAGER->EndSpriteRender();
	return true;
}

const char * MainMenuScene::GetSceneName()
{
	return "MainMenuScene";
}

void MainMenuScene::LoadStaticModelResources()
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
	_loading.LoadModelResources("../resources/Models/Environment/Tree/Tree5.X", "Tree01", true, 0.01f, 0.0f);
	_loading.LoadModelResources("../resources/Models/Environment/Tree/Tree6.X", "Tree02", true, 0.01f, 0.0f);
	_loading.LoadModelResources("../resources/Models/Environment/Tree/Tree7.X", "Tree03", true, 0.01f, 0.0f);
	_loading.LoadModelResources("../resources/Models/Environment/Tree/Tree8.X", "Tree04", true, 0.01f, 0.0f);

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

void MainMenuScene::LoadSkinnedModelResources()
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

void MainMenuScene::LoadSoundResources()
{
	_loading.LoadSoundResources("in_game_bgm_01",
		"../resources/sfx/in_game_bgm_01.mp3", true, true);

	_loading.LoadSoundResources("inven_open_01",
		"../resources/sfx/inven_open_01.wav", false, false);
	_loading.LoadSoundResources("inven_close_01",
		"../resources/sfx/inven_close_01.wav", false, false);
	_loading.LoadSoundResources("inven_select_01",
		"../resources/sfx/inven_select_01.wav", false, false);


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
		"../resources/sfx/Hydra/hydra_walk_left.wav", false, false, 1.0f, 13.0f);
	_loading.LoadSoundResources("hydra_walk_right", 
		"../resources/sfx/Hydra/hydra_walk_right.wav", false, false, 1.0f, 13.0f);

	_loading.LoadSoundResources("hydra_attack_01", 
		"../resources/sfx/Hydra/hydra_attack_01.wav", false, false, 1.0f, 13.0f);
	_loading.LoadSoundResources("hydra_attack_02", 
		"../resources/sfx/Hydra/hydra_attack_02.wav", false, false, 1.0f, 13.0f);
	_loading.LoadSoundResources("hydra_attack_03", 
		"../resources/sfx/Hydra/hydra_attack_03.wav", false, false, 1.0f, 13.0f);

	_loading.LoadSoundResources("hydra_breath_01", 
		"../resources/sfx/Hydra/hydra_breath_01.wav", false, false, 1.0f, 13.0f);

	_loading.LoadSoundResources("hydra_death_01", 
		"../resources/sfx/Hydra/hydra_death_01.wav", false, false, 1.0f, 13.0f);
	_loading.LoadSoundResources("hydra_hit_01", 
		"../resources/sfx/Hydra/hydra_hit_01.wav", false, false, 1.0f, 13.0f);

	_loading.LoadSoundResources("hydra_idle_01", 
		"../resources/sfx/Hydra/hydra_idle_01.wav", false, false, 1.0f, 13.0f);

	_loading.LoadSoundResources("hydra_whip_01", 
		"../resources/sfx/Hydra/hydra_whip_01.wav", false, false, 1.0f, 13.0f);

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
#pragma region Dragon
	_loading.LoadSoundResources("dragon_atk",
		"../resources/sfx/Dragon/dragon_atk.mp3", false, false, 4.0f, 13.0f);
	_loading.LoadSoundResources("dragon_bite",
		"../resources/sfx/Dragon/dragon_bite.mp3", false, false, 4.0f, 13.0f);

	_loading.LoadSoundResources("dragon_BreathFire",
		"../resources/sfx/Dragon/dragon_BreathFire.mp3", false, false, 4.0f, 13.0f);
	_loading.LoadSoundResources("dragon_charge",
		"../resources/sfx/Dragon/dragon_charge.mp3", false, false, 4.0f, 13.0f);

	_loading.LoadSoundResources("dragon_roar",
		"../resources/sfx/Dragon/dragon_roar.mp3", false, false, 4.0f, 13.0f);
	_loading.LoadSoundResources("dragon_step",
		"../resources/sfx/Dragon/dragon_step.mp3", false, false, 4.0f, 13.0f);
	_loading.LoadSoundResources("dragon_wing",
		"../resources/sfx/Dragon/dragon_wing.mp3", false, false, 4.0f, 13.0f);
#pragma endregion

}

void MainMenuScene::StartShowTitle()
{
	_titleGoing = true;
}




