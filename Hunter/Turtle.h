#pragma once
#include "Enemy.h"
#include "TurtleAnimationString.h"

class Turtle;
class TurtleStateMachine;

struct TurtleCallbackData
{
	TURTLE_ANIMATION_ENUM *_animtionEnum;
};

class TurtleCallbackHandler : public GameObjectAnimationCallbackHandler
{
public:
	bool Init(Turtle *pTurtle) { _pTurtle = pTurtle; }
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
	Turtle *_pTurtle{};
};

class Turtle :
	public Enemy
{
	friend class TurtleStateMachine;
	friend class TurtleCallbackHandler;
private:
	enum TURTLESTATE
	{
		TURTLESTATE_ATK1,
		TURTLESTATE_ATK2,
		TURTLESTATE_BREATH_FIRE,
		TURTLESTATE_DEATH,
		TURTLESTATE_EARTHQUAKE,
		TURTLESTATE_HURT,
		TURTLESTATE_ROAR,
		TURTLESTATE_SPIT,
		TURTLESTATE_STAND,
		TURTLESTATE_WALK,
		TURTLESTATE_FIND,
		TURTLESTATE_TRACE
	};
	TURTLESTATE _state;
public:
	Turtle();
	virtual ~Turtle();

	virtual bool CreateFromWorld(World &world, Vector3 Pos);
	void Update(float deltaTime);
	void Handle(const CollisionSystem::ActorTriggerEvent & event);

protected:
	void SetupCallbackAndCompression();

	TurtleCallbackData _callbackData;

	TurtleStateMachine *_pStateMachine;

	GameCommand _currentCommand;

	ActionComponent *_pActionComp{};

	void QueueAction(const Action &action);

	bool Turtle::findPlayer(Vector3 forward, Vector3 playerPos, Vector3 myPos, float range1, float range2, float findRadian);
};

