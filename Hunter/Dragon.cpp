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


	//카운트 세팅
	//Dragon Idle Count
	_delayTime = 600.0f;
	_delayCount = _delayTime;
	//Dragon Bite Count
	_biteTime = 40;
	_biteCount = _biteTime;
	//Dragon Whip_Tail Count
	_whipTime = 80;
	_whipCount = _whipTime;


	_normalSpeed = 2.0f;
	_speed = _normalSpeed;
	_maxSpeed = 25.0f;
	_accel = 0.3f;
	_rotateSpeed = D3DX_PI / 32;

	_atkRange = 3.0f;

	_anim = DRAGONANIMSTATE_DEFAULT;

	//이벤트 세팅
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
		//다음 인덱스로 방향을 얻고
		Vector3 direction = _moveSegment[_moveSegmentIndex] - transComp.GetWorldPosition();
		float distance = Vec3Length(&direction);
		Vec3Normalize(&direction, &direction);
		//몸이 덜 돌아갔는가?
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
			//움직이지않는다
			_state = DRAGONSTATE_DEFAULT;
			//공격한다
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

		//몸이 덜 돌아갔는가?
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

		//도망지점에 무사히 도착했다
		if (distance < _speed*deltaTime)
		{
			//다음패턴 ㄱㄱ
		}
		transComp.SetWorldPosition(transComp.GetWorldPosition() - transComp.GetForward()*_speed*deltaTime);
	}
	break;
	case DRAGONSTATE_STARTFLY:
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
			//물기 카운트 초기화
			_biteCount = _biteTime;
			//대기시간을 준다
			_stopTime = 240;
			_stopCount = _stopTime;
			//STAND ANIMATION을 실행
			_anim = DRAGONANIMSTATE_STAND;
			this->QueueAction(DRAGON_ANIM(DRAGON_STAND));
			//다음 동작을 준다
			_nextAnim = DRAGONANIMSTATE_WHIP_TAIL;
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
			//휘두르기 완료후 도망간다
			_anim = DRAGONANIMSTATE_DEFAULT;
			_state = DRAGONSTATE_MOVE_OUT;
			this->QueueAction(DRAGON_ANIM(DRAGON_RUN));
			_escapePoint = -_playerPos;
		}
		Vector3 rotatePos = _escapePoint;
		rotatePos.y = transComp.GetWorldPosition().y;
		Vector3 rotateDir = rotatePos - transComp.GetWorldPosition();
		Vec3Normalize(&rotateDir, &rotateDir);
		transComp.LookDirection(-rotateDir, _rotateSpeed*2);
	}
		break;
	case DRAGONANIMSTATE_STAND:
		//주어진 대기시간만큼 STAND후 Next Animation 실행
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
	case DRAGONANIMSTATE_DEFAULT:
		break;
	}





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
	//첫번째 moveSegMent 추출
	Vector3 firstVec = toPlayer;
	//랜덤방향만큼 회전 60도~60도
	firstVec = rotateVector(firstVec, RandFloat(D3DX_PI / 12, D3DX_PI / 6));
	//목표지점의 좌표는?
	Vector3 targetPos = myPos+firstVec*distance/4;
	//목표지점의 y를 지형에 적용시킨다
	targetPos.y = TERRAIN->GetHeight(targetPos.x, targetPos.z);
	//적용
	moveSeg.push_back(targetPos);

	//두번째 moveSegMent 추출
	Vector3 SecondVec = toPlayer;
	//랜덤방향만큼 회전 -45도~-60도
	SecondVec = rotateVector(SecondVec, -RandFloat(D3DX_PI / 12, D3DX_PI / 6));
	//목표지점의 좌표는?
	targetPos = myPos + SecondVec*distance/2;
	//목표지점의 y를 지형에 적용시킨다
	targetPos.y = TERRAIN->GetHeight(targetPos.x, targetPos.z);
	//적용
	moveSeg.push_back(targetPos);

	//세번째 moveSegMent 추출
	Vector3 ThirdVec = toPlayer;
	//랜덤방향만큼 회전 -45도~-60도
	ThirdVec = rotateVector(ThirdVec, RandFloat(D3DX_PI / 12, D3DX_PI / 6));
	//목표지점의 좌표는?
	targetPos = myPos + ThirdVec*distance/4*3;
	//목표지점의 y를 지형에 적용시킨다
	targetPos.y = TERRAIN->GetHeight(targetPos.x, targetPos.z);
	//적용
	moveSeg.push_back(targetPos);

	//마지막은 플레이어의 Pos로 한다
	moveSeg.push_back(_playerPos);

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
