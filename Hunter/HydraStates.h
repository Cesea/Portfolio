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