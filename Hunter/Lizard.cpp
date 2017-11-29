#include "stdafx.h"
#include "Lizard.h"
#include "LizardStates.h"

Lizard::Lizard()
{
}

Lizard::~Lizard()
{
}

bool Lizard::CreateFromWorld(World & world)
{
	_entity = world.CreateEntity();
	TransformComponent &transComp = _entity.AddComponent<TransformComponent>();
	transComp.MovePositionWorld(0, 12.0f, 0);

	static int32 animCount = 0;
	RenderComponent &renderComp = _entity.AddComponent<RenderComponent>();
	renderComp._type = RenderComponent::Type::eSkinned;
	renderComp._skinned = VIDEO->CreateAnimationInstance(VIDEO->GetSkinnedXMesh("Lizard"), 
		"Lizard" + std::to_string(animCount));
	renderComp._arche = ARCHE_LIZARD;

	video::AnimationInstance *pAnimation = VIDEO->GetAnimationInstance(renderComp._skinned);
	CollisionComponent &collision = _entity.AddComponent<CollisionComponent>();
	collision._boundingBox.Init(pAnimation->_pSkinnedMesh->_boundInfo._min,
		pAnimation->_pSkinnedMesh->_boundInfo._max);
	collision._boundingSphere._localCenter = pAnimation->_pSkinnedMesh->_boundInfo._center;
	collision._boundingSphere._radius = pAnimation->_pSkinnedMesh->_boundInfo._radius;
	collision._locked = false;
	collision._isTrigger = true;
	collision._triggerType = CollisionComponent::TRIGGER_TYPE_ENEMY;

	ScriptComponent &scriptComponent = _entity.AddComponent<ScriptComponent>();
	scriptComponent.SetScript(MAKE_SCRIPT_DELEGATE(Lizard, Update, *this));

	_pActionComp = &_entity.AddComponent<ActionComponent>();
	_pActionComp->CreateFrom(renderComp._skinned);
	_pActionComp->_pCallbackHandler = new LizardCallbackHandler;
	_pActionComp->_pCallbackHandler->Init(this);
	SetupCallbackAndCompression();

	_pActionComp->MakeAnimationList();
	_pActionComp->SetFirstAction(LIZARD_ANIM(LIZARD_ANIMATION_ENUM::LIZARD_IDLE));

	_entity.Activate();

	_pStateMachine = new LizardStateMachine;
	_pStateMachine->Init(this);
	_pStateMachine->RegisterState(META_TYPE(LizardIdleState)->Name(), new LizardIdleState());
	_pStateMachine->RegisterState(META_TYPE(LizardMoveState)->Name(), new LizardMoveState());
	_pStateMachine->RegisterState(META_TYPE(LizardMove2State)->Name(), new LizardMove2State());
	_pStateMachine->RegisterState(META_TYPE(LizardAttackState)->Name(), new LizardAttackState());
	_pStateMachine->RegisterState(META_TYPE(LizardAttack2State)->Name(), new LizardAttack2State());
	_pStateMachine->RegisterState(META_TYPE(LizardAttack3State)->Name(), new LizardAttack3State());
	_pStateMachine->RegisterState(META_TYPE(LizardFindState)->Name(), new LizardFindState());
	_pStateMachine->RegisterState(META_TYPE(LizardStandState)->Name(), new LizardStandState());
	_pStateMachine->RegisterState(META_TYPE(LizardHurt1State)->Name(), new LizardHurt1State());
	_pStateMachine->RegisterState(META_TYPE(LizardHurt2State)->Name(), new LizardHurt2State());
	_pStateMachine->RegisterState(META_TYPE(LizardDeadState)->Name(), new LizardDeadState());
	_pStateMachine->ChangeState(META_TYPE(LizardIdleState)->Name());

	_state = LIZARDSTATE_IDLE;

	_speed = 3.0f;
	_rotateSpeed = D3DX_PI / 64;
	_patrolIndex = 0;
	_moveSegment.push_back(Vector3(5.0f, 12.0f, 5.0f));
	_moveSegment.push_back(Vector3(-5.0f, 12.0f, 5.0f));
	_moveSegment.push_back(Vector3(-5.0f, 12.0f, -5.0f));

	_delayTime = 180.0f;
	_delayCount = _delayTime;

	_findDistance = 3.0f;
	_findRadian = D3DX_PI / 3;
	_findStareDistance = 20.0f;
	_roarTime = 210;
	_roarCount = _roarTime;

	_battle = false;

	_playerPos = Vector3(5.0f, 12.0f, 5.0f);

	_atkRange = 0.5f;
	_atkTime = 80;
	_atkCount = _atkTime;

	_standTime = 90;
	_standCount = _standTime;

	_hurtTime = 40;
	_hurtCount = _hurtTime;

	_hp = 1000;

	//�̺�Ʈ ���
	EventChannel channel;
	channel.Add<CollisionSystem::ActorTriggerEvent, Lizard>(*this);
	return true;
}

