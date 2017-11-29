#include "stdafx.h"
#include "Snake.h"
#include "SnakeStates.h"

Snake::Snake()
{
}

Snake::~Snake()
{
}

bool Snake::CreateFromWorld(World & world)
{
	_entity = world.CreateEntity();
	TransformComponent &transComp = _entity.AddComponent<TransformComponent>();
	transComp.MovePositionWorld(0, 0.0f, 0);

	static int32 animCount = 0;

	RenderComponent &renderComp = _entity.AddComponent<RenderComponent>();
	renderComp._type = RenderComponent::Type::eSkinned;
	renderComp._skinned = VIDEO->CreateAnimationInstance(VIDEO->GetSkinnedXMesh("Snake"), 
		"Snake_" + std::to_string(animCount));
	renderComp._arche = ARCHE_SNAKE;

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
	scriptComponent.SetScript(MAKE_SCRIPT_DELEGATE(Snake, Update, *this));

	_pActionComp = &_entity.AddComponent<ActionComponent>();
	_pActionComp->CreateFrom(renderComp._skinned);
	_pActionComp->_pCallbackHandler = new SnakeCallbackHandler;
	_pActionComp->_pCallbackHandler->Init(this);
	SetupCallbackAndCompression();

	_pActionComp->MakeAnimationList();
	_pActionComp->SetFirstAction(SNAKE_ANIM(SNAKE_ANIMATION_ENUM::SNAKE_IDLE));

	_entity.Activate();

	_pStateMachine = new SnakeStateMachine;
	_pStateMachine->Init(this);
	_pStateMachine->RegisterState(META_TYPE(SnakeIdleState)->Name(), new SnakeIdleState());
	_pStateMachine->RegisterState(META_TYPE(SnakeMoveState)->Name(), new SnakeMoveState());
	_pStateMachine->RegisterState(META_TYPE(SnakeFindState)->Name(), new SnakeFindState());
	_pStateMachine->RegisterState(META_TYPE(SnakeAttackState)->Name(), new SnakeAttackState());
	_pStateMachine->RegisterState(META_TYPE(SnakeAttack2State)->Name(), new SnakeAttack2State());
	_pStateMachine->RegisterState(META_TYPE(SnakeAttack3State)->Name(), new SnakeAttack3State());
	_pStateMachine->RegisterState(META_TYPE(SnakeStandState)->Name(), new SnakeStandState());
	_pStateMachine->RegisterState(META_TYPE(SnakeDeadState)->Name(), new SnakeDeadState());
	_pStateMachine->RegisterState(META_TYPE(SnakeHurtState)->Name(), new SnakeHurtState());
	_pStateMachine->ChangeState(META_TYPE(SnakeMoveState)->Name());
	_state = SNAKESTATE_PATROL;
	
	_hp = 500;

	_speed = 3.0f;
	_rotateSpeed = D3DX_PI / 128;
	_patrolIndex = 0;
	_moveSegment.push_back(Vector3(5.0f, 5.0f, 5.0f));
	_moveSegment.push_back(Vector3(-5.0f, 5.0f, 5.0f));
	_moveSegment.push_back(Vector3(-5.0f, 5.0f, -5.0f));

	_delayTime = 180.0f;
	_delayCount = _delayTime;

	_findDistance =	1.0f;
	_findRadian = D3DX_PI / 3;
	_findStareDistance = 20.0f;
	_roarTime = 180;
	_roarCount = _roarTime;

	_battle = false;

	_playerPos = Vector3(5.0f, 5.0f, 5.0f);
	
	_atkRange = 0.5f;
	_atkTime = 60;
	_atkCount = _atkTime;

	_standTime = 90;
	_standCount = _standTime;

	_hurtTime = 60;
	_hurtCount = _hurtTime;
	//이벤트 등록
	EventChannel channel;
	channel.Add<CollisionSystem::ActorTriggerEvent, Snake>(*this);
	setEvent();
	return true;
}

