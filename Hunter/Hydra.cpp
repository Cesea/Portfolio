#include "stdafx.h"
#include "Hydra.h"
#include "HydraStates.h"

Hydra::Hydra()
{
}

Hydra::~Hydra()
{
}

bool Hydra::CreateFromWorld(World & world, const Vector3 &Pos)
{
	_entity = world.CreateEntity();
	TransformComponent &transComp = _entity.AddComponent<TransformComponent>();

	transComp.SetWorldPosition(Pos);

	static int32 animCount = 0;
	RenderComponent &renderComp = _entity.AddComponent<RenderComponent>();
	renderComp._type = RenderComponent::Type::eSkinned;
	switch (rand()%4)
	{
	case 0:
		_skinType = HYDRASKINSTATE_GREEN;
		renderComp._skinned = VIDEO->CreateAnimationInstance(VIDEO->GetSkinnedXMesh("Hydra1"),
			"Hydra_" + std::to_string(animCount));
		break;
	case 1:
		_skinType = HYDRASKINSTATE_RED;
		renderComp._skinned = VIDEO->CreateAnimationInstance(VIDEO->GetSkinnedXMesh("Hydra2"),
			"Hydra_" + std::to_string(animCount));
		break;
	case 2:
		_skinType = HYDRASKINSTATE_BLACK;
		renderComp._skinned = VIDEO->CreateAnimationInstance(VIDEO->GetSkinnedXMesh("Hydra3"),
			"Hydra_" + std::to_string(animCount));
		break;
	case 3:
		_skinType = HYDRASKINSTATE_GOLD;
		renderComp._skinned = VIDEO->CreateAnimationInstance(VIDEO->GetSkinnedXMesh("Hydra4"),
			"Hydra_" + std::to_string(animCount));
		break;
	}
	renderComp._arche = ARCHE_HYDRA;

	video::AnimationInstance *pAnimation = VIDEO->GetAnimationInstance(renderComp._skinned);

	CollisionComponent &collision = _entity.AddComponent<CollisionComponent>();
	collision._boundingBox.Init(pAnimation->_pSkinnedMesh->_boundInfo._min,
		pAnimation->_pSkinnedMesh->_boundInfo._max);
	collision._boundingSphere._localCenter = pAnimation->_pSkinnedMesh->_boundInfo._center;
	collision._boundingSphere._radius = pAnimation->_pSkinnedMesh->_boundInfo._radius;
	collision._locked = false;
	collision._triggerType = CollisionComponent::TRIGGER_TYPE_ENEMY;
	collision._isTrigger = true;

	ScriptComponent &scriptComponent = _entity.AddComponent<ScriptComponent>();
	scriptComponent.SetScript(MAKE_SCRIPT_DELEGATE(Hydra, Update, *this));

	_pActionComp = &_entity.AddComponent<ActionComponent>();
	_pActionComp->CreateFrom(renderComp._skinned);
	_pActionComp->_pCallbackHandler = new HydraCallbackHandler;
	_pActionComp->_pCallbackHandler->Init(this);
	SetupCallbackAndCompression();

	_pActionComp->MakeAnimationList();
	_pActionComp->SetFirstAction(HYDRA_ANIM(HYDRA_ANIMATION_ENUM::HYDRA_IDLE));

	_entity.Activate();

	_pStateMachine = new HydraStateMachine;
	_pStateMachine->Init(this);
	_pStateMachine->RegisterState(META_TYPE(HydraIdleState)->Name(), new HydraIdleState());
	_pStateMachine->RegisterState(META_TYPE(HydraMoveState)->Name(), new HydraMoveState());
	_pStateMachine->RegisterState(META_TYPE(HydraStandState)->Name(), new HydraStandState());
	_pStateMachine->RegisterState(META_TYPE(HydraAttackState)->Name(), new HydraAttackState());
	_pStateMachine->RegisterState(META_TYPE(HydraAttack2State)->Name(), new HydraAttack2State());
	_pStateMachine->RegisterState(META_TYPE(HydraAttack3State)->Name(), new HydraAttack3State());
	_pStateMachine->RegisterState(META_TYPE(HydraHurt1State)->Name(), new HydraHurt1State());
	_pStateMachine->RegisterState(META_TYPE(HydraHurt2State)->Name(), new HydraHurt2State());
	_pStateMachine->RegisterState(META_TYPE(HydraDeadState)->Name(), new HydraDeadState());
	_pStateMachine->RegisterState(META_TYPE(HydraSpecialAttack1State)->Name(), new HydraSpecialAttack1State());
	_pStateMachine->RegisterState(META_TYPE(HydraSpecialAttack2State)->Name(), new HydraSpecialAttack2State());
	_pStateMachine->RegisterState(META_TYPE(HydraBreath1State)->Name(), new HydraBreath1State());
	_pStateMachine->RegisterState(META_TYPE(HydraBreath2State)->Name(), new HydraBreath2State());
	_pStateMachine->RegisterState(META_TYPE(HydraBreath3State)->Name(), new HydraBreath3State());
	_pStateMachine->ChangeState(META_TYPE(HydraStandState)->Name());

	_speed = 2.0f;
	_rotateSpeed = D3DX_PI / 256;
	_patrolIndex = 0;
	this->PatrolSet(rand() % 3, transComp.GetWorldPosition(), 5.0f);

	_delayTime = 180.0f;
	_delayCount = _delayTime;

	_findDistance = 5.0f;
	_findRadian = D3DX_PI / 3;
	_findStareDistance = 20.0f;
	_roarTime = 210;
	_roarCount = _roarTime;

	_battle = false;

	switch (_skinType)
	{
	case HYDRASKINSTATE_GREEN:
		_atkRange = 5.0f;
		break;
	case HYDRASKINSTATE_RED:
		_atkRange = 8.0f;
		break;
	case HYDRASKINSTATE_BLACK:
		_atkRange = 4.0f;
		break;
	case HYDRASKINSTATE_GOLD:
		_atkRange = 4.0f;
		break;
	}

	_atkTime = 60;
	_atkTime2 = 140;
	_atkCount = _atkTime;

	_standTime = 90;
	_standCount = _standTime;

	_hurtTime = 30;
	_hurtCount = _hurtTime;

	_hp = 500;

	//이벤트 등록
	EventChannel channel;
	channel.Add<CollisionSystem::ActorTriggerEvent, Hydra>(*this);
	setEvent();

	return true; 
}

