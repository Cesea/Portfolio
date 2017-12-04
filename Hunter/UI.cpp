#include "stdafx.h"
#include "UI.h"

#include "Player.h"

UI::UI()
{
	_uiOn = true;
	video::TextureHandle pHpBgHandle = VIDEO->CreateTexture("../resources/Textures/UITexture/LifeOrbBg.png", "../resources/Textures/UITexture/LifeOrbBg.png");
	_pHpBg = VIDEO->GetTexture(pHpBgHandle);
	video::TextureHandle pHpHandle = VIDEO->CreateTexture("../resources/Textures/UITexture/LifeOrb.png", "../resources/Textures/UITexture/LifeOrb.png");
	_pHp = VIDEO->GetTexture(pHpHandle);

	video::TextureHandle pFuryBgHandle = VIDEO->CreateTexture("../resources/Textures/UITexture/FuryBg.png", "../resources/Textures/UITexture/FuryBg.png");
	_pFuryBg = VIDEO->GetTexture(pFuryBgHandle);
	video::TextureHandle pFuryHandle = VIDEO->CreateTexture("../resources/Textures/UITexture/Fury.png", "../resources/Textures/UITexture/Fury.png");
	_pFury = VIDEO->GetTexture(pFuryHandle);

	video::TextureHandle pMiniMenuHandle = VIDEO->CreateTexture("../resources/Textures/UITexture/miniMenuBg.png", "../resources/Textures/UITexture/miniMenuBg.png");
	_pMiniMenu = VIDEO->GetTexture(pMiniMenuHandle);

	video::TextureHandle pSlotHandle = VIDEO->CreateTexture("../resources/Textures/UITexture/slot.png", "../resources/Textures/UITexture/slot.png");
	_pSlot = VIDEO->GetTexture(pSlotHandle);

	video::TextureHandle pBt1AHandle = VIDEO->CreateTexture("../resources/Textures/UITexture/Bt1_A.png", "../resources/Textures/UITexture/Bt1_A.png");
	_bt1_A = VIDEO->GetTexture(pBt1AHandle);
	video::TextureHandle pBt1BHandle = VIDEO->CreateTexture("../resources/Textures/UITexture/Bt1_B.png", "../resources/Textures/UITexture/Bt1_B.png");
	_bt1_B = VIDEO->GetTexture(pBt1BHandle);

	video::TextureHandle pBt2AHandle = VIDEO->CreateTexture("../resources/Textures/UITexture/Bt2_A.png", "../resources/Textures/UITexture/Bt2_A.png");
	_bt2_A = VIDEO->GetTexture(pBt2AHandle);
	video::TextureHandle pBt2BHandle = VIDEO->CreateTexture("../resources/Textures/UITexture/Bt2_B.png", "../resources/Textures/UITexture/Bt2_B.png");
	_bt2_B = VIDEO->GetTexture(pBt2BHandle);

	video::TextureHandle pBt3AHandle = VIDEO->CreateTexture("../resources/Textures/UITexture/Bt3_A.png", "../resources/Textures/UITexture/Bt3_A.png");
	_bt3_A = VIDEO->GetTexture(pBt3AHandle);
	video::TextureHandle pBt3BHandle = VIDEO->CreateTexture("../resources/Textures/UITexture/Bt3_B.png", "../resources/Textures/UITexture/Bt3_B.png");
	_bt3_B = VIDEO->GetTexture(pBt3BHandle);

	MaxHp = 158;
	CurHp = 102;
	
	MaxFury = 111;
	CurFury = 88;

	//Player* _player = GAMEOBJECTFACTORY->GetPlayerObject();

	_bt1On = false;
	_bt2On = false;
	_bt3On = false;

	_player = (Player*)GAMEOBJECTFACTORY->GetPlayerObject();

	//_player->

}


UI::~UI()
{
}

void UI::Update(float deltaTime, const InputManager & input)
{
	//input.keyboard.IsDown();
	//input.keyboard.IsPressed();
	//input.keyboard.IsReleased();

	//input.mouse.GetCurrentPoint();

	//MOUSE_BUTTON_LEFT;
	//MOUSE_BUTTON_RIGHT;
	//MOUSE_BUTTON_MIDDLE;
	//input.mouse.IsDown();
	//input.mouse.IsReleased();
	//input.mouse.IsPressed();
}

