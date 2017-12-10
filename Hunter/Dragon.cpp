#include "stdafx.h"
#include "Dragon.h"


Dragon::Dragon()
{
}


Dragon::~Dragon()
{
}

bool Dragon::CreateFromWorld(World & world, const Vector3 & Pos)
{
	_entity = world.CreateEntity();
	TransformComponent &transComp = _entity.AddComponent<TransformComponent>();

	transComp.SetWorldPosition(Pos);

	static int32 animCount = 0;
	RenderComponent &renderComp = _entity.AddComponent<RenderComponent>();
	renderComp._type = RenderComponent::Type::eSkinned;
	renderComp._skinned = VIDEO->CreateAnimationInstance(VIDEO->GetSkinnedXMesh("Dragon"),
		"_Dragon" + std::to_string(animCount));
	renderComp._arche = ARCHE_DRAGON;

	video::AnimationInstance *pAnimation = VIDEO->GetAnimationInstance(renderComp._skinned);

	CollisionComponent &collision = _entity.AddComponent<CollisionComponent>();
	collision._boundingBox.Init(pAnimation->_pSkinnedMesh->_boundInfo._min,
		pAnimation->_pSkinnedMesh->_boundInfo._max);
	collision._boundingSphere._localCenter = pAnimation->_pSkinnedMesh->_boundInfo._center;
	collision._boundingSphere._radius = pAnimation->_pSkinnedMesh->_boundInfo._radius;
	collision._locked = false;
	collision._triggerType = CollisionComponent::TRIGGER_TYPE_ENEMY;
	collision._isTrigger = false;
	collision._type = CollisionComponent::COLLISION_TYPE_OBB;

	ScriptComponent &scriptComponent = _entity.AddComponent<ScriptComponent>();
	scriptComponent.SetScript(MAKE_SCRIPT_DELEGATE(Dragon, Update, *this));

	_pActionComp = &_entity.AddComponent<ActionComponent>();
	_pActionComp->CreateFrom(renderComp._skinned);
	_pActionComp->_pCallbackHandler = new DragonCallbackHandler;
	_pActionComp->_pCallbackHandler->Init(this);
	SetupCallbackAndCompression();

	_pActionComp->MakeAnimationList();
	_pActionComp->SetFirstAction(DRAGON_ANIM(DRAGON_ANIMATION_ENUM::DRAGON_IDLE));

	_entity.Activate();


	//ī��Ʈ ����
	//Dragon Idle Count
	_delayTime = 600.0f;
	_delayCount = _delayTime;
	//Dragon Bite Count
	_biteTime = 40;
	_biteCount = _biteTime;
	_biteNum = 3;
	_biteNumCount = _biteNum;;
	//Dragon Whip_Tail Count
	_whipTime = 80;
	_whipCount = _whipTime;
	//Dragon Fly_Atk Count
	_flyAtkTime = 120;
	_flyAtkCount = _flyAtkTime;


	_normalSpeed = 2.0f;
	_speed = _normalSpeed;
	_maxSpeed = 25.0f;
	_accel = 0.3f;
	_rotateSpeed = D3DX_PI / 32;

	_atkRange = 3.0f;

	_flyHeight = 10.0f;
	_flySegment.push_back(Vector3(50.0f, _flyHeight, 0));
	_flySegment.push_back(Vector3(0, _flyHeight, 50.0f));
	_flySegment.push_back(Vector3(-50.0f, _flyHeight, 0));
	_flySegment.push_back(Vector3(0, _flyHeight, -50.0f));
	_flySegmentIndex = 0;

	_flyNum = 2;
	_flyNumCount = _flyNum;

	_anim = DRAGONANIMSTATE_DEFAULT;
	_pattern = DRAGONPATTERNSTATE_GROUND;
	//�̺�Ʈ ����
	EventChannel channel;
	channel.Broadcast<GameObjectFactory::ObjectCreatedEvent>(
		GameObjectFactory::ObjectCreatedEvent(ARCHE_HYDRA, _entity, transComp.GetWorldPosition()));
	setEvent();

	return true;
}

