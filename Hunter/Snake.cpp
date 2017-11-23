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
	transComp.MovePositionWorld(0, /*TERRAIN->GetHeight(x * 10, z * 10)*/0, 0);
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
	//_pStateMachine->RegisterState(META_TYPE(PlayerMoveState)->Name(), new PlayerMoveState());
	//_pStateMachine->RegisterState(META_TYPE(PlayerAttackState)->Name(), new PlayerAttackState());
	//_pStateMachine->RegisterState(META_TYPE(PlayerCombatState)->Name(), new PlayerCombatState());
	//_pStateMachine->RegisterState(META_TYPE(PlayerDeadState)->Name(), new PlayerDeadState());
	_pStateMachine->ChangeState(META_TYPE(SnakeIdleState)->Name());

	return true;
}

void Snake::Update(float deltaTime)
{
	_pStateMachine->Update(deltaTime, _currentCommand);
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
