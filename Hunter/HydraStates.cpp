#include "stdafx.h"
#include "HydraStates.h"


DEFINE_META(HydraIdleState)
{
	//ADD_MEMBER(_pActor);
}

DEFINE_META(HydraMoveState)
{
	//ADD_MEMBER(_pActor);
}

DEFINE_META(HydraStandState)
{
	//ADD_MEMBER(_pActor);
}

DEFINE_META(HydraAttackState)
{
	//ADD_MEMBER(_pActor);
}

DEFINE_META(HydraAttack2State)
{
	//ADD_MEMBER(_pActor);
}

DEFINE_META(HydraAttack3State)
{
	//ADD_MEMBER(_pActor);
}

bool HydraState::Init(StateMachine<Hydra>* pParent)
{
	_pParent = pParent;

	return true;
}


void HydraState::Release()
{
	_pParent = nullptr;
}
void HydraIdleState::OnEnter()
{
	_pParent->QueueAction(HYDRA_ANIM(HYDRA_IDLE));
}

void HydraIdleState::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void HydraIdleState::OnExit()
{
	_pParent->ClearActioniQueue();
}

void HydraMoveState::OnEnter()
{
	_pParent->QueueAction(HYDRA_ANIM(HYDRA_RUN));
}

void HydraMoveState::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void HydraMoveState::OnExit()
{
	_pParent->ClearActioniQueue();
}


void HydraStandState::OnEnter()
{
	_pParent->QueueAction(HYDRA_ANIM(HYDRA_STAND));
}

void HydraStandState::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void HydraStandState::OnExit()
{
	_pParent->ClearActioniQueue();
}



void HydraAttackState::OnEnter()
{
	_pParent->QueueAction(HYDRA_ANIM(HYDRA_BITE1));
}

void HydraAttackState::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void HydraAttackState::OnExit()
{
	_pParent->ClearActioniQueue();
}



void HydraAttack2State::OnEnter()
{
	_pParent->QueueAction(HYDRA_ANIM(HYDRA_BITE2));
}

void HydraAttack2State::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void HydraAttack2State::OnExit()
{
	_pParent->ClearActioniQueue();
}



void HydraAttack3State::OnEnter()
{
	_pParent->QueueAction(HYDRA_ANIM(HYDRA_BITE3));
}

void HydraAttack3State::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void HydraAttack3State::OnExit()
{
	_pParent->ClearActioniQueue();
}


