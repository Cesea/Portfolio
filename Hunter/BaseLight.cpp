#include "stdafx.h"
#include "BaseLight.h"

Light::Light()
	:_color(0xffffffff), _intensity(1.0f)
{
}

Light::~Light()
{
}

void Light::CreateFromWorld(World & world)
{
	_entity = world.CreateEntity();
	_entity.AddComponent<TransformComponent>();

	_entity.Activate();
}

void Light::SetWorldPosition(const Vector3 & position)
{
	_entity.GetComponent<TransformComponent>().SetWorldPosition(position);
}

void Light::MoveSelf(const Vector3 & position)
{
	_entity.GetComponent<TransformComponent>().MovePositionSelf(position);
}

void Light::SetRotateWorld(float x, float y, float z)
{
	_entity.GetComponent<TransformComponent>().RotateSelf(x, y, z);
}