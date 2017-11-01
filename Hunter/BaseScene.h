#ifndef BASE_SCENE_H
#define BASE_SCENE_H

#include "IScene.h"

class BaseScene : public IScene
{
public :
	BaseScene() {};
	virtual ~BaseScene() {}

	virtual bool Load();
	virtual bool Unload();

	virtual bool Init();
	virtual bool Update(float deltaTime);
	virtual bool Render();

	virtual void Release();

	virtual const char *GetSceneName();
	virtual bool IsActive();

protected :
	bool _active;
};


#endif