void Dragon::Update(float deltaTime)
{
	TransformComponent &transComp = _entity.GetComponent<TransformComponent>();

	switch (_state)
	{
	case DRAGONSTATE_START:
	{
		_delayCount--;
		if (_delayCount <= 0)
		{
			_delayCount = _delayTime;
			_state = DRAGONSTATE_MOVE_IN;
			this->QueueAction(DRAGON_ANIM(DRAGON_RUN));
			_moveSegment.empty();
			_moveSegment = GetMoveSegment(transComp.GetWorldPosition());
			_moveSegmentIndex = 0;
			_speed = _normalSpeed;
		}
	}
		break;
	case DRAGONSTATE_MOVE_IN:
	{
		//���� �ε����� ������ ���
		Vector3 direction = _moveSegment[_moveSegmentIndex] - transComp.GetWorldPosition();
		float distance = Vec3Length(&direction);
		Vec3Normalize(&direction, &direction);
		//���� �� ���ư��°�?
		Vector3 rotatePos = _moveSegment[_moveSegmentIndex];
		rotatePos.y = transComp.GetWorldPosition().y;
		Vector3 rotateDir = rotatePos - transComp.GetWorldPosition();
		Vec3Normalize(&rotateDir, &rotateDir);
		float distRadian = acos(
			ClampMinusOnePlusOne(Vec3Dot(&-rotateDir, &transComp.GetForward())));

		if (distRadian > D3DX_PI) D3DX_PI * 2 - distRadian;
		if (distRadian > _rotateSpeed)
		{
			transComp.LookDirection(-rotateDir, _rotateSpeed);
			speedDown();
		}
		else
		{
			speedUp();
		}

		if (distance < _speed*deltaTime)
		{
			_moveSegmentIndex++;
			if (_moveSegmentIndex > _moveSegment.size() - 1) _state= DRAGONSTATE_TRACE;
		}
		transComp.SetWorldPosition(transComp.GetWorldPosition() - transComp.GetForward()*_speed*deltaTime);
	}
		break;
	case DRAGONSTATE_TRACE:
	{
		Vector3 direction = _playerPos - transComp.GetWorldPosition();
		float distance = Vec3Length(&direction);
		Vec3Normalize(&direction, &direction);
		Vector3 rotatePos = _playerPos;
		rotatePos.y = transComp.GetWorldPosition().y;
		Vector3 rotateDir = rotatePos - transComp.GetWorldPosition();
		Vec3Normalize(&rotateDir, &rotateDir);
		transComp.LookDirection(-rotateDir, D3DX_PI);

		transComp.SetWorldPosition(transComp.GetWorldPosition() + direction * _speed * deltaTime);
		if (distance < _atkRange)
		{
			//���������ʴ´�
			_state = DRAGONSTATE_DEFAULT;
			//�����Ѵ�
			_anim = DRAGONANIMSTATE_BITE;
			this->QueueAction(DRAGON_ANIM(DRAGON_ATTACK1));
		}

	}
		break;
	case DRAGONSTATE_MOVE_OUT:
	{
		Vector3 direction = _escapePoint - transComp.GetWorldPosition();
		float distance = Vec3Length(&direction);
		Vec3Normalize(&direction, &direction);

		//���� �� ���ư��°�?
		Vector3 rotatePos = _escapePoint;
		rotatePos.y = transComp.GetWorldPosition().y;
		Vector3 rotateDir = rotatePos - transComp.GetWorldPosition();
		Vec3Normalize(&rotateDir, &rotateDir);
		float distRadian = acos(
			ClampMinusOnePlusOne(Vec3Dot(&-rotateDir, &transComp.GetForward())));

		if (distRadian > D3DX_PI) D3DX_PI * 2 - distRadian;
		if (distRadian > _rotateSpeed)
		{
			transComp.LookDirection(-rotateDir, _rotateSpeed);
			speedDown();
		}
		else
		{
			speedUp();
		}

		//���������� ������ �����ߴ�
		if (distance < _speed*deltaTime)
		{
			_state = DRAGONSTATE_STARTFLY;
			this->QueueAction(DRAGON_ANIM(DRAGON_FLY));
			_pattern = DRAGONPATTERNSTATE_FLY;
		}
		transComp.SetWorldPosition(transComp.GetWorldPosition() - transComp.GetForward()*_speed*deltaTime);
	}
	break;
	case DRAGONSTATE_STARTFLY:
	{
		transComp.SetWorldPosition(transComp.GetWorldPosition() + Vector3(0, 0.02f, 0));
		//�������̻� �ö󰡸� �׸�
		if (transComp.GetWorldPosition().y > _flyHeight)
		{
			_state = DRAGONSTATE_FLY_ROUND;
			_flySegmentIndex = 0;
		}
		Vector3 rotatePos = _escapePoint;
		rotatePos.y = transComp.GetWorldPosition().y;
		Vector3 rotateDir = rotatePos - transComp.GetWorldPosition();
		Vec3Normalize(&rotateDir, &rotateDir);
		transComp.LookDirection(-rotateDir, _rotateSpeed/8);
	}
	break;
	case DRAGONSTATE_FLY_ROUND:
	{
		//���� �ε����� ������ ���
		Vector3 direction = _flySegment[_flySegmentIndex] - transComp.GetWorldPosition();
		float distance = Vec3Length(&direction);
		Vec3Normalize(&direction, &direction);
		//���� �� ���ư��°�?
		Vector3 rotatePos = _flySegment[_flySegmentIndex];
		Vector3 rotateDir = rotatePos - transComp.GetWorldPosition();
		Vec3Normalize(&rotateDir, &rotateDir);
		float distRadian = acos(
			ClampMinusOnePlusOne(Vec3Dot(&-rotateDir, &transComp.GetForward())));
		if (distRadian > D3DX_PI) D3DX_PI * 2 - distRadian;
		if (distRadian > _rotateSpeed)
		{
			transComp.LookDirection(-rotateDir, _rotateSpeed);
		}

		if (distance < _speed*deltaTime)
		{
			_flySegmentIndex++;
			if (_flySegmentIndex > _flySegment.size() - 1)
			{
				_flySegmentIndex = 0;
				
				_flyNumCount--;
				if (_flyNumCount == 0)
				{
					_flyNumCount = _flyNum;
					this->QueueAction(DRAGON_ANIM(DRAGON_FLY_FAST));
					_state = DRAGONSTATE_FLY_TRACE;
				}
			}
		}
		transComp.SetWorldPosition(transComp.GetWorldPosition() - transComp.GetForward()*_speed*deltaTime);
	}
		break;
	case DRAGONSTATE_FLY_TRACE:
	{
		Vector3 targetPos = _playerPos;
		targetPos.y = _flyHeight;
		Vector3 direction = _playerPos - transComp.GetWorldPosition();
		float distance = Vec3Length(&direction);
		Vec3Normalize(&direction, &direction);

		Vector3 rotatePos = _playerPos;
		rotatePos.y = transComp.GetWorldPosition().y;
		Vector3 rotateDir = rotatePos - transComp.GetWorldPosition();
		Vec3Normalize(&rotateDir, &rotateDir);
		transComp.LookDirection(-rotateDir, D3DX_PI);

		transComp.SetWorldPosition(transComp.GetWorldPosition() + direction * _speed * deltaTime);
		if (distance < 10.0f)
		{
			_state = DRAGONSTATE_DEFAULT;
			_anim = DRAGONANIMSTATE_FLY_ATK;
			this->QueueAction(DRAGON_ANIM(DRAGON_FLY_ATTACK));
		}
	}
	break;
	case DRAGONSTATE_FLY_DOWN:
	{

	}
		break;
	case DRAGONSTATE_DEFAULT:
		break;
	}


	switch (_anim)
	{
	case DRAGONANIMSTATE_BITE:
		_biteCount--;
		if (_biteCount == 30)
		{
			Vector3 targetPos = transComp.GetWorldPosition() - transComp.GetForward()*_atkRange;
			EventChannel _channel;
			_channel.Broadcast<GameObjectFactory::DamageBoxEvent>(GameObjectFactory::DamageBoxEvent(targetPos,
				Vector3(2.0f, 2.0f, 2.0f), 10.0f, CollisionComponent::TRIGGER_TYPE_ENEMY_DMGBOX, 0.0f, 0.0f, 1.0f));
		}
		if (_biteCount < 0)
		{
			//���� ī��Ʈ �ʱ�ȭ
			_biteCount = _biteTime;

			_biteNumCount--;
			//���Ӱ����� �����ٸ�?
			if (_biteNumCount == 0)
			{
				_biteNumCount = _biteNum;
				//���ð��� �ش�
				_stopTime = 180;
				_stopCount = _stopTime;
				//STAND ANIMATION�� ����
				_anim = DRAGONANIMSTATE_STAND;
				this->QueueAction(DRAGON_ANIM(DRAGON_STAND));
				//���� ������ �ش�
				_nextAnim = DRAGONANIMSTATE_WHIP_TAIL;
				//ȸ���� ���� ��ǥ
				_escapePoint = -_playerPos;
			}
		}
		break;
	case DRAGONANIMSTATE_WHIP_TAIL:
	{
		_whipCount--;
		if (_whipCount == 30)
		{
			Vector3 targetPos = transComp.GetWorldPosition() + transComp.GetForward();
			EventChannel _channel;
			_channel.Broadcast<GameObjectFactory::DamageBoxEvent>(GameObjectFactory::DamageBoxEvent(targetPos,
				Vector3(4.0f, 2.0f, 4.0f), 10.0f, CollisionComponent::TRIGGER_TYPE_ENEMY_DMGBOX, 0.0f, 0.0f, 1.0f));
		}
		if (_whipCount < 0)
		{
			_whipCount = _whipTime;
			//�ֵθ��� �Ϸ��� ��������
			_anim = DRAGONANIMSTATE_DEFAULT;
			_state = DRAGONSTATE_MOVE_OUT;
			this->QueueAction(DRAGON_ANIM(DRAGON_RUN));

			//������ġ����
			_escapePoint = -_playerPos;
			_escapePoint.y = TERRAIN->GetHeight(_escapePoint.x, _escapePoint.z);
			Vector3 direction = _playerPos - _escapePoint;
			Vec3Normalize(&direction, &direction);
			_escapePoint = transComp.GetWorldPosition() - direction * 50.0f;

		}
		Vector3 rotatePos = _escapePoint;
		rotatePos.y = transComp.GetWorldPosition().y;
		Vector3 rotateDir = rotatePos - transComp.GetWorldPosition();
		Vec3Normalize(&rotateDir, &rotateDir);
		transComp.LookDirection(-rotateDir, _rotateSpeed*2);
	}
		break;
	case DRAGONANIMSTATE_STAND:
		//�־��� ���ð���ŭ STAND�� Next Animation ����
		_stopCount--;
		if (_stopCount < 0)
		{
			switch (_nextAnim)
			{
			case DRAGONANIMSTATE_WHIP_TAIL:
				_anim = _nextAnim;
				this->QueueAction(DRAGON_ANIM(DRAGON_WHIP_TAIL));
				break;
			}
		}
		break;
	case DRAGONANIMSTATE_FLY_ATK:
	{
		_flyAtkCount--;
		//���ݹڽ�����
		if (_flyAtkCount == 70)
		{
			Vector3 targetPos = transComp.GetWorldPosition() + transComp.GetForward();
			targetPos.y = TERRAIN->GetHeight(targetPos.x, targetPos.z);
			EventChannel _channel;
			_channel.Broadcast<GameObjectFactory::DamageBoxEvent>(GameObjectFactory::DamageBoxEvent(targetPos,
				Vector3(2.0f, 2.0f, 2.0f), 10.0f, CollisionComponent::TRIGGER_TYPE_ENEMY_DMGBOX, 0.0f, 0.0f, 1.0f));
		}

		//Forward�� �����ϸ鼭 �����Ѵ�
		if (_flyAtkCount > 60)
		{
			transComp.SetWorldPosition(transComp.GetWorldPosition() -
				transComp.GetForward()*10.0f*deltaTime);
		}
		else
		{
			transComp.SetWorldPosition(transComp.GetWorldPosition()-
				transComp.GetForward()*10.0f*deltaTime);
			

			transComp.SetWorldPosition(transComp.GetWorldPosition() + Vector3(0, 0.2f, 0));
			//�������̻� �ö󰡸� �׸�
			if (transComp.GetWorldPosition().y > _flyHeight) 
            transComp.SetWorldPosition(transComp.GetWorldPosition().x, _flyHeight,transComp.GetWorldPosition().z);
			
		}
		
		if (_flyAtkCount < 0)
		{
			_flyAtkNumCount--;
			if (_flyAtkNumCount == 0)
			{
				_flyAtkNumCount = _flyAtkCount;

				_anim = DRAGONANIMSTATE_DEFAULT;
				_state = DRAGONSTATE_FLY_DOWN;
				this->QueueAction(DRAGON_ANIM(DRAGON_FLY));
			}
			else
			{
				_anim = DRAGONANIMSTATE_DEFAULT;
				_state = DRAGONSTATE_FLY_ROUND;
				this->QueueAction(DRAGON_ANIM(DRAGON_FLY));
			}

		}

	}
	break;
	case DRAGONANIMSTATE_DEFAULT:
		break;
	}




	if(_pattern != DRAGONPATTERNSTATE_FLY)
	transComp.SetWorldPosition(transComp.GetWorldPosition().x,
		TERRAIN->GetHeight(transComp.GetWorldPosition().x, transComp.GetWorldPosition().z),
		transComp.GetWorldPosition().z);

	_prevTilePos = _tilePos;
	TERRAIN->ConvertWorldPostoTilePos(transComp.GetWorldPosition(), &_tilePos);
	RepositionEntity(_tilePos, _prevTilePos);
}

