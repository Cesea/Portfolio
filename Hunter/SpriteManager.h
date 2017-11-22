#ifndef SPRITE_MANAGER_H
#define SPRITE_MANAGER_H

#include "SingletonBase.h"

class SpriteManager : public SingletonBase<SpriteManager>
{
private:
	LPD3DXSPRITE _pSprite;
public:
	SpriteManager();
	~SpriteManager();

	bool Init(LPDIRECT3DDEVICE9 pDevice);

	void Release();

	void BeginSpriteRender();
	void EndSpriteRender();

	void DrawTexture(
		LPDIRECT3DTEXTURE9 pTex,		 //텍스쳐
		LPRECT pSour,							 //이미지의 그릴 영역
		float posX, float posY,				 //그릴 화면 위치
		uint32 color = 0xffffffff,		 //섞이는 색상
		Vector3 *pCenter = nullptr); //이미지의 중심 (NULL이면 화면 좌상단)

	void DrawTexture(
		LPDIRECT3DTEXTURE9 pTex,
		LPRECT pSour,
		float posX, float posY,
		float scaleX, float scaleY,
		float rotate,
		uint32 color = 0xFFFFFFFF,
		Vector3 *pCenter = NULL);

	void DrawArea(
		LPDIRECT3DTEXTURE9 pTex,
		LPRECT pSour,		//그릴 이미지의 영역
		LPRECT pDest,     //화면의 영역
		uint32 color = 0xFFFFFFFF);

	void DrawNinePatch(
		LPDIRECT3DTEXTURE9 pTex,
		LPRECT pSour,
		LPRECT pDest,
		LPRECT pCenter,
		uint32 color = 0xFFFFFFFF,
		bool bDrawCenter = true);

	void DrawLoop(
		LPDIRECT3DTEXTURE9 pTex,
		float offsetX, float offsetY,
		LPRECT pSour,
		LPRECT pDest,
		uint32 color = 0xFFFFFFFF
	);
};

#define SPRITEMANAGER SpriteManager::GetInstance()

#endif
