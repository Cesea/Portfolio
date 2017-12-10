#include "stdafx.h"
#include "UI.h"

#include "Player.h"
#include "BmpFont.h"

UI::UI()
{
	_uiOn = true;

	_hpBgHandle = VIDEO->CreateTexture("../resources/Textures/UITexture/LifeOrbBg.png",
		"../resources/Textures/UITexture/LifeOrbBg.png");
	_pHpBg = VIDEO->GetTexture(_hpBgHandle);

	_hpHandle = VIDEO->CreateTexture("../resources/Textures/UITexture/LifeOrb.png",
		"../resources/Textures/UITexture/LifeOrb.png");
	_pHp = VIDEO->GetTexture(_hpHandle);

	_furyBgHandle = VIDEO->CreateTexture("../resources/Textures/UITexture/FuryBg.png", 
		"../resources/Textures/UITexture/FuryBg.png");
	_pFuryBg = VIDEO->GetTexture(_furyBgHandle);

	_furyHandle = VIDEO->CreateTexture("../resources/Textures/UITexture/Fury.png", 
		"../resources/Textures/UITexture/Fury.png");
	_pFury = VIDEO->GetTexture(_furyHandle);

	_miniMenuHandle = VIDEO->CreateTexture("../resources/Textures/UITexture/miniMenuBg.png",
		"../resources/Textures/UITexture/miniMenuBg.png");
	_pMiniMenu = VIDEO->GetTexture(_miniMenuHandle);

	_slotHandle = VIDEO->CreateTexture("../resources/Textures/UITexture/slot.png", 
		"../resources/Textures/UITexture/slot.png");
	_pSlot = VIDEO->GetTexture(_slotHandle);

	_bt1AHandle = VIDEO->CreateTexture("../resources/Textures/UITexture/Bt1_A.png", 
		"../resources/Textures/UITexture/Bt1_A.png");
	_bt1_A = VIDEO->GetTexture(_bt1AHandle);

	_bt1BHandle = VIDEO->CreateTexture("../resources/Textures/UITexture/Bt1_B.png", 
		"../resources/Textures/UITexture/Bt1_B.png");
	_bt1_B = VIDEO->GetTexture(_bt1BHandle);

	_bt2AHandle = VIDEO->CreateTexture("../resources/Textures/UITexture/Bt2_A.png", 
		"../resources/Textures/UITexture/Bt2_A.png");
	_bt2_A = VIDEO->GetTexture(_bt2AHandle);
	_bt2BHandle = VIDEO->CreateTexture("../resources/Textures/UITexture/Bt2_B.png", 
		"../resources/Textures/UITexture/Bt2_B.png");
	_bt2_B = VIDEO->GetTexture(_bt2BHandle);

	_bt3AHandle = VIDEO->CreateTexture("../resources/Textures/UITexture/Bt3_A.png",
		"../resources/Textures/UITexture/Bt3_A.png");
	_bt3_A = VIDEO->GetTexture(_bt3AHandle);
	_bt3BHandle = VIDEO->CreateTexture("../resources/Textures/UITexture/Bt3_B.png",
		"../resources/Textures/UITexture/Bt3_B.png");
	_bt3_B = VIDEO->GetTexture(_bt3BHandle);

	_mush1Handle = VIDEO->CreateTexture("../resources/Textures/UITexture/mush1.png", 
		"../resources/Textures/UITexture/mush1.png");
	_mush1 = VIDEO->GetTexture(_mush1Handle);
	_mush2Handle = VIDEO->CreateTexture("../resources/Textures/UITexture/mush2.png", 
		"../resources/Textures/UITexture/mush2.png");
	_mush2 = VIDEO->GetTexture(_mush2Handle);
	_mush3Handle = VIDEO->CreateTexture("../resources/Textures/UITexture/mush3.png", 
		"../resources/Textures/UITexture/mush3.png");
	_mush3 = VIDEO->GetTexture(_mush3Handle);

	_potion1Handle = VIDEO->CreateTexture("../resources/Textures/UITexture/potion1.png", 
		"../resources/Textures/UITexture/potion1.png");
	_potion1 = VIDEO->GetTexture(_potion1Handle);
	_potion2Handle = VIDEO->CreateTexture("../resources/Textures/UITexture/potion2.png", 
		"../resources/Textures/UITexture/potion2.png");
	_potion2 = VIDEO->GetTexture(_potion2Handle);
	_potion3Handle = VIDEO->CreateTexture("../resources/Textures/UITexture/potion3.png", 
		"../resources/Textures/UITexture/potion3.png");
	_potion3 = VIDEO->GetTexture(_potion3Handle);

	_inventory = new Inventory;

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

	VIDEO->DestroyTexture(_hpBgHandle);
	VIDEO->DestroyTexture(_furyBgHandle);
	VIDEO->DestroyTexture(_miniMenuHandle);
	VIDEO->DestroyTexture(_hpHandle);
	VIDEO->DestroyTexture(_furyHandle);
	VIDEO->DestroyTexture(_slotHandle);
	VIDEO->DestroyTexture(_bt1AHandle);
	VIDEO->DestroyTexture(_bt1BHandle);
	VIDEO->DestroyTexture(_bt2AHandle);
	VIDEO->DestroyTexture(_bt2BHandle);
	VIDEO->DestroyTexture(_bt3AHandle);
	VIDEO->DestroyTexture(_bt3BHandle);
	VIDEO->DestroyTexture(_mush1Handle);
	VIDEO->DestroyTexture(_mush2Handle);
	VIDEO->DestroyTexture(_mush3Handle);
	VIDEO->DestroyTexture(_potion1Handle);
	VIDEO->DestroyTexture(_potion2Handle);
	VIDEO->DestroyTexture(_potion3Handle);
}

