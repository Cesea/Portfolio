#pragma once
#include "Enemy.h"
#include "DragonAnimationString.h"
class Dragon;
class DragonStateMachine;
struct DragonCallbackData
{
	DRAGON_ANIMATION_ENUM *_animtionEnum;
};

class DragonCallbackHandler : public GameObjectAnimationCallbackHandler
{
public:
	virtual bool Init(BaseGameObject *pDragon) { _pDragon = (Dragon *)pDragon; return true; }
	HRESULT CALLBACK HandleCallback(THIS_ UINT Track, LPVOID pCallbackData)
	{
		return S_OK;
	}

private:
	Dragon *_pDragon{};
};

class Dragon : public Enemy
{
	friend class DragonStateMachine;
	friend class DragonCallbackHandler;
public:
	Dragon();
	virtual ~Dragon();
	virtual bool CreateFromWorld(World &world, const Vector3 &Pos);
	void Update(float deltaTime);
	void Handle(const CollisionSystem::ActorTriggerEvent & event);

protected:
	void SetupCallbackAndCompression();

	DragonCallbackData _callbackData;

	GameCommand _currentCommand;

	ActionComponent *_pActionComp{};

	void QueueAction(Action &action, bool cancle = false);
public:
	std::vector<Vector3> GetMoveSegment(Vector3 myPos);
	void speedUp();
	void speedDown();
	Vector3 rotateVector(Vector3 vec, float angle);
private:
	std::vector<Vector3> _moveSegment;
	int _moveSegmentIndex;
	float _normalSpeed;
	float _maxSpeed;
	float _accel;

	Vector3 _escapePoint;

	float _flyHeight;
	std::vector<Vector3> _flySegment;
	int _flySegmentIndex;

	enum DRAGONSTATE
	{
		DRAGONSTATE_START,
		DRAGONSTATE_MOVE_IN,
		DRAGONSTATE_TRACE,
		DRAGONSTATE_MOVE_OUT,
		DRAGONSTATE_STAND,
		DRAGONSTATE_STARTFLY,
		DRAGONSTATE_FLY_ROUND,
		DRAGONSTATE_FLY_TRACE,
		DRAGONSTATE_FLY_DOWN,
		DRAGONSTATE_START_BREATH,
		DRAGONSTATE_BREATH,
		DRAGONSTATE_STAND_BREATH,
		DRAGONSTATE_DEFAULT
	};
	DRAGONSTATE _state;

	enum DRAGONANIMSTATE
	{
		DRAGONANIMSTATE_BITE,
		DRAGONANIMSTATE_WHIP_TAIL,
		DRAGONANIMSTATE_STAND,
		DRAGONANIMSTATE_FLY_ATK,
		DRAGONANIMSTATE_DEFAULT
	};
	DRAGONANIMSTATE _anim;

	enum DRAGONPATTERNSTATE
	{
		DRAGONPATTERNSTATE_GROUND,
		DRAGONPATTERNSTATE_FLY
	};
	DRAGONPATTERNSTATE _pattern;
	//물기 프레임
	int _biteTime;
	int _biteCount;
	int _biteNum;
	int _biteNumCount;
	//꼬리 프레임
	int _whipTime;
	int _whipCount;
	//대기 프레임
	int _stopCount;
	int _stopTime;
	//공중 공격 프레임
	int _flyAtkTime;
	int _flyAtkCount;
	int _flyNum;
	int _flyNumCount;
	int _flyAtkNum;
	int _flyAtkNumCount;
	//브레스 준비 프레임
	int _breathReadyTime;
	int _breathReadyCount;
	//브레스 프레임
	int _breathTime;
	int _breathCount;
	int _breathNum;
	int _breathNumCount;
	//브레스 쿨타임
	int _breathDelayTime;
	int _breathDelayCount;

	DRAGONANIMSTATE _nextAnim;
};