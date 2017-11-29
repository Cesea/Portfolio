#include "stdafx.h"
#include "Hydra.h"
#include "HydraStates.h"

Hydra::Hydra()
{
}

Hydra::~Hydra()
{
}

bool Hydra::CreateFromWorld(World & world)
{
	_entity = world.CreateEntity();
	TransformComponent &transComp = _entity.AddComponent<TransformComponent>();
	transComp.MovePositionWorld(0, 8.0f, 0);

	RenderComponent &renderComp = _entity.AddComponent<RenderComponent>();
	renderComp._type = RenderComponent::Type::eSkinned;
	renderComp._skinned = VIDEO->CreateAnimationInstance(VIDEO->GetSkinnedXMesh("Hydra"), "Anim" + std::to_string(0));
	renderComp._arche = ARCHE_HYDRA;

	video::AnimationInstance *pAnimation = VIDEO->GetAnimationInstance(renderComp._skinned);
	CollisionComponent &collision = _entity.AddComponent<CollisionComponent>();
	collision._boundingBox.Init(pAnimation->_pSkinnedMesh->_boundInfo._min,
		pAnimation->_pSkinnedMesh->_boundInfo._max);
	collision._boundingSphere._localCenter = pAnimation->_pSkinnedMesh->_boundInfo._center;
	collision._boundingSphere._radius = pAnimation->_pSkinnedMesh->_boundInfo._radius;
	collision._locked = false;

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
	_pStateMachine->ChangeState(META_TYPE(HydraStandState)->Name());

	_speed = 2.0f;
	_rotateSpeed = D3DX_PI / 256;
	_patrolIndex = 0;
	_moveSegment.push_back(Vector3(5.0f, 8.0f, 5.0f));
	_moveSegment.push_back(Vector3(-5.0f, 8.0f, 5.0f));
	_moveSegment.push_back(Vector3(-5.0f, 8.0f, -5.0f));

	_delayTime = 180.0f;
	_delayCount = _delayTime;

	_findDistance = 3.0f;
	_findRadian = D3DX_PI / 3;
	_findStareDistance = 20.0f;
	_roarTime = 210;
	_roarCount = _roarTime;

	_battle = false;

	_playerPos = Vector3(5.0f, 8.0f, 5.0f);

	_atkRange = 0.5f;
	_atkTime = 60;
	_atkCount = _atkTime;

	_standTime = 90;
	_standCount = _standTime;
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
			//���� �ε����� ������ ���
			Vector3 direction = _moveSegment[_patrolIndex] - transComp.GetWorldPosition();
			float distance = Vec3Length(&direction);
			Vec3Normalize(&direction, &direction);
			//���� �� ���ư��°�?
			float distRadian = acos(
				ClampMinusOnePlusOne(Vec3Dot(&-direction, &transComp.GetForward())));
			if (distRadian > D3DX_PI) D3DX_PI * 2 - distRadian;
			if (distRadian > _rotateSpeed)
			{
				transComp.LookDirection(-direction, _rotateSpeed);
				break;
			}
			//�̵��ӵ����� �����?
			if (distance < _speed*deltaTime)
			{
				//�Ÿ���ŭ �����̰� patrolIndex����
				transComp.SetWorldPosition(transComp.GetWorldPosition() + direction*distance);
				_patrolIndex++;
				if (_patrolIndex > _moveSegment.size() - 1) _patrolIndex = 0;
				//IDLE �ִϸ��̼� ����
				_pStateMachine->ChangeState(META_TYPE(HydraStandState)->Name());
				_state = HYDRASTATE_IDLE;
			}
			//�ƴϸ� �̵��ӵ���ŭ �̵�
			else
			{
				transComp.SetWorldPosition(transComp.GetWorldPosition() + direction*_speed*deltaTime);
			}

		}
		break;
	case HYDRASTATE_FIND:
		//roar�� ������ �÷��̾ �����ϴ� RUN����
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
		if (distance < _atkRange)
		{
			_state = HYDRASTATE_ATK1;
			_pStateMachine->ChangeState(META_TYPE(HydraAttackState)->Name());
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
			//������ �������� �ٽ��ѹ��˻�
			Vector3 direction = _playerPos - transComp.GetWorldPosition();
			float distance = Vec3Length(&direction);
			Vec3Normalize(&direction, &direction);
			if (distance < _atkRange)
			{
				_state = HYDRASTATE_ATK2;
				_pStateMachine->ChangeState(META_TYPE(HydraAttack2State)->Name());
			}
			//���ݹ����� �����?
			else
			{
				//��Ʋ�� ���߰� �⺻�ڼ� (�ٽ���������)
				_battle = false;
				_state = HYDRASTATE_IDLE;
				_pStateMachine->ChangeState(META_TYPE(HydraStandState)->Name());
			}
		}
		break;
	case HYDRASTATE_ATK2:
		_atkCount--;
		if (_atkCount < 0)
		{
			_atkCount = _atkTime;
			//������ �������� �ٽ��ѹ��˻�
			Vector3 direction = _playerPos - transComp.GetWorldPosition();
			float distance = Vec3Length(&direction);
			Vec3Normalize(&direction, &direction);
			if (distance < _atkRange)
			{
				_state = HYDRASTATE_ATK3;
				_pStateMachine->ChangeState(META_TYPE(HydraAttack3State)->Name());
				_playerPos = Vector3(RandFloat(-5.0, 5.0), 8.0f, RandFloat(-5.0, 5.0));
			}
			//���ݹ����� �����?
			else
			{
				//��Ʋ�� ���߰� �⺻�ڼ� (�ٽ���������)
				_battle = false;
				_state = HYDRASTATE_IDLE;
				_pStateMachine->ChangeState(META_TYPE(HydraStandState)->Name());
			}
		}
		break;
	case HYDRASTATE_ATK3:
		_atkCount--;
		if (_atkCount < 0)
		{
			_atkCount = _atkTime;
			//������ �������� �ٽ��ѹ��˻�
			Vector3 direction = _playerPos - transComp.GetWorldPosition();
			float distance = Vec3Length(&direction);
			Vec3Normalize(&direction, &direction);
			if (distance < _atkRange)
			{
				_state = HYDRASTATE_ATK1;
				_pStateMachine->ChangeState(META_TYPE(HydraAttackState)->Name());
			}
			//���ݹ����� �����?
			else
			{
				//��Ʋ�� ���߰� �⺻�ڼ� (�ٽ���������)
				_battle = false;
				_state = HYDRASTATE_IDLE;
				_pStateMachine->ChangeState(META_TYPE(HydraStandState)->Name());
			}
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
	}
	//�������°� �ƴ϶�� �׽� �÷��̾ �����Ѵ�.
	if (!_battle)
	{
		if (findPlayer(transComp.GetForward(), _playerPos, transComp.GetWorldPosition(), _findStareDistance, _findDistance, _findRadian))
		{
			//ã���� FIND�� �Ǹ� battle���°� ��
			_battle = true;
			_state = HYDRASTATE_FIND;
			_pStateMachine->ChangeState(META_TYPE(HydraIdleState)->Name());
			Vector3 distance = _playerPos - transComp.GetWorldPosition();
			Vec3Normalize(&distance, &distance);
			transComp.LookDirection(-distance, D3DX_PI * 2);
		}
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
	//�þ߰��� 1/2���� �۴ٸ� range1 ��ġ
	if (distRadian < findRadian / 2)
	{
		if (distance < range1)
		{
			return true;
		}
	}
	//�ƴ϶�� range2 ��ġ
	else
	{
		if (distance < range2)
		{
			return true;
		}
	}

	return false;
}