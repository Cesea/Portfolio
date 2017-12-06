#ifndef PLAYER_H
#define PLAYER_H

#include "BaseGameObject.h"
#include "PlayerAnimationString.h"

class Camera;
class Player;

struct PlayerCallbackData
{
	PlayerAnimationEnum *_animtionEnum;
};

class PlayerCallbackHandler : public GameObjectAnimationCallbackHandler
{
public :
	virtual bool Init(BaseGameObject *pPlayer) { _pPlayer = (Player *)pPlayer;  return true; }

	HRESULT CALLBACK HandleCallback(THIS_ UINT Track, LPVOID pCallbackData);
private :
	Player *_pPlayer{};
};

class Player : public BaseGameObject
{
	friend class PlayerCallbackHandler;
	friend class Camera;
public :
	friend class PlayerStanceState;
public :
	Player();
	virtual ~Player();

	virtual bool CreateFromWorld(World &world, const Vector3 &Pos);
	void Update(float deltaTime);

	void Handle(const InputManager::KeyDownEvent &event);
	void Handle(const InputManager::KeyReleasedEvent &event);
	void Handle(const InputManager::KeyPressedEvent &event);
	void Handle(const InputManager::MousePressedEvent &event);

	void Handle(const CollisionSystem::ActorTriggerEvent &event);

	void SetLinkCamera(Camera* camera) { _camera = camera; }

public:
	enum PLAYERSTATE
	{
		PLAYERSTATE_STANCE,
		PLAYERSTATE_MOVE,
		PLAYERSTATE_RUN,
		PLAYERSTATE_ATTACK,
		PLAYERSTATE_BLOCK,
		PLAYERSTATE_MOVEATTACK,
		PLAYERSTATE_HURT,
		PLAYERSTATE_DEAD,
	};
private :
	PLAYERSTATE _state;
	bool32 _inCombat{};

	Camera* _camera;
	Vector3 cForward;

	void SetupCallbackAndCompression();

	void MoveAndRotate(float deltaTime);

	PlayerCallbackData _callbackData;
	GameCommand _currentCommand;
	GameCommand _prevCommand;
	void QueueAction(const Action &action);

	void RepositionEntity(const TerrainTilePos &currentPos, const TerrainTilePos &prevPos);

	ActionComponent *_pActionComp{};
	TransformComponent *_pTransformComp{};
	CollisionComponent *_pCollisionComp{};

private :
	EventChannel _channel;
	float _walkSpeed{1.6f};
	float _runSpeed{5.5f};

	float _rotationSpeed{ 7.0f };

	int32 _hp{ 400 };

	//int32 _stamina{ 100 };

	StopWatch _combatToPeaceTimer;

	StopWatch _moveToStanceTimer;
	Movement _currentMovement;

	StopWatch _attackToStanceTimer;
	int32 _comboCount{};

	StopWatch _attackTriggerTimer;

	bool32 _canCombo{false};

	float _targetRotation{};
	float _currentRotation{};

	bool32 _camRotated{false};

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