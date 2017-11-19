#include "stdafx.h"
#include "Dragon.h"


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

	_entity.Activate();
	return true;
}

void Dragon::update(float deltaTime)
{
}
