#ifndef MAIN_MENU_SCENE_H
#define MAIN_MENU_SCENE_H

#include "IScene.h"
#include "LoadItem.h"

class MainMenuScene : public IScene
{
public:
	MainMenuScene()
		:IScene::IScene()
	{};
	virtual ~MainMenuScene() {}

	virtual bool SceneInit();
	virtual bool SceneUpdate(float deltaTime, const InputManager &input);
	virtual bool SceneRelease();

	virtual bool SceneRender0();
	//virtual bool SceneRender1() {}
	//virtual bool SceneRender2() {}
	virtual bool SceneRenderSprite();

	virtual const char *GetSceneName();

public:

	LPDIRECT3DTEXTURE9 _pBackground{};
	LPDIRECT3DTEXTURE9 _pCharacters{};
	LPDIRECT3DTEXTURE9 _pLetters{};

	Loading _loading;
	void LoadStaticModelResources();
	void LoadSkinnedModelResources();
	void LoadSoundResources();

	float _loadPercent{ 0.0f };
	bool _loaded{ false };

private :
	void StartShowTitle();
	float _titlePercent{0.0f};
	bool _titleGoing{ false };
	bool _titleShown{ false };

	bool _titlePlaying{ false };


};


#endif