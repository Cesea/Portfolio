#ifndef TEST_SCENE_H
#define TEST_SCENE_H

#include "IScene.h"
#include "UI.h"

class TestScene : public IScene
{
	friend class Terrain;
	friend class Editor;
public:
	TestScene()
		:IScene::IScene()
	{};
	virtual ~TestScene() {}

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

	Editor *_editor{};
public:
};

#endif