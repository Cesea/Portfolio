#include "stdafx.h"
#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::CreateFromWorld(World & world)
{
	EventChannel channel;
	channel.Add<InputManager::KeyPressedEvent, Player>(*this);
	entity = world.CreateEntity();

	TransformComponent &transComp = entity.AddComponent<TransformComponent>();
	transComp.MovePositionWorld(0, /*TERRAIN->GetHeight(x * 10, z * 10)*/0, 0);
	RenderComponent &renderComp = entity.AddComponent<RenderComponent>();
	renderComp._type = RenderComponent::Type::eSkinned;
	renderComp._skinned = VIDEO->CreateSkinnedAnimation(VIDEO->GetSkinnedXMesh("Knight"), "Anim" + std::to_string(0));
	ScriptComponent &scriptComponent = entity.AddComponent<ScriptComponent>();
	scriptComponent.SetScript(MAKE_SCRIPT_DELEGATE(Player, Update, *this));
	ActionComponent &actionComp = entity.AddComponent<ActionComponent>();
	actionComp.CreateFrom(renderComp._skinned);
	actionComp._pCallbackHandler = new PlayerCallBackHandler;


	entity.Activate();
}

void Player::Update(float deltaTime)
{
	if (_animationIndex != -1)
	{
		ActionComponent &refAction = entity.GetComponent<ActionComponent>();

		Action action;
		action._animationIndex = _animationIndex;
		action._blocking = true;
		action._crossFadeTime = 0.2f;

		refAction._actionQueue.PushAction(action);
		_animationIndex = -1;
	}
}

void Player::InterpretCommand()
{
}

void Player::Handle(const InputManager::KeyPressedEvent & event)
{
	char a = MapVirtualKey(event.code, MAPVK_VK_TO_CHAR);
	_animationIndex = atoi((const char *)&a);
}

