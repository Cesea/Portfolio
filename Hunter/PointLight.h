#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "Light.h"
class PointLight :public Light
{
public:
	float				_minRange;			//빛의 최소거리 
	float				_maxRange;			//빛의 최대거리
	float				_distancePow;		//거리 감쇠율

public:
	PointLight();
	~PointLight();

	virtual Matrix GetLightMatrix();

	virtual void RenderGizmo();
	virtual void SetWorldPosition(const Vector3 &position);
	virtual void SetRotateWorld(float x, float y, float z);
};



#endif
