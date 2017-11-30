#include "stdafx.h"
#include "TurtleStates.h"


DEFINE_META(TurtleIdleState)
{
	//ADD_MEMBER(_pActor);
}
DEFINE_META(TurtleMoveState)
{
	//ADD_MEMBER(_pActor);
}
DEFINE_META(TurtleBite1State)
{
	//ADD_MEMBER(_pActor);
}
DEFINE_META(TurtleBite2State)
{
	//ADD_MEMBER(_pActor);
}
DEFINE_META(TurtleFindState)
{
	//ADD_MEMBER(_pActor);
}
DEFINE_META(TurtleHurt1State)
{
	//ADD_MEMBER(_pActor);
}
DEFINE_META(TurtleHurt2State)
{
	//ADD_MEMBER(_pActor);
}
DEFINE_META(TurtleDeadState)
{
	//ADD_MEMBER(_pActor);
}
bool TurtleState::Init(StateMachine<Turtle>* pParent)
{
	_pParent = pParent;

	return true;
}


void TurtleState::Release()
{
	_pParent = nullptr;
}
void TurtleIdleState::OnEnter()
{
	_pParent->QueueAction(TURTLE_ANIM(TURTLE_STAND));
}

void TurtleIdleState::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void TurtleIdleState::OnExit()
{
	_pParent->ClearActioniQueue();
}

void TurtleMoveState::OnEnter()
{
	_pParent->QueueAction(TURTLE_ANIM(TURTLE_WALK));
}

void TurtleMoveState::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void TurtleMoveState::OnExit()
{
	_pParent->ClearActioniQueue();
}

void TurtleBite1State::OnEnter()
{
	_pParent->QueueAction(TURTLE_ANIM(TURTLE_BITE1));
}

void TurtleBite1State::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void TurtleBite1State::OnExit()
{
	_pParent->ClearActioniQueue();
}

void TurtleBite2State::OnEnter()
{
	_pParent->QueueAction(TURTLE_ANIM(TURTLE_BITE2));
}

void TurtleBite2State::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void TurtleBite2State::OnExit()
{
	_pParent->ClearActioniQueue();
}


void TurtleFindState::OnEnter()
{
	_pParent->QueueAction(TURTLE_ANIM(TURTLE_ROAR));
}

void TurtleFindState::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void TurtleFindState::OnExit()
{
	_pParent->ClearActioniQueue();
}

void TurtleHurt1State::OnEnter()
{
	_pParent->QueueAction(TURTLE_ANIM(TURTLE_HIT1));
}

void TurtleHurt1State::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void TurtleHurt1State::OnExit()
{
	_pParent->ClearActioniQueue();
}


void TurtleHurt2State::OnEnter()
{
	_pParent->QueueAction(TURTLE_ANIM(TURTLE_HIT2));
}

void TurtleHurt2State::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void TurtleHurt2State::OnExit()
{
	_pParent->ClearActioniQueue();
}


void TurtleDeadState::OnEnter()
{
	_pParent->QueueAction(TURTLE_ANIM(TURTLE_DEATH));
}

void TurtleDeadState::Update(float deltaTime, const GameCommand & command)
{
	switch (command._type)
	{
	default:
		break;
	}
}

void TurtleDeadState::OnExit()
{
	_pParent->ClearActioniQueue();
}