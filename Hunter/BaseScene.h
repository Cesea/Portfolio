#ifndef BASE_SCENE_H
#define BASE_SCENE_H

#include "IScene.h"

#include "World.h"
#include "MoveSystem.h"
#include "RenderSystem.h"
#include "ActionSystem.h"
#include "ScriptSystem.h"

#include "TransformSystem.h"

#include "Terrain.h"

#include "Camera.h"

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

	virtual bool32 Load();
	virtual bool32 Unload();

	virtual bool32 Init();
	virtual bool32 Update(float deltaTime);
	virtual bool32 Render();

	virtual void Release();

	virtual const char *GetSceneName();
	virtual bool32 IsActive();

	void RegisterEvents();

	void Handle(const InputManager::KeyPressedEvent &event);

protected :

protected :
	bool32 _active;
	video::RenderView *_mainRenderView{};

	World _world;

	TransformSystem _transformSystem;
	RenderSystem _renderSystem;
	ScriptSystem _scriptSystem;
	ActionSystem _actionSystem;
	std::vector<Entity> _entities;

	Camera _camera;
	video::StaticXMeshHandle _staticMeshHandle;
	video::SkinnedXMeshHandle _skinnedMeshHandle;

	std::vector<video::SkinnedAnimationHandle> _animations;

	video::EffectHandle _staticEffect;
	video::EffectHandle _skinnedEffect;
	video::EffectHandle _terrainEffect;

	std::vector<Vector3> _points;

	EventChannel _channel;

	Editor *_editor{};
	video::FontHandle _font;

	Player _player;

public :
};

#endif