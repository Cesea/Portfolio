#include "stdafx.h"

#include "BaseGameObject.h"

IScene::IScene()
	:_world(4096)
{
	video::EffectHandle effectHandle = VIDEO->GetEffect("PostEffect.fx");
	_pPostEffect = VIDEO->GetEffect(effectHandle);

	_screenVertices[0]._position = Vector3(-1.0f, 1.0f, 0.2f);
	_screenVertices[1]._position = Vector3(  1.0f, 1.0f, 0.2f );
	_screenVertices[2]._position = Vector3(  1.0f,-1.0f, 0.2f );
	_screenVertices[3]._position = Vector3( -1.0f,-1.0f, 0.2f );

	_screenVertices[0]._texcoord = Vector2( 0.0f, 0.0f );
	_screenVertices[1]._texcoord = Vector2( 1.0f, 0.0f );
	_screenVertices[2]._texcoord = Vector2( 1.0f, 1.0f );
	_screenVertices[3]._texcoord = Vector2( 0.0f, 1.0f );

	_screenIndices[0] = 0;
	_screenIndices[1] = 1;
	_screenIndices[2] = 3;
	_screenIndices[3] = 3;
	_screenIndices[4] = 1;
	_screenIndices[5] = 2;



	video::VertexDecl decl;
	decl.Begin();
	D3DVERTEXELEMENT9 elements;

	elements.Stream = 0;
	elements.Offset = 0;
	elements.Type = D3DDECLTYPE_FLOAT3;
	elements.Method = D3DDECLMETHOD_DEFAULT;
	elements.Usage = D3DDECLUSAGE_POSITION;
	elements.UsageIndex = 0;
	decl.Add(elements);

	elements.Stream = 0;
	elements.Offset = sizeof(Vector3);
	elements.Type = D3DDECLTYPE_FLOAT2;
	elements.Method = D3DDECLMETHOD_DEFAULT;
	elements.Usage = D3DDECLUSAGE_TEXCOORD;
	elements.UsageIndex = 0;
	decl.Add(elements);
	decl.End(sizeof(SceneVertex));

	video::VertexDeclHandle declHandle = VIDEO->CreateVertexDecl(&decl, "Scene");
	_pDecl = VIDEO->GetVertexDecl(declHandle);

	Memory mem;
	mem._data = _screenVertices;
	mem._size = sizeof(SceneVertex) * 4;
	video::VertexBufferHandle vBufferHandle = VIDEO->CreateVertexBuffer(&mem, declHandle, "Scene");

	_pScreenVertexBuffer = VIDEO->GetVertexBuffer(vBufferHandle);

	mem._data = _screenIndices;
	mem._size = sizeof(uint16) * 6;
	video::IndexBufferHandle iBufferHandle = VIDEO->CreateIndexBuffer(&mem, sizeof(uint16), "Scene");

	_pScreenIndexBuffer = VIDEO->GetIndexBuffer(iBufferHandle);

}

bool IScene::Init()
{
	//시스템 생성
	_world.AddSystem<RenderSystem>(_renderSystem);
	_world.AddSystem<TransformSystem>(_transformSystem);
	_world.AddSystem<ActionSystem>(_actionSystem);
	_world.AddSystem<ScriptSystem>(_scriptSystem);
	_world.AddSystem<CollisionSystem>(_collisionSystem);
	_world.AddSystem<ParticleSystem>(_particleSystem);

	//카메라 생성
	_camera.CreateFromWorld(_world);
	_camera.SetRotationSpeed(2.0f);
	_camera.SetMoveSpeed(3.0f);
	//_camera.GetEntity().GetComponent<TransformComponent>().MovePositionWorld(Vector3(0.0f, 4.0f, -6.0f));
	//메인카메라 RenderToTexture 준비
	_camera.ReadyRenderToTexture( WINSIZEX, WINSIZEY );


	_shadowDistance = 30.0f;
	_shadowCamera.CreateFromWorld(_world);
	_shadowCamera._ortho = true;
	_shadowCamera._camNear = 0.1f;
	_shadowCamera._camFar = _shadowDistance * 2.0f;
	_shadowCamera._aspect = 1;
	_shadowCamera._orthoSize = _shadowDistance * 1.0f;	//투영크기는 그림자크기로...
	_shadowCamera.ReadyShadowTexture(1024);


	//라이트 생성
	_pMainLight = new DirectionalLight();
	_pMainLight->CreateFromWorld(_world);

	_pEnvironmentSphere = new EnvironmentSphere;

	//씬의 초기화 이루어진다.
	if (!SceneInit())
	{
		return false;
	}
	return true;
}

bool IScene::Update(float deltaTime, const InputManager & input)
{
	if (!_editorInput)
	{
		_camera.MoveAndRotate(deltaTime, input);
	}
	_editorInput = false;


	_camera.UpdateMatrix();
	_camera.UpdateCamToDevice();
	_camera.UpdateFrustum();

	//메인카메라에 DirectionLight 를 방향을 유지한체 따라다니게....
	
	//광원 위치
	Vector3 camPos = _camera.GetEntity().GetComponent<TransformComponent>().GetWorldPosition();		//메인카메라의 위치
	Vector3 camFront = _camera.GetEntity().GetComponent<TransformComponent>().GetForward();			//메인카메라의 정면
	Vector3 lightDir = _pMainLight->GetEntity().GetComponent<TransformComponent>().GetForward();	//방향성 광원의 방향

	Vector3 lightPos = camPos + ( camFront * ( _shadowDistance * 0.5f ) ) + ( -lightDir * _shadowDistance );

	_shadowCamera.GetEntity().GetComponent<TransformComponent>().SetWorldPosition(lightPos.x, lightPos.y, lightPos.z );
	_shadowCamera.GetEntity().GetComponent<TransformComponent>().LookDirection(lightDir);

	SceneUpdate(deltaTime, input);
	return true;
}

