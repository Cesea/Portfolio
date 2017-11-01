#ifndef I_SCENE_H
#define I_SCENE_H

class IScene
{
public :
	IScene() {};
	virtual ~IScene() {}

	virtual bool Load() = 0;
	virtual bool Unload() = 0;

	virtual bool Init() = 0;
	virtual bool Update(float deltaTime) = 0;
	virtual bool Render() = 0;

	virtual void Release() = 0;

	virtual const char *GetSceneName() = 0;
	virtual bool IsActive() = 0;
};

#endif
