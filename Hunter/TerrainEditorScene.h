#ifndef TERRAIN_EDITOR_SCENE_H
#define TERRAIN_EDITOR_SCENE_H

#include "BaseScene.h"

class TerrainEditorScene : public BaseScene
{
public :
	TerrainEditorScene();
	virtual ~TerrainEditorScene();

	virtual bool Init();
	virtual bool Update(float deltaTime, const InputManager &input);
	virtual bool Render();

	virtual void Release();

	virtual const char *GetSceneName();

	void RegisterEvents();

private :

	World _world;
	std::vector<Entity> _entities;

	TransformSystem _transformSystem;
	ActionSystem _actionSystem;
	CollisionSystem _collisionSystem;
	RenderSystem _renderSystem;
	ScriptSystem _scriptSystem;

	video::RenderView *_mainRenderView{};
	Camera _camera;

	Editor *_editor{};
};


#endif