#include "State.h"
#include "StateMachine.h"

#include "Hydra.h"

class HydraState;
class HydraStateMachine;

class HydraStateMachine : public StateMachine<Hydra>
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

class HydraState : public State<Hydra>
{
public:
	virtual bool Init(StateMachine<Hydra> *pParent);
	virtual void Release();
	virtual void OnEnter() = 0;
	virtual void Update(float deltaTime, const GameCommand &command) = 0;
	virtual void OnExit() = 0;

protected:
};

class HydraIdleState : public HydraState
{
public:
	DECLARE_META(HydraIdleState);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class HydraMoveState : public HydraState
{
public:
	DECLARE_META(HydraMoveState);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class HydraStandState : public HydraState
{
public:
	DECLARE_META(HydraStandState);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class HydraAttackState : public HydraState
{
public:
	DECLARE_META(HydraAttackState);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class HydraAttack2State : public HydraState
{
public:
	DECLARE_META(HydraAttack2State);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class HydraAttack3State : public HydraState
{
public:
	DECLARE_META(HydraAttack3State);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class HydraHurt1State : public HydraState
{
public:
	DECLARE_META(HydraHurt1State);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class HydraHurt2State : public HydraState
{
public:
	DECLARE_META(HydraHurt2State);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class HydraDeadState : public HydraState
{
public:
	DECLARE_META(HydraDeadState);

	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};