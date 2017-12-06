#include "stdafx.h"
#include "DamageBox.h"



DamageBox::DamageBox()
{
}


DamageBox::~DamageBox()
{
}


bool DamageBox::CreateFromWorld(World & world, const Vector3 & Pos)
{
	_entity = world.CreateEntity();
	TransformComponent &transform = _entity.AddComponent<TransformComponent>();
	transform.SetWorldPosition((Pos));

	CollisionComponent &collision = _entity.AddComponent<CollisionComponent>();
	//collision._boundingBox.Init(_min,_max);
	collision._locked = false;
	collision._isTrigger = true;
	//collision._triggerType = _type;
	collision._type = CollisionComponent::COLLISION_TYPE_BOX;

	ScriptComponent &scriptComponent = _entity.AddComponent<ScriptComponent>();
	scriptComponent.SetScript(MAKE_SCRIPT_DELEGATE(DamageBox,Update, *this));

	_entity.Activate();
	return true;
}

void DamageBox::Update(float deltaTime)
{
	CollisionComponent &collision = _entity.GetComponent<CollisionComponent>();
	collision._duration -= deltaTime;
}