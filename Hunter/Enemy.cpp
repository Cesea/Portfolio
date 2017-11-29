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

void Enemy::setEvent()
{
	EventChannel channel;
	channel.Add<Player::PlayerImformationEvent, Enemy>(*this);
}

void Enemy::Handle(const Player::PlayerImformationEvent & event)
{
	_playerPos = event._position;
	_playerForward = event._forward;
	_playerState = event._state;
}
