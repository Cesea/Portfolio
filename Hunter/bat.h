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
	virtual bool Init(BaseGameObject *pBat) { _pBat = (Bat *)pBat; return true; }
	HRESULT CALLBACK HandleCallback(THIS_ UINT Track, LPVOID pCallbackData)
	{
		return S_OK;
	}

private:
	Bat *_pBat{};
};

class Bat : public Enemy
{
	friend class BatStateMachine;
	friend class BatCallbackHandler;
private:
	enum BATSTATE
	{
		BATSTATE_IDLE,
		BATSTATE_PATROL,
		BATSTATE_FIND,
		BATSTATE_RUN,
		BATSTATE_ATK1,
		BATSTATE_ATK2,
		BATSTATE_ATK3,
		BATSTATE_DEATH,
		BATSTATE_HURT
	};
	enum BATSKINSTATE
	{
		BATSKINSTATE_RED,
		BATSKINSTATE_BLACK,
		BATSKINSTATE_GOLD
	};
	BATSTATE _state;
	BATSKINSTATE _skinType;
public:
	Bat();
	virtual ~Bat();

	virtual bool CreateFromWorld(World &world, const Vector3 &Pos);
	void Update(float deltaTime);
	void Handle(const CollisionSystem::ActorTriggerEvent & event);

protected:
	void SetupCallbackAndCompression();

	BatCallbackData _callbackData;

	GameCommand _currentCommand;

	ActionComponent *_pActionComp{};

	void QueueAction(Action &action, bool cancle = false);
public:
	bool findPlayer(Vector3 forward, Vector3 playerPos, Vector3 myPos, float range1, float range2, float findRadian);
};

#endif