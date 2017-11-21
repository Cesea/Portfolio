#ifndef STATE_H
#define STATE_H

#include "GameCommand.h"

template <typename T>
class State
{
public:
	virtual bool Init(T *pActor) = 0;
	virtual void Release() = 0;
	virtual void OnEnter() = 0;
	virtual void Update(float deltaTime, const GameCommand &command) = 0;
	virtual void OnExit() = 0;
protected :
	T *_pActor{};
};

#endif