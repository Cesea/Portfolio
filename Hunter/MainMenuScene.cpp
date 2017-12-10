#include "stdafx.h"
#include "MainMenuScene.h"

bool MainMenuScene::SceneInit()
{
	HRESULT re = S_OK;
	re = D3DXCreateTextureFromFile(gpDevice, "../resources/Title/title_background.png", &_pBackground);
	re = D3DXCreateTextureFromFile(gpDevice, "../resources/Title/title_characters.png", &_pCharacters);
	re = D3DXCreateTextureFromFile(gpDevice, "../resources/Title/title_letters.png", &_pLetters);

	GAMEOBJECTFACTORY->SetCurrentScene(this);

	_camera.SetCameraState(CAMERASTATE_UI);
	_camera.SetTargetObject((Player *)GAMEOBJECTFACTORY->GetPlayerObject());

	return true;
}

bool MainMenuScene::SceneUpdate(float deltaTime, const InputManager & input)
{
	RECT gameStartRect;
	gameStartRect.left = 218;
	gameStartRect.top = 215;
	gameStartRect.right = gameStartRect.left + 267;
	gameStartRect.bottom = gameStartRect.top + 90;

	RECT mapToolRect;
	mapToolRect.left = 218;
	mapToolRect.top = 380;
	mapToolRect.right = mapToolRect.left + 194;
	mapToolRect.bottom = mapToolRect.top + 82;

	RECT gameEndRect;
	gameEndRect.left = 218;
	gameEndRect.top = 538;
	gameEndRect.right = gameEndRect.left + 87;
	gameEndRect.bottom = gameEndRect.top + 80;

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
	SPRITEMANAGER->DrawTexture(_pLetters, nullptr, 0, 0);
	SPRITEMANAGER->DrawTexture(_pCharacters, nullptr, 0, 0, 0xff0000ff);
	SPRITEMANAGER->DrawTexture(_pBackground, nullptr, 0, 0);
	SPRITEMANAGER->EndSpriteRender();
	return true;
}

const char * MainMenuScene::GetSceneName()
{
	return "MainMenuScene";
}
