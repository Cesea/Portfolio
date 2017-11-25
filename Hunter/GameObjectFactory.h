#ifndef GAME_OBJECT_FACTORY_H
#define GAME_OBJECT_FACTORY_H

#include "SingletonBase.h"

class IScene;

enum ARCHE_TYPE
{
	ARCHE_HERO,
	ARCHE_ROCK,
	ARCHE_GRASS,
	ARCHE_TREE,
	ARCHE_SNAKE,
	ARCHE_TURTLE,
	ARCHE_DRAGON,
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

public :
	//struct CreateObjectEvent
	//{
	//	CreateObjectEvent(ArcheType type, const Vector3 &position)
	//		:_type(type), _position(position)
	//	{
	//	}

	//	ArcheType _type;
	//	Vector3 _position;
	//};
	//void Handle(const CreateObjectEvent &event);

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