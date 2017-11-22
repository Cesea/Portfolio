#include "stdafx.h"

#include "Camera.h"
#include "DirectionalLight.h"
#include "Terrain.h"

IScene::IScene()
	:_world(4096)
{
	_world.AddSystem<TransformSystem>(_transformSystem);

	//환경 Effect 물어 놓는다.
	_environmentEffectHandle = VIDEO->GetEffect("Environment.fx");
	_pCamera = new Camera();

	//기본 광원 생성
	_pBaseDirectionalLight = new DirectionalLight();
	_pBaseDirectionalLight->CreateFromWorld(_world);
	//방향성 광원에 카메라 를 생성한다.
	_pDirectionLightCamera = new Camera();

	//방향성광원 자식에 붙인다.
	//this->pDirectionLightCamera->AttachTo( this->pSceneBaseDirectionLight->pTransform );

	//기본 값
	_pBaseDirectionalLight->_color = D3DXCOLOR(1, 1, 1, 1);
	_pBaseDirectionalLight->_intensity = 1.0f;

	//그림자 거리
	_shadowDistance = 100.0f;

	//카메라의 투영방식을 바꾼다...
	this->_pDirectionLightCamera->_ortho = true;
	this->_pDirectionLightCamera->_camNear = 0.1f;
	this->_pDirectionLightCamera->_camFar = _shadowDistance * 2.0f;
	this->_pDirectionLightCamera->_aspect = 1;
	this->_pDirectionLightCamera->_orthoSize = _shadowDistance * 1.5f;	//투영크기는 그림자크기로...

																		//방향성광원 카메라의 RenderToTexture 준비
																		//this->pDirectionLightCamera->ReadyShadowTexture(4096);
}

HRESULT IScene::Init()
{
	//씬의 초기화 이루어진다.
	if (!this->SceneInit())
	{
		return false;
	}

	//메인카메라 RenderToTexture 준비
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

	//환경 랜더
	this->RenderEnvironment();

	//랜더된다.
	this->SceneRender0();
	this->SceneRender1();
	this->SceneRender2();

#ifdef _DEBUG		//디버그 모드에서만 실행
	//디바이스 랜더링 종료 명령
	//월드 그리드
	GIZMOMANAGER->WorldGrid(1, 10);
#endif

	_pCamera->RenderTextureEnd();


	video::Effect *postEffect = VIDEO->GetEffect(_postEffectHandle);
	//Scene 랜더
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

	//환경 랜더
	this->RenderEnvironment();

	//랜더된다.
	this->SceneRender0();
	this->SceneRender1();
	this->SceneRender2();


#ifdef _DEBUG		//디버그 모드에서만 실행
	//디바이스 랜더링 종료 명령
	//월드 그리드
	GIZMOMANAGER->WorldGrid(1, 10);
#endif
	_pCamera->RenderTextureEnd();
}

void IScene::SetEnvironment(std::string cubeFilePath)
{
	//기존의 환경맵이 셋팅되어있을지 몰라..
	SAFE_RELEASE(_pEnvironmentTexture);

	//환경맵 로딩
	D3DXCreateCubeTextureFromFile(
		gpDevice,
		cubeFilePath.c_str(),
		&_pEnvironmentTexture);

	//환경구 로딩되어있지 않다면....
	if (this->_pEnvironmentMesh == NULL)
	{
		D3DXCreateSphere(gpDevice, 3.0f, 20, 20, &_pEnvironmentMesh, nullptr);
	}
}

