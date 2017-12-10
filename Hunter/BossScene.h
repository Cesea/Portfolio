#ifndef BOSS_SCENE_H
#define BOSS_SCENE_H

#include "IScene.h"
#include "UI.h"

class BossScene : public IScene
{
	friend class Terrain;
	friend class Editor;
public:
	BossScene()
		:IScene::IScene()
	{};
	virtual ~BossScene() {}

	virtual bool SceneInit();
	virtual bool SceneUpdate(float deltaTime, const InputManager &input);
	virtual bool SceneRelease();

	virtual bool SceneRender0();
	//virtual bool SceneRender1() {}
	//virtual bool SceneRender2() {}
	virtual bool SceneRenderSprite();

	virtual const char *GetSceneName();

protected :
	UI *_pInGameUI;

public:
};


#endif