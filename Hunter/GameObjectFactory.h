#ifndef GAME_OBJECT_FACTORY_H
#define GAME_OBJECT_FACTORY_H

#include "SingletonBase.h"
#include "GameObjectHandle.h"

class IScene;

enum ARCHE_TYPE
{
	ARCHE_NONE,
	ARCHE_HERO,
	ARCHE_ROCK,
	ARCHE_GRASS,
	ARCHE_TREE,
	ARCHE_MUSHROOM,
	ARCHE_BAT,
	ARCHE_CAT,
	ARCHE_LIZARD,
	ARCHE_SNAKE,
	ARCHE_TURTLE,
	ARCHE_HYDRA,
	ARCHE_COUNT,
};

struct EntitySaveInfo
{
	ARCHE_TYPE _archeType{};
	char _resourceName[MAX_FILE_NAME];
	Vector3 _position;
	//Quaternion _orientation;
};


class GameObjectFactory : public SingletonBase<GameObjectFactory>
{
public :
	void Init();
	void Release();

	void SetCurrentScene(IScene *pScene) { _pCurrentScene = pScene; }

	//void CreateObject(ArcheType type, const Vector3 &position);
	//void CreateObject();

private :
	IScene *_pCurrentScene;

	void RegisterEvent();
	void UnRegisterEvent();

	void CreateObject(ARCHE_TYPE type, ResourceHandle handle, const Vector3 &position);
	void CreateObject(ARCHE_TYPE type, char *name, const Vector3 &position);

	//ResourceHandlePool<TreeHandle> _treeHandlePool;

public :

	struct CreateObjectOnClickEvent
	{
		CreateObjectOnClickEvent(ARCHE_TYPE type, ResourceHandle handle, const Vector2 &cursorPos)
			:_type(type), _cursorPos(cursorPos), _handle(handle)
		{
		}

		ARCHE_TYPE _type;
		Vector2 _cursorPos;

		ResourceHandle _handle;
	};
	void Handle(const CreateObjectOnClickEvent &event);

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

	EventChannel _channel;
};

#define GAMEOBJECTFACTORY GameObjectFactory::GetInstance()

#endif