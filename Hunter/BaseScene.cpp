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

	im::BeginFrame(gEngine->GetInput()->mouse, gEngine->GetInput()->keyboard.GetCharInput());

	im::BeginScrollArea("Editor", 100, 100, 400, 600, &_scroll);

	im::Button("Hi", 100, true);
	im::Item("HIIII", 100, true);
	if (im::Check("TODO", _itemCheck))
	{
		_itemCheck = !_itemCheck;
	}
	im::Value("holo");
	if (im::Collapse("TODO", "meto", _collapse))
	{
		_collapse = !_collapse;
	}
	im::Slider("slider", &_sliderValue, -20, 20, 1, 200);

	im::EndScrollArea();

	im::Edit(_strings1, 100);
	im::Edit(_strings2, 100);

	im::Button("After", 100);

	im::EndFrame();

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
