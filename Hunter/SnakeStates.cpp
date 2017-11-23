#include "stdafx.h"
#include "SnakeStates.h"

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
