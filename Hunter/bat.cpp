#include "stdafx.h"
#include "Bat.h"
#include "BatStates.h"

Bat::Bat()
{
}

Bat::~Bat()
{
}

bool Bat::CreateFromWorld(World & world)
{
	_entity = world.CreateEntity();
	TransformComponent &transComp = _entity.AddComponent<TransformComponent>();
	transComp.MovePositionWorld(0, 11.0f, 0);

	static int32 animCount = 0;
	RenderComponent &renderComp = _entity.AddComponent<RenderComponent>();
	renderComp._type = RenderComponent::Type::eSkinned;
	renderComp._skinned = VIDEO->CreateAnimationInstance(
		VIDEO->GetSkinnedXMesh("Bat"), "Bat_" + std::to_string(animCount));
	renderComp._arche = ARCHE_BAT;

	video::AnimationInstance *pAnimation = VIDEO->GetAnimationInstance(renderComp._skinned);

	CollisionComponent &collision = _entity.AddComponent<CollisionComponent>();
	collision._boundingBox.Init(pAnimation->_pSkinnedMesh->_boundInfo._min,
		pAnimation->_pSkinnedMesh->_boundInfo._max);
	collision._boundingSphere._localCenter = pAnimation->_pSkinnedMesh->_boundInfo._center;
	collision._boundingSphere._radius = pAnimation->_pSkinnedMesh->_boundInfo._radius;
	collision._locked = false;

	ScriptComponent &scriptComponent = _entity.AddComponent<ScriptComponent>();
	scriptComponent.SetScript(MAKE_SCRIPT_DELEGATE(Bat, Update, *this));

	_pActionComp = &_entity.AddComponent<ActionComponent>();
	_pActionComp->CreateFrom(renderComp._skinned);
	_pActionComp->_pCallbackHandler = new BatCallbackHandler;
	_pActionComp->_pCallbackHandler->Init(this);
	SetupCallbackAndCompression();

	_pActionComp->MakeAnimationList();
	_pActionComp->SetFirstAction(BAT_ANIM(BAT_ANIMATION_ENUM::BAT_FORWARD));

	_entity.Activate();

	_pStateMachine = new BatStateMachine;
	_pStateMachine->Init(this);
	_pStateMachine->RegisterState(META_TYPE(BatIdleState)->Name(), new BatIdleState());
	_pStateMachine->RegisterState(META_TYPE(BatMoveState)->Name(), new BatMoveState());
	_pStateMachine->RegisterState(META_TYPE(BatAttackState)->Name(), new BatAttackState());
	_pStateMachine->RegisterState(META_TYPE(BatAttack2State)->Name(), new BatAttack2State());
	_pStateMachine->RegisterState(META_TYPE(BatAttack3State)->Name(), new BatAttack3State());
	_pStateMachine->RegisterState(META_TYPE(BatFindState)->Name(), new BatFindState());
	_pStateMachine->ChangeState(META_TYPE(BatIdleState)->Name());

	_state = BATSTATE_IDLE;;

	_speed = 4.0f;
	_rotateSpeed = D3DX_PI / 64;
	_patrolIndex = 0;
	_moveSegment.push_back(Vector3(5.0f, 11.0f, 5.0f));
	_moveSegment.push_back(Vector3(-5.0f, 11.0f, 5.0f));
	_moveSegment.push_back(Vector3(-5.0f, 11.0f, -5.0f));

	_delayTime = 180.0f;
	_delayCount = _delayTime;

	_findDistance = 3.0f;
	_findRadian = D3DX_PI / 3;
	_findStareDistance = 20.0f;
	_roarTime = 210;
	_roarCount = _roarTime;

	_battle = false;

	_playerPos = Vector3(5.0f, 11.0f, 5.0f);

	_atkRange = 0.5f;
	_atkTime = 80;
	_atkTime2 = 172;
	_atkTime3 = 100;
	_atkCount = _atkTime;

	_standTime = 90;
	_standCount = _standTime;

	return true;
}