void Dragon::Handle(const CollisionSystem::ActorTriggerEvent & event)
{
}

void Dragon::SetupCallbackAndCompression()
{
	ActionComponent &refActionComp = _entity.GetComponent<ActionComponent>();
	TransformComponent &refTransform = _entity.GetComponent<TransformComponent>();

	ID3DXAnimationController *pController = refActionComp._pAnimationController;
	uint32 numAnimationSet = pController->GetNumAnimationSets();
	ID3DXKeyframedAnimationSet *anim0;

	pController->GetAnimationSetByName(DragonAnimationString[DRAGON_ANIMATION_ENUM::DRAGON_IDLE], (ID3DXAnimationSet **)&anim0);

	_callbackData._animtionEnum = (DRAGON_ANIMATION_ENUM *)&_animationEnum;

	D3DXKEY_CALLBACK warSwingLeftKeys;
	warSwingLeftKeys.Time = anim0->GetPeriod() / 1.0f * anim0->GetSourceTicksPerSecond();
	warSwingLeftKeys.pCallbackData = (void *)&_callbackData;

	AddCallbackKeysAndCompress(pController, anim0, 1, &warSwingLeftKeys, D3DXCOMPRESS_DEFAULT, 0.1f);
}

void Dragon::QueueAction(Action & action, bool cancle)
{
	action._cancle = cancle;
	_pActionComp->_actionQueue.PushAction(action);
	_animationEnum = action._enum;
}

