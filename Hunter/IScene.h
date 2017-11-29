#ifndef I_SCENE_H
#define I_SCENE_H

#include "World.h"
#include "Terrain.h"
#include "Camera.h"
#include "BaseGameObject.h"

class EnvironmentSphere;

class IScene
{
public :
	friend class GameObjectFactory;
	friend class Editor;
	friend class Terrain;

	IScene()
		:_world(4096)
	{};
	virtual ~IScene() {}

	virtual bool Init() = 0;
	virtual bool Update(float deltaTime, const InputManager &input) = 0;

	//PostEffect까지 모두 렌더 한다...
	bool Render();
	//메인 카메라의 RenderToTexture 만 업데이트한다.
	bool RenderToMainCamTexture();

	LPDIRECT3DTEXTURE9 GetSceneTexture();

	virtual void Render0() {}
	virtual void Render1() {}
	virtual void Render2() {}

	virtual void Release() = 0;

	virtual const char *GetSceneName() = 0;

protected :
	World _world;
	std::vector<BaseGameObject *> _gameObjects;
	Camera _camera;
	DirectionalLight *_pMainLight;

	EventChannel _channel;

	EnvironmentSphere *_pEnvironmentSphere{};

	bool RenderEnvironmentSphere();
public :
};

#endif
