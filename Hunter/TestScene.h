#ifndef TEST_SCENE_H
#define TEST_SCENE_H

#include "BaseScene.h"

class TestScene : public BaseScene
{
public:
	TestScene() {};
	virtual ~TestScene() {}

	virtual bool32 Load();
	virtual bool32 Unload();

	virtual bool32 Init();
	virtual bool32 Update(float deltaTime);
	virtual bool32 Render();

	virtual void Release();

	virtual const char *GetSceneName();
	virtual bool32 IsActive();

private :

};


#endif