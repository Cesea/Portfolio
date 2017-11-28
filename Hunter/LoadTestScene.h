#ifndef LOAD_TEST_SCENE_H
#define LOAD_TEST_SCENE_H

#include "TransformSystem.h"
#include "RenderSystem.h"
#include "ScriptSystem.h"
#include "ActionSystem.h"
#include "CollisionSystem.h"

#include "IScene.h"

#include "WorldEditor.h"

#include "Player.h"
#include "Snake.h"

class LoadTestScene : public IScene
{
	friend class Terrain;
public:
	LoadTestScene()
		:IScene::IScene()
	{};
	virtual ~LoadTestScene() {}

	virtual bool Init();
	virtual bool Update(float deltaTime, const InputManager &input);
	virtual bool Render();

	virtual void Release();
	virtual const char *GetSceneName() { return "GameScene"; }

protected :
	TransformSystem _transformSystem;
	RenderSystem _renderSystem;
	ScriptSystem _scriptSystem;
	ActionSystem _actionSystem;
	CollisionSystem _collisionSystem;

	Editor *_editor{};

	EnvironmentSphere *_pEnvironmentSphere{};

public:
};

#endif