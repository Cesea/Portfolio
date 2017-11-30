#include "State.h"
#include "StateMachine.h"

#include "Turtle.h"

class TurtleState;
class TurtleStateMachine;

class TurtleStateMachine : public StateMachine<Turtle>
{
	friend class PlayerState;
public:
	virtual void QueueAction(const Action &action)
	{
		_pActor->_pActionComp->_actionQueue.PushAction(action);
	}

	virtual void ClearActioniQueue()
	{
		_pActor->_pActionComp->_actionQueue.ClearQueue();
	}

protected:
};

class TurtleState : public State<Turtle>
{
public:
	virtual bool Init(StateMachine<Turtle> *pParent);
	virtual void Release();
	virtual void OnEnter() = 0;
	virtual void Update(float deltaTime, const GameCommand &command) = 0;
	virtual void OnExit() = 0;

protected:
};

class TurtleIdleState : public TurtleState
{
public:
	DECLARE_META(TurtleIdleState);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class TurtleMoveState : public TurtleState
{
public:
	DECLARE_META(TurtleMoveState);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class TurtleBite1State : public TurtleState
{
public:
	DECLARE_META(TurtleBite1State);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};
class TurtleBite2State : public TurtleState
{
public:
	DECLARE_META(TurtleBite2State);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};
class TurtleFindState : public TurtleState
{
public:
	DECLARE_META(TurtleFindState);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class TurtleHurt1State : public TurtleState
{
public:
	DECLARE_META(TurtleHurt1State);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class TurtleHurt2State : public TurtleState
{
public:
	DECLARE_META(TurtleHurt2State);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class TurtleDeadState : public TurtleState
{
public:
	DECLARE_META(TurtleDeadState);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

