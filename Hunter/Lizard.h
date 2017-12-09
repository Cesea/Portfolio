#ifndef LIZARD_H
#define LIZARD_H

#include "Enemy.h"
#include "LizardAnimationString.h"

class Lizard;
class LizardStateMachine;

struct LizardCallbackData
{
	int32 *_animtionEnum;
	Vector3 *_pPosition{};
	int32 _description;
};

class LizardCallbackHandler : public GameObjectAnimationCallbackHandler
{
public:
	bool Init(BaseGameObject *pLizard) { _pLizard = (Lizard *)pLizard; return true; }
	HRESULT CALLBACK HandleCallback(THIS_ UINT Track, LPVOID pCallbackData);

private:
	Lizard *_pLizard{};
};

class Lizard : public Enemy
{
	friend class LizardStateMachine;
	friend class LizardCallbackHandler;
private:
	enum LIZARDSTATE
	{
		LIZARDSTATE_IDLE,
		LIZARDSTATE_PATROL,
		LIZARDSTATE_FIND,
		LIZARDSTATE_RUN,
		LIZARDSTATE_ATK1,
		LIZARDSTATE_ATK2,
		LIZARDSTATE_ATK3,
		LIZARDSTATE_HURT,
		LIZARDSTATE_DEATH,
		LIZARDSTATE_STAND
	};
	enum LIZARDSKINSTATE
	{
		LIZARDSKINSTATE_NORMAL,
		LIZARDSKINSTATE_BLACK
	};
	LIZARDSTATE _state;
	LIZARDSKINSTATE _skinType;
public:
	Lizard();
	virtual ~Lizard();

	virtual bool CreateFromWorld(World &world, const Vector3 &Pos);
	void Update(float deltaTime);
	void Handle(const CollisionSystem::ActorTriggerEvent & event);

protected:
	void SetupCallbackAndCompression();

	LizardCallbackData _callbackData[5];

	LizardStateMachine *_pStateMachine;

	GameCommand _currentCommand;

	ActionComponent *_pActionComp{};

	void QueueAction(Action &action, bool cancle = false);
public:
	bool findPlayer(Vector3 forward, Vector3 playerPos, Vector3 myPos, float range1, float range2, float findRadian);
};

#endif