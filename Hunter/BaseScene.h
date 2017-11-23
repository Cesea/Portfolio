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
public:
	BaseScene()
		:_world(4096)
	{};
	virtual ~BaseScene() {}

	virtual bool Init();
	virtual bool Update(float deltaTime, const InputManager &input);
	virtual bool Render();

	virtual void Release();
	virtual const char *GetSceneName() { return "BaseScene"; }

protected:

protected:
	bool32 _active;
private:
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

public:
	struct SpawnEvent
	{
		SpawnEvent(const Vector3 &position)
			:_position(position)
		{}

		bool32 _isStatic;

		Vector3 _position;
		char _name[32];
	};
	void Handle(const BaseScene::SpawnEvent &event);
};

#endif