#ifndef BASE_SCENE_H
#define BASE_SCENE_H

#include "IScene.h"

#include "World.h"
#include "MoveSystem.h"
#include "RenderSystem.h"

#include "TransformSystem.h"

#include "Camera.h"

class BaseScene : public IScene
{
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

protected :
	void RegisterEvents();

	void Handle(const InputManager::KeyDownEvent &event);
	void Handle(const InputManager::KeyPressedEvent &event);
	void Handle(const InputManager::KeyReleasedEvent &event);
	void Handle(const InputManager::MouseMoveEvent &event);
	void Handle(const InputManager::MouseDownEvent &event);
	void Handle(const InputManager::MousePressedEvent &event);
	void Handle(const InputManager::MouseReleasedEvent &event);
	void Handle(const InputManager::MouseWheelEvent &event);

protected :
	bool32 _active;

	bool32 _itemCheck{ false };
	bool _collapse{ false };

	float _sliderValue{ 0 };

	char _strings1[MAX_PATH] = {0, };
	char _strings2[MAX_PATH] = {0, };

	im::WindowPos _firstWindowPos;
	im::WindowPos _secondWindowPos;

	int32 _scroll{ 0 };

	bool _checked{ false };

	video::RenderView *_renderView;

	video::VertexBufferHandle _vertexBuffer0;
	video::VertexBufferHandle _vertexBuffer1;
	video::VertexBufferHandle _vertexBuffer2;
	video::IndexBufferHandle _indexBuffer;

	video::EffectHandle _effect;

	video::MaterialHandle _material0;
	video::MaterialHandle _material1;

	World _world;

	TransformSystem _transformSystem;
	RenderSystem _renderSystem;

	std::vector<Entity> _entities;

	Camera _camera;
};


#endif