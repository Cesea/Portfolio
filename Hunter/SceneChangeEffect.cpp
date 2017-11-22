#include "stdafx.h"

SceneChangeEffect::SceneChangeEffect(void)
	:_pPrevRenderTexture(nullptr), _pNextRenderTexture(nullptr),
	_pPrevScene(nullptr), _pNextScene(nullptr)
{
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
	//교체 효과에 사용될 이전 씬과 다음씬을 기억한다...
	_pPrevScene = pPrevScene;
	_pNextScene = pNextScene;

	//시간 초기화
	_totalTime = changeTime;
	_deltaTime = 0.0f;
	_timeFactor = 0.0f;

	//시작시간 기억
	_startTime = ( float )(APPTIMER->GetCurrentRealTimeSecond() );

	//교체씬 드로우 여부 false 로 시작
	_drawReady = false;		
}

void SceneChangeEffect::Update(float timeDelta)
{
	if (_drawReady == false)
	{
		//LOG_MGR->AddLog("씬 교체 드로우" );

		//이전씬화면 랜더
		this->_pPrevScene->Update(timeDelta);
		this->_pPrevScene->RenderToMainCamTexture();

		this->_pNextScene->Update(timeDelta);
		this->_pNextScene->RenderToMainCamTexture();

		//랜더 명령이 먹었기 때문에 씬의 화면 Texture 를 얻어올수 있다....
		this->_pPrevRenderTexture = this->_pPrevScene->GetTexture();
		this->_pNextRenderTexture = this->_pNextScene->GetTexture();

		/*
		//참고 복사

		//원본 Texture
		LPDIRECT3DTEXTURE9 pSrcTex = _pPrevScene->GetTexture();

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
	float nowTime = (float)(APPTIMER->GetCurrentRealTimeSecond());
	_deltaTime = nowTime - _startTime;


	//Factor 계산
	_timeFactor = _deltaTime / _totalTime;
	if (_timeFactor > 1.0f)
	{
		_timeFactor = 1.0f;
	}

	//교채시간이 다 되었다면....
	if (_deltaTime > _totalTime)
	{
		gEngine->GetScene()->_pCurrentScene = _pNextScene;
		gEngine->GetScene()->_pReleaseScene= _pPrevScene;
		gEngine->GetScene()->_pCurrentChangeEffect= nullptr;
		_drawReady = false;
	}
}

void SceneChangeEffect::Render()
{
	if (_drawReady)
	{
		this->ChangeEffect(_pPrevRenderTexture, _pNextRenderTexture, _timeFactor);
	}
}

void SceneChangeEffect::ChangeEffect(LPDIRECT3DTEXTURE9 pPrev, LPDIRECT3DTEXTURE9 pNext, float factor)
{
	SPRITEMANAGER->BeginSpriteRender();

	RECT rcSour = { 0, 0, WINSIZEX, WINSIZEY };
	SPRITEMANAGER->DrawTexture(pPrev, &rcSour, 0, 0, D3DXCOLOR(1, 1, 1, 1.0f - factor));
	SPRITEMANAGER->DrawTexture(pNext, &rcSour, 0, 0, D3DXCOLOR(1, 1, 1, factor));

	SPRITEMANAGER->EndSpriteRender();
}
