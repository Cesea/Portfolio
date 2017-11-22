#include "stdafx.h"

#include "Camera.h"
#include "DirectionalLight.h"
#include "Terrain.h"

IScene::IScene()
	:_world(4096)
{
	_world.AddSystem<TransformSystem>(_transformSystem);

	//ȯ�� Effect ���� ���´�.
	_environmentEffectHandle = VIDEO->GetEffect("Environment.fx");
	_pCamera = new Camera();

	//�⺻ ���� ����
	_pBaseDirectionalLight = new DirectionalLight();
	_pBaseDirectionalLight->CreateFromWorld(_world);
	//���⼺ ������ ī�޶� �� �����Ѵ�.
	_pDirectionLightCamera = new Camera();

	//���⼺���� �ڽĿ� ���δ�.
	//this->pDirectionLightCamera->AttachTo( this->pSceneBaseDirectionLight->pTransform );

	//�⺻ ��
	_pBaseDirectionalLight->_color = D3DXCOLOR(1, 1, 1, 1);
	_pBaseDirectionalLight->_intensity = 1.0f;

	//�׸��� �Ÿ�
	_shadowDistance = 100.0f;

	//ī�޶��� ��������� �ٲ۴�...
	this->_pDirectionLightCamera->_ortho = true;
	this->_pDirectionLightCamera->_camNear = 0.1f;
	this->_pDirectionLightCamera->_camFar = _shadowDistance * 2.0f;
	this->_pDirectionLightCamera->_aspect = 1;
	this->_pDirectionLightCamera->_orthoSize = _shadowDistance * 1.5f;	//����ũ��� �׸���ũ���...

																		//���⼺���� ī�޶��� RenderToTexture �غ�
																		//this->pDirectionLightCamera->ReadyShadowTexture(4096);
}

HRESULT IScene::Init()
{
	//���� �ʱ�ȭ �̷������.
	if (!this->SceneInit())
	{
		return false;
	}

	//����ī�޶� RenderToTexture �غ�
	_pCamera->ReadyRenderToTexture(WINSIZEX, WINSIZEY);

	//0----1
	//|   /|
	//|  / |
	//| /  |
	//|/   |
	//3----2

	this->_scenePlaneVertex[0]._pos = Vector3(-1.0f, 1.0f, 0.0f);
	this->_scenePlaneVertex[1]._pos = Vector3(1.0f, 1.0f, 0.0f);
	this->_scenePlaneVertex[2]._pos = Vector3(1.0f, -1.0f, 0.0f);
	this->_scenePlaneVertex[3]._pos = Vector3(-1.0f, -1.0f, 0.0f);

	this->_scenePlaneVertex[0]._uv = Vector2(0.0f, 0.0f);
	this->_scenePlaneVertex[1]._uv = Vector2(1.0f, 0.0f);
	this->_scenePlaneVertex[2]._uv = Vector2(1.0f, 1.0f);
	this->_scenePlaneVertex[3]._uv = Vector2(0.0f, 1.0f);

	this->_scenePlaneIndex[0] = 0;
	this->_scenePlaneIndex[1] = 1;
	this->_scenePlaneIndex[2] = 3;
	this->_scenePlaneIndex[3] = 3;
	this->_scenePlaneIndex[4] = 1;
	this->_scenePlaneIndex[5] = 2;

	_postEffectHandle = VIDEO->GetEffect("PostEffect.fx");

	return S_OK;
}

void IScene::Release()
{
	COM_RELEASE(_pEnvironmentMesh);
	COM_RELEASE(_pEnvironmentTexture);

	SAFE_DELETE(_pCamera);

	SAFE_DELETE(_pBaseDirectionalLight);
	SAFE_DELETE(_pDirectionLightCamera);
}

void IScene::Update(float timeDelta)
{
	this->SceneUpdate( timeDelta );
}

void IScene::Render()
{
	_pCamera->RenderTextureBegin(0x00101010);

	//ȯ�� ����
	this->RenderEnvironment();

	//�����ȴ�.
	this->SceneRender0();
	this->SceneRender1();
	this->SceneRender2();

#ifdef _DEBUG		//����� ��忡���� ����
	//����̽� ������ ���� ���
	//���� �׸���
	GIZMOMANAGER->WorldGrid(1, 10);
#endif

	_pCamera->RenderTextureEnd();


	video::Effect *postEffect = VIDEO->GetEffect(_postEffectHandle);
	//Scene ����
	postEffect->SetTechnique("Base");

	postEffect->_ptr->SetTexture("screenTex", _pCamera->GetRenderTexture());
	float pixelU = 1.0f / WINSIZEX;
	float pixelV = 1.0f / WINSIZEY;
	postEffect->SetFloat("pixelSizeU", pixelU);
	postEffect->SetFloat("pixelSizeV", pixelV);
	postEffect->SetFloat("pixelHalfSizeU", pixelU * 0.5f);
	postEffect->SetFloat("pixelHalfSizeV", pixelV * 0.5f);


	uint32 numPass = postEffect->BeginEffect();
	for (uint32 i = 0; i < numPass; i++) 
	{
		postEffect->BeginPass(i);

		gpDevice->SetFVF(SceneVertex::FVF);
		gpDevice->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST, 0,
			4, 2, this->_scenePlaneIndex, D3DFMT_INDEX16,
			this->_scenePlaneVertex, sizeof(SceneVertex));

		postEffect->EndPass();
	}
	postEffect->EndEffect();

	SPRITEMANAGER->BeginSpriteRender();
	this->SceneRenderSprite();
	SPRITEMANAGER->EndSpriteRender();
}

