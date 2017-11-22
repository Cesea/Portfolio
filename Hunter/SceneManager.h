#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "IScene.h"
#include "SceneChangeEffect.h"

#include <map>

class SceneManager
{
	friend class SceneChangeEffect;
	friend DWORD CALLBACK LoadingTread(LPVOID lpParam);

	typedef std::map<std::string, IScene *> SceneTable;
	typedef std::map<std::string, IScene *>::iterator SceneTableIter;

public :
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

	void Update(float deltaTime);
	void Render();
	EventChannel GetChannel() { return _channel; }
public :
	bool ChangeScene(const std::string &key, uint32 effectNum);
	bool ChangeSceneWithEffect(const std::string &key, int32 numEffect);
	bool ChangeSceneWithLoadingEffect(const std::string &key, int32 startEffectNum, int32 endEffectNum);

	void AddScene(const std::string &sceneName, IScene *pScene);
	void AddLoadingScene(const std::string &sceneName, IScene *pScene);


	void RemoveAllScenes();

private :
	SceneTable _scenes;
	SceneTable _loadingScene;

	IScene *_pCurrentScene{};
	IScene *_pReleaseScene{};

	std::vector<SceneChangeEffect*> _sceneChangeEffects;	
	SceneChangeEffect *_pCurrentChangeEffect{};	

	int32 _nOutEffect;	
public :
	float _loadingProgress{ 0 };
	std::string _loadingString;

	EventChannel _channel;

public :

	void Handle(const SceneChangeEvent &event)
	{
		Console::Log("Scene Changed %s\n", event.newSceneName);
		ChangeScene(event.newSceneName, 0);
	}
};


#endif