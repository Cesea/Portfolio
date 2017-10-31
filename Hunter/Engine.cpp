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
	if (!InitializeSystems())
	{
		return;
	}

	_running = true;
	_valid = true;

	_console.Init();

	_applicationTimer.Initialize(16);

	while (_valid && _running)
	{
		if (!ProcessWindowMessage())
		{
			_pVideo->Update(_applicationTimer.GetDeltaMS());

			_applicationTimer.Tick();

			#if defined HUNTER_DEBUG
			static uint64 counter = 0;
			if ((counter % 20) == 0)
			{
				Console::Log("DeltaMS : %d\n", _applicationTimer.GetDeltaMS());
			}
			counter++;
			#endif
		}
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
	//case WM_INPUT :
	//{
	//	char buffer[256] = { 0, };
	//	UINT dwSize;
	//	GetRawInputData((HRAWINPUT)lParam, RID_INPUT, nullptr, &dwSize, sizeof(RAWINPUTHEADER));
	//	BYTE *pByte = new BYTE[dwSize];

	//	if (pByte == nullptr)
	//	{
	//		return 0;
	//	}
	//	if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, pByte, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
	//	{
	//		Console::Log("GetRawInputBuffer does not return correct size!\n");
	//	}
	//	RAWINPUT *raw = (RAWINPUT *)pByte;
	//	if (raw->header.dwType == RIM_TYPEKEYBOARD)
	//	{
	//		StringCchPrintf(buffer, STRSAFE_MAX_CCH, TEXT(" Kbd: make=%04x Flags:%04x Reserved:%04x ExtraInformation:%08x, msg=%04x VK=%04x \n"),
	//			raw->data.keyboard.MakeCode,
	//			raw->data.keyboard.Flags,
	//			raw->data.keyboard.Reserved,
	//			raw->data.keyboard.ExtraInformation,
	//			raw->data.keyboard.Message,
	//			raw->data.keyboard.VKey);
	//		//if (FAILED(hResult))
	//		//{
	//		//	// TODO: write error handler
	//		//}
	//		OutputDebugString(buffer);
	//	}
	//	else if(raw->header.dwType == RIM_TYPEMOUSE)
	//	{
	//		StringCchPrintf(buffer, STRSAFE_MAX_CCH, TEXT("Mouse: usFlags=%04x ulButtons=%04x usButtonFlags=%04x usButtonData=%04x ulRawButtons=%04x lLastX=%04x lLastY=%04x ulExtraInformation=%04x\r\n"),
	//			raw->data.mouse.usFlags,
	//			raw->data.mouse.ulButtons,
	//			raw->data.mouse.usButtonFlags,
	//			raw->data.mouse.usButtonData,
	//			raw->data.mouse.ulRawButtons,
	//			raw->data.mouse.lLastX,
	//			raw->data.mouse.lLastY,
	//			raw->data.mouse.ulExtraInformation);
	//		//if (FAILED(hResult))
	//		//{
	//		//	// TODO: write error handler
	//		//}
	//		OutputDebugString(buffer);
	//	}
	//	delete[] pByte;
	//	return DefWindowProc(windowHandle, msg, wParam, lParam);
	//}
	default :
	{
		return DefWindowProc(windowHandle, msg, wParam, lParam);
	}
	}
	return result;
}

bool32 Engine::ProcessWindowMessage()
{
	bool32 result = false;

	MSG msg;
	_pInput->keyboard.Update();
	_pInput->mouse.Update();
	while (PeekMessage(&msg, _windowHandle, 0, 0, PM_REMOVE))
	{
		switch (msg.message)
		{
		case WM_DESTROY :
		{
			_running = false;
			_valid = false;
		}break;
		case WM_KEYDOWN :
		case WM_KEYUP :
		case WM_SYSKEYDOWN :
		case WM_SYSKEYUP :
		{
			_pInput->keyboard.UpdateWithMessage(msg.wParam, msg.lParam);
		}break;

		case WM_LBUTTONDOWN :
		case WM_LBUTTONUP :
		case WM_MBUTTONUP :
		case WM_MBUTTONDOWN :
		case WM_RBUTTONDOWN :
		case WM_RBUTTONUP :
		case WM_MOUSEMOVE :
		{
			_pInput->mouse.UpdateWithMessage(msg.wParam, msg.lParam);
		}break;
		default :
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}break;

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
	_pVideo = std::make_shared<VideoSystem>(VideoSystem());

	if (!_pVideo->Init("VideoSystem", SystemSetting()))
	{
		return false;
	}

	_pInput= std::make_shared<InputSystem>(InputSystem());
	if (!_pInput->Init("InputSystem", SystemSetting()))
	{
		return false;
	}

	_pInput->GetChannel().Add<InputSystem::KeyDownEvent, InputHandler>(handler);
	_pInput->GetChannel().Add<InputSystem::KeyPressedEvent, InputHandler>(handler);
	_pInput->GetChannel().Add<InputSystem::KeyReleasedEvent, InputHandler>(handler);
	_pInput->GetChannel().Add<InputSystem::MouseMoveEvent, InputHandler>(handler);
	_pInput->GetChannel().Add<InputSystem::MousePressedEvent, InputHandler>(handler);
	_pInput->GetChannel().Add<InputSystem::MouseReleasedEvent, InputHandler>(handler);
	_pInput->GetChannel().Add<InputSystem::MouseDownEvent, InputHandler>(handler);
	return true;
}

void Engine::ShutDownSystems()
{
}