void Bat::Update(float deltaTime)
{
	_pStateMachine->Update(deltaTime, _currentCommand);
	TransformComponent &transComp = _entity.GetComponent<TransformComponent>();
	switch (_state)
	{
	case BATSTATE_IDLE:
		_delayCount -= 1;
		if (_delayCount <= 0.0f)
		{
			_delayCount = _delayTime;
			_state = BATSTATE_PATROL;
			_pStateMachine->ChangeState(META_TYPE(BatMoveState)->Name());
		}
		break;
	case BATSTATE_PATROL:
		if (_moveSegment.empty())
		{
			_pStateMachine->ChangeState(META_TYPE(BatIdleState)->Name());
			_state = BATSTATE_IDLE;
		}
		else
		{
			//다음 인덱스로 방향을 얻고
			Vector3 direction = _moveSegment[_patrolIndex] - transComp.GetWorldPosition();
			float distance = Vec3Length(&direction);
			Vec3Normalize(&direction, &direction);
			//몸이 덜 돌아갔는가?
			float distRadian = acos(
				ClampMinusOnePlusOne(Vec3Dot(&-direction, &transComp.GetForward())));
			if (distRadian > D3DX_PI) D3DX_PI * 2 - distRadian;
			if (distRadian > _rotateSpeed)
			{
				transComp.LookDirection(-direction, _rotateSpeed);
				break;
			}
			//이동속도보다 가까움?
			if (distance < _speed*deltaTime)
			{
				//거리만큼 움직이고 patrolIndex변경
				transComp.SetWorldPosition(transComp.GetWorldPosition() + direction*distance);
				_patrolIndex++;
				if (_patrolIndex > _moveSegment.size() - 1) _patrolIndex = 0;
				//IDLE 애니메이션 실행
				_pStateMachine->ChangeState(META_TYPE(BatIdleState)->Name());
				_state = BATSTATE_IDLE;
			}
			//아니면 이동속도만큼 이동
			else
			{
				transComp.SetWorldPosition(transComp.GetWorldPosition() + direction*_speed*deltaTime);
			}

		}
		break;
	case BATSTATE_FIND:
		//roar가 끝나면 플레이어를 추적하는 RUN으로
		_roarCount -= 1;
		if (_roarCount < 0)
		{
			_roarCount = _roarTime;
			_state = BATSTATE_RUN;
			_pStateMachine->ChangeState(META_TYPE(BatMoveState)->Name());
		}
		break;
	case BATSTATE_RUN:
	{
		Vector3 direction = _playerPos - transComp.GetWorldPosition();
		float distance = Vec3Length(&direction);
		Vec3Normalize(&direction, &direction);
		if (distance < _atkRange)
		{
			_state = BATSTATE_ATK1;
			_pStateMachine->ChangeState(META_TYPE(BatAttackState)->Name());
		}
		else
		{
			transComp.SetWorldPosition(transComp.GetWorldPosition() + direction * _speed * deltaTime * 2);
		}
	}
	break;
	case BATSTATE_ATK1:
		_atkCount--;
		if (_atkCount < 0)
		{
			_atkCount = _atkTime2;
			//공격을 마쳤으면 다시한번검사
			Vector3 direction = _playerPos - transComp.GetWorldPosition();
			float distance = Vec3Length(&direction);
			Vec3Normalize(&direction, &direction);
			if (distance < _atkRange)
			{
				_state = BATSTATE_ATK2;
				_pStateMachine->ChangeState(META_TYPE(BatAttack2State)->Name());
			}
			//공격범위를 벗어났다?
			else
			{
				//배틀을 멈추고 기본자세 (다시추적시작)
				_battle = false;
				_state = BATSTATE_IDLE;
				_pStateMachine->ChangeState(META_TYPE(BatIdleState)->Name());
			}
		}
		break;
	case BATSTATE_ATK2:
		_atkCount--;
		if (_atkCount < 0)
		{
			_atkCount = _atkTime3;
			//공격을 마쳤으면 다시한번검사
			Vector3 direction = _playerPos - transComp.GetWorldPosition();
			float distance = Vec3Length(&direction);
			Vec3Normalize(&direction, &direction);
			if (distance < _atkRange)
			{
				_state = BATSTATE_ATK3;
				_pStateMachine->ChangeState(META_TYPE(BatAttack3State)->Name());
				_playerPos = Vector3(RandFloat(-5.0, 5.0), 11.0f, RandFloat(-5.0, 5.0));
			}
			//공격범위를 벗어났다?
			else
			{
				//배틀을 멈추고 기본자세 (다시추적시작)
				_battle = false;
				_state = BATSTATE_IDLE;
				_pStateMachine->ChangeState(META_TYPE(BatIdleState)->Name());
			}
		}
		break;
	case BATSTATE_ATK3:
		_atkCount--;
		if (_atkCount < 0)
		{
			_atkCount = _atkTime;
			//공격을 마쳤으면 다시한번검사
			Vector3 direction = _playerPos - transComp.GetWorldPosition();
			float distance = Vec3Length(&direction);
			Vec3Normalize(&direction, &direction);
			if (distance < _atkRange)
			{
				_state = BATSTATE_ATK1;
				_pStateMachine->ChangeState(META_TYPE(BatAttackState)->Name());
			}
			//공격범위를 벗어났다?
			else
			{
				//배틀을 멈추고 기본자세 (다시추적시작)
				_battle = false;
				_state = BATSTATE_IDLE;
				_pStateMachine->ChangeState(META_TYPE(BatIdleState)->Name());
			}
		}
		break;
	}
	//전투상태가 아니라면 항시 플레이어를 수색한다.
	if (!_battle)
	{
		if (findPlayer(transComp.GetForward(), _playerPos, transComp.GetWorldPosition(), _findStareDistance, _findDistance, _findRadian))
		{
			//찾으면 FIND가 되며 battle상태가 됌
			_battle = true;
			_state = BATSTATE_FIND;
			_pStateMachine->ChangeState(META_TYPE(BatFindState)->Name());
			Vector3 distance = _playerPos - transComp.GetWorldPosition();
			Vec3Normalize(&distance, &distance);
			transComp.LookDirection(-distance, D3DX_PI * 2);
			_atkCount = _atkTime;
		}
	}
}

