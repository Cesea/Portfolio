#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "IScene.h"
class SceneChangeEffect;

#include <map>

class SceneManager
{
	typedef std::map<std::string, IScene *> SceneMap;
	typedef std::map<std::string, IScene *>::iterator SceneMapIter;

public :
	friend DWORD CALLBACK LoadingThread(LPVOID lpParam);
	friend class SceneChangeEffect;

public :
	SceneManager();
	virtual ~SceneManager();

	bool Init();
	void ShutDown();

	void Update(float deltaTime, const InputManager &input);
	void Render();

	EventChannel GetChannel() { return _channel; }
private :
	//게임에 사용되는 씬추가 씬들은 Init 가 안된다.
	void AddScene(const std::string &sceneName, IScene *pScene);
	//게임에 사용되는 로딩씬 추가 씬들은 모두 Init 된상태가 된다.
	void AddLoadingScene(const std::string &sceneName, IScene *pScene );
	//씬변경
	void ChangeScene(const std::string &sceneName, int32 effectNum = 0 );
	//씬을 변경하는데 로딩씬을 추가하여 변경
	void ChangeSceneWithLoading(const std::string &sceneName, const std::string &loadingSceneName, 
		int32 inEffect, int32 outEffect );

	//bool ChangeScene(const std::string &key);
	//bool ChangeSceneWithEffect(const std::string &key, int32 numEffect);
	//bool ChangeSceneWithLoadingEffect(const std::string &key, int32 startEffectNum, int32 endEffectNum);

	//void AddScene(const std::string &sceneName, IScene *pScene);
	//void AddLoadingScene(const std::string &sceneName, IScene *pScene);
	//void RemoveScene(const std::string &key);
	//void RemoveLoadingScene(const std::string &key);

	void RemoveAllScenes();

private :
	SceneMap _scenes;
	SceneMap _loadingScenes;

	IScene *_pCurrentScene{};
	IScene *_pReleaseScene{};

	std::vector<SceneChangeEffect *> _changeEffects; //총 변경 Effect들
	SceneChangeEffect *_pCurrentChangeEffect{}; //현제 변경 Effect들
	int32 _outEffectIndex{};			//로딩시 빠져나가는 Effect 기억

	EventChannel _channel;

	//로딩 관련
public :
	float _loadingProgress{};
	std::string	_loadingString;

public :
	struct SceneChangeEvent
	{
		SceneChangeEvent(const std::string &name) : newSceneName(name) {}
		std::string newSceneName;
	};

	void Handle(const SceneChangeEvent &event)
	{
		Console::Log("Scene Changed %s\n", event.newSceneName);
		ChangeScene(event.newSceneName);
	}
};

#endif