#ifndef BASE_SCENE_H
#define BASE_SCENE_H

#include "IScene.h"

#include "World.h"
#include "MoveSystem.h"
#include "RenderSystem.h"

class BaseScene : public IScene
{
public :
	BaseScene() {};
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

	World _world;

	//MoveSystem _moveSystem;
	//RenderSystem _renderSystem;

};


#endif