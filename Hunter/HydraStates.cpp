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

DEFINE_META(HydraHurt1State)
{
	//ADD_MEMBER(_pActor);
}

DEFINE_META(HydraHurt2State)
{
	//ADD_MEMBER(_pActor);
}

DEFINE_META(HydraDeadState)
{
	//ADD_MEMBER(_pActor);
}

DEFINE_META(HydraSpecialAttack1State)
{
	//ADD_MEMBER(_pActor);
}

DEFINE_META(HydraSpecialAttack2State)
{
	//ADD_MEMBER(_pActor);
}

DEFINE_META(HydraBreath1State)
{
	//ADD_MEMBER(_pActor);
}

DEFINE_META(HydraBreath2State)
{
	//ADD_MEMBER(_pActor);
}

DEFINE_META(HydraBreath3State)
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

void HydraHurt1State::OnEnter()
{
	_pParent->QueueAction(HYDRA_ANIM(HYDRA_HIT1));
}

void HydraHurt1State::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void HydraHurt1State::OnExit()
{
	_pParent->ClearActioniQueue();
}


void HydraHurt2State::OnEnter()
{
	_pParent->QueueAction(HYDRA_ANIM(HYDRA_HIT2));
}

void HydraHurt2State::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void HydraHurt2State::OnExit()
{
	_pParent->ClearActioniQueue();
}


void HydraDeadState::OnEnter()
{
	_pParent->QueueAction(HYDRA_ANIM(HYDRA_DEATH));
}

void HydraDeadState::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void HydraDeadState::OnExit()
{
	_pParent->ClearActioniQueue();
}


void HydraSpecialAttack1State::OnEnter()
{
	_pParent->QueueAction(HYDRA_ANIM(HYDRA_WHIP_HEAD));
}

void HydraSpecialAttack1State::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void HydraSpecialAttack1State::OnExit()
{
	_pParent->ClearActioniQueue();
}

void HydraSpecialAttack2State::OnEnter()
{
	_pParent->QueueAction(HYDRA_ANIM(HYDRA_WHIP_TAIL));
}

void HydraSpecialAttack2State::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void HydraSpecialAttack2State::OnExit()
{
	_pParent->ClearActioniQueue();
}


void HydraBreath1State::OnEnter()
{
	_pParent->QueueAction(HYDRA_ANIM(HYDRA_BREATH_FIRE1));
}

void HydraBreath1State::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void HydraBreath2State::OnExit()
{
	_pParent->ClearActioniQueue();
}



void HydraBreath2State::OnEnter()
{
	_pParent->QueueAction(HYDRA_ANIM(HYDRA_BREATH_FIRE2));
}

void HydraBreath2State::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void HydraBreath3State::OnExit()
{
	_pParent->ClearActioniQueue();
}




void HydraBreath3State::OnEnter()
{
	_pParent->QueueAction(HYDRA_ANIM(HYDRA_BREATH_FIRE3));
}

void HydraBreath3State::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void HydraBreath1State::OnExit()
{
	_pParent->ClearActioniQueue();
}






