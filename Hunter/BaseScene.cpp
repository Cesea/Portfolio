#include "stdafx.h"
#include "BaseScene.h"

bool BaseScene::Load()
{
	bool result = true;
	return result;
}

bool BaseScene::Unload()
{
	bool result = true;
	return result;
}

bool BaseScene::Init()
{
	bool result = true;
	return result;
}

bool BaseScene::Update(float deltaTime)
{
	bool result = true;
	return result;
}

bool BaseScene::Render()
{
	bool result = true;
	return result;
}

void BaseScene::Release()
{
}

const char * BaseScene::GetSceneName()
{
	return "BaseScene";
}

bool BaseScene::IsActive()
{
	return _active;
}
