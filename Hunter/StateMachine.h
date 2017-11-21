#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "State.h"
#include "GameCommand.h"

template <typename T>
class StateMachine
{
public:
	StateMachine() {}
	virtual ~StateMachine() {}

	bool Init(T *actor)
	{
		_pActor = actor;

		return S_OK;
	}

	void Release()
	{
		_pActor = nullptr;
		SAFE_DELETE(_currentState);
		SAFE_DELETE(_prevState)
	}

	void Update(float deltaTime, const GameCommand &command)
	{
		State<T> *newState = _currentState->Update(deltaTime, command);
		if (nullptr != newState)
		{
			ChangeState(newState);
		}
	}

	void ChangeState(State<T> *state)
	{
		if (_currentState)
		{
			_currentState->OnExit();
			_currentState->Release();
			SAFE_DELETE(_currentState);
			_currentState = nullptr;
		}
		_currentState = state;
		_currentState->Init(_pActor);
		_currentState->OnEnter();
	}
private:
	State<T> *_currentState{};
	State<T> *_prevState{};

	T *_pActor{};
};
#endif