void UI::Update(float deltaTime, const InputManager & input)
{
	if (_uiOn == true)
	{
		POINT _pt;

		GetCursorPos(&_pt);

		RECT bt1Rect = { WINSTARTX + (WINSIZEX - 470),WINSTARTY + (WINSIZEY - 50),WINSTARTX + (WINSIZEX - 470 + 64),WINSTARTY + (WINSIZEY - 50 + 64) };
		RECT bt2Rect = { WINSTARTX + (WINSIZEX - 400),WINSTARTY + (WINSIZEY - 50),WINSTARTX + (WINSIZEX - 400 + 64),WINSTARTY + (WINSIZEY - 50 + 64) };
		RECT bt3Rect = { WINSTARTX + (WINSIZEX - 330),WINSTARTY + (WINSIZEY - 50),WINSTARTX + (WINSIZEX - 330 + 64),WINSTARTY + (WINSIZEY - 50 + 64) };

		if (PtInRect(&bt1Rect, _pt))
		{
			if (input.mouse.IsDown(MOUSE_BUTTON_LEFT))
			{
				_bt1On = true;
			}
			if (input.mouse.IsReleased(MOUSE_BUTTON_LEFT))
			{
				_bt1On = false;
				if (_inventory->GetInvenOn() == false)
				{
					SOUNDMANAGER->Play("inven_open_01");
					_inventory->SetInvenOn(true);

				}
				else if (_inventory->GetInvenOn() == true)
				{
					SOUNDMANAGER->Play("inven_close_01");
					_inventory->closeInventory();
					_inventory->SetInvenOn(false);
				}
			}
		}
		else
		{
			_bt1On = false;
		}

		if (PtInRect(&bt2Rect, _pt))
		{
			if (input.mouse.IsDown(MOUSE_BUTTON_LEFT))
			{
				_bt2On = true;
			}
			if (input.mouse.IsReleased(MOUSE_BUTTON_LEFT))
			{
				_bt2On = false;
				
			}
		}
		else
		{
			_bt2On = false;
		}

		if (PtInRect(&bt3Rect, _pt))
		{
			if (input.mouse.IsDown(MOUSE_BUTTON_LEFT))
			{
				_bt3On = true;
			}
			if (input.mouse.IsReleased(MOUSE_BUTTON_LEFT))
			{
				_bt3On = false;

			}
		}
		else
		{
			_bt3On = false;
		}

		if (input.keyboard.IsReleased('I'))
		{
			if (_inventory->GetInvenOn() == false)
			{
				SOUNDMANAGER->Play("inven_open_01");
				_inventory->SetInvenOn(true);

			}
			else if (_inventory->GetInvenOn() == true)
			{
				SOUNDMANAGER->Play("inven_close_01");
				_inventory->SetInvenOn(false);
				_inventory->closeInventory();
			}
		}
	}

	_inventory->Update(deltaTime, input);

	if (input.keyboard.IsReleased('1'))
	{
		_inventory->Additem(mush1, 1);
	}
	else if (input.keyboard.IsReleased('2'))
	{
		_inventory->Additem(mush2, 1);
	}
	else if (input.keyboard.IsReleased('3'))
	{
		_inventory->Additem(mush3, 1);
	}
	else if (input.keyboard.IsReleased('4'))
	{
		_inventory->Additem(potion1, 1);
	}
	else if (input.keyboard.IsReleased('5'))
	{
		_inventory->Additem(potion2, 1);
	}
	else if (input.keyboard.IsReleased('6'))
	{
		_inventory->Additem(potion3, 1);
	}

}

