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
	//����ȯ ȿ�� �߰�
	_sceneChangeEffects.push_back(nullptr);		//0 ���� ȿ�� ���� ���� NULL �߰�

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
	//���������ִٸ�
	if (_pReleaseScene)
	{
		bool32 sceneIsLoading = false;
		SceneTableIter iter;

		for (iter = _loadingScene.begin(); iter != _loadingScene.end(); ++iter) 
		{
			//�������� �ε����̶�� ������ �ȵȴ�.
			if (iter->second == _pReleaseScene)
			{
				sceneIsLoading = true;
				break;
			}
		}

		//�������� ������ ���� �ε����� �ƴ϶�� �Ƚ��ϰ� ������....
		if (sceneIsLoading == false) 
		{
			_pReleaseScene->Release();
		}
		_pReleaseScene = nullptr;
	}

	//���� ȿ���� �ִٸ�....
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
	//���� ȿ���� �ִٸ�...
	if (_pCurrentChangeEffect && _pCurrentChangeEffect->IsDrawReady())	//��ο� �غ� ���� �ʾҴٸ� ü���� Effect �� �׸��� �ȵȴ�.
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

	//��ü Effect
	SceneChangeEffect* pChangeEffect = _sceneChangeEffects[effectNum];

	//���� ã�´�.
	SceneTable::iterator  iter = _scenes.find(key);

	//��ã�Ҵ�.
	if (iter == _scenes.end()) 
	{
		return false;
	}

	//������ �����ִ� ���� �ְ� ü���� ȿ���� �ִٸ�...
	if (_pCurrentScene && pChangeEffect != nullptr) 
	{
		_pCurrentChangeEffect = pChangeEffect;

		//�ι�°���� Init �Ȼ��·� ����.
		iter->second->Init();

		//ü���� ȿ�� �ߵ�
		_pCurrentChangeEffect->ChangeStart( _pCurrentScene, iter->second, 1.0f);
	}
	//�װ� �ƴ϶��..
	else 
	{
		//������ �����ִ� �� ���� ������ ����
		_pReleaseScene = _pCurrentScene;

		//���ο���� ��������� 
		_pCurrentScene = iter->second;

		//���ο�� �ʱ�ȭ
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
	//���ٸ�
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
	//���ٸ�
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
