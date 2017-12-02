#include "State.h"
#include "StateMachine.h"

#include "Lizard.h"

class LizardState;
class LizardStateMachine;

class LizardStateMachine : public StateMachine<Lizard>
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

class LizardState : public State<Lizard>
{
public:
	virtual bool Init(StateMachine<Lizard> *pParent);
	virtual void Release();
	virtual void OnEnter() = 0;
	virtual void Update(float deltaTime, const GameCommand &command) = 0;
	virtual void OnExit() = 0;

protected:
};

class LizardIdleState : public LizardState
{
public:
	DECLARE_META(LizardIdleState);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class LizardMoveState : public LizardState
{
public:
	DECLARE_META(LizardMoveState);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class LizardMove2State : public LizardState
{
public:
	DECLARE_META(LizardMove2State);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class LizardAttackState : public LizardState
{
public:
	DECLARE_META(LizardAttackState);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class LizardAttack2State : public LizardState
{
public:
	DECLARE_META(LizardAttack2State);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class LizardAttack3State : public LizardState
{
public:
	DECLARE_META(LizardAttack3State);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class LizardFindState : public LizardState
{
public:
	DECLARE_META(LizardFindState);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class LizardStandState : public LizardState
{
public:
	DECLARE_META(LizardStandState);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class LizardHurt1State : public LizardState
{
public:
	DECLARE_META(LizardHurt1State);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class LizardHurt2State : public LizardState
{
public:
	DECLARE_META(LizardHurt2State);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class LizardDeadState : public LizardState
{
public:
	DECLARE_META(LizardDeadState);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class LizardSpitState : public LizardState
{
public:
	DECLARE_META(LizardSpitState);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};