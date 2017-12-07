#include "stdafx.h"
#include "Inventory.h"

Inventory::Inventory()
{
	_invenOn = false;

	video::TextureHandle invenHandle = VIDEO->CreateTexture("../resources/Textures/UITexture/inven.png", "../resources/Textures/UITexture/inven.png");
	_inven = VIDEO->GetTexture(invenHandle);

	video::TextureHandle pSlotHandle = VIDEO->CreateTexture("../resources/Textures/UITexture/BagSlot.png", "../resources/Textures/UITexture/BagSlot.png");
	_pSlot = VIDEO->GetTexture(pSlotHandle);

	video::TextureHandle pMush1Handle = VIDEO->CreateTexture("../resources/Textures/UITexture/mush1.png", "../resources/Textures/UITexture/mush1.png");
	_mush1 = VIDEO->GetTexture(pMush1Handle);
	video::TextureHandle pMush2Handle = VIDEO->CreateTexture("../resources/Textures/UITexture/mush2.png", "../resources/Textures/UITexture/mush2.png");
	_mush2 = VIDEO->GetTexture(pMush2Handle);
	video::TextureHandle pMush3Handle = VIDEO->CreateTexture("../resources/Textures/UITexture/mush3.png", "../resources/Textures/UITexture/mush3.png");
	_mush3 = VIDEO->GetTexture(pMush3Handle);

	video::TextureHandle pPotion1Handle = VIDEO->CreateTexture("../resources/Textures/UITexture/potion1.png", "../resources/Textures/UITexture/potion1.png");
	_potion1 = VIDEO->GetTexture(pPotion1Handle);
	video::TextureHandle pPotion2Handle = VIDEO->CreateTexture("../resources/Textures/UITexture/potion2.png", "../resources/Textures/UITexture/potion2.png");
	_potion2 = VIDEO->GetTexture(pPotion2Handle);
	video::TextureHandle pPotion3Handle = VIDEO->CreateTexture("../resources/Textures/UITexture/potion3.png", "../resources/Textures/UITexture/potion3.png");
	_potion3 = VIDEO->GetTexture(pPotion3Handle);

	video::TextureHandle pSynthesisHandle = VIDEO->CreateTexture("../resources/Textures/UITexture/synthesis.png", "../resources/Textures/UITexture/synthesis.png");
	_synthesisImage = VIDEO->GetTexture(pSynthesisHandle);

	video::TextureHandle pSynthesisPushHandle = VIDEO->CreateTexture("../resources/Textures/UITexture/synthesisPush.png", "../resources/Textures/UITexture/synthesisPush.png");
	_synthesisImage_push = VIDEO->GetTexture(pSynthesisPushHandle);

	video::TextureHandle pTBarHandle = VIDEO->CreateTexture("../resources/Textures/UITexture/TBar.png", "../resources/Textures/UITexture/TBar.png");
	_tBar = VIDEO->GetTexture(pTBarHandle);

	/*video::TextureHandle pSynthesisFontHandle = VIDEO->CreateTexture("../resources/Textures/UITexture/synthesisFont.png", "../resources/Textures/UITexture/synthesisFont.png");
	_systhesisFont = VIDEO->GetTexture(pSynthesisFontHandle);*/

	_tempType = Null;
	_tempValue = 0;
	_tempIndex = -1;
	_tempValueImage.init();
	_tempValueImage.Load("../resources/Font\\Font1.fnt");

	for (int i = 0; i < 8; ++i)
	{
		/*if (i < 4)
		{
			_bag[i]._slotRect = { WINSIZEX - 252 + (30 * i),WINSIZEY - 453,64,64 };
		}
		else if (i > 3)
		{
			_bag[i]._slotRect = { WINSIZEX - 252 + (30 * (i - 4)),WINSIZEY - 403,64,64 };
		}*/

		if (i < 4)
		{
			_bag[i]._pos = { WINSIZEX - 226 + (52 * i),WINSIZEY - 683};
		}
		else if (i > 3)
		{
			_bag[i]._pos = { WINSIZEX - 226 + (52 * (i - 4)),WINSIZEY - 633};
		}

		_bag[i]._slotRect.left = WINSTARTX + _bag[i]._pos.x;
		_bag[i]._slotRect.right = WINSTARTX + _bag[i]._pos.x + 50;
		_bag[i]._slotRect.top = WINSTARTY + _bag[i]._pos.y;
		_bag[i]._slotRect.bottom = WINSTARTY + _bag[i]._pos.y + 50;

		_bag[i]._type = Null;
		_bag[i]._value = 0;
		_bag[i]._index = i;

		_bag[i]._valueImage.init();
		_bag[i]._valueImage.Load("../resources/Font\\Font1.fnt");
	}

	_ivX = (WINSIZEX - 254);
	_ivY = (150);

	_invenRect = { WINSTARTX + _ivX,WINSTARTY + _ivY,WINSTARTX + _ivX + 254,WINSTARTY + _ivY + 477 };

	/*_bag[0]._type = mush1;
	_bag[1]._type = mush2;
	_bag[2]._type = mush3;*/

	_bag[8]._index = 8;
	_bag[8]._pos = { WINSIZEX - 201 - 15,WINSIZEY - 483 };
	_bag[8]._type = Null;
	_bag[8]._value = 0;
	_bag[8]._valueImage.init();
	_bag[8]._valueImage.Load("../resources/Font\\Font1.fnt");
	_bag[8]._slotRect.left = WINSTARTX + _bag[8]._pos.x;
	_bag[8]._slotRect.right = WINSTARTX + _bag[8]._pos.x + 50;
	_bag[8]._slotRect.top = WINSTARTY + _bag[8]._pos.y;
	_bag[8]._slotRect.bottom = WINSTARTY + _bag[8]._pos.y + 50;

	_bag[9]._index = 9;
	_bag[9]._pos = { WINSIZEX - 201 + (102) + 15,WINSIZEY - 483 };
	_bag[9]._type = Null;
	_bag[9]._value = 0;
	_bag[9]._valueImage.init();
	_bag[9]._valueImage.Load("../resources/Font\\Font1.fnt");
	_bag[9]._slotRect.left = WINSTARTX + _bag[9]._pos.x;
	_bag[9]._slotRect.right = WINSTARTX + _bag[9]._pos.x + 50;
	_bag[9]._slotRect.top = WINSTARTY + _bag[9]._pos.y;
	_bag[9]._slotRect.bottom = WINSTARTY + _bag[9]._pos.y + 50;

	_bag[10]._index = 10;
	_bag[10]._pos = { WINSIZEX - 201 + (51),WINSIZEY - 383 };
	_bag[10]._type = Null;
	_bag[10]._value = 0;
	_bag[10]._valueImage.init();
	_bag[10]._valueImage.Load("../resources/Font\\Font1.fnt");
	_bag[10]._slotRect.left = WINSTARTX + _bag[10]._pos.x;
	_bag[10]._slotRect.right = WINSTARTX + _bag[10]._pos.x + 50;
	_bag[10]._slotRect.top = WINSTARTY + _bag[10]._pos.y;
	_bag[10]._slotRect.bottom = WINSTARTY + _bag[10]._pos.y + 50;

	_synthesisButton = false;

	
	//_synthesis = { WINSTARTX + (WINSIZEX - 201 + (51) - 100),WINSTARTY + (WINSIZEY - 483),WINSTARTX + (WINSIZEX - 201 + (51)) - 50, WINSTARTY + (WINSIZEY - 483) + 50 };

	/*_bag[8]._type = mush1;
	_bag[8]._value = 1;*/
	//_bag[9] = _synthesis1;
}

