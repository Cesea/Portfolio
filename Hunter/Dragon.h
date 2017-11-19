#pragma once
#include "Enemy.h"
class Dragon :
	public Enemy
{
public:
	bool CreateFromWorld(World &world)override;
	void update(float deltaTime)override;
	Dragon();
	virtual ~Dragon();

private:
	Entity _entity;
};