void Lizard::Update(float deltaTime)
{
	_pStateMachine->Update(deltaTime, _currentCommand);
	TransformComponent &transComp = _entity.GetComponent<TransformComponent>();
	switch (_state)
	{
	case LIZARDSTATE_IDLE:
		_delayCount -= 1;
		if (_delayCount <= 0.0f)
		{
			_delayCount = _delayTime;
			_state = LIZARDSTATE_PATROL;
			_pStateMachine->ChangeState(META_TYPE(LizardMoveState)->Name());
		}
		break;
	case LIZARDSTATE_PATROL:
		if (_moveSegment.empty())
		{
			_pStateMachine->ChangeState(META_TYPE(LizardStandState)->Name());
			_state = LIZARDSTATE_IDLE;
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
				_pStateMachine->ChangeState(META_TYPE(LizardStandState)->Name());
				_state = LIZARDSTATE_IDLE;
			}
			//�ƴϸ� �̵��ӵ���ŭ �̵�
			else
			{
				transComp.SetWorldPosition(transComp.GetWorldPosition() + direction*_speed*deltaTime);
			}

		}
		break;
	case LIZARDSTATE_FIND:
		//roar�� ������ �÷��̾ �����ϴ� RUN����
		_roarCount -= 1;
		if (_roarCount < 0)
		{
			_roarCount = _roarTime;
			_state = LIZARDSTATE_RUN;
			_pStateMachine->ChangeState(META_TYPE(LizardMove2State)->Name());
		}
		break;
	case LIZARDSTATE_RUN:
	{
		Vector3 direction = _playerPos - transComp.GetWorldPosition();
		float distance = Vec3Length(&direction);
		Vec3Normalize(&direction, &direction);
		if (distance < _atkRange)
		{
			_state = LIZARDSTATE_ATK1;
			_pStateMachine->ChangeState(META_TYPE(LizardAttackState)->Name());
		}
		else
		{
			transComp.SetWorldPosition(transComp.GetWorldPosition() + direction * _speed * deltaTime * 3);
		}
	}
	break;
	case LIZARDSTATE_ATK1:
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
				_state = LIZARDSTATE_ATK2;
				_pStateMachine->ChangeState(META_TYPE(LizardAttack2State)->Name());
			}
			//���ݹ����� �����?
			else
			{
				//��Ʋ�� ���߰� �⺻�ڼ� (�ٽ���������)
				_battle = false;
				_state = LIZARDSTATE_IDLE;
				_pStateMachine->ChangeState(META_TYPE(LizardStandState)->Name());
			}
		}
		break;
	case LIZARDSTATE_ATK2:
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
				_state = LIZARDSTATE_ATK3;
				_pStateMachine->ChangeState(META_TYPE(LizardAttack3State)->Name());
				_playerPos = Vector3(RandFloat(-5.0, 5.0), 12.0f, RandFloat(-5.0, 5.0));
			}
			//���ݹ����� �����?
			else
			{
				//��Ʋ�� ���߰� �⺻�ڼ� (�ٽ���������)
				_battle = false;
				_state = LIZARDSTATE_IDLE;
				_pStateMachine->ChangeState(META_TYPE(LizardStandState)->Name());
			}
		}
		break;
	case LIZARDSTATE_ATK3:
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
				_state = LIZARDSTATE_ATK1;
				_pStateMachine->ChangeState(META_TYPE(LizardAttackState)->Name());
			}
			//���ݹ����� �����?
			else
			{
				//��Ʋ�� ���߰� �⺻�ڼ� (�ٽ���������)
				_battle = false;
				_state = LIZARDSTATE_IDLE;
				_pStateMachine->ChangeState(META_TYPE(LizardStandState)->Name());
			}
		}
		break;
	case LIZARDSTATE_STAND:
		_standCount--;
		if (_standCount < 0)
		{
			_standCount = _standTime;
			_state = LIZARDSTATE_PATROL;
			_pStateMachine->ChangeState(META_TYPE(LizardMoveState)->Name());
		}
		break;
	case LIZARDSTATE_HURT:
		_hurtCount--;
		if (_hurtCount < 0)
		{
			_hurtCount = _hurtTime;
			//�Ÿ��� ����ؼ� �������¸� ����
			Vector3 direction = _playerPos - transComp.GetWorldPosition();
			float distance = Vec3Length(&direction);
			Vec3Normalize(&direction, &direction);
			if (distance < _atkRange)
			{
				_state = LIZARDSTATE_ATK1;
				_pStateMachine->ChangeState(META_TYPE(LizardAttackState)->Name());
			}
			else
			{
				//�����߿� �Ÿ����������Ŷ�� roar���� ����
				if (_battle)
				{
					_state = LIZARDSTATE_RUN;
					_pStateMachine->ChangeState(META_TYPE(LizardMoveState)->Name());
				}
				//�������ε� �¾Ҵ�?
				else
				{
					// �߰�
					_battle = true;
					_state = LIZARDSTATE_FIND;
					_pStateMachine->ChangeState(META_TYPE(LizardFindState)->Name());
					Vector3 distance = _playerPos - transComp.GetWorldPosition();
					Vec3Normalize(&distance, &distance);
					transComp.LookDirection(-distance, D3DX_PI * 2);
				}
			}
		}
		break;
	case LIZARDSTATE_DEATH:
		break;
	}
	//�������°� �ƴ϶�� �׽� �÷��̾ �����Ѵ�.
	if (!_battle)
	{
		if (findPlayer(transComp.GetForward(), _playerPos, transComp.GetWorldPosition(), _findStareDistance, _findDistance, _findRadian))
		{
			//ã���� FIND�� �Ǹ� battle���°� ��
			_battle = true;
			_state = LIZARDSTATE_FIND;
			_pStateMachine->ChangeState(META_TYPE(LizardFindState)->Name());
			Vector3 distance = _playerPos - transComp.GetWorldPosition();
			Vec3Normalize(&distance, &distance);
			transComp.LookDirection(-distance, D3DX_PI * 2);
		}
	}
	transComp.SetWorldPosition(transComp.GetWorldPosition().x, TERRAIN->GetHeight(transComp.GetWorldPosition().x, transComp.GetWorldPosition().z), transComp.GetWorldPosition().z);
}

