#ifndef PLAYER_H
#define PLAYER_H

#include "BaseGameObject.h"
#include "PlayerAnimationString.h"

class Camera;

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

	virtual bool CreateFromWorld(World &world);
	void Update(float deltaTime);

	void Handle(const InputManager::KeyDownEvent &event);
	void Handle(const InputManager::KeyReleasedEvent &event);
	//void Handle(const InputManager::KeyDownEvent &event);
	void Handle(const InputManager::MousePressedEvent &event);

	void SetLinkCamera(Camera* camera) { _camera = camera; }

private :
	enum PLAYERSTATE
	{
		PLAYERSTATE_STANCE,
		PLAYERSTATE_MOVE,
		PLAYERSTATE_RUN,
		PLAYERSTATE_ATTACK,
		PLAYERSTATE_BLOCK,
		PLAYERSTATE_MOVEATTACK,
	};
	PLAYERSTATE _state;
	bool32 _inCombat{};

	Camera* _camera;
	Vector3 cForward;

	void SetupCallbackAndCompression();

	void MoveAndRotate(float deltaTime);

	PlayerCallbackData _callbackData;
	PlayerStateMachine *_pStateMachine;
	GameCommand _currentCommand;
	ActionComponent *_pActionComp{};
	void QueueAction(const Action &action);

	TerrainTilePos _tilePos;

private :
	EventChannel _channel;
	float _walkSpeed{2.0f};
	float _runSpeed{5.0f};
	float _rotationSpeed{ 0.05f };
	float _backRotationSpeed{ 0.03f };

	//int32 _stamina{ 100 };

	StopWatch _combatToPeaceTimer;

	StopWatch _moveToStanceTimer;
	Movement _currentMovement;

	StopWatch _attackToStanceTimer;
	StopWatch _comboTimer;
	int32 _comboCount{};

public :

	struct PlayerPositionEvent
	{
		PlayerPositionEvent(const Vector3 &position)
			:_position(position)
		{
		}
		Vector3 _position;
	};

};

#endif