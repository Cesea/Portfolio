#include "stdafx.h"
#include "Cat.h"
#include "CatStates.h"

Cat::Cat()
{
}

Cat::~Cat()
{
}

bool Cat::CreateFromWorld(World & world)
{
	_entity = world.CreateEntity();
	TransformComponent &transComp = _entity.AddComponent<TransformComponent>();
	transComp.MovePositionWorld(0, 7.0f, 0);

	RenderComponent &renderComp = _entity.AddComponent<RenderComponent>();
	renderComp._type = RenderComponent::Type::eSkinned;
	renderComp._skinned = VIDEO->CreateAnimationInstance(VIDEO->GetSkinnedXMesh("Cat"), "Anim" + std::to_string(0));

	ScriptComponent &scriptComponent = _entity.AddComponent<ScriptComponent>();
	scriptComponent.SetScript(MAKE_SCRIPT_DELEGATE(Cat, Update, *this));

	_pActionComp = &_entity.AddComponent<ActionComponent>();
	_pActionComp->CreateFrom(renderComp._skinned);
	_pActionComp->_pCallbackHandler = new CatCallbackHandler;
	_pActionComp->_pCallbackHandler->Init(this);
	SetupCallbackAndCompression();

	_pActionComp->MakeAnimationList();
	_pActionComp->SetFirstAction(CAT_ANIM(CAT_ANIMATION_ENUM::CAT_IDLE));

	_entity.Activate();

	_pStateMachine = new CatStateMachine;
	_pStateMachine->Init(this);
	_pStateMachine->RegisterState(META_TYPE(CatIdleState)->Name(), new CatIdleState());
	_pStateMachine->RegisterState(META_TYPE(CatMoveState)->Name(), new CatMoveState());
	_pStateMachine->RegisterState(META_TYPE(CatAttackState)->Name(), new CatAttackState());
	_pStateMachine->RegisterState(META_TYPE(CatAttack2State)->Name(), new CatAttack2State());
	_pStateMachine->RegisterState(META_TYPE(CatAttack3State)->Name(), new CatAttack3State());
	_pStateMachine->RegisterState(META_TYPE(CatAttack4State)->Name(), new CatAttack4State());
	_pStateMachine->RegisterState(META_TYPE(CatAttack5State)->Name(), new CatAttack5State());
	_pStateMachine->RegisterState(META_TYPE(CatStandState)->Name(), new CatStandState());
	_pStateMachine->ChangeState(META_TYPE(CatIdleState)->Name());

	_speed = 5.0f;
	_rotateSpeed = D3DX_PI / 64;
	_patrolIndex = 0;
	_moveSegment.push_back(Vector3(5.0f, 7.0f, 5.0f));
	_moveSegment.push_back(Vector3(-5.0f, 7.0f, 5.0f));
	_moveSegment.push_back(Vector3(-5.0f, 7.0f, -5.0f));

	_delayTime = 180.0f;
	_delayCount = _delayTime;

	_findDistance = 5.0f;
	_findRadian = D3DX_PI / 3;
	_findStareDistance = 20.0f;
	_roarTime = 180;
	_roarCount = _roarTime;

	_battle = false;

	_playerPos = Vector3(5.0f, 7.0f, 5.0f);

	_atkRange = 0.5f;
	_atkTime = 70;
	_atkCount = _atkTime;

	_standTime = 70;
	_standCount = _standTime;
	return true;
}

