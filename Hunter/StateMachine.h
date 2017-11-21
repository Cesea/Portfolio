#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "State.h"
#include "GameCommand.h"

#include <map>


template <typename T>
class StateMachine
{
public:
	typedef std::map<std::string, State<T> *> StateTable;

	StateMachine() {}
	virtual ~StateMachine() {}

	bool Init(T *actor)
	{
		_pActor = actor;

		return S_OK;
	}

	bool RegisterState(const std::string &name, State<T> *pNewState)
	{
		auto found = _stateTable.find(name);
		if (found == _stateTable.end())
		{
			_stateTable.insert(std::make_pair(name, pNewState));
			return true;
		}
		return false;
	}

	void Release()
	{
		_pActor = nullptr;
		_currentState = nullptr;
		_prevState = nullptr;
		for (auto &state : _stateTable)
		{
			SAFE_DELETE(state.second);
		}
		_stateTable.clear();
	}

	void Update(float deltaTime, const GameCommand &command)
	{
		_currentState->Update(deltaTime, command);
	}

	void ChangeState(const std::string &newStateName)
	{
		if (_currentState)
		{
			_currentState->OnExit();
			_currentState->Release();
			_prevState = _currentState;
		}
		auto &found = _stateTable.find(newStateName);
		if (found != _stateTable.end())
		{
			_currentState = found->second;
			_currentState->Init(_pActor);
			_currentState->OnEnter();
		}

	}

	const StateTable &GetStateTable() { return _stateTable; }

protected :
	StateTable _stateTable;

	State<T> *_currentState{};
	State<T> *_prevState{};

	T *_pActor{};
};
#endif