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
	transComp.MovePositionWorld(0, 5.0f, 0);

	RenderComponent &renderComp = _entity.AddComponent<RenderComponent>();
	renderComp._type = RenderComponent::Type::eSkinned;
	renderComp._skinned = VIDEO->CreateAnimationInstance(VIDEO->GetSkinnedXMesh("Snake"), "Anim" + std::to_string(0));

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
	_pStateMachine->ChangeState(META_TYPE(SnakeMoveState)->Name());
	_state = SNAKESTATE_PATROL;
	
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
			//´ÙÀ½ ÀÎµ¦½º·Î ¹æÇâÀ» ¾ò°í
			Vector3 direction = _moveSegment[_patrolIndex] - transComp.GetWorldPosition();
			float distance = Vec3Length(&direction);
			Vec3Normalize(&direction, &direction);
			//¸öÀÌ ´ú µ¹¾Æ°¬´Â°¡?
			float distRadian = acos(
				ClampMinusOnePlusOne(Vec3Dot(&-direction, &transComp.GetForward())));
			if (distRadian > D3DX_PI) D3DX_PI*2- distRadian;
			if (distRadian > _rotateSpeed)
			{
				transComp.LookDirection(-direction, _rotateSpeed);
				break;
			}
			//ÀÌµ¿¼Óµµº¸´Ù °¡±î¿ò?
			if (distance < _speed*deltaTime)
			{
				//°Å¸®¸¸Å­ ¿òÁ÷ÀÌ°í patrolIndexº¯°æ
				transComp.SetWorldPosition(transComp.GetWorldPosition() + direction*distance);
			    _patrolIndex++;
				if (_patrolIndex > _moveSegment.size() - 1) _patrolIndex = 0;
				//IDLE ¾Ö´Ï¸ÞÀÌ¼Ç ½ÇÇà
				_pStateMachine->ChangeState(META_TYPE(SnakeIdleState)->Name());
				_state = SNAKESTATE_IDLE;
			}
			//¾Æ´Ï¸é ÀÌµ¿¼Óµµ¸¸Å­ ÀÌµ¿
			else
			{
				transComp.SetWorldPosition(transComp.GetWorldPosition() + direction*_speed*deltaTime);
			}

		}
		break;
	case SNAKESTATE_FIND:
		//roar°¡ ³¡³ª¸é ÇÃ·¹ÀÌ¾î¸¦ ÃßÀûÇÏ´Â RUNÀ¸·Î
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
			//°ø°ÝÀ» ¸¶ÃÆÀ¸¸é ´Ù½ÃÇÑ¹ø°Ë»ç
			Vector3 direction = _playerPos - transComp.GetWorldPosition();
			float distance = Vec3Length(&direction);
			Vec3Normalize(&direction, &direction);
			if (distance < _atkRange)
			{
				_state = SNAKESTATE_ATK3;
				_pStateMachine->ChangeState(META_TYPE(SnakeAttack3State)->Name());
				_playerPos = Vector3(RandFloat(-5.0, 5.0), 5.0f, RandFloat(-5.0, 5.0));
			}
			//°ø°Ý¹üÀ§¸¦ ¹þ¾î³µ´Ù?
			else
			{
				//¹èÆ²À» ¸ØÃß°í ±âº»ÀÚ¼¼ (´Ù½ÃÃßÀû½ÃÀÛ)
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
			//°ø°ÝÀ» ¸¶ÃÆÀ¸¸é ´Ù½ÃÇÑ¹ø°Ë»ç
			Vector3 direction = _playerPos - transComp.GetWorldPosition();
			float distance = Vec3Length(&direction);
			Vec3Normalize(&direction, &direction);
			if (distance < _atkRange)
			{
				_state = SNAKESTATE_ATK1;
				_pStateMachine->ChangeState(META_TYPE(SnakeAttackState)->Name());
			}
			//°ø°Ý¹üÀ§¸¦ ¹þ¾î³µ´Ù?
			else
			{
				//¹èÆ²À» ¸ØÃß°í ±âº»ÀÚ¼¼ (´Ù½ÃÃßÀû½ÃÀÛ)
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
	}
	//ÀüÅõ»óÅÂ°¡ ¾Æ´Ï¶ó¸é Ç×½Ã ÇÃ·¹ÀÌ¾î¸¦ ¼ö»öÇÑ´Ù.
	if (!_battle)
	{
		if (findPlayer(transComp.GetForward(), _playerPos, transComp.GetWorldPosition(), _findStareDistance, _findDistance, _findRadian))
		{
			//Ã£À¸¸é FIND°¡ µÇ¸ç battle»óÅÂ°¡ ‰Î
			_battle = true;
			_state = SNAKESTATE_FIND;
			_pStateMachine->ChangeState(META_TYPE(SnakeFindState)->Name());
			Vector3 distance = _playerPos - transComp.GetWorldPosition();
			Vec3Normalize(&distance, &distance);
			transComp.LookDirection(-distance, D3DX_PI * 2);
		}
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

bool Snake::findPlayer(Vector3 forward,Vector3 playerPos, Vector3 myPos, float range1, float range2, float findRadian)
{
	Vector3 toPlayer = playerPos - myPos;
	float distance = Vec3Length(&toPlayer);
	Vec3Normalize(&toPlayer, &toPlayer);

	float distRadian = acos(
		ClampMinusOnePlusOne(Vec3Dot(&forward, &-toPlayer)));
	if (distRadian > D3DX_PI) D3DX_PI * 2 - distRadian;
	//½Ã¾ß°¢ÀÇ 1/2º¸´Ù ÀÛ´Ù¸é range1 ¼­Ä¡
	if (distRadian < findRadian / 2)
	{
		if (distance < range1)
		{
			return true;
		}
	}
	//¾Æ´Ï¶ó¸é range2 ¼­Ä¡
	else
	{
		if (distance < range2)
		{
			return true;
		}
	}

	return false;
}
<<<<<<< HEAD

void Snake::render()
{
	GIZMOMANAGER->Circle(_playerPos, 0.25f, Vector3(0, 1, 0), 0xFFFF0000);
}
=======
>>>>>>> parent of 4902ea6... Gizmoì¶”ê°€
