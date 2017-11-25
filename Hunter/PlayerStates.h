#ifndef PLAYER_STATE_H
#define PLAYER_STATE_H

#include "State.h"
#include "StateMachine.h"

#include "Player.h"

class PlayerState;
class PlayerStateMachine;


class PlayerStateMachine : public StateMachine<Player>
{
	friend class PlayerState;
	friend class PlayerAttackState;
	friend class PlayerCombatState;
	friend class PlayerStanceState;
	friend class PlayerMoveState;
	friend class PlayerDeadState;
public :
	virtual void QueueAction(const Action &action)
	{
		_pActor->_pActionComp->_actionQueue.PushAction(action);
	}

	virtual void ClearActioniQueue()
	{
		_pActor->_pActionComp->_actionQueue.ClearQueue();
	}

protected :
};

class PlayerState : public State<Player>
{
public :
	virtual bool Init(StateMachine<Player> *pParent);
	virtual void Release();
	virtual void OnEnter() = 0;
	virtual void Update(float deltaTime, const GameCommand &command) = 0;
	virtual void OnExit() = 0;

protected : 
	EventChannel _channel;
	GameCommand _lastCommand;
};

class PlayerCombatState : public PlayerState
{
public :
	DECLARE_META(PlayerCombatState);

	void OnEnter() override;
	void Update(float deltaTime, const GameCommand &command) override;
	void OnExit() override;

	//void Handle(const Player::AttackEvent &event);
	//void Handle(const CombatEndEvent &event);
	//void Handle(const Player::MoveEvent &event);
	//void Handle(const DamageEvent &event);

private :
	StopWatch _comboTimer;
	int32 _curretComboCount{};
	void Attack();


};

class PlayerStanceState : public PlayerState
{
public:
	DECLARE_META(PlayerStanceState);

	void OnEnter() override;
	void Update(float deltaTime, const GameCommand &command) override;
	void OnExit() override;

	//void Handle(const Player::AttackEvent &event);
	//void Handle(const Player::MoveEvent &event);
	//void Handle(const CombatBeginEvent &event);
	//void Handle(const Player::InteractEvent &event);

protected :
	//StopWatch _randomTimer;
};

class PlayerMoveState : public PlayerState
{
public :
	DECLARE_META(PlayerMoveState);

	void OnEnter() override;
	void Update(float deltaTime, const GameCommand &command) override;
	void OnExit() override;

protected :
	StopWatch _toStanceTimer;
};

class PlayerDeadState : public PlayerState
{
public :
	DECLARE_META(PlayerDeadState);

	void OnEnter() override;
	void Update(float deltaTime, const GameCommand &command) override;
	void OnExit() override;

};

//class PlayerNormalState : public PlayerState
//{
//public :
//	void OnEnter() override;
//	PlayerState *Update(float deltaTime, const GameCommand &command) override;
//	void OnExit() override;
//
//protected :
//	StopWatch _idleRandomTimer;
//
//	bool32 _sitting{ false };
//	bool32 _walking{ false };
//	bool32 _running{ false };
//};


#endif