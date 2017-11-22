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
		LPDIRECT3DTEXTURE9 pTex,		 //�ؽ���
		LPRECT pSour,							 //�̹����� �׸� ����
		float posX, float posY,				 //�׸� ȭ�� ��ġ
		uint32 color = 0xffffffff,		 //���̴� ����
		Vector3 *pCenter = nullptr); //�̹����� �߽� (NULL�̸� ȭ�� �»��)

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
		LPRECT pSour,		//�׸� �̹����� ����
		LPRECT pDest,     //ȭ���� ����
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
