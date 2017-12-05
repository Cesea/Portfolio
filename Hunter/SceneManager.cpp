#include "stdafx.h"
#include "SceneManager.h"

#include "WorldEditor.h"

//실질적으로 사용되는 씬들
#include "MapToolScene.h"
#include "TestScene.h"

//로딩 씬들
#include "LoadingScene00.h"
//#include "LoadingScene01.h"
//#include "LoadingScene02.h"

#include "SceneChangeEffect.h"
#include "SceneChangeEffectUpDown.h"


DWORD CALLBACK LoadingThread(LPVOID lpParam)
{
//매개변수로 받은씬 형변환
	IScene *pScene = (IScene*)lpParam;

	//로딩정보 초기화
	gEngine->GetScene()->_loadingProgress = 0.0f;
	gEngine->GetScene()->_loadingString = "";

	//매개변수로 받은 씬 초기화
	pScene->Init();

	//여기까지 왔다면 pScene 은 Init 가 끝난것임
	//SCENE_MGR->m_pNowScene = pScene;

	//초기화가 씬전환효과 보다 먼저이루어 질수 있다.
	//이때 효과가 진행중일때 교체하지 말자 
	//참고로 효과가 진행중일때 씬을 교체하게 되면 
	//씬효과 가끝나고 로딩씬이 불러지는 상황이라면
	//로딩씬이 호출이된다.

	while( true )
	{
		//교체 Effect 가 끝났다....
		if(nullptr == gEngine->GetScene()->_pCurrentChangeEffect)
		{
			//이곳에서 이전씬을 해제 할필요는 없다 어차피 이전씬은 로딩씬이기 때문이다..
			//SCENE_MGR->m_pReleaseScene = SCENE_MGR->m_pNowScene;

			//빠져 나가는 효과
			SceneChangeEffect* pChangeEffect = 
				gEngine->GetScene()->_changeEffects[gEngine->GetScene()->_outEffectIndex];

			//빠져나가는 효과가 존재하지 않는다면..
			if( nullptr == pChangeEffect )
			{
				gEngine->GetScene()->_pCurrentScene = pScene;
			}
			//빠져나가는 효과가 존재한다면...
			else 
			{
				//변경 효과씬 등록
				gEngine->GetScene()->_pCurrentChangeEffect = pChangeEffect;

				//이전씬은 효과가 끝나고 자동으로 해제된다. ( 이때 이전씬은 로딩씬이기 때문에 해제가 등록된다고 하더라도 해제 되지 않는다 )
				pChangeEffect->ChangeStart( gEngine->GetScene()->_pCurrentScene, pScene, 1.0f );
			}
			break;
		}
	}
	return 0;
}

SceneManager::SceneManager()
{
	_pCurrentScene = nullptr;
}

SceneManager::~SceneManager()
{
}

bool SceneManager::Init()
{

	GetChannel().Add<SceneChangeEvent, SceneManager>(*this);
	//기본 베이스 씬 세팅
	IScene *pScene = new MapToolScene;
	AddScene(pScene->GetSceneName(), pScene);
	pScene = new TestScene;
	AddScene(pScene->GetSceneName(), pScene);

	IScene *pLoadingScene = new LoadingScene00;
	AddLoadingScene(pLoadingScene->GetSceneName(), pLoadingScene);

	//씬 전환 이벤트 설정

	//씬전환 효과 추가
	_changeEffects.push_back(nullptr);		//0 번은 효과 없음 따라서 NULL 추가

	SceneChangeEffect *_pChangeEffect = new SceneChangeEffect;
	_pChangeEffect->Init();
	_changeEffects.push_back(_pChangeEffect);

	SceneChangeEffect *pEffect2 = new SceneChangeEffectUpDown;
	pEffect2->Init();
	_changeEffects.push_back(pEffect2 );

	//시작 씬을 설정한다
	ChangeScene("MapToolScene");

	return true;
}

void SceneManager::ShutDown()
{
	RemoveAllScenes();
}

void SceneManager::Update(float deltaTime, const InputManager &input)
{
	//날릴씬이있다면
	if(_pReleaseScene)
	{
		bool sceneIsLoading = false;
		SceneMap::iterator iter;

		for( iter = _loadingScenes.begin() ; iter != _loadingScenes.end() ; ++iter )
		{
			//날릴씬이 로딩씬이라면 날리면 안된다.
			if( iter->second == _pReleaseScene )
			{
				sceneIsLoading = true;
				break;
			}
		} 

		//지금현제 날리는 씬이 로딩씬이 아니라면 안심하고 날려라....
		if( sceneIsLoading == false )
		{	
			_pReleaseScene->Release();
		}
		_pReleaseScene = nullptr;
	}

	//변경 효과가 있다면....
	if( _pCurrentChangeEffect )
	{
		_pCurrentChangeEffect->Update( deltaTime );
	}
	else
	{
		if (_pCurrentScene)
		{
			_pCurrentScene->Update(deltaTime, input);
		}
	}
}

