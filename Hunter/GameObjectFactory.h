#ifndef GAME_OBJECT_FACTORY_H
#define GAME_OBJECT_FACTORY_H

#include "SingletonBase.h"

class IScene;

enum ARCHE_TYPE
{
	ARCHE_NONE,
	ARCHE_HERO,
	ARCHE_ROCK,
	ARCHE_GRASS,
	ARCHE_TREE,
	ARCHE_TREETRUNK,
	ARCHE_MUSHROOM,
	ARCHE_BAT,
	ARCHE_CAT,
	ARCHE_LIZARD,
	ARCHE_SNAKE,
	ARCHE_TURTLE,
	ARCHE_HYDRA,
	ARCHE_COUNT,
};

const char *ArcheToString(ARCHE_TYPE type);

struct EntitySaveInfo
{
	ARCHE_TYPE _archeType{};
	char _resourceName[MAX_FILE_NAME];
	Vector3 _position;
	Vector3 _scale;
	Vector3 _orientation;
};

class GameObjectFactory : public SingletonBase<GameObjectFactory>
{
public :
	void Init();
	void Release();

	void SetCurrentScene(IScene *pScene) { _pCurrentScene = pScene; }

	BaseGameObject *GetPlayerObject() { return _pPlayer; }

private :
	IScene *_pCurrentScene;

	//void CreateObject(ARCHE_TYPE type, ResourceHandle handle, const Vector3 &position);

	void CreateObject(ARCHE_TYPE type, ResourceHandle handle, const Vector3 &position,
		const Vector3 &scale = Vector3(1.0f, 1.0f, 1.0f),
		const Vector3 &orientation = Vector3(0.0f, 0.0f, 0.0f));
	//void CreateObject(ARCHE_TYPE type, char *name, const Vector3 &position);

	BaseGameObject *_pPlayer{};

public :
	//ObjectCreatedEvent///////////////////////////////////////////////////////
	struct ObjectCreatedEvent
	{
		ObjectCreatedEvent(ARCHE_TYPE type, Entity entity, const Vector3 &world)
			:_type(type), _entity(entity), _worldPosition(world)
		{}

		ARCHE_TYPE _type;
		Entity _entity;
		Vector3 _worldPosition;
	};
	/////////////////////////////////////////////////////////////////////////////////////

	//CreateObjectOnClickEvent///////////////////////////////////////////////////////
	struct CreateObjectOnClickEvent
	{
		CreateObjectOnClickEvent(ARCHE_TYPE type, ResourceHandle handle, 
			const Vector2 &cursorPos, float scale = 1.0f, float yRotation = 0.0f)
			:_type(type), _cursorPos(cursorPos), _handle(handle), _scale(scale), _yRotation(yRotation)
		{
		}

		ARCHE_TYPE _type;
		Vector2 _cursorPos;

		ResourceHandle _handle;

		float _scale;
		float _yRotation;

	};
	void Handle(const CreateObjectOnClickEvent &event);
	/////////////////////////////////////////////////////////////////////////////////////

	//CreateObjectFromSaveInfoEvent///////////////////////////////////////////////////////
	struct CreateObjectFromSaveInfoEvent
	{
		CreateObjectFromSaveInfoEvent(ARCHE_TYPE type, const char *key, 
			const Vector3 &position, 
			const Vector3 &scale = Vector3(1.0f, 1.0f, 1.0f),
			const Vector3 &orientation = Vector3(0.0f, 0.0f, 0.0f))
			:_type(type), _key(key), 
			_position(position), _scale(scale), _orientation(orientation)
		{
		}
		ARCHE_TYPE _type;
		const char *_key;
		Vector3 _position;
		Vector3 _scale;
		Vector3 _orientation;
	};
	void Handle(const CreateObjectFromSaveInfoEvent &event);
	/////////////////////////////////////////////////////////////////////////////////////

	//CreateObjectOnLocationEvent///////////////////////////////////////////////////////
	struct CreateObjectOnLocationEvent
	{
		CreateObjectOnLocationEvent(ARCHE_TYPE type, ResourceHandle handle, const Vector3 &position)
			:_type(type), _handle(handle), _position(position)
		{
		}
		ARCHE_TYPE _type;
		ResourceHandle _handle;
		Vector3 _position;
	};
	void Handle(const CreateObjectOnLocationEvent &event);
	/////////////////////////////////////////////////////////////////////////////////////

	EventChannel _channel;
};

#define GAMEOBJECTFACTORY GameObjectFactory::GetInstance()

#endif