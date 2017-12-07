#pragma once
#include "Enemy.h"
#include "DragonAnimationString.h"
class Dragon;
class DragonStateMachine;
struct DragonCallbackData
{
	DRAGON_ANIMATION_ENUM *_animtionEnum;
};

class DragonCallbackHandler : public GameObjectAnimationCallbackHandler
{
public:
	virtual bool Init(BaseGameObject *pDragon) { _pDragon = (Dragon *)pDragon; return true; }
	HRESULT CALLBACK HandleCallback(THIS_ UINT Track, LPVOID pCallbackData)
	{
		return S_OK;
	}

private:
	Dragon *_pDragon{};
};

class Dragon : public Enemy
{
	friend class DragonStateMachine;
	friend class DragonCallbackHandler;
public:
	Dragon();
	virtual ~Dragon();

	virtual bool CreateFromWorld(World &world, const Vector3 &Pos);
	void Update(float deltaTime);
	void Handle(const CollisionSystem::ActorTriggerEvent & event);

protected:
	void SetupCallbackAndCompression();

	HydraCallbackData _callbackData;

	GameCommand _currentCommand;

	ActionComponent *_pActionComp{};

	void QueueAction(Action &action, bool cancle = false);
public:
	bool findPlayer(Vector3 forward, Vector3 playerPos, Vector3 myPos, float range1, float range2, float findRadian);
};