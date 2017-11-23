#ifndef I_SCENE_H
#define I_SCENE_H

class IScene
{
public :
	IScene() {};
	virtual ~IScene() {}

	virtual bool Init() = 0;
	virtual bool Update(float deltaTime, const InputManager &input) = 0;
	virtual bool Render() = 0;

	virtual void Release() = 0;

	virtual const char *GetSceneName() = 0;
};

#endif
