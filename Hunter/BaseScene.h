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
#include "Turtle.h"
#include "bat.h"
#include "Cat.h"
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

private:
	TransformSystem _transformSystem;
	RenderSystem _renderSystem;
	ScriptSystem _scriptSystem;
	ActionSystem _actionSystem;
	CollisionSystem _collisionSystem;

	EventChannel _channel;

	Editor *_editor{};

	Turtle _turtle;
	Bat _bat;
	Cat _cat;
public:

	void Handle(const Editor::GetObjectFromSceneEvent &event);
};

#endif