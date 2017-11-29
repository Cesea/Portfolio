#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "BaseLight.h"
class PointLight :public Light
{
public:
	float				_minRange{};			//���� �ּҰŸ� 
	float				_maxRange{};			//���� �ִ�Ÿ�
	float				_distancePow{};		//�Ÿ� ������

public:
	PointLight();
	~PointLight();

	virtual Matrix GetLightMatrix() const;

	virtual void RenderGizmo();
};



#endif