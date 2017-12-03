#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "BaseLight.h"
class PointLight :public Light
{
public:
	float				_minRange{};			//빛의 최소거리 
	float				_maxRange{};			//빛의 최대거리
	float				_distancePow{};		//거리 감쇠율

public:
	PointLight();
	~PointLight();

	virtual Matrix GetLightMatrix() const;

	virtual void RenderGizmo();
};



#endif