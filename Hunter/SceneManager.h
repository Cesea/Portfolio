#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "IScene.h"
//#include "SceneChangeEffect.h"

#include <map>

class SceneManager
{
	typedef std::map<std::string, IScene *> SceneMap;
	typedef std::map<std::string, IScene *>::iterator SceneMapIter;

public :
	friend DWORD CALLBACK LoadingThread(LPVOID lpParam);

	struct SceneChangeEvent
	{
		SceneChangeEvent(const std::string &name) : newSceneName(name) {}
		std::string newSceneName;
	};

public :

	SceneManager();
	virtual ~SceneManager();

	bool Init();
	void ShutDown();

	virtual void Update(float deltaTime, const InputManager &input);
	EventChannel GetChannel() { return _channel; }
private :
	bool ChangeScene(const std::string &key);
	bool ChangeSceneWithEffect(const std::string &key, int32 numEffect);
	bool ChangeSceneWithLoadingEffect(const std::string &key, int32 startEffectNum, int32 endEffectNum);

	void AddScene(const std::string &sceneName, IScene *pScene);
	void AddLoadingScene(const std::string &sceneName, IScene *pScene);
	void RemoveScene(const std::string &key);
	void RemoveLoadingScene(const std::string &key);

	void RemoveAllScenes();

private :
	SceneMap _scenes;
	SceneMap _loadingScene;

	IScene *_pCurrentScene;
	IScene *_pReleaseScene;

	EventChannel _channel;

public :

	void Handle(const SceneChangeEvent &event)
	{
		Console::Log("Scene Changed %s\n", event.newSceneName);
		ChangeScene(event.newSceneName);
	}
};


#endif