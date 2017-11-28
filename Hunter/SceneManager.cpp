#include "stdafx.h"
#include "SceneManager.h"

#include "BaseScene.h"
//#include "MapToolScene.h"

SceneManager::SceneManager()
{
	_pCurrentScene = nullptr;
}

SceneManager::~SceneManager()
{
}

bool SceneManager::Init()
{
	//기본 베이스 씬 세팅
	//IScene *pScene = new MapToolScene;
	//AddScene(pScene->GetSceneName(), pScene);

	IScene *pScene = new BaseScene;
	AddScene(pScene->GetSceneName(), pScene);

	ChangeScene(pScene->GetSceneName());

	GetChannel().Add<SceneChangeEvent, SceneManager>(*this);

	return true;
}

void SceneManager::ShutDown()
{
	RemoveAllScenes();
}

void SceneManager::Update(float deltaTime, const InputManager &input)
{
	_pCurrentScene->Update(deltaTime, input);
	_pCurrentScene->Render();
}

bool SceneManager::ChangeScene(const std::string &key)
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

void SceneManager::AddScene(const std::string &sceneName, IScene *pScene)
{
	if (pScene == nullptr)
	{
		return;
	}
	auto find = _scenes.find(sceneName);
	//없다면
	if (find == _scenes.end())
	{
		_scenes.insert(std::make_pair(sceneName, pScene));
	}
}

void SceneManager::RemoveScene(const std::string & key)
{
	auto find = _scenes.find(key);
	if (find == _scenes.end())
	{
		find->second->Release();
		_scenes.erase(find);
	}
}

void SceneManager::RemoveAllScenes()
{
	for (auto scene : _scenes)
	{
		scene.second->Release();
	}
	_scenes.clear();
}