Inventory::~Inventory()
{

}

void Inventory::Update(float deltaTime, const InputManager &input)
{
	POINT _pt;

	GetCursorPos(&_pt);

	for (int i = 0; i < 8; ++i)
	{
		if (_tempType == Null)
		{
			if (PtInRect(&_bag[i]._slotRect, _pt))
			{
				if (_bag[i]._type != Null)
				{
					if (input.mouse.IsDown(MOUSE_BUTTON_LEFT))
					{
						_tempType = _bag[i]._type;
						_tempValue = _bag[i]._value;
						_tempIndex = _bag[i]._index;
						_bag[i]._type = Null;
						_bag[i]._value = 0;
					}
				}
			}
		}
		else if (_tempType != Null)
		{
			if (input.mouse.IsReleased(MOUSE_BUTTON_LEFT))
			{
				if (PtInRect(&_bag[i]._slotRect, _pt))
				{
					if (_bag[i]._type == Null)
					{
						_bag[i]._type = _tempType;
						_bag[i]._value = _tempValue;
						_tempType = Null;
						_tempValue = 0;
						_tempIndex = -1;
					}
					else if (_bag[i]._type != Null)
					{
						if (_bag[i]._type == _tempType)
						{
							_bag[i]._value += _tempValue;
							_tempType = Null;
							_tempValue = 0;
							_tempIndex = -1;
							break;
						}
						else
						{
							_bag[_tempIndex]._type = _bag[i]._type;
							_bag[_tempIndex]._value = _bag[i]._value;

							_bag[i]._type = _tempType;
							_bag[i]._value = _tempValue;

							_tempType = Null;
							_tempValue = 0;
							_tempIndex = -1;
							break;
						}
						
					}
				}
				else if (i == 7)
				{
					if (PtInRect(&_invenRect, _pt))
					{
						if (PtInRect(&_bag[8]._slotRect, _pt))
						{
							if (_bag[8]._type == Null)
							{
		
								_bag[8]._type = _tempType;
								_bag[8]._value = _tempValue;
								
								_tempType = Null;
								_tempValue = 0;
								_tempIndex = -1;
								break;
							}
							else if (_bag[8]._type != Null)
							{
								if (_bag[8]._type == _tempType)
								{
									_bag[8]._value += _tempValue;

									_tempType = Null;
									_tempValue = 0;
									_tempIndex = -1;
									break;
								}
								else if (_bag[8]._type != _tempType)
								{
									_bag[_tempIndex]._type = _bag[8]._type;
									_bag[_tempIndex]._value = _bag[8]._value;

									_bag[8]._type = _tempType;
									_bag[8]._value = _tempValue;

									_tempType = Null;
									_tempValue = 0;
									_tempIndex = -1;
									break;
								}
							}
						}

						/////////////////////////////////////////////

						if (PtInRect(&_bag[9]._slotRect, _pt))
						{
							if (_bag[9]._type == Null)
							{

								_bag[9]._type = _tempType;
								_bag[9]._value = _tempValue;

								_tempType = Null;
								_tempValue = 0;
								_tempIndex = -1;
								break;
							}
							else if (_bag[9]._type != Null)
							{
								if (_bag[9]._type == _tempType)
								{
									_bag[9]._value += _tempValue;

									_tempType = Null;
									_tempValue = 0;
									_tempIndex = -1;
									break;
								}
								else if (_bag[9]._type != _tempType)
								{
									_bag[_tempIndex]._type = _bag[9]._type;
									_bag[_tempIndex]._value = _bag[9]._value;

									_bag[9]._type = _tempType;
									_bag[9]._value = _tempValue;

									_tempType = Null;
									_tempValue = 0;
									_tempIndex = -1;
									break;
								}
							}
						}

						_bag[_tempIndex]._type = _tempType;
						_bag[_tempIndex]._value = _tempValue;
						_tempType = Null;
						_tempValue = 0;
						_tempIndex = -1;
						break;
					}
					else
					{
						/////
						DropItem(_tempType, _tempValue);
						//
						_tempType = Null;
						_tempValue = 0;
						_tempIndex = -1;
						break;
					}
					
				}
			}
			
		}
	}

	if (_tempType == Null)
	{
		if (PtInRect(&_bag[8]._slotRect, _pt))
		{
			if (_bag[8]._type != Null)
			{
				if (input.mouse.IsDown(MOUSE_BUTTON_LEFT))
				{
					_tempType = _bag[8]._type;
					_tempValue = _bag[8]._value;
					_tempIndex = _bag[8]._index;
					_bag[8]._type = Null;
					_bag[8]._value = 0;
					
				}
			}
		}
	}
	
	if (_tempType == Null)
	{
		if (PtInRect(&_bag[9]._slotRect, _pt))
		{
			if (_bag[9]._type != Null)
			{
				if (input.mouse.IsDown(MOUSE_BUTTON_LEFT))
				{
					_tempType = _bag[9]._type;
					_tempValue = _bag[9]._value;
					_tempIndex = _bag[9]._index;
					_bag[9]._type = Null;
					_bag[9]._value = 0;

				}
			}
		}
	}

	/*if (_tempType == Null)
	{
		if (PtInRect(&_bag[10]._slotRect, _pt))
		{
			if (_bag[10]._type != Null)
			{
				if (input.mouse.IsDown(MOUSE_BUTTON_LEFT))
				{
					_tempType = _bag[10]._type;
					_tempValue = _bag[10]._value;
					_tempIndex = _bag[10]._index;
					_bag[10]._type = Null;
					_bag[10]._value = 0;

				}
			}
		}
	}*/

	if (_tempType == Null)
	{
		if (PtInRect(&_bag[10]._slotRect, _pt))
		{
			if (_bag[10]._type == Null && (_bag[8]._type != Null && _bag[9]._type != Null))
			{
				if (_synthesisButton == false)
				{
					if (input.mouse.IsDown(MOUSE_BUTTON_LEFT))
					{
						_synthesisButton = true;
					}
				}
				else if (_synthesisButton == true)
				{
					if (input.mouse.IsReleased(MOUSE_BUTTON_LEFT))
					{
						_synthesisButton = false;
						Synthesis();
					}
				}
			}

			if (_bag[10]._type != Null)
			{
				
					if (input.mouse.IsDown(MOUSE_BUTTON_LEFT))
					{
						_tempType = _bag[10]._type;
						_tempValue = _bag[10]._value;
						_tempIndex = _bag[10]._index;
						_bag[10]._type = Null;
						_bag[10]._value = 0;
					}
				
			}
			
		}
		else
		{
			_synthesisButton = false;
		}
	}

	for (int i = 0; i < 11; ++i)
	{
		if (PtInRect(&_bag[i]._slotRect, _pt))
		{
			if (_bag[i]._type != Null)
			{
				if (_bag[i]._value > 0)
				{
					
						if (input.mouse.IsPressed(MOUSE_BUTTON_RIGHT))
						{
							if (_tempType == Null)
							{
								_tempType = _bag[i]._type;
								_tempValue += 1;
								_bag[i]._value -= 1;
								if (_bag[i]._value <= 0)
								{
									_bag[i]._type = Null;
								}
								break;
							}
							else if (_tempType == _bag[i]._type)
							{
								_tempValue += 1;
								_bag[i]._value -= 1;
								if (_bag[i]._value <= 0)
								{
									_bag[i]._type = Null;
								}
								break;
							}
						}
					
				}
				
			}
			
		}
	}
	
}

