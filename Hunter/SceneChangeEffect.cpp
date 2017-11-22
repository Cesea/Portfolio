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
	//��ü ȿ���� ���� ���� ���� �������� ����Ѵ�...
	_pPrevScene = pPrevScene;
	_pNextScene = pNextScene;

	//�ð� �ʱ�ȭ
	_totalTime = changeTime;
	_deltaTime = 0.0f;
	_timeFactor = 0.0f;

	//���۽ð� ���
	_startTime = ( float )(APPTIMER->GetCurrentRealTimeSecond() );

	//��ü�� ��ο� ���� false �� ����
	_drawReady = false;		
}

void SceneChangeEffect::Update(float timeDelta)
{
	if (_drawReady == false)
	{
		//LOG_MGR->AddLog("�� ��ü ��ο�" );

		//������ȭ�� ����
		this->_pPrevScene->Update(timeDelta);
		this->_pPrevScene->RenderToMainCamTexture();

		this->_pNextScene->Update(timeDelta);
		this->_pNextScene->RenderToMainCamTexture();

		//���� ����� �Ծ��� ������ ���� ȭ�� Texture �� ���ü� �ִ�....
		this->_pPrevRenderTexture = this->_pPrevScene->GetTexture();
		this->_pNextRenderTexture = this->_pNextScene->GetTexture();

		/*
		//���� ����

		//���� Texture
		LPDIRECT3DTEXTURE9 pSrcTex = _pPrevScene->GetTexture();

		//������ ��ġ Texture ( ��Texture �� ��������־�� �Ѵ� )
		LPDIRECT3DTEXTURE9 pDstTex = m_pPrevRenderTexture;


		// pSrcTex �� Surface �� ��´�.
		LPDIRECT3DSURFACE9 srcSurface = NULL;
		pSrcTex->GetSurfaceLevel( 0, &srcSurface );

		// pDstTex �� Surface �� ��´�.
		LPDIRECT3DSURFACE9 destSurface = NULL;
		pDstTex->GetSurfaceLevel( 0, &destSurface );

		//��Ӻ���
		Device->StretchRect(
		srcSurface,				//���� Surface
		NULL,					//���� Surface �� ���� ( NULL �̸� ��ü )
		destSurface, 			//�ҽ� Surface
		NULL, 					//�ҽ� Surface �� ���� ( NULL �̸� ��ü )
		D3DTEXF_LINEAR			//���͸�
		);

		srcSurface->Release();
		pDstTex->Release();
		*/

		_drawReady = true;
	}


	//�����ð� 
	float nowTime = (float)(APPTIMER->GetCurrentRealTimeSecond());
	_deltaTime = nowTime - _startTime;


	//Factor ���
	_timeFactor = _deltaTime / _totalTime;
	if (_timeFactor > 1.0f)
	{
		_timeFactor = 1.0f;
	}

	//��ä�ð��� �� �Ǿ��ٸ�....
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
