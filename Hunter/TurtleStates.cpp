#include "stdafx.h"
#include "TurtleStates.h"


DEFINE_META(TurtleIdleState)
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
	_pParent->QueueAction(TURTLE_ANIM(TURTLE_IDLE));
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