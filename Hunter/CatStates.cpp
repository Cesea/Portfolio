#include "stdafx.h"
#include "CatStates.h"


DEFINE_META(CatIdleState)
{
	//ADD_MEMBER(_pActor);
}

DEFINE_META(CatMoveState)
{
	//ADD_MEMBER(_pActor);
}

DEFINE_META(CatAttackState)
{
	//ADD_MEMBER(_pActor);
}

DEFINE_META(CatAttack2State)
{
	//ADD_MEMBER(_pActor);
}

DEFINE_META(CatAttack3State)
{
	//ADD_MEMBER(_pActor);
}

DEFINE_META(CatAttack4State)
{
	//ADD_MEMBER(_pActor);
}

DEFINE_META(CatAttack5State)
{
	//ADD_MEMBER(_pActor);
}

DEFINE_META(CatStandState)
{
	//ADD_MEMBER(_pActor);
}

DEFINE_META(CatHurt1State)
{
	//ADD_MEMBER(_pActor);
}

DEFINE_META(CatHurt2State)
{
	//ADD_MEMBER(_pActor);
}

DEFINE_META(CatDeadState)
{
	//ADD_MEMBER(_pActor);
}

bool CatState::Init(StateMachine<Cat>* pParent)
{
	_pParent = pParent;

	return true;
}


void CatState::Release()
{
	_pParent = nullptr;
}
void CatIdleState::OnEnter()
{
	_pParent->QueueAction(CAT_ANIM(CAT_IDLE));
}

void CatIdleState::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void CatIdleState::OnExit()
{
	_pParent->ClearActioniQueue();
}

void CatMoveState::OnEnter()
{
	_pParent->QueueAction(CAT_ANIM(CAT_RUN));
}

void CatMoveState::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void CatMoveState::OnExit()
{
	_pParent->ClearActioniQueue();
}


void CatAttackState::OnEnter()
{
	_pParent->QueueAction(CAT_ANIM(CAT_ATTACK1));
}

void CatAttackState::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void CatAttackState::OnExit()
{
	_pParent->ClearActioniQueue();
}

void CatAttack2State::OnEnter()
{
	_pParent->QueueAction(CAT_ANIM(CAT_ATTACK2));
}

void CatAttack2State::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void CatAttack2State::OnExit()
{
	_pParent->ClearActioniQueue();
}

void CatAttack3State::OnEnter()
{
	_pParent->QueueAction(CAT_ANIM(CAT_ATTACK3));
}

void CatAttack3State::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void CatAttack3State::OnExit()
{
	_pParent->ClearActioniQueue();
}

void CatAttack4State::OnEnter()
{
	_pParent->QueueAction(CAT_ANIM(CAT_ATTACK4));
}

void CatAttack4State::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void CatAttack4State::OnExit()
{
	_pParent->ClearActioniQueue();
}

void CatAttack5State::OnEnter()
{
	_pParent->QueueAction(CAT_ANIM(CAT_ATTACK5));
}

void CatAttack5State::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void CatAttack5State::OnExit()
{
	_pParent->ClearActioniQueue();
}


void CatStandState::OnEnter()
{
	_pParent->QueueAction(CAT_ANIM(CAT_STAND));
}

void CatStandState::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void CatStandState::OnExit()
{
	_pParent->ClearActioniQueue();
}


void CatHurt1State::OnEnter()
{
	_pParent->QueueAction(CAT_ANIM(CAT_HIT1));
}

void CatHurt1State::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void CatHurt1State::OnExit()
{
	_pParent->ClearActioniQueue();
}


void CatHurt2State::OnEnter()
{
	_pParent->QueueAction(CAT_ANIM(CAT_HIT2));
}

void CatHurt2State::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void CatHurt2State::OnExit()
{
	_pParent->ClearActioniQueue();
}



void CatDeadState::OnEnter()
{
	_pParent->QueueAction(CAT_ANIM(CAT_DEATH));
}

void CatDeadState::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void CatDeadState::OnExit()
{
	_pParent->ClearActioniQueue();
}


