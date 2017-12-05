#ifndef MAP_TOOL_SCENE_H
#define MAP_TOOL_SCENE_H

#include "TransformSystem.h"
#include "RenderSystem.h"
#include "ScriptSystem.h"
#include "ActionSystem.h"
#include "CollisionSystem.h"
#include "ParticleSystem.h"
#include "IScene.h"

#include "WorldEditor.h"
#include "Player.h"
#include "Snake.h"

class MapToolScene : public IScene
{
	friend class Terrain;
	friend class Editor;
public:
	MapToolScene()
		:IScene::IScene()
	{};
	virtual ~MapToolScene() {}

	virtual bool SceneInit();
	virtual bool SceneUpdate(float deltaTime, const InputManager &input);
	virtual bool SceneRelease();

	virtual bool SceneRender0();
	//virtual bool SceneRender1() {}
	//virtual bool SceneRender2() {}
	virtual bool SceneRenderSprite();

	virtual const char *GetSceneName();
protected :
	Editor *_editor{};

	Entity trash;

public:
	virtual void Handle(const Editor::GetObjectFromSceneEvent &event);
};

#endif