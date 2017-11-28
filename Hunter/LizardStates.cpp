#include "stdafx.h"
#include "LizardStates.h"


DEFINE_META(LizardIdleState)
{
	//ADD_MEMBER(_pActor);
}

DEFINE_META(LizardMoveState)
{
	//ADD_MEMBER(_pActor);
}

DEFINE_META(LizardMove2State)
{
	//ADD_MEMBER(_pActor);
}

DEFINE_META(LizardAttackState)
{
	//ADD_MEMBER(_pActor);
}

DEFINE_META(LizardAttack2State)
{
	//ADD_MEMBER(_pActor);
}

DEFINE_META(LizardAttack3State)
{
	//ADD_MEMBER(_pActor);
}

DEFINE_META(LizardFindState)
{
	//ADD_MEMBER(_pActor);
}

DEFINE_META(LizardStandState)
{
	//ADD_MEMBER(_pActor);
}

bool LizardState::Init(StateMachine<Lizard>* pParent)
{
	_pParent = pParent;

	return true;
}


void LizardState::Release()
{
	_pParent = nullptr;
}
void LizardIdleState::OnEnter()
{
	_pParent->QueueAction(LIZARD_ANIM(LIZARD_IDLE));
}

void LizardIdleState::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void LizardIdleState::OnExit()
{
	_pParent->ClearActioniQueue();
}

void LizardMoveState::OnEnter()
{
	_pParent->QueueAction(LIZARD_ANIM(LIZARD_WALK));
}

void LizardMoveState::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void LizardMoveState::OnExit()
{
	_pParent->ClearActioniQueue();
}

void LizardMove2State::OnEnter()
{
	_pParent->QueueAction(LIZARD_ANIM(LIZARD_RUN));
}

void LizardMove2State::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void LizardMove2State::OnExit()
{
	_pParent->ClearActioniQueue();
}

void LizardAttackState::OnEnter()
{
	_pParent->QueueAction(LIZARD_ANIM(LIZARD_ATTACK));
}

void LizardAttackState::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void LizardAttackState::OnExit()
{
	_pParent->ClearActioniQueue();
}

void LizardAttack2State::OnEnter()
{
	_pParent->QueueAction(LIZARD_ANIM(LIZARD_BITE));
}

void LizardAttack2State::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void LizardAttack2State::OnExit()
{
	_pParent->ClearActioniQueue();
}

void LizardAttack3State::OnEnter()
{
	_pParent->QueueAction(LIZARD_ANIM(LIZARD_SPIT));
}

void LizardAttack3State::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void LizardAttack3State::OnExit()
{
	_pParent->ClearActioniQueue();
}

void LizardFindState::OnEnter()
{
	_pParent->QueueAction(LIZARD_ANIM(LIZARD_ROAR));
}

void LizardFindState::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void LizardFindState::OnExit()
{
	_pParent->ClearActioniQueue();
}

void LizardStandState::OnEnter()
{
	_pParent->QueueAction(LIZARD_ANIM(LIZARD_STAND));
}

void LizardStandState::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void LizardStandState::OnExit()
{
	_pParent->ClearActioniQueue();
}