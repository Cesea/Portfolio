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

private:
	TransformSystem _transformSystem;
	RenderSystem _renderSystem;
	ScriptSystem _scriptSystem;
	ActionSystem _actionSystem;

	EventChannel _channel;

	Editor *_editor{};

	Player _player;
	Snake _snake;
public:
	//struct SpawnEvent
	//{
	//	SpawnEvent(const Vector3 &position)
	//		:_position(position)
	//	{}
	//	bool32 _isStatic;
	//	Vector3 _position;
	//	char _name[32];
	//};
	//void Handle(const BaseScene::SpawnEvent &event);
};

#endif