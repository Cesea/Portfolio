#include "stdafx.h"
#include "PointLight.h"

PointLight::PointLight()
{
}

PointLight::~PointLight()
{
}

Matrix PointLight::GetLightMatrix()
{
	// Pos.x,         Pos.y,          Pos.z,         lightType ( 0 Direction, 1 Point, 2 Spot )
	// Dir.x,         Dir.y,          Dir.z,         DistancePow;
	// Color.r,       Color.g,        Color.b,       strength
	// MaxRange,      MinRange,       InAngle        OutAngle 

	Matrix matLight;
	MatrixIdentity(&matLight);

	TransformComponent &refTransform = _entity.GetComponent<TransformComponent>();

	matLight._14 = 1;		//Point Light Type
	Vector3 pos = refTransform.GetWorldPosition();
	memcpy(&matLight._11, &pos, sizeof(D3DXVECTOR3));
	memcpy(&matLight._31, &_color, sizeof(FLOAT) * 3);
	matLight._24 = _distancePow;
	matLight._41 = this->_maxRange;
	matLight._42 = this->_minRange;
	matLight._34 = this->_intensity;

	return matLight;
}

void PointLight::RenderGizmo()
{
}

void PointLight::SetWorldPosition(const Vector3 & position)
{
}

void PointLight::SetRotateWorld(float x, float y, float z)
{
}
