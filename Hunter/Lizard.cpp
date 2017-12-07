#include "stdafx.h"
#include "Lizard.h"

Lizard::Lizard()
{
}

Lizard::~Lizard()
{
}

bool Lizard::CreateFromWorld(World & world, const Vector3 &Pos)
{
	_entity = world.CreateEntity();
	TransformComponent &transComp = _entity.AddComponent<TransformComponent>();

	transComp.SetWorldPosition(Pos);

	static int32 animCount = 0;
	RenderComponent &renderComp = _entity.AddComponent<RenderComponent>();
	renderComp._type = RenderComponent::Type::eSkinned;
	switch (rand()%2)
	{
	case 0:
		_skinType = LIZARDSKINSTATE_NORMAL;
		renderComp._skinned = VIDEO->CreateAnimationInstance(VIDEO->GetSkinnedXMesh("Lizard1"),
			"Lizard" + std::to_string(animCount));
		break;
	case 1:
		_skinType = LIZARDSKINSTATE_BLACK;
		renderComp._skinned = VIDEO->CreateAnimationInstance(VIDEO->GetSkinnedXMesh("Lizard2"),
			"Lizard" + std::to_string(animCount));
		break;
	}
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
	collision._type = CollisionComponent::COLLISION_TYPE_OBB;

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


	_state = LIZARDSTATE_IDLE;

	_speed = 3.0f;
	_rotateSpeed = D3DX_PI / 64;
	_patrolIndex = 0;
	this->PatrolSet(rand() % 3, transComp.GetWorldPosition(), 5.0f);

	_delayTime = 180.0f;
	_delayCount = _delayTime;

	_findDistance = 3.0f;
	_findRadian = D3DX_PI / 3;
	_findStareDistance = 20.0f;
	_roarTime = 210;
	_roarCount = _roarTime;

	_battle = false;

	_atkRange = 1.6f;
	if (_skinType == LIZARDSKINSTATE_BLACK)
	{
		_atkRange = 2.0f;
	}
	_atkTime = 80;
	_atkTime2 = 90;
	_atkCount = _atkTime;

	_standTime = 90;
	_standCount = _standTime;

	_hurtTime = 40;
	_hurtCount = _hurtTime;

	_hp = 1000;

	_isHurt = false;
	_unBeatableTime = 15;
	_unBeatableCount = _unBeatableTime;

	//�̺�Ʈ ���
	EventChannel channel;

	channel.Broadcast<GameObjectFactory::ObjectCreatedEvent>(
		GameObjectFactory::ObjectCreatedEvent(ARCHE_LIZARD, _entity, transComp.GetWorldPosition()));
	channel.Add<CollisionSystem::ActorTriggerEvent, Lizard>(*this);
	setEvent();
	return true;
}

