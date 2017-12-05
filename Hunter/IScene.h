#ifndef I_SCENE_H
#define I_SCENE_H

#include "World.h"
#include "Camera.h"

#include "TransformSystem.h"
#include "RenderSystem.h"
#include "ActionSystem.h"
#include "ScriptSystem.h"
#include "CollisionSystem.h"

class EnvironmentSphere;
class BaseGameObject;

namespace video
{
	class Effect;
	class VertexBuffer;
	class IndexBuffer;
	class VertexDecl;
}

class IScene
{
	struct SceneVertex
	{
		Vector3 _position;
		Vector2 _texcoord;
		enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1};
	};
	//IDirect3DVertexDeclaration9 *_pDecl;

	video::VertexBuffer *_pScreenVertexBuffer{};
	video::IndexBuffer *_pScreenIndexBuffer{};
	video::VertexDecl *_pDecl{};

public :
	friend class GameObjectFactory;
	friend class Editor;
	friend class Terrain;

	IScene();
	virtual ~IScene() {}

	bool Init();
	bool Update(float deltaTime, const InputManager &input);
	void Release();
	bool Render();

	//PostEffect까지 모두 렌더 한다...
	//메인 카메라의 RenderToTexture 만 업데이트한다.
	bool RenderToMainCamTexture();

	void ReadyShadowMap(Terrain *pTerrain);

	LPDIRECT3DTEXTURE9 GetSceneTexture();

	virtual const char *GetSceneName() = 0;

protected :
	virtual bool SceneInit() = 0;
	virtual bool SceneUpdate(float deltaTime, const InputManager &input) = 0;
	virtual bool SceneRelease() = 0;

	virtual bool SceneRender0() = 0;
	virtual bool SceneRender1() { return true; }
	virtual bool SceneRender2() { return true; }
	virtual bool SceneRenderSprite() { return true; }

	SceneVertex _screenVertices[4];
	uint16 _screenIndices[6];

	World _world;
	std::vector<BaseGameObject *> _gameObjects;
	Camera _camera;

	DirectionalLight *_pMainLight;
	Camera _shadowCamera;

	EventChannel _channel;

	EnvironmentSphere *_pEnvironmentSphere{};
	video::Effect *_pPostEffect{};

	float _shadowDistance{ 100.0f };

	bool RenderEnvironmentSphere();

	TransformSystem _transformSystem;
	RenderSystem _renderSystem;
	ScriptSystem _scriptSystem;
	ActionSystem _actionSystem;
	CollisionSystem _collisionSystem;
	ParticleSystem _particleSystem;

	bool32 _editorInput{ false };

public :
};

#endif
