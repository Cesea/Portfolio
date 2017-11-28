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

