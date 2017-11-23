#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "BaseLight.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight();
	~DirectionalLight();

	virtual Matrix GetLightMatrix();

	virtual void SetWorldPosition(const Vector3 &position);
	virtual void SetRotateWorld(float x, float y, float z);

	virtual void RenderGizmo();
};

#endif
