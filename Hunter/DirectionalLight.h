#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "BaseLight.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight();
	~DirectionalLight();

	virtual Matrix GetLightMatrix() const;

	virtual void SetTarget(const Vector3 &target);
	virtual void RenderGizmo();
};

#endif