void Snake::Update(float deltaTime)
{
	_pStateMachine->Update(deltaTime, _currentCommand);
	TransformComponent &transComp = _entity.GetComponent<TransformComponent>();
	switch (_state)
	{
	case SNAKESTATE_IDLE:
		_delayCount -= 1;
		if (_delayCount <= 0.0f)
		{
			_delayCount = _delayTime;
			_state = SNAKESTATE_PATROL;
			_pStateMachine->ChangeState(META_TYPE(SnakeMoveState)->Name());
		}
		break;
	case SNAKESTATE_PATROL:
		if (_moveSegment.empty())
		{
			_pStateMachine->ChangeState(META_TYPE(SnakeIdleState)->Name());
			_state = SNAKESTATE_IDLE;
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
			if (distRadian > D3DX_PI) D3DX_PI*2- distRadian;
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
				_pStateMachine->ChangeState(META_TYPE(SnakeIdleState)->Name());
				_state = SNAKESTATE_IDLE;
			}
			//아니면 이동속도만큼 이동
			else
			{
				transComp.SetWorldPosition(transComp.GetWorldPosition() + direction*_speed*deltaTime);
			}

		}
		break;
	case SNAKESTATE_FIND:
		//roar가 끝나면 플레이어를 추적하는 RUN으로
		_roarCount -= 1;
		if (_roarCount < 0)
		{
			_roarCount = _roarTime;
			_state = SNAKESTATE_RUN;
			_pStateMachine->ChangeState(META_TYPE(SnakeMoveState)->Name());
		}
		break;
	case SNAKESTATE_RUN:
	{
		Vector3 direction = _playerPos - transComp.GetWorldPosition();
		float distance = Vec3Length(&direction);
		Vec3Normalize(&direction, &direction);
		if (distance < _atkRange)
		{
			_state = SNAKESTATE_ATK1;
			_pStateMachine->ChangeState(META_TYPE(SnakeAttackState)->Name());
		}
		else
		{
			transComp.SetWorldPosition(transComp.GetWorldPosition() + direction * _speed * deltaTime * 2);
		}
	}
		break;
	case SNAKESTATE_ATK1:
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
				_state = SNAKESTATE_ATK3;
				_pStateMachine->ChangeState(META_TYPE(SnakeAttack3State)->Name());
				_playerPos = Vector3(RandFloat(-5.0, 5.0), 5.0f, RandFloat(-5.0, 5.0));
			}
			//공격범위를 벗어났다?
			else
			{
				//배틀을 멈추고 기본자세 (다시추적시작)
				_battle = false;
				_state = SNAKESTATE_IDLE;
				_pStateMachine->ChangeState(META_TYPE(SnakeIdleState)->Name());
			}
		}
		break;
	case SNAKESTATE_ATK2:
		break;
	case SNAKESTATE_ATK3:
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
				_state = SNAKESTATE_ATK1;
				_pStateMachine->ChangeState(META_TYPE(SnakeAttackState)->Name());
			}
			//공격범위를 벗어났다?
			else
			{
				//배틀을 멈추고 기본자세 (다시추적시작)
				_battle = false;
				_state = SNAKESTATE_STAND;
				_pStateMachine->ChangeState(META_TYPE(SnakeStandState)->Name());
			}
		}
		break;
	case SNAKESTATE_STAND:
		_standCount--;
		if (_standCount < 0)
		{
			_standCount = _standTime;
			_state = SNAKESTATE_PATROL;
			_pStateMachine->ChangeState(META_TYPE(SnakeMoveState)->Name());
		}
		break;
	case SNAKESTATE_HURT:
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
				_state = SNAKESTATE_ATK1;
				_pStateMachine->ChangeState(META_TYPE(SnakeAttackState)->Name());
			}
			else
			{
				//전투중에 거리가벌어진거라면 roar없이 돌격
				if (_battle)
				{
					_state = SNAKESTATE_RUN;
					_pStateMachine->ChangeState(META_TYPE(SnakeMoveState)->Name());
				}
				//비전투인데 맞았다?
				else
				{
					// 추격
					_battle = true;
					_state = SNAKESTATE_FIND;
					_pStateMachine->ChangeState(META_TYPE(SnakeFindState)->Name());
					Vector3 distance = _playerPos - transComp.GetWorldPosition();
					Vec3Normalize(&distance, &distance);
					transComp.LookDirection(-distance, D3DX_PI * 2);
				}
			}
		}
		break;
	case SNAKESTATE_DIE:
		break;
	}
	//전투상태가 아니라면 항시 플레이어를 수색한다.
	if (!_battle)
	{
		if (findPlayer(transComp.GetForward(), _playerPos, transComp.GetWorldPosition(), _findStareDistance, _findDistance, _findRadian))
		{
			//찾으면 FIND가 되며 battle상태가 됌
			_battle = true;
			_state = SNAKESTATE_FIND;
			_pStateMachine->ChangeState(META_TYPE(SnakeFindState)->Name());
			Vector3 distance = _playerPos - transComp.GetWorldPosition();
			Vec3Normalize(&distance, &distance);
			transComp.LookDirection(-distance, D3DX_PI * 2);
		}
	}

	transComp.SetWorldPosition(transComp.GetWorldPosition().x, TERRAIN->GetHeight(transComp.GetWorldPosition().x, transComp.GetWorldPosition().z), transComp.GetWorldPosition().z); 
}

