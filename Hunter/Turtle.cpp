#include "stdafx.h"
#include "Turtle.h"
#include "TurtleStates.h"

Turtle::Turtle()
{
}


Turtle::~Turtle()
{
}

bool Turtle::CreateFromWorld(World & world)
{
	_entity = world.CreateEntity();
	TransformComponent &transComp = _entity.AddComponent<TransformComponent>();
	transComp.MovePositionWorld(0, 6.0f, 0);

	static int32 animCount = 0;
	RenderComponent &renderComp = _entity.AddComponent<RenderComponent>();
	renderComp._type = RenderComponent::Type::eSkinned;
	renderComp._skinned = VIDEO->CreateAnimationInstance(VIDEO->GetSkinnedXMesh("Turtle"), 
		"Turtle_" + std::to_string(animCount));
	renderComp._arche = ARCHE_TURTLE;

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
	scriptComponent.SetScript(MAKE_SCRIPT_DELEGATE(Turtle, Update, *this));

	_pActionComp = &_entity.AddComponent<ActionComponent>();
	_pActionComp->CreateFrom(renderComp._skinned);
	_pActionComp->_pCallbackHandler = new TurtleCallbackHandler;
	_pActionComp->_pCallbackHandler->Init(this);
	SetupCallbackAndCompression();

	_pActionComp->MakeAnimationList();
	_pActionComp->SetFirstAction(TURTLE_ANIM(TURTLE_ANIMATION_ENUM::TURTLE_STAND));

	_entity.Activate();

	
	_pStateMachine = new TurtleStateMachine;
	_pStateMachine->Init(this);
	_pStateMachine->RegisterState(META_TYPE(TurtleIdleState)->Name(), new TurtleIdleState());
	_pStateMachine->RegisterState(META_TYPE(TurtleMoveState)->Name(), new TurtleMoveState());
	_pStateMachine->RegisterState(META_TYPE(TurtleBite1State)->Name(), new TurtleBite1State());
	_pStateMachine->RegisterState(META_TYPE(TurtleBite2State)->Name(), new TurtleBite2State());
	_pStateMachine->RegisterState(META_TYPE(TurtleHurt1State)->Name(), new TurtleHurt1State());
	_pStateMachine->RegisterState(META_TYPE(TurtleHurt2State)->Name(), new TurtleHurt2State());
	_pStateMachine->RegisterState(META_TYPE(TurtleDeadState)->Name(), new TurtleDeadState());
	_pStateMachine->RegisterState(META_TYPE(TurtleFindState)->Name(), new TurtleFindState());
	_pStateMachine->ChangeState(META_TYPE(TurtleBite1State)->Name());
	_state = TURTLESTATE_STAND;

	_entity.Activate();


	_speed = 1.0f;
	_rotateSpeed = D3DX_PI / 256;
	_patrolIndex = 0;
	_moveSegment.push_back(Vector3(5.0f, TERRAIN->GetHeight(5.0f, 5.0f), 5.0f));
	_moveSegment.push_back(Vector3(-5.0f, TERRAIN->GetHeight(-5.0f,6.0f), 6.0f));
	_moveSegment.push_back(Vector3(-5.0f, TERRAIN->GetHeight(-5.0f, -5.0f), -5.0f));
	_delayTime = 180.0f;
	_delayCount = _delayTime;

	_findDistance = 1.0f;
	_findRadian = D3DX_PI / 3;
	_findStareDistance = 5.0f;
	_roarTime = 180;
	_roarCount = _roarTime;

	_battle = false;

	_playerPos = Vector3(-5.0f, 6.0f, 5.0f);

	_atkRange = 0.5f;
	_atkTime = 90;
	_atkCount = _atkTime;

	_standTime = 84;
	_standCount = _standTime;

	_hp = 300;

	_hurtTime = 60;
	_hurtCount = _hurtTime;

	//이벤트 등록
	EventChannel channel;
	channel.Add<CollisionSystem::ActorTriggerEvent, Turtle>(*this);
	setEvent();
	return true;
}

