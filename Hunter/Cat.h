#ifndef CAT_H
#define CAT_H

#include "Enemy.h"
#include "CatAnimationString.h"

class Cat;
class CatStateMachine;

struct CatCallbackData
{
	CAT_ANIMATION_ENUM *_animtionEnum;
};

class CatCallbackHandler : public GameObjectAnimationCallbackHandler
{
public:
	bool Init(Cat *pCat) { _pCat = pCat; }
	HRESULT CALLBACK HandleCallback(THIS_ UINT Track, LPVOID pCallbackData)
	{
		//      SnakeCallbackData* pData = ( SnakeCallbackData* )pCallbackData;
		//if (nullptr == pData)
		//{
		//          return S_OK;
		//}

		//switch (*pData->_animtionEnum)
		//{

		//}
		return S_OK;
	}

private:
	Cat *_pCat{};
};

class Cat : public Enemy
{
	friend class CatStateMachine;
	friend class CatCallbackHandler;
private:
	enum CATSTATE
	{
		CATSTATE_IDLE,
		CATSTATE_PATROL,
		CATSTATE_FIND,
		CATSTATE_RUN,
		CATSTATE_ATK1,
		CATSTATE_ATK2,
		CATSTATE_ATK3,
		CATSTATE_ATK4,
		CATSTATE_ATK5,
		CATSTATE_HURT,
		CATSTATE_DIE,
		CATSTATE_STAND
	};
	CATSTATE _state;
public:
	Cat();
	virtual ~Cat();

	virtual bool CreateFromWorld(World &world);
	void Update(float deltaTime);

protected:
	void SetupCallbackAndCompression();

	CatCallbackData _callbackData;

	CatStateMachine *_pStateMachine;

	GameCommand _currentCommand;

	ActionComponent *_pActionComp{};

	void QueueAction(const Action &action);
public:
	bool findPlayer(Vector3 forward, Vector3 playerPos, Vector3 myPos, float range1, float range2, float findRadian);
};

#endif