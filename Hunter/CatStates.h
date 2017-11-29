#include "State.h"
#include "StateMachine.h"

#include "Cat.h"

class CatState;
class CatStateMachine;

class CatStateMachine : public StateMachine<Cat>
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

class CatState : public State<Cat>
{
public:
	virtual bool Init(StateMachine<Cat> *pParent);
	virtual void Release();
	virtual void OnEnter() = 0;
	virtual void Update(float deltaTime, const GameCommand &command) = 0;
	virtual void OnExit() = 0;

protected:
};

class CatIdleState : public CatState
{
public:
	DECLARE_META(CatIdleState);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class CatMoveState : public CatState
{
public:
	DECLARE_META(CatMoveState);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};


class CatAttackState : public CatState
{
public:
	DECLARE_META(CatAttackState);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class CatAttack2State : public CatState
{
public:
	DECLARE_META(CatAttack2State);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class CatAttack3State : public CatState
{
public:
	DECLARE_META(CatAttack3State);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class CatAttack4State : public CatState
{
public:
	DECLARE_META(CatAttack4State);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class CatAttack5State : public CatState
{
public:
	DECLARE_META(CatAttack5State);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class CatStandState : public CatState
{
public:
	DECLARE_META(CatStandState);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class CatHurt1State : public CatState
{
public:
	DECLARE_META(CatHurt1State);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class CatHurt2State : public CatState
{
public:
	DECLARE_META(CatHurt2State);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class CatDeadState : public CatState
{
public:
	DECLARE_META(CatDeadState);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};