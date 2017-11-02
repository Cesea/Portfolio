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

	POINT mousePT = gEngine->GetInput()->mouse.GetCurrentPoint();

	im::BeginFrame(mousePT.x, mousePT.y, gEngine->GetInput()->mouse.IsDown(MOUSE_BUTTON_LEFT), 0);

	im::BeginScrollArea("tt", 100, 100, 400, 400, &_scroll);

	if (im::Button("Hi", 100, true))
	{
		std::cout << "HOOHHO" << std::endl;
	}
	im::Item("HIIII", 100, true);

	if (im::Check("TODO", _itemCheck))
	{
		_itemCheck = !_itemCheck;
	}

	im::EndScrollArea();

	if (im::Collapse("TODO", "meto", _collapse))
	{
		_collapse = !_collapse;
	}

	im::Slider("slider", &_sliderValue, -20, 20, 1, 200);

	//im::Value("tada");
	//im::Label("aaaaaa");
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
