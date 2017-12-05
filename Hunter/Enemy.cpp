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

void Enemy::PatrolSet(int type, Vector3 Position,float range)
{
	_moveSegment.empty();
	switch (type)
	{
	case 0:
	{
		float first = Position.x+RandFloat(0.0f, range);
		float second = Position.y+RandFloat(0.0f, range);
		Vector3 a = Vector3(first, TERRAIN->GetHeight(first, second), second);

		first = Position.x + RandFloat(range, 0.0f);
		second = Position.z + RandFloat(range, 0.0f);
		Vector3 b = Vector3(first, TERRAIN->GetHeight(first, second), second);

		_moveSegment.push_back(a);
		_moveSegment.push_back(b);
	}
	break;
	case 1:
	{
		float first = Position.x+RandFloat(0.0f, range);
		float second = Position.z+RandFloat(0.0f, range);
		Vector3 a = Vector3(first, TERRAIN->GetHeight(first, second), second);

		first = Position.x+RandFloat(range, 0.0f);
		second = Position.z+RandFloat(range, 0.0f);
		Vector3 b = Vector3(first, TERRAIN->GetHeight(first, second), second);

		first = Position.x + RandFloat(range, 0.0f);
		second = Position.z + RandFloat(range, 0.0f);
		Vector3 c = Vector3(first, TERRAIN->GetHeight(first, second), second);

		_moveSegment.push_back(a);
		_moveSegment.push_back(b);
		_moveSegment.push_back(c);
	}
	break;
	case 2:
	{
		float first = Position.x + RandFloat(range, 0.0f);
		float second = Position.z + RandFloat(range, 0.0f);
		Vector3 a = Vector3(first, TERRAIN->GetHeight(first, second), second);

		first = Position.x + RandFloat(range, 0.0f);
		second = Position.z + RandFloat(range, 0.0f);
		Vector3 b = Vector3(first, TERRAIN->GetHeight(first, second), second);

		first = Position.x + RandFloat(range, 0.0f);
		second = Position.z + RandFloat(range, 0.0f);
		Vector3 c = Vector3(first, TERRAIN->GetHeight(first, second), second);

		first = Position.x + RandFloat(range, 0.0f);
		second = Position.z + RandFloat(range, 0.0f);
		Vector3 d = Vector3(first, TERRAIN->GetHeight(first, second), second);

		_moveSegment.push_back(a);
		_moveSegment.push_back(b);
		_moveSegment.push_back(c);
		_moveSegment.push_back(d);
	}
	break;
	}
}
