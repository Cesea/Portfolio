#include "stdafx.h"
#include "UI.h"

#include "Player.h"

UI::UI()
{
	video::TextureHandle pHpBgHandle = VIDEO->CreateTexture("../resources/Textures/UITexture/LifeOrbBg.png", "../resources/Textures/UITexture/LifeOrbBg.png");
	_pHpBg = VIDEO->GetTexture(pHpBgHandle);

	video::TextureHandle pFuryBgHandle = VIDEO->CreateTexture("../resources/Textures/UITexture/FuryBg.png", "../resources/Textures/UITexture/FuryBg.png");
	_pFuryBg = VIDEO->GetTexture(pFuryBgHandle);

	video::TextureHandle pMiniMenuHandle = VIDEO->CreateTexture("../resources/Textures/UITexture/miniMenuBg.png", "../resources/Textures/UITexture/miniMenuBg.png");
	_pMiniMenu = VIDEO->GetTexture(pMiniMenuHandle);
}


UI::~UI()
{
}

void UI::RenderUI(void)
{
	
	SPRITEMANAGER->BeginSpriteRender();
	SPRITEMANAGER->EndSpriteRender();
}