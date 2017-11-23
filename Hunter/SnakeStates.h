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
	virtual void OnEnter() override;
	virtual void Update(float deltaTime, const GameCommand &command) override;
	virtual void OnExit() override;
};


#endif