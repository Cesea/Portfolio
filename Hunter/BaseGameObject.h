#ifndef BASE_GAME_OBJECT_H
#define BASE_GAME_OBJECT_H

#include "World.h"

class BaseGameObject
{
	//������Ʈ ������ ���ؼ� Friend class�� ��
	friend class GameObjectFactory;
	//Ÿ�� ������Ʈ�� Ʈ�������� �ޱ� ���ؼ� Friend class�� ��
	friend class Camera;
public :
	BaseGameObject() {}
	virtual ~BaseGameObject() {}

	virtual bool CreateFromWorld(World &world,Vector3 Pos) = 0;

protected :
	Entity _entity;
	int32 _animationEnum;
};

class GameObjectAnimationCallbackHandler : public ID3DXAnimationCallbackHandler
{
public :
	bool Init(BaseGameObject *pBaseObject) { _pOwner = pBaseObject; return true; }
	virtual HRESULT CALLBACK HandleCallback(THIS_ UINT Track, LPVOID pCallbackData) = 0;

private :
	BaseGameObject *_pOwner{};
};

#endif