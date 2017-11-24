#ifndef GAME_OBJECT_FACTORY_H
#define GAME_OBJECT_FACTORY_H

#include "SingletonBase.h"

class IScene;

enum ArcheType
{
	eHero,
	eRock,
	eTree,
	eGrass,
	eSnake,
	eTurtle,
	eDragon,
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

	void CreateObject(ArcheType type, ResourceHandle handle, const Vector3 &position);

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
		CreateObjectOnClickEvent(ArcheType type, ResourceHandle handle, const Vector2 &cursorPos)
			:_type(type), _cursorPos(cursorPos), _handle(handle)
		{
		}

		ArcheType _type;
		Vector2 _cursorPos;

		ResourceHandle _handle;
	};
	void Handle(const CreateObjectOnClickEvent &event);

	EventChannel _channel;
};

#define GAMEOBJECTFACTORY GameObjectFactory::GetInstance()

#endif