void Hydra::Update(float deltaTime)
{
	_pStateMachine->Update(deltaTime, _currentCommand);
	TransformComponent &transComp = _entity.GetComponent<TransformComponent>();
	switch (_state)
	{
	case HYDRASTATE_IDLE:
		_delayCount -= 1;
		if (_delayCount <= 0.0f)
		{
			_delayCount = _delayTime;
			_state = HYDRASTATE_PATROL;
			_pStateMachine->ChangeState(META_TYPE(HydraMoveState)->Name());
		}
		break;
	case HYDRASTATE_PATROL:
		if (_moveSegment.empty())
		{
			_pStateMachine->ChangeState(META_TYPE(HydraStandState)->Name());
			_state = HYDRASTATE_IDLE;
		}
		else
		{
			//다음 인덱스로 방향을 얻고
			Vector3 direction = _moveSegment[_patrolIndex] - transComp.GetWorldPosition();
			float distance = Vec3Length(&direction);
			Vec3Normalize(&direction, &direction);
			//몸이 덜 돌아갔는가?
			Vector3 rotatePos = _moveSegment[_patrolIndex];
			rotatePos.y = transComp.GetWorldPosition().y;
			Vector3 rotateDir = rotatePos - transComp.GetWorldPosition();
			Vec3Normalize(&rotateDir, &rotateDir);
			float distRadian = acos(
				ClampMinusOnePlusOne(Vec3Dot(&-rotateDir, &transComp.GetForward())));
			if (distRadian > D3DX_PI) D3DX_PI * 2 - distRadian;
			if (distRadian > _rotateSpeed)
			{
				transComp.LookDirection(-rotateDir, _rotateSpeed);
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
				_pStateMachine->ChangeState(META_TYPE(HydraStandState)->Name());
				_state = HYDRASTATE_IDLE;
			}
			//아니면 이동속도만큼 이동
			else
			{
				transComp.SetWorldPosition(transComp.GetWorldPosition() + direction*_speed*deltaTime);
			}
		}
		break;
	case HYDRASTATE_FIND:
		//roar가 끝나면 플레이어를 추적하는 RUN으로
		_roarCount -= 1;
		if (_roarCount < 0)
		{
			_roarCount = _roarTime;
			_state = HYDRASTATE_RUN;
			_pStateMachine->ChangeState(META_TYPE(HydraMoveState)->Name());
		}
		break;
	case HYDRASTATE_RUN:
	{
		Vector3 direction = _playerPos - transComp.GetWorldPosition();
		float distance = Vec3Length(&direction);
		Vec3Normalize(&direction, &direction);
		Vector3 rotatePos = _playerPos;
		rotatePos.y = transComp.GetWorldPosition().y;
		Vector3 rotateDir = rotatePos - transComp.GetWorldPosition();
		Vec3Normalize(&rotateDir, &rotateDir);
		transComp.LookDirection(-rotateDir, D3DX_PI);
		if (distance < _atkRange)
		{
			switch (_skinType)
			{
			case HYDRASKINSTATE_GREEN:
				_state = HYDRASTATE_ATK1;
				_pStateMachine->ChangeState(META_TYPE(HydraAttackState)->Name());
				break;
			case HYDRASKINSTATE_RED:
				_state = HYDRASTATE_BREATH1;
				_pStateMachine->ChangeState(META_TYPE(HydraBreath1State)->Name());
				_atkCount = _atkTime2;
				break;
			case HYDRASKINSTATE_BLACK:
				_state = HYDRASTATE_SP_ATK1;
				_pStateMachine->ChangeState(META_TYPE(HydraSpecialAttack1State)->Name());
				break;
			case HYDRASKINSTATE_GOLD:
				_state = HYDRASTATE_ATK1;
				_pStateMachine->ChangeState(META_TYPE(HydraAttackState)->Name());
				break;
			}
		}
		else
		{
			transComp.SetWorldPosition(transComp.GetWorldPosition() + direction * _speed * deltaTime * 2);
		}
	}
	break;
	case HYDRASTATE_ATK1:
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
				_state = HYDRASTATE_ATK2;
				_pStateMachine->ChangeState(META_TYPE(HydraAttack2State)->Name());
			}
			//공격범위를 벗어났다?
			else
			{
				//배틀을 멈추고 기본자세 (다시추적시작)
				_battle = false;
				_state = HYDRASTATE_IDLE;
				_pStateMachine->ChangeState(META_TYPE(HydraStandState)->Name());
			}
			Vector3 rotatePos = _playerPos;
			rotatePos.y = transComp.GetWorldPosition().y;
			Vector3 rotateDir = rotatePos - transComp.GetWorldPosition();
			Vec3Normalize(&rotateDir, &rotateDir);
			transComp.LookDirection(-rotateDir, D3DX_PI);
		}
		break;
	case HYDRASTATE_ATK2:
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
				_state = HYDRASTATE_ATK3;
				_pStateMachine->ChangeState(META_TYPE(HydraAttack3State)->Name());
				//_playerPos = Vector3(RandFloat(-5.0, 5.0), 8.0f, RandFloat(-5.0, 5.0));
			}
			//공격범위를 벗어났다?
			else
			{
				//배틀을 멈추고 기본자세 (다시추적시작)
				_battle = false;
				_state = HYDRASTATE_IDLE;
				_pStateMachine->ChangeState(META_TYPE(HydraStandState)->Name());
			}
			Vector3 rotatePos = _playerPos;
			rotatePos.y = transComp.GetWorldPosition().y;
			Vector3 rotateDir = rotatePos - transComp.GetWorldPosition();
			Vec3Normalize(&rotateDir, &rotateDir);
			transComp.LookDirection(-rotateDir, D3DX_PI);
		}
		break;
	case HYDRASTATE_ATK3:
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
				_state = HYDRASTATE_ATK1;
				_pStateMachine->ChangeState(META_TYPE(HydraAttackState)->Name());
			}
			//공격범위를 벗어났다?
			else
			{
				//배틀을 멈추고 기본자세 (다시추적시작)
				_battle = false;
				_state = HYDRASTATE_IDLE;
				_pStateMachine->ChangeState(META_TYPE(HydraStandState)->Name());
			}
			Vector3 rotatePos = _playerPos;
			rotatePos.y = transComp.GetWorldPosition().y;
			Vector3 rotateDir = rotatePos - transComp.GetWorldPosition();
			Vec3Normalize(&rotateDir, &rotateDir);
			transComp.LookDirection(-rotateDir, D3DX_PI);
		}
		break;
	case HYDRASTATE_STAND:
		_standCount--;
		if (_standCount < 0)
		{
			_standCount = _standTime;
			_state = HYDRASTATE_PATROL;
			_pStateMachine->ChangeState(META_TYPE(HydraMoveState)->Name());
		}
		break;
	case HYDRASTATE_HURT:
		_hurtCount--;
		if (_hurtCount < 0)
		{
			_hurtCount = _hurtTime;
			//거리를 계산해서 가까운상태면 어택
			Vector3 direction = _playerPos - transComp.GetWorldPosition();
			float distance = Vec3Length(&direction);
			Vec3Normalize(&direction, &direction);
			if (distance < _atkRange)
			{
				_state = HYDRASTATE_ATK1;
				_pStateMachine->ChangeState(META_TYPE(HydraAttackState)->Name());
			}
			else
			{
				//전투중에 거리가벌어진거라면 roar없이 돌격
				if (_battle)
				{
					_state = HYDRASTATE_RUN;
					_pStateMachine->ChangeState(META_TYPE(HydraMoveState)->Name());
				}
				//비전투인데 맞았다?
				else
				{
					// 추격
					_battle = true;
					_state = HYDRASTATE_FIND;
					_pStateMachine->ChangeState(META_TYPE(HydraIdleState)->Name());
					Vector3 rotatePos = _playerPos;
					rotatePos.y = transComp.GetWorldPosition().y;
					Vector3 rotateDir = rotatePos - transComp.GetWorldPosition();
					Vec3Normalize(&rotateDir, &rotateDir);
					transComp.LookDirection(-rotateDir, D3DX_PI);
				}
			}
		}
		break;
	case HYDRASTATE_DEATH:
		break;
	case HYDRASTATE_SP_ATK1:
		_atkCount--;
		if (_atkCount < 0)
		{
			_atkCount = _atkTime;
			//공격을 마쳤으면 다시한번검사
			Vector3 direction = _playerPos - transComp.GetWorldPosition();
			float distance = Vec3Length(&direction);
			Vec3Normalize(&direction, &direction);
			//공격범위를 벗어났다?
			if (distance > _atkRange)
			{
				//배틀을 멈추고 기본자세 (다시추적시작)
				_battle = false;
				_state = HYDRASTATE_IDLE;
				_pStateMachine->ChangeState(META_TYPE(HydraStandState)->Name());
			}
			Vector3 rotatePos = _playerPos;
			rotatePos.y = transComp.GetWorldPosition().y;
			Vector3 rotateDir = rotatePos - transComp.GetWorldPosition();
			Vec3Normalize(&rotateDir, &rotateDir);
			transComp.LookDirection(-rotateDir, D3DX_PI);
		}
		break;
	case HYDRASTATE_SP_ATK2:
		_atkCount--;
		if (_atkCount < 0)
		{
			_atkCount = _atkTime;
			//공격을 마쳤으면 다시한번검사
			Vector3 direction = _playerPos - transComp.GetWorldPosition();
			float distance = Vec3Length(&direction);
			Vec3Normalize(&direction, &direction);
			_state = HYDRASTATE_RUN;
			_pStateMachine->ChangeState(META_TYPE(HydraMoveState)->Name());
			Vector3 rotatePos = _playerPos;
			rotatePos.y = transComp.GetWorldPosition().y;
			Vector3 rotateDir = rotatePos - transComp.GetWorldPosition();
			Vec3Normalize(&rotateDir, &rotateDir);
			transComp.LookDirection(-rotateDir, D3DX_PI);
		}
		break;
	case HYDRASTATE_BREATH1:
	{
		_atkCount--;
		if (_atkCount < 0)
		{
			_atkCount = _atkTime2;
			//공격을 마쳤으면 다시한번검사
			Vector3 direction = _playerPos - transComp.GetWorldPosition();
			float distance = Vec3Length(&direction);
			Vec3Normalize(&direction, &direction);
			//공격범위를 벗어났다?
			if (distance < _atkRange)
			{
				_state = HYDRASTATE_BREATH2;
				_pStateMachine->ChangeState(META_TYPE(HydraBreath2State)->Name());
			}
			//공격범위를 벗어났다?
			else
			{
				//배틀을 멈추고 기본자세 (다시추적시작)
				_battle = false;
				_state = HYDRASTATE_IDLE;
				_pStateMachine->ChangeState(META_TYPE(HydraStandState)->Name());
			}
		}
		Vector3 rotatePos = _playerPos;
		rotatePos.y = transComp.GetWorldPosition().y;
		Vector3 rotateDir = rotatePos - transComp.GetWorldPosition();
		Vec3Normalize(&rotateDir, &rotateDir);
		transComp.LookDirection(-rotateDir, _rotateSpeed);
	}
	break;
	case HYDRASTATE_BREATH2:
	{
		_atkCount--;
		if (_atkCount < 0)
		{
			_atkCount = _atkTime2;
			//공격을 마쳤으면 다시한번검사
			Vector3 direction = _playerPos - transComp.GetWorldPosition();
			float distance = Vec3Length(&direction);
			Vec3Normalize(&direction, &direction);
			//공격범위를 벗어났다?
			if (distance < _atkRange)
			{
				_state = HYDRASTATE_BREATH3;
				_pStateMachine->ChangeState(META_TYPE(HydraBreath3State)->Name());
			}
			//공격범위를 벗어났다?
			else
			{
				//배틀을 멈추고 기본자세 (다시추적시작)
				_battle = false;
				_state = HYDRASTATE_IDLE;
				_pStateMachine->ChangeState(META_TYPE(HydraStandState)->Name());
			}
		}
		Vector3 rotatePos = _playerPos;
		rotatePos.y = transComp.GetWorldPosition().y;
		Vector3 rotateDir = rotatePos - transComp.GetWorldPosition();
		Vec3Normalize(&rotateDir, &rotateDir);
		transComp.LookDirection(-rotateDir, _rotateSpeed);
	}
	break;
	case HYDRASTATE_BREATH3:
	{
		_atkCount--;
		if (_atkCount < 0)
		{
			_atkCount = _atkTime2;
			//공격을 마쳤으면 다시한번검사
			Vector3 direction = _playerPos - transComp.GetWorldPosition();
			float distance = Vec3Length(&direction);
			Vec3Normalize(&direction, &direction);
			//공격범위를 벗어났다?
			if (distance < _atkRange)
			{
				_state = HYDRASTATE_BREATH1;
				_pStateMachine->ChangeState(META_TYPE(HydraBreath1State)->Name());
			}
			//공격범위를 벗어났다?
			else
			{
				//배틀을 멈추고 기본자세 (다시추적시작)
				_battle = false;
				_state = HYDRASTATE_IDLE;
				_pStateMachine->ChangeState(META_TYPE(HydraStandState)->Name());
			}
		}
		Vector3 rotatePos = _playerPos;
		rotatePos.y = transComp.GetWorldPosition().y;
		Vector3 rotateDir = rotatePos - transComp.GetWorldPosition();
		Vec3Normalize(&rotateDir, &rotateDir);
		transComp.LookDirection(-rotateDir, _rotateSpeed);
	}
		break;
	}
	//전투상태가 아니라면 항시 플레이어를 수색한다.
	if (!_battle)
	{
		if (findPlayer(transComp.GetForward(), _playerPos, transComp.GetWorldPosition(), _findStareDistance, _findDistance, _findRadian))
		{
			_battle = true;
			//바로 뒤에있으면 FIND를 생략하며 SP_ATK2상태가 됌
			Vector3 rotatePos = _playerPos;
			rotatePos.y = transComp.GetWorldPosition().y;
			Vector3 rotateDir = rotatePos - transComp.GetWorldPosition();
			Vec3Normalize(&rotateDir, &rotateDir);
			float distRadian = acos(
				ClampMinusOnePlusOne(Vec3Dot(&-rotateDir, &transComp.GetForward())));
			if (distRadian > D3DX_PI / 4 * 3)
			{
				_state = HYDRASTATE_SP_ATK2;
				_pStateMachine->ChangeState(META_TYPE(HydraSpecialAttack2State)->Name());
			}
			else
			{
				_state = HYDRASTATE_FIND;
				_pStateMachine->ChangeState(META_TYPE(HydraIdleState)->Name());
				transComp.LookDirection(-rotateDir, D3DX_PI * 2);
			}
			//찾으면 FIND가 되며 battle상태가 ROAR가 됌
		}
	}
	transComp.SetWorldPosition(transComp.GetWorldPosition().x, TERRAIN->GetHeight(transComp.GetWorldPosition().x, transComp.GetWorldPosition().z), transComp.GetWorldPosition().z);
}

