#include "stdafx.h"
#include "Engine.h"

#define HUNTER_DEBUG 1

Engine *gEngine;

LRESULT CALLBACK WindowProc(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return gEngine->EngineWindowCallback(windowHandle, msg, wParam, lParam);
}

Engine::Engine()
{
	gEngine = this;
}

Engine::~Engine()
{
}

void Engine::Run()
{
	_console.Init();
	if (!InitializeSystems())
	{
		return;
	}

	_running = true;
	_valid = true;

	APPTIMER->Initialize(16);

	while (_valid && _running)
	{
		MSG msg{};
		_pInput->Update(0.0f);
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			switch (msg.message)
			{
				case WM_SYSKEYDOWN:
				case WM_KEYDOWN :
				case WM_SYSKEYUP :
				case WM_KEYUP :
				case WM_CHAR :
				{
					_pInput->keyboard.UpdateWithMessage(msg.message, msg.wParam, msg.lParam);
				}break;

				default :
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}break;
			}
		}
		float deltaTime = APPTIMER->GetTargetTime();

		_pScene->Update(deltaTime);
		_pVideo->Update(deltaTime);

		_pVideo->Render();

		APPTIMER->Tick();

		#if defined HUNTER_DEBUG
		static uint64 counter = 0;
		if ((counter % 20) == 0)
		{
			Console::Log("DeltaMS : %d\n", APPTIMER->GetDeltaMS());
		}
		counter++;
		#endif
	}
	ShutDownSystems();
}

void Engine::Stop()
{
	_running = false;
}

void Engine::ReStart()
{
	_running = true;
}

HWND Engine::GetWindowHandle()
{
	return _windowHandle;
}

HINSTANCE Engine::GetInstanceHandle()
{
	return _instanceHandle;
}

LRESULT Engine::EngineWindowCallback(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result{};
	switch (msg)
	{
	case WM_DESTROY : 
	{
		PostQuitMessage(0);
		_running = false;
		_valid = false;
	}break;

	//case WM_SYSKEYDOWN:
	//case WM_KEYDOWN :
	//case WM_SYSKEYUP :
	//case WM_KEYUP :
	//case WM_CHAR :
	//{
	//	_pInput->keyboard.UpdateWithMessage(msg, wParam, lParam);
	//}break;

	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
	{
		_pInput->mouse.UpdateWithMessage(wParam, lParam);
	}break;

	default :
	{
		return DefWindowProc(windowHandle, msg, wParam, lParam);
	}
	}
	return result;
}

bool Engine::InitializePlatform(HINSTANCE instanceHandle)
{
	WNDCLASS wc = {};
	_instanceHandle = instanceHandle;
	wc.hInstance = _instanceHandle;
	wc.lpfnWndProc = WindowProc;
	wc.lpszClassName = "Hunter";

	if (!RegisterClass(&wc))
	{
		return false;
	}

	RECT clientRect = { 0, 0, WINSIZEX, WINSIZEY };
	AdjustWindowRect(&clientRect, WS_OVERLAPPEDWINDOW, false);

	int32 clientWidth = clientRect.right - clientRect.left;
	int32 clientHeight = clientRect.bottom - clientRect.top;

	_windowHandle = CreateWindow(wc.lpszClassName, "Hunter", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, clientWidth, clientHeight, nullptr, nullptr, _instanceHandle, nullptr);

	if (!_windowHandle)
	{
		return false;
	}

	ShowWindow(_windowHandle, SW_SHOW);
	UpdateWindow(_windowHandle);

	return true;
}

bool Engine::InitializeSystems()
{
	_pVideo = std::make_shared<VideoDevice>(VideoDevice());

	if (!_pVideo->Init("VideoSystem", SystemSetting()))
	{
		return false;
	}

	_pInput= std::make_shared<InputSystem>(InputSystem());
	if (!_pInput->Init("InputSystem", SystemSetting()))
	{
		return false;
	}

	//_pInput->GetChannel().Add<InputSystem::KeyDownEvent, InputHandler>(handler);
	//_pInput->GetChannel().Add<InputSystem::KeyPressedEvent, InputHandler>(handler);
	//_pInput->GetChannel().Add<InputSystem::KeyReleasedEvent, InputHandler>(handler);
	//_pInput->GetChannel().Add<InputSystem::MouseMoveEvent, InputHandler>(handler);
	//_pInput->GetChannel().Add<InputSystem::MousePressedEvent, InputHandler>(handler);
	//_pInput->GetChannel().Add<InputSystem::MouseReleasedEvent, InputHandler>(handler);
	//_pInput->GetChannel().Add<InputSystem::MouseDownEvent, InputHandler>(handler);

	_pScene = std::make_shared<SceneSystem>(SceneSystem());
	if (!_pScene->Init("SceneSystem", SystemSetting()))
	{
		return false;
	}

	return true;
}

void Engine::ShutDownSystems()
{
}
