#include "stdafx.h"
#include "SceneChangeEffectUpDown.h"

SceneChangeEffectUpDown::SceneChangeEffectUpDown(void)
{
}

SceneChangeEffectUpDown::~SceneChangeEffectUpDown(void)
{
}

void SceneChangeEffectUpDown::ChangeEffect(LPDIRECT3DTEXTURE9 pPrev, LPDIRECT3DTEXTURE9 pNext, float factor)
{
	SPRITEMANAGER->BeginSpriteRender();

	RECT rcSour = { 0, 0, WINSIZEX, WINSIZEX };
	float y1 = 1.0f - factor;
	float y2 = factor;

	SPRITEMANAGER->DrawTexture( pPrev, &rcSour, 0, -WINSIZEY * factor , D3DXCOLOR(1, 1, 1, 1 ) );
	SPRITEMANAGER->DrawTexture( pNext, &rcSour, 0, WINSIZEY * factor, D3DXCOLOR(1, 1, 1, 1 ) );

	SPRITEMANAGER->EndSpriteRender();
}
