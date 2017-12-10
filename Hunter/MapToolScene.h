#ifndef MAP_TOOL_SCENE_H
#define MAP_TOOL_SCENE_H

#include "IScene.h"

#include "WorldEditor.h"
//#include "Player.h"
//#include "Snake.h"
//#include "Dragon.h"
//#include "UI.h"

#include "LoadItem.h"

class MapToolScene : public IScene
{
	friend class Terrain;
	friend class Editor;
public:
	MapToolScene()
		:IScene::IScene()
	{};
	virtual ~MapToolScene() {}

	virtual bool SceneInit();
	virtual bool SceneUpdate(float deltaTime, const InputManager &input);
	virtual bool SceneRelease();

	virtual bool SceneRender0();
	//virtual bool SceneRender1() {}
	//virtual bool SceneRender2() {}
	virtual bool SceneRenderSprite();

	virtual const char *GetSceneName();

protected :
	Editor *_editor{};

private :


public:
	virtual void Handle(const Editor::GetObjectFromSceneEvent &event);
};

#endif