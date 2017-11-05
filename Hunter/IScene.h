#ifndef I_SCENE_H
#define I_SCENE_H

class IScene
{
public :
	IScene() {};
	virtual ~IScene() {}

	virtual bool32 Load() = 0;
	virtual bool32 Unload() = 0;

	virtual bool32 Init() = 0;
	virtual bool32 Update(float deltaTime) = 0;
	virtual bool32 Render() = 0;

	virtual void Release() = 0;

	virtual const char *GetSceneName() = 0;
	virtual bool32 IsActive() = 0;
};

#endif