void Snake::Handle(const CollisionSystem::ActorTriggerEvent & event)
{

	if (event._entity1 != _entity) return;
	CollisionComponent & _collision = event._entity2.GetComponent<CollisionComponent>();
	switch (_collision._triggerType)
	{
		//플레이어와 충돌했다(내가 가해자)
	case CollisionComponent::TRIGGER_TYPE_PLAYER:
		if (_state != SNAKESTATE_HURT&&_state != SNAKESTATE_DIE)
		{
			resetAllCount();
			_state = SNAKESTATE_HURT;
			_pStateMachine->ChangeState(META_TYPE(SnakeHurtState)->Name());
			_battle = true;
			_hp -= 50;
			if (_hp <= 0)
			{
				_state = SNAKESTATE_DIE;
				_pStateMachine->ChangeState(META_TYPE(SnakeDeadState)->Name());
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

void Snake::SetupCallbackAndCompression()
{
	ActionComponent &refActionComp = _entity.GetComponent<ActionComponent>();
	TransformComponent &refTransform = _entity.GetComponent<TransformComponent>();

	ID3DXAnimationController *pController = refActionComp._pAnimationController;
	uint32 numAnimationSet = pController->GetNumAnimationSets();
	ID3DXKeyframedAnimationSet *anim0;

	pController->GetAnimationSetByName(SnakeAnimationString[SNAKE_ANIMATION_ENUM::SNAKE_IDLE], (ID3DXAnimationSet **)&anim0);

	_callbackData._animtionEnum = (SNAKE_ANIMATION_ENUM *)&_animationEnum;

	D3DXKEY_CALLBACK warSwingLeftKeys;
	warSwingLeftKeys.Time = anim0->GetPeriod() / 1.0f * anim0->GetSourceTicksPerSecond();
	warSwingLeftKeys.pCallbackData = (void *)&_callbackData;

	AddCallbackKeysAndCompress(pController, anim0, 1, &warSwingLeftKeys, D3DXCOMPRESS_DEFAULT, 0.1f);
}

void Snake::QueueAction(const Action & action)
{
	_pActionComp->_actionQueue.PushAction(action);
}

bool Snake::findPlayer(Vector3 forward, Vector3 playerPos, Vector3 myPos, float range1, float range2, float findRadian)
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

void Snake::render()
{
	GIZMOMANAGER->Circle(_playerPos, 0.25f, Vector3(0, 1, 0), 0xFFFF0000);
}
