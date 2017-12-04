#ifndef TEST_ENGINE_H
#define TEST_ENGINE_H

#include "ApplicationTimer.h"

#include "Console.h"

#include "InputManager.h"
#include "VideoDevice.h"
#include "SceneManager.h"

#include "VideoDevice.h"

#include "GizmoManager.h"
#include "SpriteManager.h"

//#include "GameObjectFactory.h"


#include <memory>

class Engine
{
public :
	Engine();
	~Engine();

	bool InitializePlatform(HINSTANCE instanceHandle);

	void Run();
	void Stop();
	void ReStart();

	HWND GetWindowHandle();
	HINSTANCE GetInstanceHandle();

	LRESULT CALLBACK EngineWindowCallback(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam);

	video::VideoDevice *GetVideo() { return _pVideo.get(); }
	InputManager *GetInput() { return _pInput.get(); }
	SceneManager *GetScene() { return _pScene.get(); }

private :
	bool InitializeSystems();
	void ShutDownSystems();

private :
	EventChannel _channel;
	bool _valid;
	bool _running;

	HWND _windowHandle;
	HINSTANCE _instanceHandle;

	Console _console;

	std::shared_ptr<video::VideoDevice> _pVideo;
	std::shared_ptr<InputManager> _pInput;
	std::shared_ptr<SceneManager> _pScene;

};

#endif