void Turtle::Update(float deltaTime)
{
	_pStateMachine->Update(deltaTime, _currentCommand);
	_pStateMachine->Update(deltaTime, _currentCommand);
	TransformComponent &transComp = _entity.GetComponent<TransformComponent>();

	switch (_state)
	{
	case TURTLESTATE_STAND:
		_delayCount -= 1;
		if (_delayCount <= 0.0f)
		{
			_delayCount = _delayTime;
			_state = TURTLESTATE_WALK;
			_pStateMachine->ChangeState(META_TYPE(TurtleMoveState)->Name());
		}
		break;
	case TURTLESTATE_WALK:
		if (_moveSegment.empty())
		{
			_pStateMachine->ChangeState(META_TYPE(TurtleIdleState)->Name());
			_state = TURTLESTATE_STAND;
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
				_pStateMachine->ChangeState(META_TYPE(TurtleIdleState)->Name());
				_state = TURTLESTATE_STAND;
			}
			//아니면 이동속도만큼 이동
			else
			{
				transComp.SetWorldPosition(transComp.GetWorldPosition() + direction*_speed*deltaTime);
			}

		}
		break;
	case TURTLESTATE_FIND:
		//roar가 끝나면 플레이어를 추적하는 RUN으로
		_roarCount -= 1;
		if (_roarCount < 0)
		{
			_roarCount = _roarTime;
			_state = TURTLESTATE_TRACE;
			_pStateMachine->ChangeState(META_TYPE(TurtleMoveState)->Name());
		}
		break;
	case TURTLESTATE_TRACE:
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
			_state = TURTLESTATE_ATK1;
			_pStateMachine->ChangeState(META_TYPE(TurtleBite1State)->Name());
		}
		else
		{
			transComp.SetWorldPosition(transComp.GetWorldPosition() + direction * _speed * deltaTime * 2);
		}
	}
	break;
	case TURTLESTATE_ATK1:
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
				_state = TURTLESTATE_ATK2;
				_pStateMachine->ChangeState(META_TYPE(TurtleBite2State)->Name());
				_playerPos = Vector3(RandFloat(-5.0, 5.0), 5.0f, RandFloat(-5.0, 5.0));
			}
			//공격범위를 벗어났다?
			else
			{
				//배틀을 멈추고 기본자세 (다시추적시작)
				_battle = false;
				_state = TURTLESTATE_STAND;
				_pStateMachine->ChangeState(META_TYPE(TurtleIdleState)->Name());
			}
		}
		break;
	case TURTLESTATE_ATK2:
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
				_state = TURTLESTATE_ATK1;
				_pStateMachine->ChangeState(META_TYPE(TurtleBite1State)->Name());
			}
			//공격범위를 벗어났다?
			else
			{
				//배틀을 멈추고 기본자세 (다시추적시작)
				_battle = false;
				_state = TURTLESTATE_STAND;
				_pStateMachine->ChangeState(META_TYPE(TurtleIdleState)->Name());
			}
		}
		break;
	case TURTLESTATE_HURT:
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
				_state = TURTLESTATE_ATK1;
				_pStateMachine->ChangeState(META_TYPE(TurtleBite1State)->Name());
			}
			else
			{
				//전투중에 거리가벌어진거라면 roar없이 돌격
				if (_battle)
				{
					_state = TURTLESTATE_TRACE;
					_pStateMachine->ChangeState(META_TYPE(TurtleMoveState)->Name());
				}
				//비전투인데 맞았다?
				else
				{
					// 추격
					_battle = true;
					_state = TURTLESTATE_FIND;
					_pStateMachine->ChangeState(META_TYPE(TurtleFindState)->Name());
					Vector3 distance = _playerPos - transComp.GetWorldPosition();
					Vec3Normalize(&distance, &distance);
					transComp.LookDirection(-distance, D3DX_PI * 2);
				}
			}
		}
		break;
	case TURTLESTATE_DEATH:
		break;
	}
	//전투상태가 아니라면 항시 플레이어를 수색한다.
	if (!_battle)
	{
		if (findPlayer(transComp.GetForward(), _playerPos, transComp.GetWorldPosition(), _findStareDistance, _findDistance, _findRadian))
		{
			//찾으면 FIND가 되며 battle상태가 
			_battle = true;
			_state = TURTLESTATE_FIND;
			_pStateMachine->ChangeState(META_TYPE(TurtleFindState)->Name());
			Vector3 rotatePos = _playerPos;
			rotatePos.y = transComp.GetWorldPosition().y;
			Vector3 distance = rotatePos - transComp.GetWorldPosition();
			Vec3Normalize(&distance, &distance);
			transComp.LookDirection(-distance, D3DX_PI * 2);
		}
	}

	transComp.SetWorldPosition(transComp.GetWorldPosition().x, TERRAIN->GetHeight(transComp.GetWorldPosition().x, transComp.GetWorldPosition().z), transComp.GetWorldPosition().z);
}

void Turtle::Handle(const CollisionSystem::ActorTriggerEvent & event)
{
	if (event._entity1 != _entity) return;
	CollisionComponent & _collision = event._entity2.GetComponent<CollisionComponent>();
	switch (_collision._triggerType)
	{
		//플레이어와 충돌했다(내가 가해자)
	case CollisionComponent::TRIGGER_TYPE_PLAYER:
		if (_state != TURTLESTATE_HURT&&_state != TURTLESTATE_DEATH)
		{
			resetAllCount();
			_state = TURTLESTATE_HURT;
			_pStateMachine->ChangeState(META_TYPE(TurtleHurt1State)->Name());
			_battle = true;
			_hp -= 50;
			if (_hp <= 0)
			{
				_state = TURTLESTATE_DEATH;
				_pStateMachine->ChangeState(META_TYPE(TurtleDeadState)->Name());
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

void Turtle::SetupCallbackAndCompression()
{
	ActionComponent &refActionComp = _entity.GetComponent<ActionComponent>();
	TransformComponent &refTransform = _entity.GetComponent<TransformComponent>();

	ID3DXAnimationController *pController = refActionComp._pAnimationController;
	uint32 numAnimationSet = pController->GetNumAnimationSets();
	ID3DXKeyframedAnimationSet *anim0;

	pController->GetAnimationSetByName(TurtleAnimationString[TURTLE_ANIMATION_ENUM::TURTLE_STAND], (ID3DXAnimationSet **)&anim0);

	_callbackData._animtionEnum = (TURTLE_ANIMATION_ENUM *)&_animationEnum;

	D3DXKEY_CALLBACK warSwingLeftKeys;
	warSwingLeftKeys.Time = anim0->GetPeriod() / 1.0f * anim0->GetSourceTicksPerSecond();
	warSwingLeftKeys.pCallbackData = (void *)&_callbackData;

	AddCallbackKeysAndCompress(pController, anim0, 1, &warSwingLeftKeys, D3DXCOMPRESS_DEFAULT, 0.1f);
}

void Turtle::QueueAction(const Action & action)
{
	_pActionComp->_actionQueue.PushAction(action);
}

bool Turtle::findPlayer(Vector3 forward, Vector3 playerPos, Vector3 myPos, float range1, float range2, float findRadian)
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