void SceneManager::Render()
{
	//변경 효과가 있다면...
	//드로우 준비가 되지 않았다면 체인지 Effect 를 그리면 안된다.
	if( _pCurrentChangeEffect && _pCurrentChangeEffect->IsDrawReady() )	
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

void SceneManager::AddScene(const std::string & sceneName, IScene *pScene)
{
	SceneMap::iterator  iter = _scenes.find( sceneName );
	
	//중복방지
	if( iter == _scenes.end() )
	{
		_scenes.insert( std::make_pair(sceneName, pScene) );
	}
	
}

void SceneManager::AddLoadingScene(const std::string & sceneName, IScene * pScene)
{
	SceneMap::iterator  iter = _loadingScenes.find(sceneName);
	
	//중복방지
	if( iter == _loadingScenes.end() )
	{
		//Init 를 하고 는다.
		pScene->Init();
		_loadingScenes.insert( std::make_pair(sceneName, pScene));
	}
}

void SceneManager::ChangeScene(const std::string & sceneName, int32 effectNum)
{
	//교체 Effect
	Assert(effectNum < _changeEffects.size());

	SceneChangeEffect *pChangeEffect = _changeEffects[ effectNum ];

	//씬을 찾는다.
	SceneMap::iterator  iter = _scenes.find( sceneName );

	//못찾았다.
	if(iter == _scenes.end())
	{
		return;
	}

	//기존에 물려있는 씬이 있고 체인지 효과가 있다면...
	if( _pCurrentScene && nullptr != pChangeEffect )
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
}

void SceneManager::ChangeSceneWithLoading(const std::string & sceneName, const std::string & loadingSceneName, 
	int32 inEffect, int32 outEffect)
{
	//로딩 씬을 찾는다.
	SceneMap::iterator  iter = _loadingScenes.find( loadingSceneName );

	//못찾았다.
	if(iter == _loadingScenes.end())
	{
		return;
	}

	//로딩씬기억
	IScene *pLoadingScene = iter->second;

	//변경될 씬을 찾는다.
	iter = _scenes.find( sceneName );

	//못찾았다.
	if(iter == _scenes.end())
	{
		return;
	}

	//변경될씬 기억
	IScene *pNextScene = iter->second;

	//나가는 Effect 기억
	_outEffectIndex = outEffect;

	//
	// 멀티 쓰레드 로딩 시작
	//
	//다른 쓰레드로 다음 씬을 Init 한다.
	DWORD dwThreadID;
	HANDLE hThread;

	//일꾼 생성
	hThread = CreateThread(	
		NULL,					//쓰레드 속성 일단 NULL				
		0,						//쓰레드의 스텍 사이즈 (0 으로하면 MainThread 와 같은 1MB 가 된다 )
		LoadingThread,			//새롭게생성되는 쓰레드의 메인함수 CALLBACK 함수포인터
		pNextScene,				//새롭게생성되는 쓰레드의 메인함수의 매개변수 포인터
		0,						//쓰레드 플레그 ( 0 으로 주면 바로 LoadingThread 가실행 )
		&dwThreadID				//만들어진 쓰레드 ID 
		);
	
	//더이상 조작할필요는 없느니 핸들은 날려도 무방
	CloseHandle( hThread );

	//씬 변경 효과 Effect
	Assert(inEffect < _changeEffects.size());
	SceneChangeEffect* pChangeEffect = _changeEffects[inEffect];

	//진입시 효과 전환이 없다면..
	if(nullptr == pChangeEffect)
	{
		//이전씬은 해제씬으로 등록
		_pReleaseScene = _pCurrentScene;
		//새로운씬을 로딩씬으로 
		_pCurrentScene = pLoadingScene;
	}
	//진입씬 효과가 존재 한다면...
	else
	{
		//변경 효과씬 등록
		_pCurrentChangeEffect = pChangeEffect;

		//이전씬은 효과가 끝나고 자동으로 해제된다.
		pChangeEffect->ChangeStart(_pCurrentScene, pLoadingScene, 1.0f);
	}
}

//bool SceneManager::ChangeScene(const std::string &key)
//{
//	auto find = _scenes.find(key);
//	if (find == _scenes.end())
//	{
//		Console::Log("There is no such scene %s\n", key);
//		return false;
//	}
//
//	bool initResult = find->second->Init();
//	if (!initResult)
//	{
//		Console::Log("Scene initialization failed.");
//		return false;
//	}
//
//	if (_pCurrentScene)
//	{
//		_pCurrentScene->Release();
//	}
//
//	_pCurrentScene = find->second;
//	return true;
//}
//
//void SceneManager::AddScene(const std::string &sceneName, IScene *pScene)
//{
//	if (pScene == nullptr)
//	{
//		return;
//	}
//	auto find = _scenes.find(sceneName);
//	//없다면
//	if (find == _scenes.end())
//	{
//		_scenes.insert(std::make_pair(sceneName, pScene));
//	}
//}
//
//void SceneManager::RemoveScene(const std::string & key)
//{
//	auto find = _scenes.find(key);
//	if (find == _scenes.end())
//	{
//		find->second->Release();
//		_scenes.erase(find);
//	}
//}

void SceneManager::RemoveAllScenes()
{
	for (auto scene : _scenes)
	{
		scene.second->Release();
	}
	_scenes.clear();
}