void IScene::RenderEnvironment()
{
	//환경 맵 로딩된게 없다면 리턴해라.....
	if (nullptr == _pEnvironmentTexture)
	{
		return;
	}

	video::Effect *pEnvironmentEffect = VIDEO->GetEffect(_environmentEffectHandle);
	//환경 Effect 셋팅
	pEnvironmentEffect->_ptr->SetTexture("MyCube_Tex", _pEnvironmentTexture);

	//WVP 매트릭스
	Matrix matWorld = this->_pCamera->GetTransform().GetFinalMatrix();
	Matrix matViewProj = this->_pCamera->GetViewProjectionMatrix();
	Matrix matWVP = matWorld * matViewProj;

	pEnvironmentEffect->SetMatrix("matWVP", matWVP);
	pEnvironmentEffect->SetMatrix("matWorld",matWorld);

	//그려라...
	uint32 numPass = pEnvironmentEffect->BeginEffect();		//셰이더로 그린다는 것을 알리고 pass 수를 얻는다.

	for (uint32 i = 0; i < numPass; i++)	//공정 수대로 돈다.
	{
		pEnvironmentEffect->BeginPass(i);			//i번째 공정시작

		_pEnvironmentMesh->DrawSubset(0);

		pEnvironmentEffect->EndPass();				//i번째 공정끝
	}
	pEnvironmentEffect->EndEffect();						//셰이더로 그리기 끝

}

void IScene::ReadyShadowMap(std::vector<Entity>& renderObjects, Terrain * pTerrain)
{
	////방향성광원에 붙은 카메라의 Frustum 업데이트
	//this->pDirectionLightCamera->UpdateMatrix();
	//this->pDirectionLightCamera->UpdateFrustum();


	////다이렉션라이팅 카메라에 들어오는 애들만 그린다...
	//static std::vector<cBaseObject*>		shadowCullObject;
	//shadowCullObject.clear();

	//for (int i = 0; i < renderObjects->size(); i++) {

	//	//프러스텀 안에 있니?
	//	if (this->pDirectionLightCamera->Frustum.IsInFrustum((*renderObjects)[i]))
	//	{
	//		shadowCullObject.push_back((*renderObjects)[i]);
	//	}
	//}


	////쉐도우 맵 그린다.
	//this->pDirectionLightCamera->RenderTextureBegin(0xffffffff);

	//cXMesh_Static::SetCamera(this->pDirectionLightCamera);
	//cXMesh_Static::SetTechniqueName("CreateShadow");

	//cXMesh_Skinned::SetCamera(this->pDirectionLightCamera);
	//cXMesh_Skinned::SetTechniqueName("CreateShadow");



	//for (int i = 0; i < shadowCullObject.size(); i++) {
	//	if (shadowCullObject[i]->IgnoreCreateShadow == false)
	//		shadowCullObject[i]->Render();
	//}

	////만약 Terrain 도 쉐도우 맵을 그려야한다면...
	//if (pTerrain != NULL)
	//{
	//	pTerrain->RenderShadow(this->pDirectionLightCamera);
	//}


	//this->pDirectionLightCamera->RenderTextureEnd();


	////만약 Terrain 도 쉐도우 맵을 셋팅한다면...
	//if (pTerrain != NULL)
	//{
	//	pTerrain->m_pTerrainEffect->SetTexture("Shadow_Tex",
	//		this->pDirectionLightCamera->GetRenderTexture());

	//	pTerrain->m_pTerrainEffect->SetMatrix("matLightViewProjection",
	//		&this->pDirectionLightCamera->GetViewProjectionMatrix());
	//}

	////쉐도우 Texture
	//cXMesh_Static::sStaticMeshEffect->SetTexture("Shadow_Tex",
	//	this->pDirectionLightCamera->GetRenderTexture());

	////쉐도우 행렬
	//cXMesh_Static::sStaticMeshEffect->SetMatrix("matLightViewProjection",
	//	&this->pDirectionLightCamera->GetViewProjectionMatrix());


	////쉐도우 Texture
	//cXMesh_Skinned::sSkinnedMeshEffect->SetTexture("Shadow_Tex",
	//	this->pDirectionLightCamera->GetRenderTexture());

	////쉐도우 행렬
	//cXMesh_Skinned::sSkinnedMeshEffect->SetMatrix("matLightViewProjection",
	//	&this->pDirectionLightCamera->GetViewProjectionMatrix());
}

LPDIRECT3DTEXTURE9 IScene::GetTexture()
{
	return LPDIRECT3DTEXTURE9();
}
