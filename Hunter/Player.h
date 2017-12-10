#ifndef PLAYER_H
#define PLAYER_H

#include "BaseGameObject.h"
#include "PlayerAnimationString.h"

#define PLAYER_ATTACK_ONE_DAMAGE (40)
#define PLAYER_ATTACK_TWO_DAMAGE (50)
#define PLAYER_ATTACK_THREE_DAMAGE (70)

class Camera;
class Player;
class DamageBox;

void PlayPlayerAttackSound(int32 damage, const Vector3 &position);

struct KeyConfig
{
	uint32 _up;
	uint32 _down;
	uint32 _left;
	uint32 _right;
};

struct PlayerCallbackData
{
	int32 *_animtionEnum{};
	Vector3 *_pPosition{};
	//0이면 left 1이면 right
	int32 _description;
};

class PlayerCallbackHandler : public GameObjectAnimationCallbackHandler
{
public :
	virtual bool Init(BaseGameObject *pPlayer) { _pPlayer = (Player *)pPlayer;  return true; }

	HRESULT CALLBACK HandleCallback(THIS_ UINT Track, LPVOID pCallbackData);
private :
	Player *_pPlayer{};
	EventChannel _channel;
};

class Player : public BaseGameObject
{
	friend class PlayerCallbackHandler;
	friend class Camera;
	friend class UI;

public :
	friend class PlayerStanceState;
public :
	Player();
	virtual ~Player();

	virtual bool CreateFromWorld(World &world, const Vector3 &pos);
	void Update(float deltaTime);

	void RegisterEvents();
	void UnRegisterEvents();

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
		PLAYERSTATE_HARVEST,
	};
private :
	PLAYERSTATE _state;
	bool32 _inCombat{};

	Camera* _camera;
	Vector3 cForward;

	void SetupCallbackAndCompression();

	void MoveAndRotate(float deltaTime);

	PlayerCallbackData _callbackData[6];
	GameCommand _currentCommand;
	GameCommand _prevCommand;
	void QueueAction(Action &action, bool cancle = false);

	void RepositionEntity(const TerrainTilePos &currentPos, const TerrainTilePos &prevPos);

	ActionComponent *_pActionComp{};
	TransformComponent *_pTransformComp{};
	CollisionComponent *_pCollisionComp{};

private :
	EventChannel _channel;
	float _walkSpeed{1.6f};
	float _runSpeed{5.5f};

	float _rotationSpeed{ 6.0f };

	int32 _maxHP{ 2000 };
	int32 _currentHP{ 2000 };

	int32 _maxFury{ 100 };
	int32 _currentFury{ 100 };

	//int32 _stamina{ 100 };

	StopWatch _combatToPeaceTimer;

	StopWatch _moveToStanceTimer;
	Movement _currentMovement;

	StopWatch _attackToStanceTimer;
	int32 _comboCount{};

	bool32 _canCombo{false};

	float _targetRotation{};
	float _currentRotation{};
	bool32 _rotating{ false };
	void RotatePlayer(float angle);

	bool32 _camRotated{false};
	bool32 _superArmor = true;
	StopWatch _superArmorTimer;

	Vector3 _startForward;
	Vector3 _runStartForward;

	KeyConfig _keyConfig;

	DamageBox *_pDamageBox;
	Bone *_pSwordFrame;
	Vector3 _worldSwordPos;

	StopWatch _deadTimer;

public :

	struct PlayerImformationEvent
	{
		PlayerImformationEvent(const Vector3 &position, 
			const PLAYERSTATE &state,
			const Vector3 &forward, 
			const Vector3 &swordPos) 
			: _position(position), _state(state), _forward(forward), _swordPos(swordPos)
		{
		}
		Vector3 _position;
		PLAYERSTATE _state;
		Vector3 _forward;
		Vector3 _swordPos;
	};

};

#endif