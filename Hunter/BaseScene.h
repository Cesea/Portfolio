#ifndef BASE_SCENE_H
#define BASE_SCENE_H

#include "IScene.h"

#include "RenderSystem.h"
#include "ActionSystem.h"
#include "ScriptSystem.h"

#include "TransformSystem.h"

#include "WorldEditor.h"
#include "Player.h"
#include "Snake.h"

class BaseScene : public IScene
{
	friend class Terrain;
public:
	BaseScene()
		:IScene::IScene()
	{};
	virtual ~BaseScene() {}

	virtual bool Init();
	virtual bool Update(float deltaTime, const InputManager &input);
	virtual bool Render();

	virtual void Release();
	virtual const char *GetSceneName() { return "BaseScene"; }

protected :
	TransformSystem _transformSystem;
	RenderSystem _renderSystem;
	ScriptSystem _scriptSystem;
	ActionSystem _actionSystem;
	CollisionSystem _collisionSystem;

	EventChannel _channel;

	Editor *_editor{};

	EnvironmentSphere *_pEnvironmentSphere{};

public:

};

#endif