void IScene::Release()
{
	SceneRelease();
}

bool IScene::Render()
{
	_camera.RenderTextureBegin( 0xff404040 );
	
	//환경 랜더
	RenderEnvironmentSphere();

	//랜더된다.
	SceneRender0();
	SceneRender1();
	SceneRender2();

	_camera.RenderTextureEnd();

	//Scene 랜더
	_pPostEffect->SetTechnique( "Base" );
	HRESULT re = _pPostEffect->_ptr->SetTexture( "screenTex", _camera.GetRenderTexture() );

	float pixelU = 1.0f / WINSIZEX;
	float pixelV = 1.0f / WINSIZEY;
	_pPostEffect->SetFloat( "pixelSizeU", pixelU );
	_pPostEffect->SetFloat( "pixelSizeV", pixelV );
	_pPostEffect->SetFloat( "pixelHalfSizeU", pixelU * 0.5f );
	_pPostEffect->SetFloat( "pixelHalfSizeV", pixelV * 0.5f );

	gpDevice->SetStreamSource(0, _pScreenVertexBuffer->_ptr, 0, sizeof(SceneVertex));
	gpDevice->SetIndices(_pScreenIndexBuffer->_ptr);
	gpDevice->SetVertexDeclaration(_pDecl->_ptr);

	uint32 numPass = _pPostEffect->BeginEffect();
	for (uint32 i = 0; i < numPass; ++i)
	{
		_pPostEffect->BeginPass(i);

		gpDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
			
		_pPostEffect->EndPass();
	}
	_pPostEffect->EndEffect();
	
	SceneRenderSprite();

	imguiRenderDraw();

	return true;
}

bool IScene::RenderToMainCamTexture()
{
	_camera.RenderTextureBegin(0x00101010);
	//환경 랜더
	RenderEnvironmentSphere();
	//랜더된다.
	SceneRender0();
	SceneRender1();
	SceneRender2();
	
	_camera.RenderTextureEnd();
	return true;
}

void IScene::ReadyShadowMap(Terrain *pTerrain)
{
	//방향성광원에 붙은 카메라의 Frustum 업데이트
	_shadowCamera.UpdateMatrix();
	_shadowCamera.UpdateFrustum();

	////다이렉션라이팅 카메라에 들어오는 애들만 그린다...
	//static std::vector<cBaseObject*>		shadowCullObject;
	//shadowCullObject.clear();
	//for( int i = 0 ; i < renderObjects->size() ; i++ )
	//{
	//	//프러스텀 안에 있니?
	//	if( this->pDirectionLightCamera->Frustum.IsInFrustum( (*renderObjects)[i] ) )
	//	{
	//		shadowCullObject.push_back( (*renderObjects)[i] );
	//	}
	//}

	//쉐도우 맵 그린다.
	_shadowCamera.RenderTextureBegin( 0xffffffff );

	video::StaticXMesh::SetCamera( _shadowCamera );

	video::SkinnedXMesh::SetCamera( _shadowCamera);

	_renderSystem.RenderShadow(_shadowCamera);

	//만약 Terrain 도 쉐도우 맵을 그려야한다면...
	//if(nullptr != pTerrain)
	//{
	//	pTerrain->RenderShadow( _shadowCamera ); 
	//}
	_shadowCamera.RenderTextureEnd();

	//만약 Terrain 도 쉐도우 맵을 셋팅한다면...
	if(nullptr != pTerrain)
	{
		pTerrain->EffectSetTexture( "ShadowTexture", 
				_shadowCamera.GetRenderTexture() );

		pTerrain->EffectSetMatrix( "matLightViewProjection", 
				_shadowCamera.GetViewProjectionMatrix() );
	}

	//쉐도우 Texture
	VIDEO->GetEffect(video::StaticXMesh::_sEffectHandle)->_ptr->SetTexture( "ShadowTexture", _shadowCamera.GetRenderTexture());
	VIDEO->GetEffect(video::StaticXMesh::_sEffectHandle)->SetMatrix( "matLightViewProjection", _shadowCamera.GetViewProjectionMatrix());
	
	VIDEO->GetEffect(video::SkinnedXMesh::_sEffectHandle)->_ptr->SetTexture( "ShadowTexture", _shadowCamera.GetRenderTexture());
	VIDEO->GetEffect(video::SkinnedXMesh::_sEffectHandle)->SetMatrix( "matLightViewProjection", _shadowCamera.GetViewProjectionMatrix());
}

LPDIRECT3DTEXTURE9 IScene::GetSceneTexture()
{
	return _camera.GetRenderTexture();
}

bool IScene::RenderEnvironmentSphere()
{
	_pEnvironmentSphere->Render(_camera);
	return true;
}