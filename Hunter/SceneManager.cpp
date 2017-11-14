#include "stdafx.h"
#include "SceneManager.h"

#include "BaseScene.h"
#include "TestScene.h"

SceneManager::SceneManager()
{
	_pCurrentScene = nullptr;
}

SceneManager::~SceneManager()
{
}

bool SceneManager::Init()
{
	//�⺻ ���̽� �� ����
	IScene *pScene = new BaseScene;
	AddScene(pScene->GetSceneName(), pScene);
	ChangeScene(pScene->GetSceneName());

	pScene = new TestScene;
	AddScene(pScene->GetSceneName(), pScene);

	GetChannel().Add<SceneChangeEvent, SceneManager>(*this);

	return true;
}

void SceneManager::ShutDown()
{
	RemoveAllScenes();
}

void SceneManager::Update(float deltaTime)
{
	_pCurrentScene->Update(deltaTime);
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
	//���ٸ�
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
		find->second->Unload();
		_scenes.erase(find);
	}
}

void SceneManager::RemoveAllScenes()
{
	for (auto scene : _scenes)
	{
		scene.second->Release();
		scene.second->Unload();
	}
	_scenes.clear();
}