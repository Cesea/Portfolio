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
	transComp.MovePositionWorld(0, 5.0f, 0);

	RenderComponent &renderComp = _entity.AddComponent<RenderComponent>();
	renderComp._type = RenderComponent::Type::eSkinned;
	renderComp._skinned = VIDEO->CreateAnimationInstance(VIDEO->GetSkinnedXMesh("Snake"), "Anim" + std::to_string(0));

	ScriptComponent &scriptComponent = _entity.AddComponent<ScriptComponent>();
	scriptComponent.SetScript(MAKE_SCRIPT_DELEGATE(Turtle, Update, *this));

	_pActionComp = &_entity.AddComponent<ActionComponent>();
	_pActionComp->CreateFrom(renderComp._skinned);
	_pActionComp->_pCallbackHandler = new TurtleCallbackHandler;
	_pActionComp->_pCallbackHandler->Init(this);
	SetupCallbackAndCompression();

	_pActionComp->MakeAnimationList();
	_pActionComp->SetFirstAction(TURTLE_ANIM(TURTLE_ANIMATION_ENUM::TURTLE_IDLE));

	_entity.Activate();

	_pStateMachine = new TurtleStateMachine;
	_pStateMachine->Init(this);
	_pStateMachine->RegisterState(META_TYPE(TurtleIdleState)->Name(), new TurtleIdleState());
	_pStateMachine->ChangeState(META_TYPE(TurtleIdleState)->Name());

	return true;
}

void Turtle::Update(float deltaTime)
{
	_pStateMachine->Update(deltaTime, _currentCommand);
}

void Turtle::SetupCallbackAndCompression()
{
	ActionComponent &refActionComp = _entity.GetComponent<ActionComponent>();
	TransformComponent &refTransform = _entity.GetComponent<TransformComponent>();

	ID3DXAnimationController *pController = refActionComp._pAnimationController;
	uint32 numAnimationSet = pController->GetNumAnimationSets();
	ID3DXKeyframedAnimationSet *anim0;

	pController->GetAnimationSetByName(TurtleAnimationString[TURTLE_ANIMATION_ENUM::TURTLE_IDLE], (ID3DXAnimationSet **)&anim0);

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