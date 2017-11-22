#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "Light.h"
class PointLight :public Light
{
public:
	float				_minRange;			//���� �ּҰŸ� 
	float				_maxRange;			//���� �ִ�Ÿ�
	float				_distancePow;		//�Ÿ� ������

public:
	PointLight();
	~PointLight();

	virtual Matrix GetLightMatrix();

	virtual void RenderGizmo();
	virtual void SetWorldPosition(const Vector3 &position);
	virtual void SetRotateWorld(float x, float y, float z);
};



#endif
