#include "stdafx.h"
#include "Engine.h"

#include "GameObjectFactory.h"

#include <time.h>

#define HUNTER_DEBUG 1

Engine *gEngine;
HWND gWindowHandle;

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
	GAMEOBJECTFACTORY->Release();
	GAMEOBJECTFACTORY->ReleaseInstance();
	GIZMOMANAGER->Release();
	GIZMOMANAGER->ReleaseInstance();
	SPRITEMANAGER->Release();
	SPRITEMANAGER->ReleaseInstance();
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
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			switch (msg.message)
			{
			case WM_SYSKEYDOWN:
			case WM_KEYDOWN:
			case WM_SYSKEYUP:
			case WM_KEYUP:
			case WM_CHAR:
			{
				_pInput->keyboard.UpdateWithMessage(msg.message, msg.wParam, msg.lParam);
			}break;

			case WM_MOUSEWHEEL:
			{
				_pInput->mouse.UpdateWheelWithMessage(msg.wParam, msg.lParam);
			}break;

			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP:
			case WM_MBUTTONUP:
			case WM_MBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case WM_RBUTTONUP:
			case WM_MOUSEMOVE:
			{
				_pInput->mouse.UpdateWithMessage(msg.wParam, msg.lParam);
			}break;

			default:
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}break;
			}
		}

		_pInput->Update();
		float deltaTime = APPTIMER->GetTargetTime();
		_pScene->Update(deltaTime, *_pInput.get());

		gpDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, 0xff303040, 1.0f, 0);
		gpDevice->BeginScene();
		_pScene->Render();
		gpDevice->EndScene();
		gpDevice->Present(nullptr, nullptr, NULL, nullptr);

		_pInput->UpdatePrevInput();

		APPTIMER->Tick();

//#if defined (DEBUG) || defined (_DEBUG)
		static uint64 counter = 0;
		if ((counter % 60) == 0)
		{
			Console::Log("DeltaMS : %d\n", APPTIMER->GetDeltaMS());
		}
		counter++;
//#endif
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
	AdjustWindowRect(&clientRect, WS_POPUP | WS_BORDER, false);

	int32 clientWidth = clientRect.right - clientRect.left;
	int32 clientHeight = clientRect.bottom - clientRect.top;

	_windowHandle = CreateWindow(wc.lpszClassName, "Hunter", WS_POPUP | WS_BORDER,
		WINSTARTX, WINSTARTY, clientWidth, clientHeight, nullptr, nullptr, _instanceHandle, nullptr);

	if (!_windowHandle)
	{
		return false;
	}

	//SetCapture(_windowHandle);
	//SetCursorPos(WINSTARTX + WINSIZEX / 2, WINSTARTY + WINSIZEY / 2);

	gWindowHandle = _windowHandle;

	ShowWindow(_windowHandle, SW_SHOW);
	UpdateWindow(_windowHandle);


	srand(time(NULL));

	return true;
}

bool Engine::InitializeSystems()
{
	_pInput= std::make_shared<InputManager>(InputManager());
	if (!_pInput->Init())
	{
		return false;
	}

	_pVideo = std::shared_ptr<video::VideoDevice>(new video::VideoDevice());
	if (!_pVideo->Init())
	{
		return false;
	}

	GIZMOMANAGER->Init(gpDevice);
	SPRITEMANAGER->Init(gpDevice);

	GAMEOBJECTFACTORY->Init();

	_pScene = std::make_shared<SceneManager>(SceneManager());
	if (!_pScene->Init())
	{
		return false;
	}

	return true;
}

void Engine::ShutDownSystems()
{
}
