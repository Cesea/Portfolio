#pragma once

#include "Enemy.h"

class Dragon : public Enemy
{
public:
	bool CreateFromWorld(World &world)override;
	void Update(float deltaTime)override;
	Dragon();
	virtual ~Dragon();

private:
	Entity _entity;
};