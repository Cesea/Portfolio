#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#include "State.h"

template <typename T>
class StateManagerFoo
{
public:
	StateManager() {}
	virtual ~StateManager() {}

	bool Init(T *actor, State<T> *firstState)
	{
		_pActor = actor;

		_currentState = firstState;
		return S_OK;
	}

	void Release()
	{
		_pActor = nullptr;
		SAFE_DELETE(_currentState);
		SAFE_DELETE(_prevState)
	}

	void Update(float deltaTime, int32 command)
	{
		State<T> *newState = _currentState->Update(_pActor, deltaTime, command);
		if (nullptr != newState)
		{
			ChangeState(newState);
		}
	}

	void ChangeState(State<T> *state)
	{
		if (_currentState)
		{
			_currentState->OnExit(_pActor);
			SAFE_DELETE(_currentState);
			_currentState = nullptr;
		}
		_currentState = state;
		_currentState->OnEnter(_pActor);
	}
private:
	State<T> *_currentState{};
	State<T> *_prevState{};

	T *_pActor{};
};
#endif