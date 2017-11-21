#ifndef PLAYER_STATE_H
#define PLAYER_STATE_H


#include "State.h"
#include "StateMachine.h"

class Player;
class PlayerState;
class PlayerStateMachine;

class PlayerStateMachine : public StateMachine<Player>
{
public :


protected :

};

class PlayerState : public State<Player>
{
public :
	virtual bool Init(Player *pPlayer);
	virtual void Release();
	virtual void OnEnter() = 0;
	virtual PlayerState *Update(float deltaTime, const GameCommand &command) = 0;
	virtual void OnExit() = 0;
};

class PlayerNormalState : public PlayerState
{
public :
	void OnEnter() override;
	PlayerState *Update(float deltaTime, const GameCommand &command) override;
	void OnExit() override;

protected :
	StopWatch _idleRandomTimer;

};

class PlayerWarState : public PlayerState
{
public :
	void OnEnter() override;
	PlayerState *Update(float deltaTime, const GameCommand &command) override;
	void OnExit() override;

protected :

};

class PlayerDeadState : public PlayerState
{
public :
	void OnEnter() override;
	PlayerState *Update(float deltaTime, const GameCommand &command) override;
	void OnExit() override;

protected :

};

#endif