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

protected : 
	GameCommand _lastCommand;
};

class PlayerNormalState : public PlayerState
{
public :
	void OnEnter() override;
	PlayerState *Update(float deltaTime, const GameCommand &command) override;
	void OnExit() override;

protected :
	StopWatch _idleRandomTimer;

	bool32 _sitting{ false };
	bool32 _walking{ false };
	bool32 _running{ false };

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