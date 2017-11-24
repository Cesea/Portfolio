#ifndef I_SCENE_H
#define I_SCENE_H

#include "World.h"
#include "Terrain.h"
#include "Camera.h"

class IScene
{
public :
	friend class GameObjectFactory;

	IScene()
		:_world(4096), _entities(4096)
	{};
	virtual ~IScene() {}

	virtual bool Init() = 0;
	virtual bool Update(float deltaTime, const InputManager &input) = 0;
	virtual bool Render() = 0;

	virtual void Release() = 0;

	virtual const char *GetSceneName() = 0;

protected :
	World _world;
	std::vector<Entity> _entities;
	Camera _camera;
	Terrain *_pTerrain{};

	DirectionalLight *_pMainLight;
};

#endif
