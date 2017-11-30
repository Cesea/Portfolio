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
	enum HYDRASTATE
	{
		HYDRASTATE_IDLE,
		HYDRASTATE_PATROL,
		HYDRASTATE_RUN,
		HYDRASTATE_FIND,
		HYDRASTATE_ATK1,
		HYDRASTATE_ATK2,
		HYDRASTATE_ATK3,
		HYDRASTATE_BREATH1,
		HYDRASTATE_BREATH2,
		HYDRASTATE_BREATH3,
		HYDRASTATE_STAND,
		HYDRASTATE_HURT,
		HYDRASTATE_DEATH
	};
	HYDRASTATE _state;
public:
	Hydra();
	virtual ~Hydra();

	virtual bool CreateFromWorld(World &world, Vector3 Pos);
	void Update(float deltaTime);
	void Handle(const CollisionSystem::ActorTriggerEvent & event);

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