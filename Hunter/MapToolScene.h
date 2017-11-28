#ifndef MAP_TOOL_SCENE_H
#define MAP_TOOL_SCENE_H

#include "TransformSystem.h"
#include "RenderSystem.h"
#include "ScriptSystem.h"
#include "ActionSystem.h"
#include "CollisionSystem.h"

#include "IScene.h"

#include "WorldEditor.h"
#include "Player.h"
#include "Snake.h"

class MapToolScene : public IScene
{
	friend class Terrain;
public:
	MapToolScene()
		:IScene::IScene()
	{};
	virtual ~MapToolScene() {}

	virtual bool Init();
	virtual bool Update(float deltaTime, const InputManager &input);
	virtual bool Render();

	virtual void Release();
	virtual const char *GetSceneName() { return "MapToolScene"; }

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