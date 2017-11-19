#include "stdafx.h"
#include "WorldEditor.h"

Editor::Editor()
{
}

Editor::~Editor()
{
}

void Editor::RegisterEvents()
{
	EventChannel channel;
	channel.Add<InputManager::MouseMoveEvent, Editor>(*this);
	channel.Add<InputManager::MouseReleasedEvent, Editor>(*this);
	channel.Add<InputManager::MousePressedEvent, Editor>(*this);
	channel.Add<InputManager::MouseWheelEvent, Editor>(*this);
	channel.Add<InputManager::KeyPressedEvent, Editor>(*this);
}

void Editor::Init()
{
	strcpy(_button, "Button");
	strcpy(_collapse, "Collapse");
	
	this->RegisterEvents();
}

void Editor::Shutdown()
{
	if (nullptr != _pEdittingObject)
	{
		_pEdittingObject = nullptr;
	}
}

void Editor::Edit(RefVariant &object)
{
	ImguiBeginFrame(_mx, _my, _mb, _scroll, _key);

	//Console::Log("%d, %d, %d \n", _mx, _my, _mb);

	ImguiBeginScrollArea(_name, 30, 30, 400, 800, &_scroll);


	for (int32 i = 0; i < 10; ++i)
	{
		ImguiButton(_button);
	}
	ImguiCollapse(_collapse, nullptr, false);

	ImguiSlider(_slider, &_val, -10, 10, 1.0f);
	imguiEdit(_edit, 90);

	ImguiEndScrollArea();

	ImguiEndFrame();

	_scroll = 0;
	_key = 0;
}


void Editor::Handle(const InputManager::MousePressedEvent & event)
{
	if (event.code == MOUSE_BUTTON_LEFT)
	{
		_mb = true;
	}
	_mx = event.coord.x;
	_my = event.coord.y;
}

void Editor::Handle(const InputManager::MouseMoveEvent & event)
{
	_mx = event.current.x;
	_my = event.current.y;
}

void Editor::Handle(const InputManager::MouseWheelEvent & event)
{
	_scroll = event.delta / 120;
	Console::Log("%d\n", event.delta);
}

void Editor::Handle(const InputManager::MouseReleasedEvent & event)
{
	if (event.code == MOUSE_BUTTON_LEFT)
	{
		_mb = false;
	}
	_mx = event.coord.x;
	_my = event.coord.y;
}

void Editor::Handle(const InputManager::KeyPressedEvent & event)
{
	_key = event.code;
}
