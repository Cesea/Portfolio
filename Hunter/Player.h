#ifndef PLAYER_H
#define PLAYER_H

#include "BaseGameObject.h"
#include "PlayerAnimationString.h"

class Camera;
class Player;
class DamageBox;

struct KeyConfig
{
	uint32 _up;
	uint32 _down;
	uint32 _left;
	uint32 _right;
};

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
	EventChannel _channel;
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
	void QueueAction(Action &action, bool cancle = false);

	void RepositionEntity(const TerrainTilePos &currentPos, const TerrainTilePos &prevPos);

	ActionComponent *_pActionComp{};
	TransformComponent *_pTransformComp{};
	CollisionComponent *_pCollisionComp{};
private :
	EventChannel _channel;
	float _walkSpeed{1.6f};
	float _runSpeed{5.5f};

	float _rotationSpeed{ 7.0f };

	int32 _hp{ INT_MAX };

	//int32 _stamina{ 100 };

	StopWatch _combatToPeaceTimer;

	StopWatch _moveToStanceTimer;
	Movement _currentMovement;

	StopWatch _attackToStanceTimer;
	int32 _comboCount{};

	bool32 _canCombo{false};

	float _targetRotation{};
	float _currentRotation{};

	bool32 _camRotated{false};
	bool32 _superArmor = true;
	StopWatch _superArmorTimer;

	Vector3 _startForward;
	Vector3 _runStartForward;

	KeyConfig _keyConfig;

	DamageBox *_pDamageBox;
	Bone *_pSwordFrame;
	Vector3 _worldSwordPos;

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