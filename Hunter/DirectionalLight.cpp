#include "stdafx.h"
#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
{
}

DirectionalLight::~DirectionalLight()
{
}

Matrix DirectionalLight::GetLightMatrix()
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
	memcpy(&matLight._21, &forward, sizeof(D3DXVECTOR3));
	memcpy(&matLight._31, &_color, sizeof(FLOAT) * 3);
	matLight._34 = _intensity;


	return matLight;
}

void DirectionalLight::SetWorldPosition(const Vector3 &position)
{
	TransformComponent &refTransform = _entity.GetComponent<TransformComponent>();
	refTransform.SetWorldPosition(position);
}

void DirectionalLight::SetRotateWorld(float x, float y, float z)
{
	TransformComponent &refTransform = _entity.GetComponent<TransformComponent>();
	refTransform.SetRotateWorld(x, y, z);
}

void DirectionalLight::RenderGizmo()
{
}