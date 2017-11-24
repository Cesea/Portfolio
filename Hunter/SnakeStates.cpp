#include "stdafx.h"
#include "SnakeStates.h"

DEFINE_META(SnakeIdleState)
{
	//ADD_MEMBER(_pActor);
}

DEFINE_META(SnakeMoveState)
{
	//ADD_MEMBER(_pActor);
}
DEFINE_META(SnakeFindState)
{
	//ADD_MEMBER(_pActor);
}
DEFINE_META(SnakeAttackState)
{
	//ADD_MEMBER(_pActor);
}
DEFINE_META(SnakeAttack2State)
{
	//ADD_MEMBER(_pActor);
}
DEFINE_META(SnakeAttack3State)
{
	//ADD_MEMBER(_pActor);
}
DEFINE_META(SnakeStandState)
{
	//ADD_MEMBER(_pActor);
}
DEFINE_META(SnakeDeadState)
{
	//ADD_MEMBER(_pActor);
}


bool SnakeState::Init(StateMachine<Snake>* pParent)
{
	_pParent = pParent;

	return true;
}

void SnakeState::Release()
{
	_pParent = nullptr;
}

void SnakeIdleState::OnEnter()
{
	_pParent->QueueAction(SNAKE_ANIM(SNAKE_IDLE));
}

void SnakeIdleState::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void SnakeIdleState::OnExit()
{
	_pParent->ClearActioniQueue();
}

void SnakeMoveState::OnEnter()
{
	_pParent->QueueAction(SNAKE_ANIM(SNAKE_MOVE));
}

void SnakeMoveState::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void SnakeFindState::OnExit()
{
	_pParent->ClearActioniQueue();
}

void SnakeFindState::OnEnter()
{
	_pParent->QueueAction(SNAKE_ANIM(SNAKE_ROAR));
}

void SnakeFindState::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void SnakeMoveState::OnExit()
{
	_pParent->ClearActioniQueue();
}

void SnakeAttackState::OnEnter()
{
	_pParent->QueueAction(SNAKE_ANIM(SNAKE_ATTACK1));
}

void SnakeAttackState::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void SnakeAttackState::OnExit()
{
	_pParent->ClearActioniQueue();
}

void SnakeAttack2State::OnEnter()
{
	_pParent->QueueAction(SNAKE_ANIM(SNAKE_ATTACK2));
}

void SnakeAttack2State::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void SnakeAttack2State::OnExit()
{
	_pParent->ClearActioniQueue();
}

void SnakeAttack3State::OnEnter()
{
	_pParent->QueueAction(SNAKE_ANIM(SNAKE_ATTACK3));
}

void SnakeAttack3State::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void SnakeAttack3State::OnExit()
{
	_pParent->ClearActioniQueue();
}

void SnakeStandState::OnEnter()
{
	_pParent->QueueAction(SNAKE_ANIM(SNAKE_STAND));
}

void SnakeStandState::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void SnakeStandState::OnExit()
{
	_pParent->ClearActioniQueue();
}

void SnakeDeadState::OnEnter()
{
	_pParent->QueueAction(SNAKE_ANIM(SNAKE_DEATH));
}

void SnakeDeadState::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void SnakeDeadState::OnExit()
{
	_pParent->ClearActioniQueue();
}

