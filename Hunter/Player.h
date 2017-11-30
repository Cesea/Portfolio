#ifndef PLAYER_H
#define PLAYER_H

#include "BaseGameObject.h"
#include "PlayerAnimationString.h"

class Player;
class PlayerStateMachine;

struct PlayerCallbackData
{
	PlayerAnimationEnum *_animtionEnum;
};

class PlayerCallbackHandler : public GameObjectAnimationCallbackHandler
{
public :
	bool Init(Player *pPlayer) { _pPlayer = pPlayer; }
	HRESULT CALLBACK HandleCallback(THIS_ UINT Track, LPVOID pCallbackData);
private :
	Player *_pPlayer{};
};

class Player : public BaseGameObject
{
	friend class PlayerStateMachine;
	friend class PlayerCallbackHandler;
public :
	friend class PlayerStanceState;
public :
	Player();
	virtual ~Player();

	virtual bool CreateFromWorld(World &world, Vector3 Pos);
	void Update(float deltaTime);

	void Handle(const InputManager::KeyPressedEvent &event);
	void Handle(const InputManager::MousePressedEvent &event);
	void Handle(const InputManager::KeyDownEvent &event);

public:
	enum PLAYERSTATE
	{
		PLAYERSTATE_STANCE,
		PLAYERSTATE_MOVE,
		PLAYER_STATE_RUN,
		PLAYERSTATE_ATTACK,
		PLAYERSTATE_BLOCK,
		PLAYERSTATE_MOVEATTACK,
	};
private :
	PLAYERSTATE _state;
	bool32 _inCombat{};


	void SetupCallbackAndCompression();
	PlayerCallbackData _callbackData;
	PlayerStateMachine *_pStateMachine;
	GameCommand _currentCommand;
	ActionComponent *_pActionComp{};
	void QueueAction(const Action &action);

	TerrainTilePos _tilePos;

private :
	EventChannel _channel;
	float _speed{2.0f};

	StopWatch _combatToPeaceTimer;

	StopWatch _moveToStanceTimer;
	Movement _currentMovement;

	StopWatch _attackToStanceTimer;
	StopWatch _attackCombo1Timer;
	StopWatch _attackCombo2Timer;
	int32 _comboCount{};

public :

	struct PlayerImformationEvent
	{
		PlayerImformationEvent(const Vector3 &position,const PLAYERSTATE &state,const Vector3 &forward)
			:_position(position),
			_state(state),
			_forward(forward)
		{
		}
		Vector3 _position;
		PLAYERSTATE _state;
		Vector3 _forward;
	};

};

#endif