void IScene::RenderToMainCamTexture()
{
	_pCamera->RenderTextureBegin(0x00101010);

	//ȯ�� ����
	this->RenderEnvironment();

	//�����ȴ�.
	this->SceneRender0();
	this->SceneRender1();
	this->SceneRender2();


#ifdef _DEBUG		//����� ��忡���� ����
	//����̽� ������ ���� ���
	//���� �׸���
	GIZMOMANAGER->WorldGrid(1, 10);
#endif
	_pCamera->RenderTextureEnd();
}

void IScene::SetEnvironment(std::string cubeFilePath)
{
	//������ ȯ����� ���õǾ������� ����..
	SAFE_RELEASE(_pEnvironmentTexture);

	//ȯ��� �ε�
	D3DXCreateCubeTextureFromFile(
		gpDevice,
		cubeFilePath.c_str(),
		&_pEnvironmentTexture);

	//ȯ�汸 �ε��Ǿ����� �ʴٸ�....
	if (this->_pEnvironmentMesh == NULL)
	{
		D3DXCreateSphere(gpDevice, 3.0f, 20, 20, &_pEnvironmentMesh, nullptr);
	}
}

void IScene::RenderEnvironment()
{
	//ȯ�� �� �ε��Ȱ� ���ٸ� �����ض�.....
	if (nullptr == _pEnvironmentTexture)
	{
		return;
	}

	video::Effect *pEnvironmentEffect = VIDEO->GetEffect(_environmentEffectHandle);
	//ȯ�� Effect ����
	pEnvironmentEffect->_ptr->SetTexture("MyCube_Tex", _pEnvironmentTexture);

	//WVP ��Ʈ����
	Matrix matWorld = this->_pCamera->GetTransform().GetFinalMatrix();
	Matrix matViewProj = this->_pCamera->GetViewProjectionMatrix();
	Matrix matWVP = matWorld * matViewProj;

	pEnvironmentEffect->SetMatrix("matWVP", matWVP);
	pEnvironmentEffect->SetMatrix("matWorld",matWorld);

	//�׷���...
	uint32 numPass = pEnvironmentEffect->BeginEffect();		//���̴��� �׸��ٴ� ���� �˸��� pass ���� ��´�.

	for (uint32 i = 0; i < numPass; i++)	//���� ����� ����.
	{
		pEnvironmentEffect->BeginPass(i);			//i��° ��������

		_pEnvironmentMesh->DrawSubset(0);

		pEnvironmentEffect->EndPass();				//i��° ������
	}
	pEnvironmentEffect->EndEffect();						//���̴��� �׸��� ��

}

void IScene::ReadyShadowMap(std::vector<Entity>& renderObjects, Terrain * pTerrain)
{
	////���⼺������ ���� ī�޶��� Frustum ������Ʈ
	//this->pDirectionLightCamera->UpdateMatrix();
	//this->pDirectionLightCamera->UpdateFrustum();


	////���̷��Ƕ����� ī�޶� ������ �ֵ鸸 �׸���...
	//static std::vector<cBaseObject*>		shadowCullObject;
	//shadowCullObject.clear();

	//for (int i = 0; i < renderObjects->size(); i++) {

	//	//�������� �ȿ� �ִ�?
	//	if (this->pDirectionLightCamera->Frustum.IsInFrustum((*renderObjects)[i]))
	//	{
	//		shadowCullObject.push_back((*renderObjects)[i]);
	//	}
	//}


	////������ �� �׸���.
	//this->pDirectionLightCamera->RenderTextureBegin(0xffffffff);

	//cXMesh_Static::SetCamera(this->pDirectionLightCamera);
	//cXMesh_Static::SetTechniqueName("CreateShadow");

	//cXMesh_Skinned::SetCamera(this->pDirectionLightCamera);
	//cXMesh_Skinned::SetTechniqueName("CreateShadow");



	//for (int i = 0; i < shadowCullObject.size(); i++) {
	//	if (shadowCullObject[i]->IgnoreCreateShadow == false)
	//		shadowCullObject[i]->Render();
	//}

	////���� Terrain �� ������ ���� �׷����Ѵٸ�...
	//if (pTerrain != NULL)
	//{
	//	pTerrain->RenderShadow(this->pDirectionLightCamera);
	//}


	//this->pDirectionLightCamera->RenderTextureEnd();


	////���� Terrain �� ������ ���� �����Ѵٸ�...
	//if (pTerrain != NULL)
	//{
	//	pTerrain->m_pTerrainEffect->SetTexture("Shadow_Tex",
	//		this->pDirectionLightCamera->GetRenderTexture());

	//	pTerrain->m_pTerrainEffect->SetMatrix("matLightViewProjection",
	//		&this->pDirectionLightCamera->GetViewProjectionMatrix());
	//}

	////������ Texture
	//cXMesh_Static::sStaticMeshEffect->SetTexture("Shadow_Tex",
	//	this->pDirectionLightCamera->GetRenderTexture());

	////������ ���
	//cXMesh_Static::sStaticMeshEffect->SetMatrix("matLightViewProjection",
	//	&this->pDirectionLightCamera->GetViewProjectionMatrix());


	////������ Texture
	//cXMesh_Skinned::sSkinnedMeshEffect->SetTexture("Shadow_Tex",
	//	this->pDirectionLightCamera->GetRenderTexture());

	////������ ���
	//cXMesh_Skinned::sSkinnedMeshEffect->SetMatrix("matLightViewProjection",
	//	&this->pDirectionLightCamera->GetViewProjectionMatrix());
}

LPDIRECT3DTEXTURE9 IScene::GetTexture()
{
	return LPDIRECT3DTEXTURE9();
}
