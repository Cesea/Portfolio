#ifndef TEST_ENGINE_H
#define TEST_ENGINE_H

#include "System.h"
#include "ApplicationTimer.h"

#include "Console.h"

#include "InputSystem.h"
#include "VideoSystem.h"
#include "SceneSystem.h"
#include "PhysicsSystem.h"
#include "CommandSystem.h"

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

typedef std::shared_ptr<System> SystemPtr;

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

	VideoSystem *GetVideo() { return _pVideo.get(); }
	InputSystem *GetInput() { return _pInput.get(); }
	SceneSystem *GetScene() { return _pScene.get(); }

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

	std::shared_ptr<VideoSystem> _pVideo;
	std::shared_ptr<InputSystem> _pInput;
	std::shared_ptr<SceneSystem> _pScene;

};

#endif