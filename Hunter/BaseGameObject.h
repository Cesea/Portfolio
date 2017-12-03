#ifndef BASE_GAME_OBJECT_H
#define BASE_GAME_OBJECT_H

#include "World.h"

class BaseGameObject
{
	//오브젝트 생성을 위해서 Friend class로 함
	friend class GameObjectFactory;
	//타겟 오브젝트의 트랜스폼을 받기 위해서 Friend class로 함
	friend class Camera;
public :
	BaseGameObject() {}
	virtual ~BaseGameObject() {}

	virtual bool CreateFromWorld(World &world, const Vector3 &pos) = 0;
	Entity &GetEntity() { return _entity; }


protected :
	Entity _entity;
	int32 _animationEnum;
};

class GameObjectAnimationCallbackHandler : public ID3DXAnimationCallbackHandler
{
public :
	virtual bool Init(BaseGameObject *pBaseObject) = 0;
	virtual HRESULT CALLBACK HandleCallback(THIS_ UINT Track, LPVOID pCallbackData) = 0;
};

#endif