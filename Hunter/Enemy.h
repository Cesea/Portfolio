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
	//Base
	float _hp;
	float _speed;
	float _rotateSpeed;
	bool _battle;
	//Patrol
	std::vector<Vector3> _moveSegment;
	int _patrolIndex;
	//Idle
	int _delayTime;
	int _delayCount;
	//find
	float _findDistance; //�� �Ÿ��� ������ ������ ����
	float _findRadian; //������ �þ߰�
	float _findStareDistance; // �þ߾��� �ִ� �� �����Ÿ�
	int _roarTime;
	int _roarCount;
	//atk
	float _atkRange;
	int _atkTime;
	int _atkCount;
	//stand
	int _standTime;
	int _standCount;
	//������
    Vector3 _playerPos;
};
