#ifndef HYDRA_H
#define HYDRA_H

#include "Enemy.h"
#include "HydraAnimationString.h"

class Hydra;
class HydraStateMachine;

struct HydraCallbackData
{
	HYDRA_ANIMATION_ENUM *_animtionEnum;
};

class HydraCallbackHandler : public GameObjectAnimationCallbackHandler
{
public:
	bool Init(Hydra *pHydra) { _pHydra = pHydra; }
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
	Hydra *_pHydra{};
};

class Hydra : public Enemy
{
	friend class HydraStateMachine;
	friend class HydraCallbackHandler;
private:
public:
	Hydra();
	virtual ~Hydra();

	virtual bool CreateFromWorld(World &world);
	void Update(float deltaTime);

protected:
	void SetupCallbackAndCompression();

	HydraCallbackData _callbackData;

	HydraStateMachine *_pStateMachine;

	GameCommand _currentCommand;

	ActionComponent *_pActionComp{};

	void QueueAction(const Action &action);
public:
	bool findPlayer(Vector3 forward, Vector3 playerPos, Vector3 myPos, float range1, float range2, float findRadian);
};

#endif