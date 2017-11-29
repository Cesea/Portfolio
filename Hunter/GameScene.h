#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "TransformSystem.h"
#include "RenderSystem.h"
#include "ScriptSystem.h"
#include "ActionSystem.h"
#include "CollisionSystem.h"

#include "IScene.h"

#include "WorldEditor.h"

#include "Player.h"
#include "Snake.h"


class GameScene : public IScene
{
	friend class Terrain;
public:
	GameScene()
		:IScene::IScene()
	{};
	virtual ~GameScene() {}

	virtual bool Init();
	virtual bool Update(float deltaTime, const InputManager &input);

	virtual void Render0();
	virtual void Render1();
	virtual void Render2();

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
	virtual void Handle(const Editor::GetObjectFromSceneEvent &event);
};

#endif