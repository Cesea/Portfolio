#include "stdafx.h"
#include "TestScene.h"

bool32 TestScene::Load()
{
	bool32 result = true;
	return result;
}

bool32 TestScene::Unload()
{
	bool32 result = true;
	return result;
}

bool32 TestScene::Init()
{
	bool32 result = true;
	return result;
}

bool32 TestScene::Update(float deltaTime)
{
	bool32 result = true;
	return result;
}

bool32 TestScene::Render()
{
	bool32 result = true;

	im::BeginFrame(gEngine->GetInput()->mouse, 
		gEngine->GetInput()->keyboard.GetCharInput(), 
		gEngine->GetInput()->keyboard.GetShiftDown() );

	if (im::Button("toto"))
	{
		EventChannel channel;
		channel.Broadcast(SceneManager::SceneChangeEvent("BaseScene"));
	}

	im::EndFrame();
	return result;
}

void TestScene::Release()
{
}

const char * TestScene::GetSceneName()
{
	return "TestScene";
}

bool32 TestScene::IsActive()
{
	return _active;
}
