#include "stdafx.h"
#include "Dragon.h"


Dragon::Dragon()
{
}


Dragon::~Dragon()
{
}

bool Dragon::CreateFromWorld(World & world, const Vector3 & Pos)
{
	return false;
}

void Dragon::Update(float deltaTime)
{
}

void Dragon::Handle(const CollisionSystem::ActorTriggerEvent & event)
{
}

void Dragon::SetupCallbackAndCompression()
{
}

void Dragon::QueueAction(Action & action, bool cancle)
{
}

bool Dragon::findPlayer(Vector3 forward, Vector3 playerPos, Vector3 myPos, float range1, float range2, float findRadian)
{
	return false;
}
