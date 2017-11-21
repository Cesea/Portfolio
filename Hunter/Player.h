#ifndef PLAYER_H
#define PLAYER_H

#define TRANSITION_TIME 0.125f

enum PlayerAnimationEnum
{
	eWarRetreat = 0,
	eWalkingBack,
	eClimbingLadder,
	eCrouching,
	eDrinking,
	eEating,
	eExplanation,
	eJumpRunning,
	eRun,
	eSayNo,
	eSneaking,
	eStandingFree,
	eSwimming,
	eTalking,
	eWarThrowingAxe,
	eWarThrowingSpear,
	eWarCombatMode,
	eWarDying,
	eWarBackwards,
	eWarBelowHighSwing,
	eWarCharging,
	eWarDodgeToLeft,
	eWarDodgeToRight,
	eWarMovingLeft,
	eWarMovingRight,
	eWarParryFromStraightDown,
	eWarParryFront,
	eWarRunSwingLeft,
	eWarRunSwingRight,
	eWarRunSwingUpDown,
	eWarShieldBlock,
	eWarShieldBlow,
	eWarSpecialAttackA,
	eWarSpecialAttackB,
	eWarSwingHighStraigtDown,
	eWarSwingLeft,
	eWarSwingRight,
	eWarTakingHit,
	eWarThrustMid,
	eBoring,
	eCameUp,
	eLookingAround,
	eMakingItem,
	ePickObject,
	eSalute,
	eSitDown,
	eSitting,
	eHeroWalk
};

static const char *PlayerAnimationString[] = 
{
	{"Hero_War_retreat"},
	{ "Hero_walking_back"},
	{ "hero_Climbing_ladder"},
	{ "hero_Crouching"},
	{ "hero_Drinking"},
	{ "hero_Eating"},
	{ "hero_Explanation"},
	{ "hero_Jump_running"},
	{ "hero_Run"},
	{"hero_Say_No"},
	{"hero_Sneaking"},
	{"hero_Standing_Free"},
	{"hero_Swimming"},
	{"hero_Talking"},
	{"hero_ThrowingAxe_throwing"},
	{"hero_ThrowingSpear_throwing"},
	{"hero_War_COMBAT_MODE"},
	{"hero_War_Dying"},
	{"hero_War_backwards"},
	{"hero_War_below_high_swing"},
	{"hero_War_charging"},
	{"hero_War_dodge_to_left"},
	{"hero_War_dodge_to_right"},
	{"hero_War_moving_left"},
	{"hero_War_moving_right"},
	{"hero_War_parry_from_stright_dow"},
	{"hero_War_parry_front"},
	{"hero_War_run_swing_left"},
	{"hero_War_run_swing_right"},
	{"hero_War_run_swing_up_down"},
	{"hero_War_shield_block"},
	{"hero_War_shield_blow"},
	{"hero_War_special_attack_A"},
	{"hero_War_special_attack_B"},
	{"hero_war_swing_high_straight_do"},
	{"hero_War_swing_left"},
	{"hero_War_swing_right"},
	{"hero_War_taking_hit"},
	{"hero_War_thrust_mid"},
	{"hero_boring"},
	{"hero_came_up"},
	{"hero_looking_around"},
	{"hero_making_item"},
	{"hero_pick_object"},
	{"hero_salute"},
	{"hero_sit_down"},
	{"hero_sitting"},
	{"hero_walk"}
};

struct LocalTimer
{
	bool Tick(float deltaTime);
	void Restart(float targetTime = -1.0f);
	float _currentTime{};
	float _targetTime{};
};

struct Command
{
	enum Type
	{
		eNone,
		eMove,
		eAction,
		eJump,
		eInteract,
	};

	enum MoveDirection
	{
		eLeft,
		eUp,
		eRight,
		eDown
	};
	enum ActionType
	{
		eAttack,
		eBlock,
		eSpecial
	};


	Type _type{ Command::Type::eNone };
	int32 _detail{0};
	bool32 _interpreted{false};
};

struct PlayerCallbackData
{
	int32 _a;
	Vector3 *_pPlayerPosition;
};

class PlayerCallbackHandler : public ID3DXAnimationCallbackHandler
{
public :
    HRESULT CALLBACK HandleCallback( THIS_ UINT Track, LPVOID pCallbackData )
    {
        PlayerCallbackData* pData = ( PlayerCallbackData* )pCallbackData;

        // this is set to NULL if we're not playing sounds
		if (nullptr == pData)
		{
            return S_OK;
		}

		Console::Log("%d\n", pData->_a);
        return S_OK;
    }
};

struct InputConfig
{
	uint32 _up;
	uint32 _down;
	uint32 _left;
	uint32 _right;
	uint32 _jump;
	uint32 _special;
	//마우스 키로 하자..
	//uint32 _attack;
	//uint32 _block;
};

class Player 
{
public :
	Player();
	virtual ~Player();

	void CreateFromWorld(World &world);
	void Update(float deltaTime);

	void Handle(const InputManager::KeyPressedEvent &event);
protected :

	Action InterpretCommand(const Command &command);

	void SetupCallbackAndCompression();
	bool AddCallbackKeysAndCompress(LPD3DXANIMATIONCONTROLLER pAnimationController,
		LPD3DXKEYFRAMEDANIMATIONSET pAnimationSet,
		DWORD numCallbackKeys,
		D3DXKEY_CALLBACK *pKeys,
		DWORD compressionFlags,
		float compression);

	Entity _entity;

	InputConfig _keyConfig;

	PlayerCallbackData _callbackData;
	LocalTimer _comboTimer;

	Command _lastCommand;

	//상태 관련 변수들
	bool32 _inWar{false};
	bool32 _moving{ false };
	bool32 _attacking{ false };
	bool32 _blocking{ false };
};

#endif