void Inventory::Render()
{
	POINT _pt;

	GetCursorPos(&_pt);

	if (_tempType != Null)
	{
		if (_tempValue > 0)
		{
			char val[64];
			sprintf(val, "%d", _tempValue);
			_tempValueImage.RenderText(val, (_pt.x - 125) + 35, (_pt.y - 125) + 50, 0.5f, 0.5f, 0xFFFFFFFF);

			

			switch (_tempType)
			{
			case mush1:
				SPRITEMANAGER->DrawTexture(_mush1->_ptr, NULL, (_pt.x - WINSTARTX) - 25, (_pt.y - WINSTARTY) - 25, 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			case mush2:
				SPRITEMANAGER->DrawTexture(_mush2->_ptr, NULL, (_pt.x - WINSTARTX) - 25, (_pt.y - WINSTARTY) - 25, 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			case mush3:
				SPRITEMANAGER->DrawTexture(_mush3->_ptr, NULL, (_pt.x - WINSTARTX) - 25, (_pt.y - WINSTARTY) - 25, 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			case potion1:
				SPRITEMANAGER->DrawTexture(_potion1->_ptr, NULL, (_pt.x - WINSTARTX) - 25, (_pt.y - WINSTARTY) - 25, 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			case potion2:
				SPRITEMANAGER->DrawTexture(_potion2->_ptr, NULL, (_pt.x - WINSTARTX) - 25, (_pt.y - WINSTARTY) - 25, 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			case potion3:
				SPRITEMANAGER->DrawTexture(_potion3->_ptr, NULL, (_pt.x - WINSTARTX) - 25, (_pt.y - WINSTARTY) - 25, 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			}
		}
	}


	RECT _invenRect = { 0,1,252,510 };
	RECT _slotRect = { 0,0,50,50 };
	if (_invenOn == true)
	{


		if (_bag[8]._type != Null)
		{
			char val[64];
			sprintf(val, "%d", _bag[8]._value);
			_bag[8]._valueImage.RenderText(val, _bag[8]._pos.x + 35, _bag[8]._pos.y + 50, 0.5f, 0.5f, 0xFFFFFFFF);
		}

		if (_bag[9]._type != Null)
		{
			char val[64];
			sprintf(val, "%d", _bag[9]._value);
			_bag[9]._valueImage.RenderText(val, _bag[9]._pos.x + 35, _bag[9]._pos.y + 50, 0.5f, 0.5f, 0xFFFFFFFF);
		}

		if (_bag[10]._type != Null)
		{
			char val[64];
			sprintf(val, "%d", _bag[10]._value);
			_bag[10]._valueImage.RenderText(val, _bag[10]._pos.x + 35, _bag[10]._pos.y + 50, 0.5f, 0.5f, 0xFFFFFFFF);
		}
		else if (_bag[10]._type == Null && (_bag[8]._type != Null && _bag[9]._type != Null))
		{
			if (_synthesisButton == false)
			{
				SPRITEMANAGER->DrawTexture(_synthesisImage->_ptr, NULL, _bag[10]._pos.x, _bag[10]._pos.y, 0.75, 0.75, 0, 0xFFFFFFFF);
			}
			else if (_synthesisButton == true)
			{
				SPRITEMANAGER->DrawTexture(_synthesisImage_push->_ptr, NULL, _bag[10]._pos.x, _bag[10]._pos.y, 0.75, 0.75, 0, 0xFFFFFFFF);
			}
		}

		for (int i = 0; i < 8; ++i)
		{
			if (_bag[i]._type != Null)
			{
				char val[64];
				sprintf(val, "%d", _bag[i]._value);
				_bag[i]._valueImage.RenderText(val, _bag[i]._pos.x + 35, _bag[i]._pos.y + 50, 0.5f, 0.5f, 0xFFFFFFFF);
			}

			switch (_bag[i]._type)
			{
			case mush1:
				SPRITEMANAGER->DrawTexture(_mush1->_ptr, NULL, _bag[i]._pos.x, _bag[i]._pos.y, 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			case mush2:
				SPRITEMANAGER->DrawTexture(_mush2->_ptr, NULL, _bag[i]._pos.x, _bag[i]._pos.y, 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			case mush3:
				SPRITEMANAGER->DrawTexture(_mush3->_ptr, NULL, _bag[i]._pos.x, _bag[i]._pos.y, 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			case potion1:
				SPRITEMANAGER->DrawTexture(_potion1->_ptr, NULL, _bag[i]._pos.x, _bag[i]._pos.y, 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			case potion2:
				SPRITEMANAGER->DrawTexture(_potion2->_ptr, NULL, _bag[i]._pos.x, _bag[i]._pos.y, 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			case potion3:
				SPRITEMANAGER->DrawTexture(_potion3->_ptr, NULL, _bag[i]._pos.x, _bag[i]._pos.y, 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			}

			SPRITEMANAGER->DrawTexture(_pSlot->_ptr, NULL, _bag[i]._pos.x, _bag[i]._pos.y, 0.75, 0.75, 0, 0xFFFFFFFF);
		}

		if (_bag[8]._type != Null)
		{
			switch (_bag[8]._type)
			{
			case mush1:
				SPRITEMANAGER->DrawTexture(_mush1->_ptr, NULL, _bag[8]._pos.x, _bag[8]._pos.y, 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			case mush2:
				SPRITEMANAGER->DrawTexture(_mush2->_ptr, NULL, _bag[8]._pos.x, _bag[8]._pos.y, 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			case mush3:
				SPRITEMANAGER->DrawTexture(_mush3->_ptr, NULL, _bag[8]._pos.x, _bag[8]._pos.y, 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			case potion1:
				SPRITEMANAGER->DrawTexture(_potion1->_ptr, NULL, _bag[8]._pos.x, _bag[8]._pos.y, 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			case potion2:
				SPRITEMANAGER->DrawTexture(_potion2->_ptr, NULL, _bag[8]._pos.x, _bag[8]._pos.y, 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			case potion3:
				SPRITEMANAGER->DrawTexture(_potion3->_ptr, NULL, _bag[8]._pos.x, _bag[8]._pos.y, 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			}
		}
		if (_bag[9]._type != Null)
		{
			switch (_bag[9]._type)
			{
			case mush1:
				SPRITEMANAGER->DrawTexture(_mush1->_ptr, NULL, _bag[9]._pos.x, _bag[9]._pos.y, 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			case mush2:
				SPRITEMANAGER->DrawTexture(_mush2->_ptr, NULL, _bag[9]._pos.x, _bag[9]._pos.y, 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			case mush3:
				SPRITEMANAGER->DrawTexture(_mush3->_ptr, NULL, _bag[9]._pos.x, _bag[9]._pos.y, 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			case potion1:
				SPRITEMANAGER->DrawTexture(_potion1->_ptr, NULL, _bag[9]._pos.x, _bag[9]._pos.y, 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			case potion2:
				SPRITEMANAGER->DrawTexture(_potion2->_ptr, NULL, _bag[9]._pos.x, _bag[9]._pos.y, 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			case potion3:
				SPRITEMANAGER->DrawTexture(_potion3->_ptr, NULL, _bag[9]._pos.x, _bag[9]._pos.y, 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			}
		}
		if (_bag[10]._type != Null)
		{
			switch (_bag[10]._type)
			{
			case mush1:
				SPRITEMANAGER->DrawTexture(_mush1->_ptr, NULL, _bag[10]._pos.x, _bag[10]._pos.y, 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			case mush2:
				SPRITEMANAGER->DrawTexture(_mush2->_ptr, NULL, _bag[10]._pos.x, _bag[10]._pos.y, 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			case mush3:
				SPRITEMANAGER->DrawTexture(_mush3->_ptr, NULL, _bag[10]._pos.x, _bag[10]._pos.y, 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			case potion1:
				SPRITEMANAGER->DrawTexture(_potion1->_ptr, NULL, _bag[10]._pos.x, _bag[10]._pos.y, 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			case potion2:
				SPRITEMANAGER->DrawTexture(_potion2->_ptr, NULL, _bag[10]._pos.x, _bag[10]._pos.y, 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			case potion3:
				SPRITEMANAGER->DrawTexture(_potion3->_ptr, NULL, _bag[10]._pos.x, _bag[10]._pos.y, 0.75, 0.75, 0, 0xFFFFFFFF);
				break;
			}
		}

		//SPRITEMANAGER->DrawTexture(_systhesisFont->_ptr, NULL, _bag[8]._pos.x, _bag[8]._pos.y - 60,1.25,1.25,0,0xFFFFFFFF);

		SPRITEMANAGER->DrawTexture(_pSlot->_ptr, NULL, _bag[8]._pos.x, _bag[8]._pos.y, 0.75, 0.75, 0, 0xFFFFFFFF);
		SPRITEMANAGER->DrawTexture(_pSlot->_ptr, NULL, _bag[9]._pos.x, _bag[9]._pos.y, 0.75, 0.75, 0, 0xFFFFFFFF);
		if (_bag[10]._type != Null)
		{
			SPRITEMANAGER->DrawTexture(_pSlot->_ptr, NULL, _bag[10]._pos.x, _bag[10]._pos.y, 0.75, 0.75, 0, 0xFFFFFFFF);
		}
		if (_bag[8]._type != Null && _bag[9]._type != Null)
		{
			RECT tb = { 0,0,125,75 };
			SPRITEMANAGER->DrawTexture(_tBar->_ptr, &tb, _bag[8]._pos.x + 25, _bag[8]._pos.y + 25, 0xFFFFFFFF);
		}
		
		SPRITEMANAGER->DrawTexture(_inven->_ptr, &_invenRect, /*WINSTARTX + */(WINSIZEX - 252), /*WINSTARTY + */(WINSIZEY - 383) - 400, 0xFFFFFFFF);
	}

	
	//SPRITEMANAGER->DrawTexture(_Box->_ptr, &_BoxRect, _ivX, _ivY, 0xFFFFFFFF);
}

bool Inventory::Additem(itemType type, int val)
{

	for (int i = 0; i < 8; ++i)
	{
		if (_bag[i]._type == type)
		{
			_bag[i]._value += val;

			return true;
		}
	}
	bool empty = CheckBag();

	if (empty == true)
	{
		for (int i = 0; i < 8; ++i)
		{
			if (_bag[i]._type == Null)
			{
				_bag[i]._type = type;
				_bag[i]._value = val;
				break;
			}
		}
	}
	else if (empty == false)
	{

		return false;
	}

}

void Inventory::DropItem(itemType type, int val)
{
	

	//Console::Log("%d : %d °³ \n", type, val);
}

bool Inventory::CheckBag(void)
{
	for (int j = 0; j < 8; ++j)
	{
		if (_bag[j]._type == Null)
		{
			return true;
		}
	}
	return false;
}

void Inventory::closeInventory(void)
{
	if (_bag[8]._type != Null)
	{
		bool is = false;

		for (int i = 0; i < 8; ++i)
		{
			////////////////////////////////////////////
			if (_bag[8]._type == _bag[i]._type)
			{
				is = true;
				_bag[i]._value += _bag[8]._value;
				_bag[8]._type = Null;
				_bag[8]._value = 0;
				break;
			}
		}

		if (is == false)
		{

			bool empty = CheckBag();

			if (empty == true)
			{
				bool isExist = false;

				for (int i = 0; i < 8; ++i)
				{
					if (_bag[8]._type == _bag[i]._type)
					{
						isExist = true;
						_bag[i]._value += _bag[8]._value;
						_bag[8]._type = Null;
						_bag[8]._value = 0;
						break;
					}
				}

				if (isExist == false)
				{
					for (int i = 0; i < 8; ++i)
					{
						if (_bag[i]._type == Null)
						{
							_bag[i]._type = _bag[8]._type;
							_bag[i]._value = _bag[8]._value;
							_bag[8]._type = Null;
							_bag[8]._value = 0;
							break;
						}
					}
				}


			}
			else if (empty == false)
			{
				//
				DropItem(_bag[8]._type, _bag[8]._value);
				//
				_bag[8]._type = Null;
				_bag[8]._value = 0;
				return;
			}
		}
	}

	if (_bag[9]._type != Null)
	{
		bool is = false;

		for (int i = 0; i < 8; ++i)
		{
			////////////////////////////////////////////
			if (_bag[9]._type == _bag[i]._type)
			{
				is = true;
				_bag[i]._value += _bag[9]._value;
				_bag[9]._type = Null;
				_bag[9]._value = 0;
				break;
			}
		}

		if (is == false)
		{
			bool empty = CheckBag();

			if (empty == true)
			{
				bool isExist = false;

				for (int i = 0; i < 8; ++i)
				{
					if (_bag[9]._type == _bag[i]._type)
					{
						isExist = true;
						_bag[i]._value += _bag[9]._value;
						_bag[9]._type = Null;
						_bag[9]._value = 0;
						break;
					}
				}

				if (isExist == false)
				{

					for (int i = 0; i < 8; ++i)
					{
						if (_bag[i]._type == Null)
						{
							_bag[i]._type = _bag[9]._type;
							_bag[i]._value = _bag[9]._value;
							_bag[9]._type = Null;
							_bag[9]._value = 0;
							break;
						}
					}
				}
			}
			else if (empty == false)
			{
				//
				DropItem(_bag[9]._type, _bag[9]._value);
				//
				_bag[9]._type = Null;
				_bag[9]._value = 0;
				return;
			}
		}
	}

	if (_bag[10]._type != Null)
	{
		bool is = false;

		for (int i = 0; i < 8; ++i)
		{
			////////////////////////////////////////////
			if (_bag[10]._type == _bag[i]._type)
			{
				is = true;
				_bag[i]._value += _bag[10]._value;
				_bag[10]._type = Null;
				_bag[10]._value = 0;
				break;
			}
		}

		if (is == false)
		{
			bool empty = CheckBag();

			if (empty == true)
			{
				bool isExist = false;

				for (int i = 0; i < 8; ++i)
				{
					if (_bag[10]._type == _bag[i]._type)
					{
						isExist = true;
						_bag[i]._value += _bag[10]._value;
						_bag[10]._type = Null;
						_bag[10]._value = 0;
						break;
					}
				}

				if (isExist == false)
				{

					for (int i = 0; i < 8; ++i)
					{
						if (_bag[i]._type == Null)
						{
							_bag[i]._type = _bag[10]._type;
							_bag[i]._value = _bag[10]._value;
							_bag[10]._type = Null;
							_bag[10]._value = 0;
							break;
						}
					}
				}
			}
			else if (empty == false)
			{
				//
				DropItem(_bag[10]._type, _bag[10]._value);
				//
				_bag[10]._type = Null;
				_bag[10]._value = 0;
				return;
			}
		}
	}

}

void Inventory::Synthesis(void)
{
	itemType _result = Null;

	switch (_bag[8]._type)
	{
	case mush1:
		switch (_bag[9]._type)
		{
		case mush1:
			_result = potion1;
			break;
		case mush2:

			break;
		case mush3:

			break;
		}
		break;
	case mush2:
		switch (_bag[9]._type)
		{
		case mush1:

			break;
		case mush2:
			_result = potion2;
			break;
		case mush3:

			break;
		}
		break;
	case mush3:
		switch (_bag[9]._type)
		{
		case mush1:

			break;
		case mush2:

			break;
		case mush3:
			_result = potion3;
			break;
		}
		break;
	}

	if (_bag[8]._type != Null && _bag[9]._type != Null && _bag[10]._type == Null)
	{
		if (_bag[8]._value > 0)
		{
			if (_bag[9]._value > 0)
			{
				if (_bag[10]._type == _result || _bag[10]._type == Null)
				{
					_bag[8]._value -= 1;
					_bag[9]._value -= 1;
					if (_bag[8]._value <= 0)
					{
						_bag[8]._type = Null;
					}
					if (_bag[9]._value <= 0)
					{
						_bag[9]._type = Null;
					}
					if (_bag[10]._type != _result)
					{
						_bag[10]._type = _result;
					}
					if (_bag[10]._type != Null)
					{
						_bag[10]._value += 1;
					}
					
				}
			}
		}
	}
}