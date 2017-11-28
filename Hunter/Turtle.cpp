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

	RenderComponent &renderComp = _entity.AddComponent<RenderComponent>();
	renderComp._type = RenderComponent::Type::eSkinned;
	renderComp._skinned = VIDEO->CreateAnimationInstance(VIDEO->GetSkinnedXMesh("Turtle"), "Animal" + std::to_string(0));

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
	_pStateMachine->RegisterState(META_TYPE(TurtleFindState)->Name(), new TurtleFindState());
	_pStateMachine->ChangeState(META_TYPE(TurtleBite1State)->Name());
	_state = TURTLESTATE_STAND;

	_entity.Activate();


	_speed = 1.0f;
	_rotateSpeed = D3DX_PI / 256;
	_patrolIndex = 0;
	_moveSegment.push_back(Vector3(5.0f, 6.0f, 5.0f));
	_moveSegment.push_back(Vector3(-5.0f, 6.0f, 5.0f));
	_moveSegment.push_back(Vector3(-5.0f, 6.0f, -5.0f));
	_delayTime = 180.0f;
	_delayCount = _delayTime;

	_findDistance = 1.0f;
	_findRadian = D3DX_PI / 3;
	_findStareDistance = 3.0f;
	_roarTime = 180;
	_roarCount = _roarTime;

	_battle = false;

	_playerPos = Vector3(-5.0f, 6.0f, 5.0f);

	_atkRange = 0.5f;
	_atkTime = 90;
	_atkCount = _atkTime;

	_standTime = 84;
	_standCount = _standTime;
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
				_pStateMachine->ChangeState(META_TYPE(TurtleIdleState)->Name());
				_state = TURTLESTATE_STAND;
			}
			//�ƴϸ� �̵��ӵ���ŭ �̵�
			else
			{
				transComp.SetWorldPosition(transComp.GetWorldPosition() + direction*_speed*deltaTime);
			}

		}
		break;
	case TURTLESTATE_FIND:
		//roar�� ������ �÷��̾ �����ϴ� RUN����
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
			//������ �������� �ٽ��ѹ��˻�
			Vector3 direction = _playerPos - transComp.GetWorldPosition();
			float distance = Vec3Length(&direction);
			Vec3Normalize(&direction, &direction);
			if (distance < _atkRange)
			{
				_state = TURTLESTATE_ATK2;
				_pStateMachine->ChangeState(META_TYPE(TurtleBite2State)->Name());
				_playerPos = Vector3(RandFloat(-5.0, 5.0), 5.0f, RandFloat(-5.0, 5.0));
			}
			//���ݹ����� �����?
			else
			{
				//��Ʋ�� ���߰� �⺻�ڼ� (�ٽ���������)
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
			//������ �������� �ٽ��ѹ��˻�
			Vector3 direction = _playerPos - transComp.GetWorldPosition();
			float distance = Vec3Length(&direction);
			Vec3Normalize(&direction, &direction);
			if (distance < _atkRange)
			{
				_state = TURTLESTATE_ATK1;
				_pStateMachine->ChangeState(META_TYPE(TurtleBite1State)->Name());
			}
			//���ݹ����� �����?
			else
			{
				//��Ʋ�� ���߰� �⺻�ڼ� (�ٽ���������)
				_battle = false;
				_state = TURTLESTATE_STAND;
				_pStateMachine->ChangeState(META_TYPE(TurtleIdleState)->Name());
			}
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
			_state = TURTLESTATE_FIND;
			_pStateMachine->ChangeState(META_TYPE(TurtleFindState)->Name());
			Vector3 distance = _playerPos - transComp.GetWorldPosition();
			Vec3Normalize(&distance, &distance);
			transComp.LookDirection(-distance, D3DX_PI * 2);
		}
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