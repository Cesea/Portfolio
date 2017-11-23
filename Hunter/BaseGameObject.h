#ifndef BASE_GAME_OBJECT_H
#define BASE_GAME_OBJECT_H

#include "World.h"

class BaseGameObject
{
public :
	BaseGameObject() {}
	virtual ~BaseGameObject() {}

	virtual bool CreateFromWorld(World &world) = 0;

protected :
	Entity _entity;

	bool32 _inCombat{false};
};

class GameObjectAnimationCallbackHandler : public ID3DXAnimationCallbackHandler
{
public :
	bool Init(BaseGameObject *pBaseObject) { _pOwner = pBaseObject; }
	virtual HRESULT CALLBACK HandleCallback(THIS_ UINT Track, LPVOID pCallbackData) = 0;

private :
	BaseGameObject *_pOwner{};
};

#endif