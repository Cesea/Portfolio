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

	DirectionalLight* _pBaseDirectionalLight{};	//���� ��ġ�� �⺻������
	Camera *_pDirectionLightCamera{};		//���⼺ ������ ���� ī�޶�...

	float _shadowDistance;				//�׸��� �Ÿ�

	World _world;
	TransformSystem _transformSystem;


public :
	IScene();
	virtual ~IScene() {}

	HRESULT Init();
	void Release();
	void Update(float timeDelta);
	void Render();

	//���� ī�޶��� RenderToTexture �� ������Ʈ�Ѵ�.
	void RenderToMainCamTexture();

	void SetEnvironment(std::string cubeFilePath);
	void RenderEnvironment();

	void ReadyShadowMap(std::vector<Entity> &renderObjects, Terrain *pTerrain = nullptr);

	//����ī�޶��� ���� Texture �� ��´�.
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
