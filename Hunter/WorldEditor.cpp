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
	//channel.Add<InputManager::MouseWheelEvent, Editor>(*this);
	channel.Add<InputManager::KeyPressedEvent, Editor>(*this);
}

void Editor::Init()
{
	this->RegisterEvents();
}

void Editor::Shutdown()
{
	if (nullptr != _pEdittingObject)
	{
		_pEdittingObject = nullptr;
	}
}

void Editor::Edit(void * object)
{
	ImguiBeginFrame(_mx, _my, _mb, _scroll);
	Console::Log("%d, %d, %d \n", _mx, _my, _mb);

	ImguiBeginScrollArea(_name, 100, 100, 400, 500, &_scroll);

	ImguiButton("asdasfd");
	ImguiCollapse("asdasfd", nullptr, false);

	ImguiEndScrollArea();

	ImguiEndFrame();
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
	
}
