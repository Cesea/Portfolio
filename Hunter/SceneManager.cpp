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
	//씬전환 효과 추가
	_sceneChangeEffects.push_back(nullptr);		//0 번은 효과 없음 따라서 NULL 추가

	SceneChangeEffect* pChangeEffect1 = new SceneChangeEffect;
	pChangeEffect1->Init();
	_sceneChangeEffects.push_back(pChangeEffect1);


	SceneChangeEffect* pChangeEffect2 = new SceneChangeEffect;
	pChangeEffect2->Init();
	_sceneChangeEffects.push_back(pChangeEffect2);

	return true;
}

void SceneManager::ShutDown()
{
	//RemoveAllScenes();
}

void SceneManager::Update(float deltaTime)
{
	//날릴씬이있다면
	if (_pReleaseScene)
	{
		bool32 sceneIsLoading = false;
		SceneTableIter iter;

		for (iter = _loadingScene.begin(); iter != _loadingScene.end(); ++iter) 
		{
			//날릴씬이 로딩씬이라면 날리면 안된다.
			if (iter->second == _pReleaseScene)
			{
				sceneIsLoading = true;
				break;
			}
		}

		//지금현제 날리는 씬이 로딩씬이 아니라면 안심하고 날려라....
		if (sceneIsLoading == false) 
		{
			_pReleaseScene->Release();
		}
		_pReleaseScene = nullptr;
	}

	//변경 효과가 있다면....
	if (_pCurrentChangeEffect)
	{
		_pCurrentChangeEffect->Update(deltaTime);
	}
	else
	{
		if (_pCurrentScene)
		{
			_pCurrentScene->Update(deltaTime);
		}
	}

}

void SceneManager::Render()
{
	//변경 효과가 있다면...
	if (_pCurrentChangeEffect && _pCurrentChangeEffect->IsDrawReady())	//드로우 준비가 되지 않았다면 체인지 Effect 를 그리면 안된다.
	{
		_pCurrentChangeEffect->Render();
	}
	else 
	{
		if (_pCurrentScene)
		{
			_pCurrentScene->Render();
		}
	}
}

bool SceneManager::ChangeScene(const std::string &key, uint32 effectNum)
{

	//교체 Effect
	SceneChangeEffect* pChangeEffect = _sceneChangeEffects[effectNum];

	//씬을 찾는다.
	SceneTable::iterator  iter = _scenes.find(key);

	//못찾았다.
	if (iter == _scenes.end()) 
	{
		return false;
	}

	//기존에 물려있는 씬이 있고 체인지 효과가 있다면...
	if (_pCurrentScene && pChangeEffect != nullptr) 
	{
		_pCurrentChangeEffect = pChangeEffect;

		//두번째씬은 Init 된상태로 들어간다.
		iter->second->Init();

		//체인지 효과 발동
		_pCurrentChangeEffect->ChangeStart( _pCurrentScene, iter->second, 1.0f);
	}
	//그게 아니라면..
	else 
	{
		//기존에 물려있던 씬 해제 씬으로 대입
		_pReleaseScene = _pCurrentScene;

		//새로운씬을 현재씬으로 
		_pCurrentScene = iter->second;

		//새로운씬 초기화
		_pCurrentScene->Init();
	}
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

void SceneManager::AddLoadingScene(const std::string & sceneName, IScene * pScene)
{
	if (pScene == nullptr)
	{
		return;
	}
	auto find = _scenes.find(sceneName);
	//없다면
	if (find == _scenes.end())
	{
		pScene->Init();
		_scenes.insert(std::make_pair(sceneName, pScene));
	}
}

//void SceneManager::RemoveScene(const std::string & key)
//{
//	auto find = _scenes.find(key);
//	if (find == _scenes.end())
//	{
//		find->second->Release();
//		find->second->Unload();
//		_scenes.erase(find);
//	}
//}
//
//void SceneManager::RemoveAllScenes()
//{
//	for (auto scene : _scenes)
//	{
//		scene.second->Release();
//		scene.second->Unload();
//	}
//	_scenes.clear();
//}