void UI::RenderUI(void)
{
	SPRITEMANAGER->BeginSpriteRender();
	if (_uiOn == true)
	{
		POINT _pt;

		GetCursorPos(&_pt);

		RECT hpRect = { 0,1,255,256 };
		RECT hpBarRect = { 0,1,255,(CurHp / MaxHp) * 256 };

		RECT furyRect = { 1,1,255,256 };
		RECT furyBarRect = { 1,1,255,(CurFury / MaxFury) * 256 };

		RECT slotRect = { 0,0,126,126 };

		RECT btRect = { 0,0,64,64 };

		//

		RECT bt1Rect = { WINSTARTX + (WINSIZEX - 470),WINSTARTY + (WINSIZEY - 50),WINSTARTX + (WINSIZEX - 470 + 64),WINSTARTY + (WINSIZEY - 50 + 64) };
		RECT bt2Rect = { WINSTARTX + (WINSIZEX - 400),WINSTARTY + (WINSIZEY - 50),WINSTARTX + (WINSIZEX - 400 + 64),WINSTARTY + (WINSIZEY - 50 + 64) };
		RECT bt3Rect = { WINSTARTX + (WINSIZEX - 330),WINSTARTY + (WINSIZEY - 50),WINSTARTX + (WINSIZEX - 330 + 64),WINSTARTY + (WINSIZEY - 50 + 64) };

		if (PtInRect(&bt1Rect, _pt))
		{
			_bt1On = true;
		}
		else
		{
			_bt1On = false;
		}

		if (PtInRect(&bt2Rect, _pt))
		{
			_bt2On = true;
		}
		else
		{
			_bt2On = false;
		}

		if (PtInRect(&bt3Rect, _pt))
		{
			_bt3On = true;
		}
		else
		{
			_bt3On = false;
		}

		RECT miniMenuRect = { 0,0,290,82 };


		if (_bt1On == false)
		{
			SPRITEMANAGER->DrawTexture(_bt1_A->_ptr, NULL, WINSIZEX - 470, WINSIZEY - 50, 0.75, 0.75, 0, 0xFFFFFFFF);
		}
		else if (_bt1On == true)
		{
			SPRITEMANAGER->DrawTexture(_bt1_B->_ptr, NULL, WINSIZEX - 470, WINSIZEY - 50, 0.75, 0.75, 0, 0xFFFFFFFF);
		}

		if (_bt2On == false)
		{
			SPRITEMANAGER->DrawTexture(_bt2_A->_ptr, NULL, WINSIZEX - 400, WINSIZEY - 50, 0.75, 0.75, 0, 0xFFFFFFFF);
		}
		else if (_bt2On == true)
		{
			SPRITEMANAGER->DrawTexture(_bt2_B->_ptr, NULL, WINSIZEX - 400, WINSIZEY - 50, 0.75, 0.75, 0, 0xFFFFFFFF);
		}

		if (_bt3On == false)
		{
			SPRITEMANAGER->DrawTexture(_bt3_A->_ptr, NULL, WINSIZEX - 330, WINSIZEY - 50, 0.75, 0.75, 0, 0xFFFFFFFF);
		}
		else if (_bt3On == true)
		{
			SPRITEMANAGER->DrawTexture(_bt3_B->_ptr, NULL, WINSIZEX - 330, WINSIZEY - 50, 0.75, 0.75, 0, 0xFFFFFFFF);
		}

		SPRITEMANAGER->DrawTexture(_pSlot->_ptr, &slotRect, 300, WINSIZEY - (slotRect.bottom * 0.50), 0.50, 0.50, 0, 0xFFFFFFFF);

		SPRITEMANAGER->DrawTexture(_pSlot->_ptr, &slotRect, 365, WINSIZEY - (slotRect.bottom * 0.50), 0.50, 0.50, 0, 0xFFFFFFFF);

		SPRITEMANAGER->DrawTexture(_pSlot->_ptr, &slotRect, 430, WINSIZEY - (slotRect.bottom * 0.50), 0.50, 0.50, 0, 0xFFFFFFFF);

		SPRITEMANAGER->DrawTexture(_pSlot->_ptr, &slotRect, 495, WINSIZEY - (slotRect.bottom * 0.50), 0.50, 0.50, 0, 0xFFFFFFFF);

		SPRITEMANAGER->DrawTexture(_pHpBg->_ptr, &hpBarRect, 0, 708, 0.75f, 0.75f, 0, 0xFFFFFFFF);
		SPRITEMANAGER->DrawTexture(_pHp->_ptr, &hpRect, 0, 708, 0.75f, 0.75f, 0, 0xFFFFFFFF);

		SPRITEMANAGER->DrawTexture(_pFuryBg->_ptr, &furyBarRect, WINSIZEX - (furyRect.right * 0.75), 708, 0.75f, 0.75f, 0, 0xFFFFFFFF);
		SPRITEMANAGER->DrawTexture(_pFury->_ptr, &furyRect, WINSIZEX - (furyRect.right * 0.75), 708, 0.75f, 0.75f, 0, 0xFFFFFFFF);

		SPRITEMANAGER->DrawTexture(_pMiniMenu->_ptr, NULL, WINSIZEX - 510, WINSIZEY - 64, 0.5f, 0.5f, 0, 0xFFFFFFFF);
	}
	//708

	
	SPRITEMANAGER->EndSpriteRender();
}