#include "stdafx.h"
#include "Dragon.h"
#include "ScriptComponent.h"

Dragon::Dragon()
{
}


Dragon::~Dragon()
{
}

bool Dragon::CreateFromWorld(World & world)
{
	Matrix correctionMat;
	MatrixScaling(&correctionMat, 0.1f, 0.1f, 0.1f);
	video::SkinnedXMeshHandle _skinnedMeshHandle = VIDEO->CreateSkinnedXMesh("../resources/Models/knights/Knight.X", &correctionMat, "Knight");


	_entity = world.CreateEntity();
	TransformComponent &transComp = _entity.AddComponent<TransformComponent>();
	transComp.MovePositionWorld(0, 0, 0);
	RenderComponent &renderComp = _entity.AddComponent<RenderComponent>();
	renderComp._type = RenderComponent::Type::eSkinned;

	renderComp._skinned = VIDEO->CreateSkinnedAnimation(_skinnedMeshHandle, "Anim0");
	ScriptComponent & script = _entity.AddComponent<ScriptComponent>();
	script.SetScript(MAKE_SCRIPT_DELEGATE(Dragon, update, *this));


	_entity.Activate();
	return true;
}

void Dragon::update(float deltaTime)
{
	TransformComponent &transComp = _entity.GetComponent<TransformComponent>();
	transComp.SetWorldPosition(transComp.GetWorldPosition() + Vector3(0.0f, 0.0f, 0.1f));
}
