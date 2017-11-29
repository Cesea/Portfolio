#include "State.h"
#include "StateMachine.h"

#include "bat.h"

class BatState;
class BatStateMachine;

class BatStateMachine : public StateMachine<Bat>
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

class BatState : public State<Bat>
{
public:
	virtual bool Init(StateMachine<Bat> *pParent);
	virtual void Release();
	virtual void OnEnter() = 0;
	virtual void Update(float deltaTime, const GameCommand &command) = 0;
	virtual void OnExit() = 0;

protected:
};

class BatIdleState : public BatState
{
public:
	DECLARE_META(BatIdleState);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class BatMoveState : public BatState
{
public:
	DECLARE_META(BatMoveState);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class BatAttackState : public BatState
{
public:
	DECLARE_META(BatAttackState);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class BatAttack2State : public BatState
{
public:
	DECLARE_META(BatAttack2State);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class BatAttack3State : public BatState
{
public:
	DECLARE_META(BatAttack3State);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class BatFindState : public BatState
{
public:
	DECLARE_META(BatFindState);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class BatHurt1State : public BatState
{
public:
	DECLARE_META(BatHurt1State);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class BatHurt2State : public BatState
{
public:
	DECLARE_META(BatHurt2State);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class BatDeadState : public BatState
{
public:
	DECLARE_META(BatDeadState);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};