void UI::RenderUI(void)
{
	SPRITEMANAGER->BeginSpriteRender();
	if (_uiOn == true)
	{
		RECT hpRect = { 0,1,255,256 };
		RECT hpBarRect = { 0,1,255,(CurHp / MaxHp) * 256 };

		RECT furyRect = { 1,1,255,256 };
		RECT furyBarRect = { 1,1,255,(CurFury / MaxFury) * 256 };

		RECT slotRect = { 0,0,126,126 };

		RECT btRect = { 0,0,64,64 };

		//

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

		if (_inventory->GetBag1()->_type != Null)
		{
			char val[64];

			sprintf(val, "%d", _inventory->GetBag1()->_value);

			_inventory->GetBag1()->_valueImage.RenderText(val, 310 + 35, (WINSIZEY - 5), 0.75, 0.75, 0xFFFFFFFF);

			switch (_inventory->GetBag1()->_type)
			{
			case mush1:
				SPRITEMANAGER->DrawTexture(_mush1->_ptr, NULL, 310, (WINSIZEY - 55), 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			case mush2:
				SPRITEMANAGER->DrawTexture(_mush2->_ptr, NULL, 310, (WINSIZEY - 55), 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			case mush3:
				SPRITEMANAGER->DrawTexture(_mush3->_ptr, NULL, 310, (WINSIZEY - 55), 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			case potion1:
				SPRITEMANAGER->DrawTexture(_potion1->_ptr, NULL, 310, (WINSIZEY - 55), 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			case potion2:
				SPRITEMANAGER->DrawTexture(_potion2->_ptr, NULL, 310, (WINSIZEY - 55), 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			case potion3:
				SPRITEMANAGER->DrawTexture(_potion3->_ptr, NULL, 310, (WINSIZEY - 55), 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			}
		}

		if (_inventory->GetBag2()->_type != Null)
		{

			char val[64];

			sprintf(val, "%d", _inventory->GetBag2()->_value);

			_inventory->GetBag2()->_valueImage.RenderText(val, 375 + 35, (WINSIZEY - 5), 0.75, 0.75, 0xFFFFFFFF);

			switch (_inventory->GetBag2()->_type)
			{
			case mush1:
				SPRITEMANAGER->DrawTexture(_mush1->_ptr, NULL, 375, (WINSIZEY - 55), 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			case mush2:
				SPRITEMANAGER->DrawTexture(_mush2->_ptr, NULL, 375, (WINSIZEY - 55), 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			case mush3:
				SPRITEMANAGER->DrawTexture(_mush3->_ptr, NULL, 375, (WINSIZEY - 55), 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			case potion1:
				SPRITEMANAGER->DrawTexture(_potion1->_ptr, NULL, 375, (WINSIZEY - 55), 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			case potion2:
				SPRITEMANAGER->DrawTexture(_potion2->_ptr, NULL, 375, (WINSIZEY - 55), 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			case potion3:
				SPRITEMANAGER->DrawTexture(_potion3->_ptr, NULL, 375, (WINSIZEY - 55), 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			}
		}

		if (_inventory->GetBag3()->_type != Null)
		{

			char val[64];

			sprintf(val, "%d", _inventory->GetBag3()->_value);

			_inventory->GetBag3()->_valueImage.RenderText(val, 440 + 35, (WINSIZEY - 5), 0.75, 0.75, 0xFFFFFFFF);

			switch (_inventory->GetBag3()->_type)
			{
			case mush1:
				SPRITEMANAGER->DrawTexture(_mush1->_ptr, NULL, 440, (WINSIZEY - 55), 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			case mush2:
				SPRITEMANAGER->DrawTexture(_mush2->_ptr, NULL, 440, (WINSIZEY - 55), 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			case mush3:
				SPRITEMANAGER->DrawTexture(_mush3->_ptr, NULL, 440, (WINSIZEY - 55), 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			case potion1:
				SPRITEMANAGER->DrawTexture(_potion1->_ptr, NULL, 440, (WINSIZEY - 55), 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			case potion2:
				SPRITEMANAGER->DrawTexture(_potion2->_ptr, NULL, 440, (WINSIZEY - 55), 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			case potion3:
				SPRITEMANAGER->DrawTexture(_potion3->_ptr, NULL, 440, (WINSIZEY - 55), 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			}
		}

		if (_inventory->GetBag4()->_type != Null)
		{

			char val[64];

			sprintf(val, "%d", _inventory->GetBag4()->_value);

			_inventory->GetBag4()->_valueImage.RenderText(val, 505 + 35, (WINSIZEY - 5), 0.75, 0.75, 0xFFFFFFFF);

			switch (_inventory->GetBag4()->_type)
			{
			case mush1:
				SPRITEMANAGER->DrawTexture(_mush1->_ptr, NULL, 505, (WINSIZEY - 55), 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			case mush2:
				SPRITEMANAGER->DrawTexture(_mush2->_ptr, NULL, 505, (WINSIZEY - 55), 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			case mush3:
				SPRITEMANAGER->DrawTexture(_mush3->_ptr, NULL, 505, (WINSIZEY - 55), 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			case potion1:
				SPRITEMANAGER->DrawTexture(_potion1->_ptr, NULL, 505, (WINSIZEY - 55), 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			case potion2:
				SPRITEMANAGER->DrawTexture(_potion2->_ptr, NULL, 505, (WINSIZEY - 55), 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			case potion3:
				SPRITEMANAGER->DrawTexture(_potion3->_ptr, NULL, 505, (WINSIZEY - 55), 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			}
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

	_inventory->Render();
	SPRITEMANAGER->EndSpriteRender();
}