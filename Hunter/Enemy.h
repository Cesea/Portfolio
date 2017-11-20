#pragma once
class TransformComponent;
class Enemy
{
	enum stateTag
	{
		IDLE,
		FIND,
		RUN,
		ATK1,
		ATK2,
		ATK3,
		ATK4,
		ATK5,
		HURT,
		DIE
	};
protected:
	float _hp;
	float _speed;
	stateTag _state;
	std::vector<Vector3> _moveSegment;
public:
	virtual bool CreateFromWorld(World &world) = 0;
	virtual void update(float deltaTime) = 0;
	Enemy();
	virtual ~Enemy();
};