void Hydra::Handle(const CollisionSystem::ActorTriggerEvent & event)
{
	if (event._entity1 != _entity) return;
	CollisionComponent & _collision = event._entity2.GetComponent<CollisionComponent>();
	switch (_collision._triggerType)
	{
		//플레이어와 충돌했다(내가 가해자)
	case CollisionComponent::TRIGGER_TYPE_PLAYER:
		if (_state != HYDRASTATE_HURT&&_state != HYDRASTATE_DEATH)
		{
			resetAllCount();
			_state = HYDRASTATE_HURT;
			_pStateMachine->ChangeState(META_TYPE(HydraHurt1State)->Name());
			_battle = true;
			_hp -= 50;
			if (_hp <= 0)
			{
				_state = HYDRASTATE_DEATH;
				_pStateMachine->ChangeState(META_TYPE(HydraDeadState)->Name());
			}
		}
		break;
		//오브젝트와 충돌했다
	case CollisionComponent::TRIGGER_TYPE_OBJECT:
		break;
	case CollisionComponent::TRIGGER_TYPE_DEFAULT:
		break;
	}
}

void Hydra::SetupCallbackAndCompression()
{
	ActionComponent &refActionComp = _entity.GetComponent<ActionComponent>();
	TransformComponent &refTransform = _entity.GetComponent<TransformComponent>();

	ID3DXAnimationController *pController = refActionComp._pAnimationController;
	uint32 numAnimationSet = pController->GetNumAnimationSets();
	ID3DXKeyframedAnimationSet *anim0;

	pController->GetAnimationSetByName(HydraAnimationString[HYDRA_ANIMATION_ENUM::HYDRA_IDLE], (ID3DXAnimationSet **)&anim0);

	_callbackData._animtionEnum = (HYDRA_ANIMATION_ENUM *)&_animationEnum;

	D3DXKEY_CALLBACK warSwingLeftKeys;
	warSwingLeftKeys.Time = anim0->GetPeriod() / 1.0f * anim0->GetSourceTicksPerSecond();
	warSwingLeftKeys.pCallbackData = (void *)&_callbackData;

	AddCallbackKeysAndCompress(pController, anim0, 1, &warSwingLeftKeys, D3DXCOMPRESS_DEFAULT, 0.1f);
}

void Hydra::QueueAction(const Action & action)
{
	_pActionComp->_actionQueue.PushAction(action);

}

bool Hydra::findPlayer(Vector3 forward, Vector3 playerPos, Vector3 myPos, float range1, float range2, float findRadian)
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