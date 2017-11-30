#include "stdafx.h"
#include "Enemy.h"
#include "TransformComponent.h"

Enemy::Enemy()
{
}


Enemy::~Enemy()
{
}

void Enemy::resetAllCount()
{
	_delayCount = _delayTime;
	_roarCount = _roarTime;
	_atkCount = _atkTime;
	_standCount = _standTime;
	_hurtCount = _hurtTime;
}
