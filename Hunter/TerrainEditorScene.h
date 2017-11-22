#ifndef TERRAIN_EDITOR_SCENE_H
#define TERRAIN_EDITOR_SCENE_H

#include "BaseScene.h"

class TerrainEditorScene : public BaseScene
{
public :
	TerrainEditorScene();
	virtual ~TerrainEditorScene();

	virtual bool32 Load();
	virtual bool32 Unload();

	virtual bool32 Init();
	virtual bool32 Update(float deltaTime);
	virtual bool32 Render();

	virtual void Release();

	virtual const char *GetSceneName();
	virtual bool32 IsActive();

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

	video::EffectHandle _staticEffect;
	video::EffectHandle _skinnedEffect;
	video::EffectHandle _terrainEffect;

	Editor *_editor{};
	video::FontHandle _font;

};


#endif