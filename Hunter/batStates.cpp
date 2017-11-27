#include "stdafx.h"
#include "BatStates.h"


DEFINE_META(BatIdleState)
{
	//ADD_MEMBER(_pActor);
}

DEFINE_META(BatMoveState)
{
	//ADD_MEMBER(_pActor);
}

bool BatState::Init(StateMachine<Bat>* pParent)
{
	_pParent = pParent;

	return true;
}


void BatState::Release()
{
	_pParent = nullptr;
}
void BatIdleState::OnEnter()
{
	_pParent->QueueAction(BAT_ANIM(BAT_IDLE));
}

void BatIdleState::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void BatIdleState::OnExit()
{
	_pParent->ClearActioniQueue();
}

void BatMoveState::OnEnter()
{
	_pParent->QueueAction(BAT_ANIM(BAT_FORWARD));
}

void BatMoveState::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void BatMoveState::OnExit()
{
	_pParent->ClearActioniQueue();
}