void Lizard::Update(float deltaTime)
{
	TransformComponent &transComp = _entity.GetComponent<TransformComponent>();
	switch (_state)
	{
	case LIZARDSTATE_IDLE:
		_delayCount -= 1;
		if (_delayCount <= 0.0f)
		{
			_delayCount = _delayTime;
			_state = LIZARDSTATE_PATROL;

			this->QueueAction(LIZARD_ANIM(LIZARD_WALK));
		}
		break;
	case LIZARDSTATE_PATROL:
		if (_moveSegment.empty())
		{
			this->QueueAction(LIZARD_ANIM(LIZARD_STAND));
			_state = LIZARDSTATE_IDLE;
		}
		else
		{
			//���� �ε����� ������ ���
			Vector3 direction = _moveSegment[_patrolIndex] - transComp.GetWorldPosition();
			float distance = Vec3Length(&direction);
			Vec3Normalize(&direction, &direction);
			//���� �� ���ư��°�?
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
			//�̵��ӵ����� �����?
			if (distance < _speed*deltaTime)
			{
				//�Ÿ���ŭ �����̰� patrolIndex����
				transComp.SetWorldPosition(transComp.GetWorldPosition() + direction*distance);
				_patrolIndex++;
				if (_patrolIndex > _moveSegment.size() - 1) _patrolIndex = 0;
				//IDLE �ִϸ��̼� ����
				this->QueueAction(LIZARD_ANIM(LIZARD_STAND));
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
			this->QueueAction(LIZARD_ANIM(LIZARD_RUN));
		}
		break;
	case LIZARDSTATE_RUN:
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
			case LIZARDSKINSTATE_NORMAL:
				_state = LIZARDSTATE_ATK1;
				this->QueueAction(LIZARD_ANIM(LIZARD_ATTACK));
				_atkCount = _atkTime;
				break;
			case LIZARDSKINSTATE_BLACK:
				_state = LIZARDSTATE_ATK3;
				this->QueueAction(LIZARD_ANIM(LIZARD_SPIT));
				_atkCount = _atkTime2;
				break;
			}
		}
		else
		{
			transComp.SetWorldPosition(transComp.GetWorldPosition() + direction * _speed * deltaTime * 3);
		}
	}
	break;
	case LIZARDSTATE_ATK1:
		_atkCount--;
		if (_atkCount == 60)
		{
			Vector3 targetPos = transComp.GetWorldPosition() - transComp.GetForward();
			EventChannel _channel;
			_channel.Broadcast<GameObjectFactory::DamageBoxEvent>(GameObjectFactory::DamageBoxEvent(targetPos,
				Vector3(_atkRange * 0.5f, _atkRange * 0.5f, _atkRange * 0.5f), 10.0f, CollisionComponent::TRIGGER_TYPE_ENEMY_DMGBOX, 0.0f, 0.0f, 1.0f));
		}
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
				this->QueueAction(LIZARD_ANIM(LIZARD_BITE));
			}
			//���ݹ����� �����?
			else
			{
				//��Ʋ�� ���߰� �⺻�ڼ� (�ٽ���������)
				_battle = false;
				_state = LIZARDSTATE_IDLE;
				this->QueueAction(LIZARD_ANIM(LIZARD_STAND));
			}
		}
		break;
	case LIZARDSTATE_ATK2:
		_atkCount--;
		if (_atkCount == 50)
		{
			Vector3 targetPos = transComp.GetWorldPosition() - transComp.GetForward();
			EventChannel _channel;
			_channel.Broadcast<GameObjectFactory::DamageBoxEvent>(GameObjectFactory::DamageBoxEvent(targetPos,
				Vector3(_atkRange * 0.5f, _atkRange * 0.5f, _atkRange * 0.5f), 10.0f, CollisionComponent::TRIGGER_TYPE_ENEMY_DMGBOX, 0.0f, 0.0f, 1.0f));
		}
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
				this->QueueAction(LIZARD_ANIM(LIZARD_ATTACK));
			}
			//���ݹ����� �����?
			else
			{
				//��Ʋ�� ���߰� �⺻�ڼ� (�ٽ���������)
				_battle = false;
				_state = LIZARDSTATE_IDLE;
				this->QueueAction(LIZARD_ANIM(LIZARD_STAND));
			}
		}
		break;
	case LIZARDSTATE_ATK3:
	{
		_atkCount--;
		if (_atkCount < 0)
		{
			_atkCount = _atkTime2;
			//������ �������� �ٽ��ѹ��˻�
			Vector3 direction = _playerPos - transComp.GetWorldPosition();
			float distance = Vec3Length(&direction);
			Vec3Normalize(&direction, &direction);
			if (distance < _atkRange)
			{
				_state = LIZARDSTATE_ATK3;
				this->QueueAction(LIZARD_ANIM(LIZARD_SPIT));
			}
			//���ݹ����� �����?
			else
			{
				//��Ʋ�� ���߰� �⺻�ڼ� (�ٽ���������)
				_battle = false;
				_state = LIZARDSTATE_IDLE;
				this->QueueAction(LIZARD_ANIM(LIZARD_STAND));
			}
		}
		Vector3 rotatePos = _playerPos;
		rotatePos.y = transComp.GetWorldPosition().y;
		Vector3 rotateDir = rotatePos - transComp.GetWorldPosition();
		Vec3Normalize(&rotateDir, &rotateDir);
		transComp.LookDirection(-rotateDir, _rotateSpeed);
	}
		break;
	case LIZARDSTATE_STAND:
		_standCount--;
		if (_standCount < 0)
		{
			_standCount = _standTime;
			_state = LIZARDSTATE_PATROL;
			this->QueueAction(LIZARD_ANIM(LIZARD_WALK));
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
				this->QueueAction(LIZARD_ANIM(LIZARD_ATTACK));
			}
			else
			{
				//�����߿� �Ÿ����������Ŷ�� roar���� ����
				if (_battle)
				{
					_state = LIZARDSTATE_RUN;
					this->QueueAction(LIZARD_ANIM(LIZARD_WALK));
				}
				//�������ε� �¾Ҵ�?
				else
				{
					// �߰�
					_battle = true;
					_state = LIZARDSTATE_FIND;

					this->QueueAction(LIZARD_ANIM(LIZARD_ROAR));

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
			this->QueueAction(LIZARD_ANIM(LIZARD_ROAR));
			Vector3 rotatePos = _playerPos;
			rotatePos.y = transComp.GetWorldPosition().y;
			Vector3 distance = rotatePos - transComp.GetWorldPosition();
			Vec3Normalize(&distance, &distance);
			transComp.LookDirection(-distance, D3DX_PI * 2);
		}
	}

	transComp.SetWorldPosition(transComp.GetWorldPosition().x, 
		TERRAIN->GetHeight(transComp.GetWorldPosition().x, transComp.GetWorldPosition().z), 
		transComp.GetWorldPosition().z);

	_prevTilePos = _tilePos;
	TERRAIN->ConvertWorldPostoTilePos(transComp.GetWorldPosition(), &_tilePos);
	RepositionEntity(_tilePos, _prevTilePos);

	if (_isHurt)
	{
		_unBeatableCount--;
		if (_unBeatableCount < 0)
		{
			_unBeatableCount = _unBeatableTime;
			_isHurt = false;
		}
	}

	if (_isDie)
	{
		_dieCount--;
		if (_dieCount <= 0)
		{
			this->_valid = false;
			EventChannel channel;
			TERRAIN->RemoveEntityInTile(_entity, _tilePos);
			channel.Broadcast<IScene::SceneDirty>(IScene::SceneDirty());
		}
	}
}

