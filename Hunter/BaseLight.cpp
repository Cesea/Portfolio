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
