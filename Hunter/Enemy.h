#pragma once

#include "BaseGameObject.h"

enum ENEMYSTATE
	{
		ENEMYSTATE_IDLE,
		ENEMYSTATE_RUN
	};

class Enemy : public BaseGameObject	
{
public:
	virtual bool CreateFromWorld(World &world) = 0;
	virtual void Update(float deltaTime) = 0;
	Enemy();
	virtual ~Enemy();

protected:
	float _hp;
	float _speed;
	std::vector<Vector3> _moveSegment;
};
