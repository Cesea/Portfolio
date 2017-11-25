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
public:
	Turtle();
	virtual ~Turtle();

	virtual bool CreateFromWorld(World &world);
	void Update(float deltaTime);

protected:
	void SetupCallbackAndCompression();

	TurtleCallbackData _callbackData;

	TurtleStateMachine *_pStateMachine;

	GameCommand _currentCommand;

	ActionComponent *_pActionComp{};

	void QueueAction(const Action &action);
};