void Bat::SetupCallbackAndCompression()
{
	ActionComponent &refActionComp = _entity.GetComponent<ActionComponent>();
	TransformComponent &refTransform = _entity.GetComponent<TransformComponent>();

	ID3DXAnimationController *pController = refActionComp._pAnimationController;
	uint32 numAnimationSet = pController->GetNumAnimationSets();
	ID3DXKeyframedAnimationSet *anim0;

	pController->GetAnimationSetByName(BatAnimationString[BAT_ANIMATION_ENUM::BAT_IDLE], (ID3DXAnimationSet **)&anim0);

	_callbackData._animtionEnum = (BAT_ANIMATION_ENUM *)&_animationEnum;

	D3DXKEY_CALLBACK warSwingLeftKeys;
	warSwingLeftKeys.Time = anim0->GetPeriod() / 1.0f * anim0->GetSourceTicksPerSecond();
	warSwingLeftKeys.pCallbackData = (void *)&_callbackData;

	AddCallbackKeysAndCompress(pController, anim0, 1, &warSwingLeftKeys, D3DXCOMPRESS_DEFAULT, 0.1f);
}

void Bat::QueueAction(const Action & action)
{
	_pActionComp->_actionQueue.PushAction(action);
}

bool Bat::findPlayer(Vector3 forward, Vector3 playerPos, Vector3 myPos, float range1, float range2, float findRadian)
{
	Vector3 toPlayer = playerPos - myPos;
	float distance = Vec3Length(&toPlayer);
	Vec3Normalize(&toPlayer, &toPlayer);

	float distRadian = acos(
		ClampMinusOnePlusOne(Vec3Dot(&forward, &-toPlayer)));
	if (distRadian > D3DX_PI) D3DX_PI * 2 - distRadian;
	//시야각의 1/2보다 작다면 range1 서치
	if (distRadian < findRadian / 2)
	{
		if (distance < range1)
		{
			return true;
		}
	}
	//아니라면 range2 서치
	else
	{
		if (distance < range2)
		{
			return true;
		}
	}

	return false;
}
