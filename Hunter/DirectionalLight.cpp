#include "stdafx.h"
#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
{
	_color = 0xffffffff;
	_intensity = 1.0f;
}

DirectionalLight::~DirectionalLight()
{
}

Matrix DirectionalLight::GetLightMatrix() const
{
	// Pos.x,         Pos.y,          Pos.z,         lightType ( 0 Direction, 1 Point, 2 Spot )
	// Dir.x,         Dir.y,          Dir.z,         DistancePow;
	// Color.r,       Color.g,        Color.b,       strength
	// MaxRange,      MinRange,       InAngle        OutAngle 

	Matrix matLight;
	MatrixIdentity(&matLight);

	TransformComponent &refTransform = _entity.GetComponent<TransformComponent>();

	matLight._14 = 0;
	Vector3 forward = refTransform.GetForward();
	memcpy(&matLight._21, &forward, sizeof(Vector3));
	memcpy(&matLight._31, &_color, sizeof(float) * 3);
	matLight._34 = _intensity;

	return matLight;
}

void DirectionalLight::SetTarget(const Vector3 & target)
{
	TransformComponent &refTransform = _entity.GetComponent<TransformComponent>();
	Vector3 direction = target - refTransform._position;
	Vec3Normalize(&direction, &direction);
	refTransform.LookDirection(direction);
}

void DirectionalLight::RenderGizmo()
{
}