std::vector<Vector3> Dragon::GetMoveSegment(Vector3 myPos)
{
	std::vector<Vector3> moveSeg;
	moveSeg.empty();
	Vector3 toPlayer = myPos - _playerPos;
	float distance = Vec3Length(&toPlayer);
	Vec3Normalize(&toPlayer, &toPlayer);
	toPlayer = -toPlayer;
	//ù��° moveSegMent ����
	Vector3 firstVec = toPlayer;
	//�������⸸ŭ ȸ�� 60��~60��
	firstVec = rotateVector(firstVec, RandFloat(D3DX_PI / 12, D3DX_PI / 10));
	//��ǥ������ ��ǥ��?
	Vector3 targetPos = myPos+firstVec*distance/4;
	//��ǥ������ y�� ������ �����Ų��
	targetPos.y = TERRAIN->GetHeight(targetPos.x, targetPos.z);
	//����
	moveSeg.push_back(targetPos);

	//�ι�° moveSegMent ����
	Vector3 SecondVec = toPlayer;
	//�������⸸ŭ ȸ�� -45��~-60��
	SecondVec = rotateVector(SecondVec, -RandFloat(D3DX_PI / 12, D3DX_PI / 10));
	//��ǥ������ ��ǥ��?
	targetPos = myPos + SecondVec*distance/2;
	//��ǥ������ y�� ������ �����Ų��
	targetPos.y = TERRAIN->GetHeight(targetPos.x, targetPos.z);
	//����
	moveSeg.push_back(targetPos);

	//����° moveSegMent ����
	Vector3 ThirdVec = toPlayer;
	//�������⸸ŭ ȸ�� -45��~-60��
	ThirdVec = rotateVector(ThirdVec, RandFloat(D3DX_PI / 12, D3DX_PI / 10));
	//��ǥ������ ��ǥ��?
	targetPos = myPos + ThirdVec*distance/4*3;
	//��ǥ������ y�� ������ �����Ų��
	targetPos.y = TERRAIN->GetHeight(targetPos.x, targetPos.z);
	//����
	moveSeg.push_back(targetPos);
	return moveSeg;
}

void Dragon::speedUp()
{
	_speed += _accel;
	if (_speed > _maxSpeed) _speed = _maxSpeed;
}

void Dragon::speedDown()
{
	_speed -= _accel*3;
	if (_speed < _normalSpeed) _speed = _normalSpeed;
}

Vector3 Dragon::rotateVector(Vector3 vec, float angle)
{
	Matrix matVec;
	MatrixTranslation(&matVec, vec.x, vec.y, vec.z);

	Matrix matRotate;
	MatrixRotationY(&matRotate, angle);

	Matrix matFinal =  matVec * matRotate;

	Vector3 finalVec;
	Vector3 t;
	Vec3TransformCoord(&finalVec, &t, &matFinal);

	return finalVec;
}
