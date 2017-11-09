#ifndef TEST_ENGINE_H
#define TEST_ENGINE_H

#include "ApplicationTimer.h"

#include "Console.h"

#include "InputManager.h"
#include "VideoDevice.h"
#include "SceneManager.h"

#include <memory>

//#include "SurfaceMaterialManager.h"
//#include "EffectManager.h"
//#include "FontManager.h"
//#include "MeshManager.h"

//struct ServiceProvider
//{
//	InputSystem *pInput{};
//	VideoSystem *pVideo{};
//	SceneSystem *pScene{};
//	PhysicsSystem *pPhysics{};
//	CommandSystem *pCommand{};
//};


//class InputHandler
//{
//public :
//
//	void Handle(const InputSystem::KeyDownEvent &event)
//	{
//		std::cout << event.code << " down" << std::endl;
//	}
//	void Handle(const InputSystem::KeyPressedEvent &event)
//	{
//		std::cout << event.code << " pressed" << std::endl;
//	}
//	void Handle(const InputSystem::KeyReleasedEvent &event)
//	{
//		std::cout << event.code << " released" << std::endl;
//	}
//	void Handle(const InputSystem::MouseMoveEvent &event)
//	{
//		std::cout << event.coord.x << ", " << event.coord.y << " move" << std::endl;
//	}
//	void Handle(const InputSystem::MouseDownEvent &event)
//	{
//		std::cout << event.coord.x << ", " << event.coord.y << " move, " << event.code << " down" << std::endl;
//	}
//	void Handle(const InputSystem::MousePressedEvent &event)
//	{
//		std::cout << event.coord.x << ", " << event.coord.y << " move, " << event.code << " pressed" << std::endl;
//	}
//	void Handle(const InputSystem::MouseReleasedEvent &event)
//	{
//		std::cout << event.coord.x << ", " << event.coord.y << " move, " << event.code << " released" << std::endl;
//	}
//	void Handle(const InputSystem::MouseWheelEvent &event)
//	{
//		std::cout << event.code << " down" << std::endl;
//	}
//};

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