#ifndef LOADING_SCENE_00_H
#define LOADING_SCENE_00_H

#include "TransformSystem.h"
#include "RenderSystem.h"
#include "ScriptSystem.h"
#include "ActionSystem.h"
#include "CollisionSystem.h"

#include "IScene.h"

#include "Player.h"
#include "Snake.h"

class LoadingScene00 : public IScene
{
public:
	LoadingScene00()
		:IScene::IScene()
	{};
	virtual ~LoadingScene00() {}

	virtual bool SceneInit();
	virtual bool SceneUpdate(float deltaTime, const InputManager &input);
	virtual bool SceneRelease();

	virtual bool SceneRender0();
	//virtual bool SceneRender1() {}
	//virtual bool SceneRender2() {}
	//virtual bool SceneRenderSprite() {}

	virtual const char *GetSceneName();

protected :
	TransformSystem _transformSystem;
	RenderSystem _renderSystem;
	ScriptSystem _scriptSystem;
	ActionSystem _actionSystem;
	CollisionSystem _collisionSystem;

	BaseGameObject *_pRenderObject{};
	video::StaticXMesh _pMesh{};
};

#endif