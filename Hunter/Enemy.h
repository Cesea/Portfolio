#pragma once
class TransformComponent;
class Enemy
{
	enum stateTag
	{
		IDLE,
		RUN,
		ATK,
		HURT,
		DIE
	};
protected:
	float _hp;
	float _speed;
	stateTag _state;
public:
	virtual bool CreateFromWorld(World &world) = 0;
	virtual void update(float deltaTime) = 0;
	Enemy();
	virtual ~Enemy();
};

class Dragon : Enemy
{
public :
	virtual bool CreateFromWorld(World &world)
	{
		_entity = world.CreateEntity();

		_entity.AddComponent<TransformComponent>();
	}

	void update(float deltaTime)override
	{
	}
	Dragon() {}
	virtual ~Dragon() {}

private :
	Entity _entity;
};

