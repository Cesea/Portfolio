#ifndef I_SCENE_H
#define I_SCENE_H

#include "World.h"
#include "Terrain.h"
#include "Camera.h"
#include "BaseGameObject.h"



class IScene
{
public :
	friend class GameObjectFactory;
	friend class Terrain;

	IScene()
		:_world(4096)
	{};
	virtual ~IScene() {}

	virtual bool Init() = 0;
	virtual bool Update(float deltaTime, const InputManager &input) = 0;
	virtual bool Render() = 0;

	virtual void Release() = 0;

	virtual const char *GetSceneName() = 0;

protected :
	World _world;
	std::vector<BaseGameObject *> _gameObjects;
	Camera _camera;
	DirectionalLight *_pMainLight;


	EventChannel _channel;

public :
	//virtual void Handle(const Editor::GetObjectFromSceneEvent &event)
	//{
		//Vector3 position;
		//Vector3 terrainHitPos;
		//Ray ray;
		//_camera.ComputeRay(event._cursorPos, &ray);
		//std::vector<Entity> collidingEntity{};
		//std::vector<float> collidingDistance;

		//_collisionSystem.QueryRayEntityHit(ray, &collidingEntity, &collidingDistance);
		//if (collidingEntity.size() > 0)
		//{
		//	float minDistance = 9999.0f;
		//	int32 minIndex = 0;
		//	for (int32 i = 0; i < collidingDistance.size(); ++i)
		//	{
		//		if (collidingDistance[i] < minDistance)
		//		{
		//			minDistance = collidingDistance[i];
		//			minIndex = i;
		//		}
		//	}
		//	_editor->SetEdittingEntity(collidingEntity[minIndex]);
		//}
	//}
};

#endif
