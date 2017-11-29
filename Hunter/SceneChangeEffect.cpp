#include "stdafx.h"
#include "SceneChangeEffect.h"

SceneChangeEffect::SceneChangeEffect(void)
{
	_pPrevScene = nullptr;
	_pNextScene = nullptr;

	_pPrevRenderTexture = nullptr;
	_pNextRenderTexture = nullptr;
}

SceneChangeEffect::~SceneChangeEffect(void)
{
}

void SceneChangeEffect::Init()
{
}

void SceneChangeEffect::Relase()
{
}

void SceneChangeEffect::ChangeStart(IScene * pPrevScene, IScene * pNextScene, float changeTime)
{
	_pPrevScene = pPrevScene;
	_pNextScene = pNextScene;

	//시간 초기화
	_totalTime = changeTime;
	_deltaTime = 0.0f;
	_timeFactor = 0.0f;

	//시작시간 기억
	_startTime = ( float )(APPTIMER->GetTotalTimeElapsed());

	//교체씬 드로우 여부 false 로 시작
	_drawReady = false;		
}

void SceneChangeEffect::Update(float deltaTime)
{
	if (_drawReady == false)
	{
		//LOG_MGR->AddLog("씬 교체 드로우" );

		//이전씬화면 랜더
		this->_pPrevScene->Update(deltaTime, *gEngine->GetInput());
		this->_pPrevScene->RenderToMainCamTexture();

		this->_pNextScene->Update(deltaTime, *gEngine->GetInput());
		this->_pNextScene->RenderToMainCamTexture();

		//랜더 명령이 먹었기 때문에 씬의 화면 Texture 를 얻어올수 있다....
		_pPrevRenderTexture = _pPrevScene->GetSceneTexture();
		_pNextRenderTexture =  _pNextScene->GetSceneTexture();

		/*
		//참고 복사
		//원본 Texture
		LPDIRECT3DTEXTURE9 pSrcTex = m_pPrevScene->GetTexture();
		//복사할 위치 Texture ( 빈Texture 라도 만들어져있어야 한다 )
		LPDIRECT3DTEXTURE9 pDstTex = m_pPrevRenderTexture;
		// pSrcTex 의 Surface 를 얻는다.
		LPDIRECT3DSURFACE9 srcSurface = NULL;
		pSrcTex->GetSurfaceLevel( 0, &srcSurface );
		// pDstTex 의 Surface 를 얻는다.
		LPDIRECT3DSURFACE9 destSurface = NULL;
		pDstTex->GetSurfaceLevel( 0, &destSurface );
		//고속복사
		Device->StretchRect( 
			srcSurface,				//원본 Surface
			NULL,					//원본 Surface 의 영역 ( NULL 이면 전체 )
			destSurface, 			//소스 Surface
			NULL, 					//소스 Surface 의 영역 ( NULL 이면 전체 )
			D3DTEXF_LINEAR			//필터링
			);
		srcSurface->Release();
		pDstTex->Release();
		*/

		_drawReady = true;
	}


	//현제시간 
	float nowTime = ( float )(APPTIMER->GetTotalTimeElapsed() );
	_deltaTime = nowTime - _startTime;


	//Factor 계산
	_timeFactor = _deltaTime / _totalTime;
	if( _timeFactor > 1.0f )
		_timeFactor = 1.0f;

	//교채시간이 다 되었다면....
	if( _deltaTime > _totalTime )
	{
		SCENE_MGR->GetInstance()->m_pNowScene = _pNextScene;

		//이전씬은 해제씬으로...
		SCENE_MGR->GetInstance()->m_pReleaseScene = _pPrevScene;
		
		//씬메니져의 교체 Effect 은 없다.
		SCENE_MGR->GetInstance()->_pCurrentChangingEffect = nullptr;

		_drawReady = false;
	}
}

void SceneChangeEffect::Render()
{
	//드로우 레디가 되어있지 안다면 그리지 않는다
	if( _drawReady )
	{
		this->ChangeEffect( _pPrevRenderTexture, _pNextRenderTexture, _timeFactor  );
	}
}

void SceneChangeEffect::ChangeEffect(LPDIRECT3DTEXTURE9 pPrev, LPDIRECT3DTEXTURE9 pNext, float factor)
{
	SPRITEMANAGER->BeginSpriteRender();
		
	RECT rcSour = { 0, 0, WINSIZEX, WINSIZEY };
	SPRITEMANAGER->DrawTexture( pPrev, &rcSour, 0, 0, D3DXCOLOR(1, 1, 1, 1.0f - factor ) );
	SPRITEMANAGER->DrawTexture( pNext, &rcSour, 0, 0, D3DXCOLOR(1, 1, 1, factor ) );

	SPRITEMANAGER->EndSpriteRender();
}
