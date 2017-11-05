#include "stdafx.h"
#include "SceneSystem.h"

#include "BaseScene.h"
#include "TestScene.h"

SceneSystem::SceneSystem()
{
	_pCurrentScene = nullptr;
}

SceneSystem::~SceneSystem()
{
}

bool SceneSystem::Init(const std::string & name, const SystemSetting & setting)
{
	_name = name;

	IScene *pScene = new BaseScene;
	AddScene(pScene->GetSceneName(), pScene);
	ChangeScene(pScene->GetSceneName());

	pScene = new TestScene;
	AddScene(pScene->GetSceneName(), pScene);


	GetChannel().Add<SceneChangeEvent, SceneSystem>(*this);

	return true;
}

void SceneSystem::ShutDown()
{
	RemoveAllScenes();
}

void SceneSystem::Update(float deltaTime)
{
	_pCurrentScene->Update(deltaTime);
	_pCurrentScene->Render();
}

bool SceneSystem::ChangeScene(const std::string &key)
{
	auto find = _scenes.find(key);
	if (find == _scenes.end())
	{
		Console::Log("There is no such scene %s\n", key);
		return false;
	}

	bool initResult = find->second->Init();
	if (!initResult)
	{
		Console::Log("Scene initialization failed.");
		return false;
	}

	if (_pCurrentScene)
	{
		_pCurrentScene->Release();
	}

	_pCurrentScene = find->second;
	return true;
}

void SceneSystem::AddScene(const std::string &sceneName, IScene *pScene)
{
	if (pScene == nullptr)
	{
		return;
	}
	auto find = _scenes.find(sceneName);
	//���ٸ�
	if (find == _scenes.end())
	{
		_scenes.insert(std::make_pair(sceneName, pScene));
	}
}

void SceneSystem::RemoveScene(const std::string & key)
{
	auto find = _scenes.find(key);
	if (find == _scenes.end())
	{
		find->second->Release();
		find->second->Unload();
		_scenes.erase(find);
	}
}

void SceneSystem::RemoveAllScenes()
{
	for (auto scene : _scenes)
	{
		scene.second->Release();
		scene.second->Unload();
	}
	_scenes.clear();
}
