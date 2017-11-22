#ifndef BASE_SCENE_H
#define BASE_SCENE_H

#include "IScene.h"

#include "World.h"
#include "RenderSystem.h"
#include "ActionSystem.h"
#include "ScriptSystem.h"

#include "TransformSystem.h"

#include "Terrain.h"

#include "WorldEditor.h"
#include "Player.h"


class BaseScene : public IScene
{
	friend class Terrain;
public :
	BaseScene() 
		:_world(4096)
	{};
	virtual ~BaseScene() {}

	virtual bool SceneInit();
	virtual void SceneRelease();
	virtual bool SceneUpdate(float timDelta);
	virtual void SceneRender1();
	virtual void SceneRenderSprite();

protected :

protected :
	bool32 _active;
private :
	World _world;
	Camera _camera;

	TransformSystem _transformSystem;
	RenderSystem _renderSystem;
	ScriptSystem _scriptSystem;
	ActionSystem _actionSystem;
	std::vector<Entity> _entities;

	EventChannel _channel;

	Editor *_editor{};

	Player _player;

public :
	struct SpawnEvent
	{
		SpawnEvent(const Vector3 &position)
			:_position(position)
		{}
		Vector3 _position;
	};
	void Handle(const BaseScene::SpawnEvent &event);
};

#endif