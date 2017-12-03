#ifndef TEST_SCENE_H
#define TEST_SCENE_H

//���̰� �׽�Ʈ �� �� �̴�.

#include "TransformSystem.h"
#include "RenderSystem.h"
#include "ScriptSystem.h"
#include "ActionSystem.h"
#include "CollisionSystem.h"

#include "IScene.h"

#include "WorldEditor.h"
#include "Player.h"
#include "Snake.h"


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
	//virtual bool SceneRenderSprite() {}

	virtual const char *GetSceneName();

protected :

	Editor *_editor{};

public:
	virtual void Handle(const Editor::GetObjectFromSceneEvent &event);
};

#endif