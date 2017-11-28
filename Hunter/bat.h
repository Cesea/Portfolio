#ifndef BAT_H
#define BAT_H

#include "Enemy.h"
#include "BatAnimationString.h"

class Bat;
class BatStateMachine;

struct BatCallbackData
{
	BAT_ANIMATION_ENUM *_animtionEnum;
};

class BatCallbackHandler : public GameObjectAnimationCallbackHandler
{
public:
	bool Init(Bat *pbat) { _pBat = pbat; }
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
	Bat *_pBat{};
};

class Bat : public Enemy
{
	friend class BatStateMachine;
	friend class BatCallbackHandler;

public:
	Bat();
	virtual ~Bat();

	virtual bool CreateFromWorld(World &world);
	void Update(float deltaTime);

protected:
	void SetupCallbackAndCompression();

	BatCallbackData _callbackData;

	BatStateMachine *_pStateMachine;

	GameCommand _currentCommand;

	ActionComponent *_pActionComp{};

	void QueueAction(const Action &action);
public:
	bool findPlayer(Vector3 forward, Vector3 playerPos, Vector3 myPos, float range1, float range2, float findRadian);
};

#endif