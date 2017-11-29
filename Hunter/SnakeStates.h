#ifndef SNAKE_STATES_H
#define SNAKE_STATES_H

#include "State.h"
#include "StateMachine.h"

#include "Snake.h"

class SnakeState;
class SnakeStateMachine;

class SnakeStateMachine : public StateMachine<Snake>
{
	friend class PlayerState;
public :
	virtual void QueueAction(const Action &action)
	{
		_pActor->_pActionComp->_actionQueue.PushAction(action);
	}

	virtual void ClearActioniQueue()
	{
		_pActor->_pActionComp->_actionQueue.ClearQueue();
	}

protected :
};

class SnakeState : public State<Snake>
{
public :
	virtual bool Init(StateMachine<Snake> *pParent);
	virtual void Release();
	virtual void OnEnter() = 0;
	virtual void Update(float deltaTime, const GameCommand &command) = 0;
	virtual void OnExit() = 0;

protected : 
};

class SnakeIdleState : public SnakeState
{
public :
	DECLARE_META(SnakeIdleState);


	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class SnakeMoveState : public SnakeState
{

public:
	DECLARE_META(SnakeMoveState);


	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class SnakeFindState : public SnakeState
{

public:
	DECLARE_META(SnakeFindState);


	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class SnakeAttackState : public SnakeState
{

public:
	DECLARE_META(SnakeAttackState);


	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};
class SnakeAttack2State : public SnakeState
{

public:
	DECLARE_META(SnakeAttack2State);


	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};
class SnakeStandState : public SnakeState
{

public:
	DECLARE_META(SnakeStandState);


	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class SnakeAttack3State : public SnakeState
{

public:
	DECLARE_META(SnakeAttack3State);


	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class SnakeDeadState : public SnakeState
{

public:
	DECLARE_META(SnakeDeadState);


	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

class SnakeHurtState : public SnakeState
{

public:
	DECLARE_META(SnakeHurtState);


	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};

#endif