#include "stdafx.h"
#include "SpriteManager.h"

SpriteManager::SpriteManager()
{
}

SpriteManager::~SpriteManager()
{
}

bool SpriteManager::Init(LPDIRECT3DDEVICE9 pDevice)
{
	if (FAILED(D3DXCreateSprite(pDevice, &_pSprite)))
	{
		return false;

	}
	return true;
}

void SpriteManager::Release()
{
	COM_RELEASE(_pSprite);
}

void SpriteManager::BeginSpriteRender()
{
	_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void SpriteManager::EndSpriteRender()
{
	_pSprite->End();
}

void SpriteManager::DrawTexture(LPDIRECT3DTEXTURE9 pTex, LPRECT pSour, float posX, float posY, uint32 color, Vector3 * pCenter)
{
	//화면 절대좌표로 그리니 Spriteanvas의 transform 초기화
	Matrix matFinal;
	MatrixIdentity(&matFinal);
	_pSprite->SetTransform(&matFinal);

	//그릴 화면의 위치(Vector2가 아니라 Vector3로 만들건게
	//일반적으로 무시되지만 깊이를 주고 싶으면 사용해라);

	D3DXVECTOR3 pos(posX, posY, 0.0f);

	_pSprite->Draw( pTex,  pSour, (pCenter == NULL) ? &D3DXVECTOR3(0, 0, 0) : pCenter,
		&pos, color );
}

void SpriteManager::DrawTexture(LPDIRECT3DTEXTURE9 pTex, LPRECT pSour, float posX, float posY, float scaleX, float scaleY, float rotate, uint32 color, Vector3 * pCenter)
{
	Vector3 pivot = (pCenter == NULL) ?  Vector3(0, 0, 0) : *pCenter;

	//스케일 행렬
	Matrix matScale;
	MatrixScaling(&matScale,
		scaleX, scaleY, 1.0f);

	//이동 행렬
	Matrix matTrans;
	MatrixTranslation(&matTrans,
		posX, posY, 0.0f);

	//회전 
	Matrix matRot;
	MatrixRotationZ(&matRot, rotate);

	Matrix matFinal;
	matFinal = matScale * matRot * matTrans;

	_pSprite->SetTransform(&matFinal);
	_pSprite->Draw( pTex,  pSour, &pivot, &Vector3(0, 0, 0), color );
}

void SpriteManager::DrawArea(LPDIRECT3DTEXTURE9 pTex, LPRECT pSour, LPRECT pDest, uint32 color)
{
	float destWidth = pDest->right - pDest->left;
	float destHeight = pDest->bottom - pDest->top;

	float sourWidth = pSour->right - pSour->left;
	float sourHeight = pSour->bottom - pSour->top;

	//영역 비율에 의한 스케일 값
	float scaleX = destWidth / sourWidth;
	float scaleY = destHeight / sourHeight;

	//스케일 행렬
	Matrix matScale;
	MatrixScaling(&matScale,
		scaleX, scaleY, 1.0f);

	//이동 행렬
	Matrix matTrans;
	MatrixTranslation(&matTrans,
		pDest->left, pDest->top, 0.0f);

	Matrix matFinal;
	matFinal = matScale * matTrans;
	_pSprite->SetTransform(&matFinal);


	_pSprite->Draw( pTex, pSour, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 0), color );
}