void Cat::Update(float deltaTime)
{
	_pStateMachine->Update(deltaTime, _currentCommand);
	TransformComponent &transComp = _entity.GetComponent<TransformComponent>();
	switch (_state)
	{
	case CATSTATE_IDLE:
		_delayCount -= 1;
		if (_delayCount <= 0.0f)
		{
			_delayCount = _delayTime;
			_state = CATSTATE_PATROL;
			_pStateMachine->ChangeState(META_TYPE(CatMoveState)->Name());
		}
		break;
	case CATSTATE_PATROL:
		if (_moveSegment.empty())
		{
			_pStateMachine->ChangeState(META_TYPE(CatStandState)->Name());
			_state = CATSTATE_IDLE;
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
				_pStateMachine->ChangeState(META_TYPE(CatStandState)->Name());
				_state = CATSTATE_IDLE;
			}
			//아니면 이동속도만큼 이동
			else
			{
				transComp.SetWorldPosition(transComp.GetWorldPosition() + direction*_speed*deltaTime);
			}

		}
		break;
	case CATSTATE_FIND:
		//roar가 끝나면 플레이어를 추적하는 RUN으로
		_roarCount -= 1;
		if (_roarCount < 0)
		{
			_roarCount = _roarTime;
			_state = CATSTATE_RUN;
			_pStateMachine->ChangeState(META_TYPE(CatMoveState)->Name());
		}
		break;
	case CATSTATE_RUN:
	{
		Vector3 direction = _playerPos - transComp.GetWorldPosition();
		float distance = Vec3Length(&direction);
		Vec3Normalize(&direction, &direction);
		if (distance < _atkRange)
		{
			_state = CATSTATE_ATK1;
			_pStateMachine->ChangeState(META_TYPE(CatAttackState)->Name());
		}
		else
		{
			transComp.SetWorldPosition(transComp.GetWorldPosition() + direction * _speed * deltaTime * 2);
		}
	}
	break;
	case CATSTATE_ATK1:
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
				_state = CATSTATE_ATK2;
				_pStateMachine->ChangeState(META_TYPE(CatAttack2State)->Name());
			}
			//공격범위를 벗어났다?
			else
			{
				//배틀을 멈추고 기본자세 (다시추적시작)
				_battle = false;
				_state = CATSTATE_IDLE;
				_pStateMachine->ChangeState(META_TYPE(CatStandState)->Name());
			}
		}
		break;
	case CATSTATE_ATK2:
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
				_state = CATSTATE_ATK3;
				_pStateMachine->ChangeState(META_TYPE(CatAttack3State)->Name());
			}
			//공격범위를 벗어났다?
			else
			{
				//배틀을 멈추고 기본자세 (다시추적시작)
				_battle = false;
				_state = CATSTATE_IDLE;
				_pStateMachine->ChangeState(META_TYPE(CatStandState)->Name());
			}
		}
		break;
	case CATSTATE_ATK3:
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
				_state = CATSTATE_ATK5;
				_pStateMachine->ChangeState(META_TYPE(CatAttack5State)->Name());
			}
			//공격범위를 벗어났다?
			else
			{
				//배틀을 멈추고 기본자세 (다시추적시작)
				_battle = false;
				_state = CATSTATE_IDLE;
				_pStateMachine->ChangeState(META_TYPE(CatStandState)->Name());
			}
		}
		break;
	case CATSTATE_ATK4:
		break;
	case CATSTATE_ATK5:
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
				_state = CATSTATE_ATK1;
				_pStateMachine->ChangeState(META_TYPE(CatAttackState)->Name());
				_playerPos = Vector3(RandFloat(-5.0, 5.0), 7.0f, RandFloat(-5.0, 5.0));
			}
			//공격범위를 벗어났다?
			else
			{
				//배틀을 멈추고 기본자세 (다시추적시작)
				_battle = false;
				_state = CATSTATE_IDLE;
				_pStateMachine->ChangeState(META_TYPE(CatStandState)->Name());
			}
		}
		break;
	case CATSTATE_STAND:
		_standCount--;
		if (_standCount < 0)
		{
			_standCount = _standTime;
			_state = CATSTATE_PATROL;
			_pStateMachine->ChangeState(META_TYPE(CatMoveState)->Name());
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
			_state = CATSTATE_FIND;
			_pStateMachine->ChangeState(META_TYPE(CatIdleState)->Name());
			Vector3 distance = _playerPos - transComp.GetWorldPosition();
			Vec3Normalize(&distance, &distance);
			transComp.LookDirection(-distance, D3DX_PI * 2);
		}
	}
}

void Cat::SetupCallbackAndCompression()
{
	ActionComponent &refActionComp = _entity.GetComponent<ActionComponent>();
	TransformComponent &refTransform = _entity.GetComponent<TransformComponent>();

	ID3DXAnimationController *pController = refActionComp._pAnimationController;
	uint32 numAnimationSet = pController->GetNumAnimationSets();
	ID3DXKeyframedAnimationSet *anim0;

	pController->GetAnimationSetByName(CatAnimationString[CAT_ANIMATION_ENUM::CAT_IDLE], (ID3DXAnimationSet **)&anim0);

	_callbackData._animtionEnum = (CAT_ANIMATION_ENUM *)&_animationEnum;

	D3DXKEY_CALLBACK warSwingLeftKeys;
	warSwingLeftKeys.Time = anim0->GetPeriod() / 1.0f * anim0->GetSourceTicksPerSecond();
	warSwingLeftKeys.pCallbackData = (void *)&_callbackData;

	AddCallbackKeysAndCompress(pController, anim0, 1, &warSwingLeftKeys, D3DXCOMPRESS_DEFAULT, 0.1f);
}

void Cat::QueueAction(const Action & action)
{
	_pActionComp->_actionQueue.PushAction(action);
}

bool Cat::findPlayer(Vector3 forward, Vector3 playerPos, Vector3 myPos, float range1, float range2, float findRadian)
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