void Lizard::Handle(const CollisionSystem::ActorTriggerEvent & event)
{
	if (event._entity2 != _entity) return;
	CollisionComponent & _collision = event._entity1.GetComponent<CollisionComponent>();
	switch (_collision._triggerType)
	{
		//�÷��̾�� �浹�ߴ�(���� ������)
	case CollisionComponent::TRIGGER_TYPE_PLAYER:
		break;
		//������Ʈ�� �浹�ߴ�
	case CollisionComponent::TRIGGER_TYPE_OBJECT:
		break;
	case CollisionComponent::TRIGGER_TYPE_PLAYER_DMGBOX:
		if (_isDie) break;
		if (!_isHurt)
		{
			//�Ϲݰ����߿� ��������
			if (_state != LIZARDSTATE_HURT&&_state != LIZARDSTATE_DEATH&&_state != LIZARDSTATE_ATK1&&_state != LIZARDSTATE_ATK2)
			{
				resetAllCount();
				_state = LIZARDSTATE_HURT;
				this->QueueAction(LIZARD_ANIM(LIZARD_HIT1));
				_battle = true;
			}
			_hp -= 50;
			if (_hp <= 0)
			{
				_state = LIZARDSTATE_DEATH;
				this->QueueAction(LIZARD_ANIM(LIZARD_DEATH));
				_isDie = true;
			}
			_isHurt = true;
			_collision._valid = false;
			EventChannel channel;
			channel.Broadcast<GameObjectFactory::CreateBlood>(
				GameObjectFactory::CreateBlood(_playerPos));
		}
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

void Lizard::QueueAction(Action & action, bool cancle)
{
	action._cancle = cancle;
	_pActionComp->_actionQueue.PushAction(action);
   _animationEnum = action._enum;
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
