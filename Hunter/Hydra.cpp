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
	_pStateMachine->ChangeState(META_TYPE(HydraIdleState)->Name());

	_speed = 5.0f;
	_rotateSpeed = D3DX_PI / 64;
	_patrolIndex = 0;
	_moveSegment.push_back(Vector3(5.0f, 8.0f, 5.0f));
	_moveSegment.push_back(Vector3(-5.0f, 8.0f, 5.0f));
	_moveSegment.push_back(Vector3(-5.0f, 8.0f, -5.0f));

	_delayTime = 180.0f;
	_delayCount = _delayTime;

	_findDistance = 5.0f;
	_findRadian = D3DX_PI / 3;
	_findStareDistance = 20.0f;
	_roarTime = 180;
	_roarCount = _roarTime;

	_battle = false;

	_playerPos = Vector3(5.0f, 8.0f, 5.0f);

	_atkRange = 0.5f;
	_atkTime = 70;
	_atkCount = _atkTime;

	_standTime = 70;
	_standCount = _standTime;
	return true;
}

void Hydra::Update(float deltaTime)
{
	_pStateMachine->Update(deltaTime, _currentCommand);
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