#ifndef I_SCENE_H
#define I_SCENE_H

#include "GameUtils.h"
class Camera;
class DirectionalLight;
class Terrain;

class IScene
{
protected :
	struct SceneVertex
	{
		Vector3 _pos;
		Vector2 _uv;

		enum {FVF = D3DFVF_XYZ | D3DFVF_TEX1};
	};
	Camera *_pCamera{};
	LPDIRECT3DCUBETEXTURE9 _pEnvironmentTexture{};
	LPD3DXMESH _pEnvironmentMesh{};
	video::EffectHandle _environmentEffectHandle;

	SceneVertex	_scenePlaneVertex[4];
	uint16 _scenePlaneIndex[6];

	video::EffectHandle _postEffectHandle;					//PostEffect

	DirectionalLight* _pBaseDirectionalLight{};	//씬에 배치된 기본라이팅
	Camera *_pDirectionLightCamera{};		//방향성 광원에 따른 카메라...

	float _shadowDistance;				//그림자 거리

	World _world;
	TransformSystem _transformSystem;


public :
	IScene();
	virtual ~IScene() {}

	HRESULT Init();
	void Release();
	void Update(float timeDelta);
	void Render();

	//메인 카메라의 RenderToTexture 만 업데이트한다.
	void RenderToMainCamTexture();

	void SetEnvironment(std::string cubeFilePath);
	void RenderEnvironment();

	void ReadyShadowMap(std::vector<Entity> &renderObjects, Terrain *pTerrain = nullptr);

	//메인카메라의 랜더 Texture 를 얻는다.
	LPDIRECT3DTEXTURE9 GetTexture();

	virtual bool SceneInit() = 0;
	virtual void SceneRelease() = 0;
	virtual bool SceneUpdate(float deltaTime) = 0;
	virtual void SceneRender0() {}
	virtual void SceneRender1() = 0;
	virtual void SceneRender2() {}

	virtual void SceneRenderSprite() {}



};

#endif
