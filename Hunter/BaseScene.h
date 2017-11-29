#ifndef BASE_SCENE_H
#define BASE_SCENE_H

#include "IScene.h"

#include "RenderSystem.h"
#include "ActionSystem.h"
#include "ScriptSystem.h"

#include "TransformSystem.h"

#include "Player.h"
#include "Snake.h"
#include "Turtle.h"
#include "bat.h"
#include "Cat.h"
#include "Hydra.h"
#include "Lizard.h"

#include "WorldEditor.h"

class BaseScene : public IScene
{
	friend class Terrain;
public:
	BaseScene()
		:IScene::IScene()
	{};
	virtual ~BaseScene() {}
	
	virtual bool SceneInit();
	virtual bool SceneUpdate(float deltaTime, const InputManager &input);
	virtual bool SceneRelease();

	virtual bool SceneRender0();
	//virtual bool SceneRender1() {}
	//virtual bool SceneRender2() {}
	//virtual bool SceneRenderSprite() {}

	virtual const char *GetSceneName() { return "BaseScene"; }

protected :
	TransformSystem _transformSystem;
	RenderSystem _renderSystem;
	ScriptSystem _scriptSystem;
	ActionSystem _actionSystem;
	CollisionSystem _collisionSystem;

	EventChannel _channel;

	Editor *_pEditor{};


	Turtle _turtle;
	Bat _bat;
	Cat _cat;
	Hydra _hydra;
	Lizard _lizard;

};

#endif