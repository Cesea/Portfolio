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

	TERRAIN->ConvertWorldPostoTilePos(Pos, &_tilePos);

	CollisionComponent &collision = _entity.AddComponent<CollisionComponent>();
	collision._locked = false;
	collision._isTrigger = true;
	collision._type = CollisionComponent::COLLISION_TYPE_BOX;

	ScriptComponent &scriptComponent = _entity.AddComponent<ScriptComponent>();
	scriptComponent.SetScript(MAKE_SCRIPT_DELEGATE(DamageBox,Update, *this));

	collision._valid = true;

	_entity.Activate();
	return true;
}

void DamageBox::Update(float deltaTime)
{
	CollisionComponent &collision = _entity.GetComponent<CollisionComponent>();
	TransformComponent &refTrans = _entity.GetComponent<TransformComponent>();
	collision._duration -= deltaTime;
	//위치값 업데이트....

	//위치값을 모두 업데이트 해 준 후에...
	_tilePos = _prevTilePos;
	TERRAIN->ConvertWorldPostoTilePos(refTrans.GetWorldPosition(), &_tilePos);

	if (collision._duration < 0.0f || collision._valid == false)
	{
		EventChannel channel;
		_valid = false;

		TERRAIN->RemoveEntityInTile(_entity, _tilePos);
		channel.Broadcast<IScene::SceneDirty>(IScene::SceneDirty());
	}
}