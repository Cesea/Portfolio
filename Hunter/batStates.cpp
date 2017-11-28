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

DEFINE_META(BatAttackState)
{
	//ADD_MEMBER(_pActor);
}

DEFINE_META(BatAttack2State)
{
	//ADD_MEMBER(_pActor);
}

DEFINE_META(BatAttack3State)
{
	//ADD_MEMBER(_pActor);
}
DEFINE_META(BatFindState)
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

void BatAttackState::OnEnter()
{
	_pParent->QueueAction(BAT_ANIM(BAT_ATTACK1));
}

void BatAttackState::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void BatAttackState::OnExit()
{
	_pParent->ClearActioniQueue();
}

void BatAttack2State::OnEnter()
{
	_pParent->QueueAction(BAT_ANIM(BAT_ATTACK2));
}

void BatAttack2State::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void BatAttack2State::OnExit()
{
	_pParent->ClearActioniQueue();
}

void BatAttack3State::OnEnter()
{
	_pParent->QueueAction(BAT_ANIM(BAT_ATTACK3));
}

void BatAttack3State::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void BatAttack3State::OnExit()
{
	_pParent->ClearActioniQueue();
}

void BatFindState::OnEnter()
{
	_pParent->QueueAction(BAT_ANIM(BAT_ROAR));
}

void BatFindState::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void BatFindState::OnExit()
{
	_pParent->ClearActioniQueue();
}