void SpriteManager::DrawNinePatch(LPDIRECT3DTEXTURE9 pTex, LPRECT pSour, LPRECT pDest, LPRECT pCenter, uint32 color, bool bDrawCenter)
{
	float fCenterW = pCenter->right - pCenter->left;
	float fCenterH = pCenter->bottom - pCenter->top;

	//sour의 영역
	RECT rcSrcLetfTop = { 0, 0, pCenter->left, pCenter->top };
	RECT rcSrcTop = { pCenter->left, 0, pCenter->right, pCenter->top };
	RECT rcSrcRightTop = { pCenter->right, 0, pSour->right, pCenter->top };

	RECT rcSrcLeft = { 0, pCenter->top, pCenter->left, pCenter->bottom };
	RECT rcSrcCenter = { pCenter->left, pCenter->top, pCenter->right, pCenter->bottom };
	RECT rcSrcRight = { pCenter->right, pCenter->top, pSour->right, pCenter->bottom };

	RECT rcSrcLeftBottom = { 0, pCenter->bottom, pCenter->left, pSour->bottom };
	RECT rcSrcBottom = { pCenter->left, pCenter->bottom, pCenter->right, pSour->bottom };
	RECT rcSrcRightBottom = { pCenter->right, pCenter->bottom, pSour->right, pSour->bottom };


	float left = pDest->left;
	float top = pDest->top;
	float right = pDest->right;
	float bottom = pDest->bottom;

	float centerLeft = pDest->left + (pCenter->left - pSour->left);
	float centerRight = pDest->right - (pSour->right - pCenter->right);
	float centerTop = pDest->top + (pCenter->top - pSour->top);
	float centerBottom = pDest->bottom - (pSour->bottom - pCenter->bottom);

	RECT rcDstLeftTop = { left,top, centerLeft,centerTop };
	RECT rcDstTop = { centerLeft,top, centerRight,centerTop };
	RECT rcDstRightTop = { centerRight,top, right,centerTop };

	RECT rcDstLeft = { left,centerTop, centerLeft,centerBottom };
	RECT rcDstCenter = { centerLeft,centerTop, centerRight,centerBottom };
	RECT rcDstRight = { centerRight,centerTop, right,centerBottom };

	RECT rcDstLeftBottom = { left, centerBottom, centerLeft,bottom };
	RECT rcDstBottom = { centerLeft, centerBottom, centerRight, bottom };
	RECT rcDstRightBottom = { centerRight, centerBottom, right, bottom };

	DrawArea(pTex, &rcSrcLetfTop, &rcDstLeftTop, color);
	DrawArea(pTex, &rcSrcTop, &rcDstTop, color);
	DrawArea(pTex, &rcSrcRightTop, &rcDstRightTop, color);

	DrawArea(pTex, &rcSrcLeft, &rcDstLeft, color);
	//true 이면 그려준다.
	if (bDrawCenter)
		DrawArea(pTex, &rcSrcCenter, &rcDstCenter, color);
	DrawArea(pTex, &rcSrcRight, &rcDstRight, color);

	DrawArea(pTex, &rcSrcLeftBottom, &rcDstLeftBottom, color);
	DrawArea(pTex, &rcSrcBottom, &rcDstBottom, color);
	DrawArea(pTex, &rcSrcRightBottom, &rcDstRightBottom, color);
}

void SpriteManager::DrawLoop(LPDIRECT3DTEXTURE9 pTex, float offsetX, float offsetY, LPRECT pSour, LPRECT pDest, uint32 color)
{
	//소스의 가로크기
	int sourW = pSour->right - pSour->left;
	int sourH = pSour->bottom - pSour->top;

	//sour offset 구하자
	int offSetX = static_cast<int>(offsetX) % sourW;
	int offSetY = static_cast<int>(offsetY) % sourH;

	if (offSetX < 0)
		offSetX = sourW + offSetX;
	if (offSetY < 0)
		offSetY = sourH + offSetY;

	//dest 가로크기 세로크기
	int destW = pDest->right - pDest->left;
	int destH = pDest->bottom - pDest->top;

	//현재 한번 그려진 양 (루프를 한번 돌때마다 그려진 량)
	int drawX = 0;
	int drawY = 0;

	//계산된 sour,dest 영역
	RECT rcDrawSour = { 0,0,0,0 };
	RECT rcDrawDest = { 0,0,0,0 };

	//가로 포문
	for (int i = 0; i < destW; i += destW)
	{
		//가로 소스 영역을 구하자
		rcDrawSour.left = (offSetX + i) % sourW + pSour->left;
		rcDrawSour.right = pSour->right;

		//드로우 되는 가로 량
		drawX = rcDrawSour.right - rcDrawSour.left;

		if (i + drawX > destW)
		{
			//넘어간 만큼 오른쪽은 왼쪽으로 떙긴다/.
			int overW = (i + drawX) - destW;
			rcDrawSour.right -= overW;

			//드로우 되는 가로 크기 갱신
			drawX = rcDrawSour.right - rcDrawSour.left;
		}

		//가로 dest 영역을 구한다.
		rcDrawDest.left = pDest->left + i;
		rcDrawDest.right = rcDrawDest.left + drawX;

		//세로 포문
		for (int j = 0; j < destH; j += destH)
		{
			//가로 소스 영역을 구하자
			rcDrawSour.top = (offSetY + j) % sourH + pSour->top;
			rcDrawSour.bottom = pSour->bottom;

			//드로우 되는 가로 량
			drawY = rcDrawSour.bottom - rcDrawSour.top;

			if (j + drawY > destH)
			{
				//넘어간 만큼 오른쪽은 왼쪽으로 떙긴다/.
				int overH = (j + drawY) - destH;
				rcDrawSour.bottom -= overH;

				//드로우 되는 가로 크기 갱신
				drawY = rcDrawSour.bottom - rcDrawSour.top;
			}

			//가로 dest 영역을 구한다.
			rcDrawDest.top = pDest->top + j;
			rcDrawDest.bottom = rcDrawDest.top + drawY;

			DrawArea(pTex, &rcDrawSour, &rcDrawDest, color);
		}

	}

}
