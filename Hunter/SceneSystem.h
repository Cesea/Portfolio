#ifndef SCENE_SYSTEM_H
#define SCENE_SYSTEM_H

#include "System.h"

#include "IScene.h"

#include <map>

class SceneSystem : public System
{
	typedef std::map<std::string, IScene *> SceneMap;
	typedef std::map<std::string, IScene *>::iterator SceneMapIter;

public :
	struct SceneChangeEvent
	{
		std::string newSceneName;
	};

public :

	SceneSystem();
	virtual ~SceneSystem();

	virtual bool Init(const std::string &name, const SystemSetting &setting);
	virtual void ShutDown();

	virtual void Update(float deltaTime);
	virtual EventChannel GetChannel() { return _channel; }
private :
	bool ChangeScene(const std::string &key);

	void AddScene(const std::string &sceneName, IScene *pScene);
	void RemoveScene(const std::string &key);

	void RemoveAllScenes();

private :

	SceneMap _scenes;
	IScene *_pCurrentScene;

};


#endif