void Lizard::Handle(const CollisionSystem::ActorTriggerEvent & event)
{
	if (event._entity1 != _entity) return;
	CollisionComponent & _collision = event._entity2.GetComponent<CollisionComponent>();
	switch (_collision._triggerType)
	{
		//�÷��̾�� �浹�ߴ�(���� ������)
	case CollisionComponent::TRIGGER_TYPE_PLAYER:
		if (_state != LIZARDSTATE_HURT&&_state != LIZARDSTATE_DEATH)
		{
			resetAllCount();
			_state = LIZARDSTATE_HURT;
			_pStateMachine->ChangeState(META_TYPE(LizardHurt1State)->Name());
			_battle = true;
			_hp -= 50;
			if (_hp <= 0)
			{
				_state = LIZARDSTATE_DEATH;
				_pStateMachine->ChangeState(META_TYPE(LizardDeadState)->Name());
			}
		}
		break;
		//������Ʈ�� �浹�ߴ�
	case CollisionComponent::TRIGGER_TYPE_OBJECT:
		break;
	case CollisionComponent::TRIGGER_TYPE_DEFAULT:
		break;
	}

}

void Lizard::SetupCallbackAndCompression()
{
	ActionComponent &refActionComp = _entity.GetComponent<ActionComponent>();
	TransformComponent &refTransform = _entity.GetComponent<TransformComponent>();

	ID3DXAnimationController *pController = refActionComp._pAnimationController;
	uint32 numAnimationSet = pController->GetNumAnimationSets();
	ID3DXKeyframedAnimationSet *anim0;

	pController->GetAnimationSetByName(LizardAnimationString[LIZARD_ANIMATION_ENUM::LIZARD_IDLE], (ID3DXAnimationSet **)&anim0);

	_callbackData._animtionEnum = (LIZARD_ANIMATION_ENUM *)&_animationEnum;

	D3DXKEY_CALLBACK warSwingLeftKeys;
	warSwingLeftKeys.Time = anim0->GetPeriod() / 1.0f * anim0->GetSourceTicksPerSecond();
	warSwingLeftKeys.pCallbackData = (void *)&_callbackData;

	AddCallbackKeysAndCompress(pController, anim0, 1, &warSwingLeftKeys, D3DXCOMPRESS_DEFAULT, 0.1f);
}

void Lizard::QueueAction(const Action & action)
{
	_pActionComp->_actionQueue.PushAction(action);
}

bool Lizard::findPlayer(Vector3 forward, Vector3 playerPos, Vector3 myPos, float range1, float range2, float findRadian)
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
