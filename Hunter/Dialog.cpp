#include "stdafx.h"
#include "Dialog.h"

#include "GuiResource.h"

using namespace gui;

Control *Dialog::_spControlFocus = nullptr;
Control *Dialog::_spControlPressed = nullptr;;

Dialog::Dialog()
{
	_x = 0;
	_y = 0;
	_width = 0;
	_height = 0;

	_pManager = NULL;
	_visible = true;
	_showCaption = false;
	_minimized = false;
	_drag = false;
	_captionText[0] = '\0';
	_captionHeight = 18;

	_colorTopLeft = 0;
	_colorTopRight = 0;
	_colorBottomLeft = 0;
	_colorBottomRight = 0;

	_pNextDialog = nullptr;
	_pPrevDialog = nullptr;

	_callbackEvent = nullptr;
	_pUserContext = nullptr;

	_pControlMouseOver = nullptr;

	_nonUserEvent = false;
	_keyboardInput= false;
	_mouseInput = true;
}

Dialog::~Dialog()
{
	RemoveAllControls();

	_fonts.clear();
	_textures.clear();

	for (uint32 i = 0; i < _defaultElements.size(); i++)
	{
		//E* pElementHolder = m_DefaultElements.GetAt(i);
		//SAFE_DELETE(pElementHolder);
	}

	//m_DefaultElements.RemoveAll();
}

void Dialog::Init(DialogResourceManager * pResourceManager)
{
	_pManager = pResourceManager;
	_pManager->RegisterDialog(this);

	InitDefaultElement();
}

bool Dialog::InternalMessageProc(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam)
{
	bool handled = false;

	// For invisible dialog, do not handle anything.
	if (!_visible)
		return false;

	// If automation command-line switch is on, enable this dialog's keyboard input
	// upon any key press or mouse click.
	if (/*DXUTGetAutomation() &&*/
		(WM_LBUTTONDOWN == msg || WM_LBUTTONDBLCLK == msg || WM_KEYDOWN == msg))
	{
		_pManager->EnableKeyboardInputForAllDialogs();
	}

	// If caption is enable, check for clicks in the caption area.
	if (_showCaption)
	{
		if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONDBLCLK)
		{
			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };

			if (mousePoint.x >= _x && mousePoint.x < _x + _width &&
				mousePoint.y >= _y && mousePoint.y < _y + _captionHeight)
			{
				_drag = true;
				SetCapture(windowHandle);
				return true;
			}
		}
		else if (msg == WM_LBUTTONUP && _drag)
		{
			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };

			if (mousePoint.x >= _x && mousePoint.x < _x + _width &&
				mousePoint.y >= _y && mousePoint.y < _y + _captionHeight)
			{
				ReleaseCapture();
				_drag = false;
				_minimized = !_minimized;
				return true;
			}
		}
	}

	// If the dialog is minimized, don't send any messages to controls.
	if (_minimized)
		return false;

	// If a control is in focus, it belongs to this dialog, and it's enabled, then give
	// it the first chance at handling the message.
	if (_spControlFocus &&
		_spControlFocus->_pParent == this &&
		_spControlFocus->GetEnabled())
	{
		// If the control MsgProc handles it, then we don't.
		if (_spControlFocus->MessageProc(windowHandle, msg, wParam, lParam))
			return true;
	}

	switch (msg)
	{
	case WM_SIZE:
	case WM_MOVE:
	{
		// Handle sizing and moving messages so that in case the mouse cursor is moved out
		// of an UI control because of the window adjustment, we can properly
		// unhighlight the highlighted control.
		POINT pt = { -1, -1 };
		OnMouseMove(pt);
		break;
	}

	case WM_ACTIVATEAPP:
		// Call OnFocusIn()/OnFocusOut() of the control that currently has the focus
		// as the application is activated/deactivated.  This matches the Windows
		// behavior.
		if (_spControlFocus &&
			_spControlFocus->_pParent == this &&
			_spControlFocus->GetEnabled())
		{
			if (wParam)
				_spControlFocus->OnFocusIn();
			else
				_spControlFocus->OnFocusOut();
		}
		break;

		// Keyboard messages
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
	{
		// If a control is in focus, it belongs to this dialog, and it's enabled, then give
		// it the first chance at handling the message.
		if (_spControlFocus &&
			_spControlFocus->_pParent == this &&
			_spControlFocus->GetEnabled())
		{
			if (_spControlFocus->HandleKeyboard(msg, wParam, lParam))
				return true;
		}

		// Not yet handled, see if this matches a control's hotkey
		// Activate the hotkey if the focus doesn't belong to an
		// edit box.
		if (msg == WM_KEYDOWN && (!_spControlFocus || (_spControlFocus->GetType() != ControlType::CONTROL_EDITBOX)))
		{
			for (int i = 0; i < _controls.size(); i++)
			{
				Control* pControl = _controls[i];
				if (pControl->GetHotKey() == wParam)
				{
					pControl->OnHotkey();
					return true;
				}
			}
		}

		// Not yet handled, check for focus messages
		if (msg == WM_KEYDOWN)
		{
			// If keyboard input is not enabled, this message should be ignored
			if (!_keyboardInput)
				return false;

			switch (wParam)
			{
			case VK_RIGHT:
			case VK_DOWN:
				if (_spControlFocus != NULL)
				{
					return OnCycleFocus(true);
				}
				break;

			case VK_LEFT:
			case VK_UP:
				if (_spControlFocus != NULL)
				{
					return OnCycleFocus(false);
				}
				break;

			case VK_TAB:
			{
				bool shiftDown = ((GetKeyState(VK_SHIFT) & 0x8000) != 0);
				return OnCycleFocus(!shiftDown);
			}
			}
		}

		break;
	}


	// Mouse messages
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
	case WM_XBUTTONDBLCLK:
	case WM_MOUSEWHEEL:
	{
		// If not accepting mouse input, return false to indicate the message should still 
		// be handled by the application (usually to move the camera).
		if (!_mouseInput)
			return false;

		POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
		mousePoint.x -= _x;
		mousePoint.y -= _y;

		// If caption is enabled, offset the Y coordinate by the negative of its height.
		if (_showCaption)
			mousePoint.y -= _captionHeight;

		// If a control is in focus, it belongs to this dialog, and it's enabled, then give
		// it the first chance at handling the message.
		if (_spControlFocus &&
			_spControlFocus->_pParent == this &&
			_spControlFocus->GetEnabled())
		{
			if (_spControlFocus->HandleMouse(msg, mousePoint, wParam, lParam))
				return true;
		}

		// Not yet handled, see if the mouse is over any controls
		Control* pControl = GetControlAtPoint(mousePoint);
		if (pControl != NULL && pControl->GetEnabled())
		{
			handled = pControl->HandleMouse(msg, PointMake(LOWORD(lParam), HIWORD(lParam)), wParam, lParam);
			if (handled)
			{
				return true;
			}
		}
		else
		{
			// Mouse not over any controls in this dialog, if there was a control
			// which had focus it just lost it
			if (msg == WM_LBUTTONDOWN &&
				_spControlFocus &&
				_spControlFocus->_pParent == this)
			{
				_spControlFocus->OnFocusOut();
				_spControlFocus = NULL;
			}
		}

		// Still not handled, hand this off to the dialog. Return false to indicate the
		// message should still be handled by the application (usually to move the camera).
		switch (msg)
		{
		case WM_MOUSEMOVE:
			OnMouseMove(mousePoint);
			return false;
		}

		break;
	}

	case WM_CAPTURECHANGED:
	{
		// The application has lost mouse capture.
		// The dialog object may not have received
		// a WM_MOUSEUP when capture changed. Reset
		// m_bDrag so that the dialog does not mistakenly
		// think the mouse button is still held down.
		if ((HWND)lParam != windowHandle)
			_drag = false;
	}
	}
	return false;
}

bool Dialog::AddStatic(int32 id, const char * text, int32 x, int32 y, int32 width, int32 height, bool isDefault, StaticControl ** ppCreated)
{
	return false;
}

void Dialog::RemoveControl(int32 ID)
{
	for (int32 i = 0; i < _controls.size(); ++i)
	{
		Control *pControl = _controls[i];
		if (pControl->GetID() == ID)
		{
			ClearFocus();

			if (_spControlFocus == pControl)
			{
				_spControlFocus = nullptr;
			}
			if (_spControlPressed == pControl)
			{
				_spControlPressed = nullptr;
			}
			if (_pControlMouseOver == pControl)
			{
				_pControlMouseOver = nullptr;
			}
			SAFE_DELETE(pControl);
			_controls.shrink_to_fit();
			return;
		}
	}
}

void Dialog::RemoveAllControls()
{
}

void Dialog::SendEvent(UINT event, bool triggeredByUser, Control * pControl)
{
	// If no callback has been registered there's nowhere to send the event to
	if (_callbackEvent == NULL)
		return;

	// Discard events triggered programatically if these types of events haven't been
	// enabled
	if (!triggeredByUser && !_nonUserEvent)
		return;

	_callbackEvent(event, pControl->GetID(), pControl, _pUserContext);
}

void Dialog::RequestFocus(Control * pControl)
{
	if (_spControlFocus == pControl)
		return;

	if (!pControl->CanHaveFocus())
		return;

	if (_spControlFocus)
		_spControlFocus->OnFocusOut();

	pControl->OnFocusIn();
	_spControlFocus = pControl;
}

bool Dialog::DrawRect(RECT * pRect, D3DCOLOR color)
{
	RECT rcScreen = *pRect;
	OffsetRect(&rcScreen, _x, _y);

	// If caption is enabled, offset the Y position by its height.
	if (_showCaption)
		OffsetRect(&rcScreen, 0, _captionHeight);

	ScreenVertex vertices[4] =
	{
		(float)rcScreen.left - 0.5f,  (float)rcScreen.top - 0.5f,    0.5f, 1.0f, color, 0, 0,
		(float)rcScreen.right - 0.5f, (float)rcScreen.top - 0.5f,    0.5f, 1.0f, color, 0, 0,
		(float)rcScreen.right - 0.5f, (float)rcScreen.bottom - 0.5f, 0.5f, 1.0f, color, 0, 0,
		(float)rcScreen.left - 0.5f,  (float)rcScreen.bottom - 0.5f, 0.5f, 1.0f, color, 0, 0,
	};

	// Since we're doing our own drawing here we need to flush the sprites
	_pManager->_pSprite->Flush();
	IDirect3DVertexDeclaration9* pDecl = NULL;
	gpDevice->GetVertexDeclaration(&pDecl);  // Preserve the sprite's current vertex decl
	gpDevice->SetFVF(ScreenVertex::FVF);

	gpDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG2);
	gpDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);

	gpDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vertices, sizeof(ScreenVertex));

	gpDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	gpDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	// Restore the vertex decl
	gpDevice->SetVertexDeclaration(pDecl);
	pDecl->Release();

	return true;
}

bool Dialog::DrawPolyLine(POINT * pPoints, UINT numPoints, D3DCOLOR color)
{
	ScreenVertex* vertices = new ScreenVertex[numPoints];
	if (vertices == NULL)
		return E_OUTOFMEMORY;

	ScreenVertex* pVertex = vertices;
	POINT* pt = pPoints;
	for (UINT i = 0; i < numPoints; i++)
	{
		pVertex->x = _x + (float)pt->x;
		pVertex->y = _y + (float)pt->y;
		pVertex->z = 0.5f;
		pVertex->w = 1.0f;
		pVertex->color = color;
		pVertex->u = 0.0f;
		pVertex->v = 0.0f;

		pVertex++;
		pt++;
	}

	// Since we're doing our own drawing here we need to flush the sprites
	_pManager->_pSprite->Flush();
	IDirect3DVertexDeclaration9* pDecl = NULL;
	gpDevice->GetVertexDeclaration(&pDecl);  // Preserve the sprite's current vertex decl
	gpDevice->SetFVF(ScreenVertex::FVF);

	gpDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG2);
	gpDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);

	gpDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, numPoints - 1, vertices, sizeof(ScreenVertex));

	gpDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	gpDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	// Restore the vertex decl
	gpDevice->SetVertexDeclaration(pDecl);
	pDecl->Release();

	SAFE_DELETE_ARRAY(vertices);
	return S_OK;
}

bool Dialog::DrawSprite(Element * pElement, RECT * prcDest)
{
	// No need to draw fully transparent layers
	if (pElement->textureColor.current.a == 0)
		return S_OK;

	RECT rcTexture = pElement->textureRect;

	RECT rcScreen = *prcDest;
	OffsetRect(&rcScreen, _x, _y);

	// If caption is enabled, offset the Y position by its height.
	if (_showCaption)
		OffsetRect(&rcScreen, 0, _captionHeight);

	TextureNode* pTextureNode = GetTexture(pElement->textureIndex);
	if (pTextureNode == nullptr)
	{
		return false;
	}

	float fScaleX = (float)RectWidth(rcScreen) / RectWidth(rcTexture);
	float fScaleY = (float)RectHeight(rcScreen) / RectHeight(rcTexture);

	D3DXMATRIXA16 matTransform;
	D3DXMatrixScaling(&matTransform, fScaleX, fScaleY, 1.0f);

	_pManager->_pSprite->SetTransform(&matTransform);

	D3DXVECTOR3 vPos((float)rcScreen.left, (float)rcScreen.top, 0.0f);

	vPos.x /= fScaleX;
	vPos.y /= fScaleY;

	return _pManager->_pSprite->Draw(pTextureNode->pTexture, &rcTexture, NULL, &vPos,
		pElement->textureColor.current);
}


bool Dialog::CalcTextRect(const char *text, Element * pElement, RECT * pDestRect, int count)
{
	HRESULT hr = S_OK;

	FontNode* pFontNode = GetFont(pElement->fontIndex);
	if (pFontNode == NULL)
		return E_FAIL;

	DWORD textFormat = pElement->fontFormat | DT_CALCRECT;
	// Since we are only computing the rectangle, we don't need a sprite.
	if (pFontNode->pFont)
	{
		hr = pFontNode->pFont->DrawTextA(NULL, text, count, pDestRect, textFormat, pElement->fontColor.current);
		if (FAILED(hr))
			return false;
	}
	return false;
}

bool Dialog::DrawFont(const char *text, Element* pElement, RECT *pDestRect, bool shadow, int count)
{
	HRESULT hr = S_OK;

	// No need to draw fully transparent layers
	if (pElement->fontColor.current.a == 0)
		return S_OK;

	RECT rcScreen = *pDestRect;
	OffsetRect(&rcScreen, _x, _y);

	// If caption is enabled, offset the Y position by its height.
	if (_showCaption)
		OffsetRect(&rcScreen, 0, _captionHeight);

	D3DXMATRIX matTransform;
	D3DXMatrixIdentity(&matTransform);
	_pManager->_pSprite->SetTransform(&matTransform);

	FontNode* pFontNode = GetFont(pElement->fontIndex);

	if (shadow)
	{
		RECT rcShadow = rcScreen;
		OffsetRect(&rcShadow, 1, 1);
		hr = pFontNode->pFont->DrawTextA(_pManager->_pSprite, text, count, &rcShadow, pElement->fontFormat,
			D3DCOLOR_ARGB(DWORD(pElement->fontColor.current.a * 255), 0, 0, 0));
		if (FAILED(hr))
			return false;
	}

	hr = pFontNode->pFont->DrawTextA(_pManager->_pSprite, text, count, &rcScreen, pElement->fontFormat,
		pElement->fontColor.current);
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

Control * Dialog::GetControl(uint32 id)
{
	for (int32 i = 0; i < _controls.size(); ++i)
	{
		Control *pControl = _controls[i];
		if (pControl->GetID() == id)
		{
			return pControl;
		}
	}
	return nullptr;
}

Control * Dialog::GetControl(uint32 id, ControlType type)
{
	for (int32 i = 0; i < _controls.size(); ++i)
	{
		Control *pControl = _controls[i];
		if (pControl->GetID() == id && pControl->GetType() == type)
		{
			return pControl;
		}
	}
	return nullptr;
}

Control * Dialog::GetControlAtPoint(POINT pt)
{
	for (int32 i = 0; i < _controls.size(); ++i)
	{
		Control *pControl = _controls[i];
		if (pControl->ContainsPoint(pt) &&
			pControl->GetVisible() &&
			pControl->GetEnabled())
		{
			return pControl;
		}
	}
	return nullptr;
}

bool Dialog::GetControlEnabled(uint32 id)
{
	for (int32 i = 0; i < _controls.size(); ++i)
	{
		Control *pControl = _controls[i];
		if (pControl->GetID() == id)
		{
			return pControl->GetEnabled();
		}
	}

	return false;
}

void Dialog::SetControlEnabled(uint32 id, bool enable)
{
	for (int32 i = 0; i < _controls.size(); ++i)
	{
		Control *pControl = _controls[i];
		if (pControl->GetID() == id)
		{
			pControl->SetEnabled(enable);
		}
	}
}

void Dialog::ClearFocus()
{
	if (_spControlFocus)
	{
		_spControlFocus->OnFocusOut();
		_spControlFocus = nullptr;
	}
}

void Dialog::FocusDefaultControl()
{
	// Check for default control in this dialog
	for (int i = 0; i < _controls.size(); i++)
	{
		Control* pControl = _controls[i];
		if (pControl->_isDefault)
		{
			// Remove focus from the current control
			ClearFocus();
			// Give focus to the default control
			_spControlFocus = pControl;
			_spControlFocus->OnFocusIn();
			return;
		}
	}
}

bool Dialog::SetDefaultElement(ControlType type, uint32 elementIndex, Element * pElement)
{
	//check already exist 
	for (int32 i = 0; i < _defaultElements.size(); ++i)
	{
		ElementHolder *pHolder = _defaultElements[i];
		if (pHolder->type == type &&
			pHolder->elementIndex == elementIndex)
		{
			pHolder->element = *pElement;
			return true;
		}
	}

	ElementHolder* pNewHolder;
	pNewHolder = new ElementHolder;
	if (pNewHolder == NULL)
	{
		return false;
	}

	pNewHolder->type = type;
	pNewHolder->elementIndex = elementIndex;
	pNewHolder->element = *pElement;

	_defaultElements.push_back(pNewHolder);
	return true;
}

Element * Dialog::GetDefaultElement(ControlType type, uint32 elementIndex)
{
	for (int i = 0; i < _defaultElements.size(); i++)
	{
		ElementHolder* pElementHolder = _defaultElements[i];

		if (pElementHolder->type == type &&
			pElementHolder->elementIndex == elementIndex)
		{
			return &pElementHolder->element;
		}
	}
	return nullptr;
}

void Dialog::InitDefaultElement()
{
}

void Dialog::SetCallback(GUICALLBACKEVENT pCallback, void * pUserContext)
{
	_callbackEvent = pCallback;
	_pUserContext = pUserContext;
}

void Dialog::EnableNonUserEvent(bool enable)
{
	_nonUserEvent = enable;
}

bool Dialog::SetFont(uint32 index, const char * faceName, int32 height, int32 width)
{
	for (uint32 i = _fonts.size(); i < index; ++i)
	{
		_fonts.push_back(-1);
	}

	int32 fontIndex = _pManager->AddFont(faceName, height, width);
	_fonts[index] = fontIndex;
	return true;
}

FontNode * Dialog::GetFont(uint32 index)
{
	if (_pManager == nullptr)
	{
		return nullptr;
	}
	return _pManager->GetFontNode(_fonts[index]);
}

bool Dialog::SetTexture(uint32 index, const char * fileName)
{
	for (uint32 i = _textures.size(); i < index; ++i)
	{
		_textures.push_back(-1);
	}

	int32 textureIndex = _pManager->AddTexture(fileName);
	_textures[index] = textureIndex;
	return true;
}

TextureNode * Dialog::GetTexture(uint32 index)
{
	if (_pManager == nullptr)
	{
		return nullptr;
	}
	return _pManager->GetTextureNode(_textures[index]);
}

void Dialog::SetBackgroundColors(D3DCOLOR colorTopLeft, D3DCOLOR colorTopRight,
	D3DCOLOR colorBottomLeft, D3DCOLOR colorBottomRight)
{
	_colorTopLeft = colorTopLeft;
	_colorTopRight = colorTopRight;
	_colorBottomLeft = colorBottomLeft;
	_colorBottomRight = colorBottomRight;
}
Control * Dialog::GetNextControl(Control * pControl)
{
	int index = pControl->GetIndex() + 1;

	Dialog* pDialog = pControl->_pParent;

	// Cycle through dialogs in the loop to find the next control. Note
	// that if only one control exists in all looped dialogs it will
	// be the returned 'next' control.
	while (index >= (int)pDialog->_controls.size())
	{
		pDialog = pDialog->_pNextDialog;
		index = 0;
	}

	return pDialog->_controls[index];
}

Control * Dialog::GetPrevControl(Control * pControl)
{
	uint32 index = pControl->GetIndex() - 1;

	Dialog* pDialog = pControl->_pParent;

	// Cycle through dialogs in the loop to find the next control. Note
	// that if only one control exists in all looped dialogs it will
	// be the returned 'previous' control.
	while (index < 0)
	{
		pDialog = pDialog->_pPrevDialog;
		if (pDialog == NULL)
			pDialog = pControl->_pParent;

		index = pDialog->_controls.size() - 1;
	}

	return pDialog->_controls[index];
}

void Dialog::Refresh()
{
	if (_spControlFocus)
		_spControlFocus->OnFocusOut();

	if (_pControlMouseOver)
		_pControlMouseOver->OnMouseLeave();

	_spControlFocus = NULL;
	_spControlFocus = NULL;
	_pControlMouseOver = NULL;

	for (int i = 0; i < _controls.size(); i++)
	{
		Control* pControl = _controls[i];
		pControl->Refresh();
	}

	if (_keyboardInput)
	{
		FocusDefaultControl();
	}
}

bool Dialog::OnRender(uint32 deltaMS)
{
	//if (m_fTimeLastRefresh < s_fTimeRefresh)
	//{
	//	m_fTimeLastRefresh = DXUTGetTime();
	//	Refresh();
	//}
	_pManager->_pStateBlock->Capture();
	gpDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	gpDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	gpDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	gpDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	gpDevice->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, FALSE);
	gpDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	gpDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_BLUE |
		D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_RED);
	gpDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	gpDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
	gpDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	gpDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	gpDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	gpDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG2);
	gpDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	gpDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	gpDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	gpDevice->SetTextureStageState(0, D3DTSS_RESULTARG, D3DTA_CURRENT);
	gpDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	gpDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	BOOL backgroundIsVisible = (_colorTopLeft | _colorTopRight | _colorBottomRight | _colorBottomLeft) &
		0xff000000;
	if (!_minimized && backgroundIsVisible)
	{
		ScreenVertex vertices[4] =
		{
			(float)_x,           (float)_y,            0.5f, 1.0f, _colorTopLeft,
			(float)_x + _width, (float)_y,            0.5f, 1.0f, _colorTopRight,
			(float)_x + _width, (float)_y + _height, 0.5f, 1.0f, _colorBottomRight,
			(float)_x,           (float)_y + _height, 0.5f, 1.0f, _colorBottomLeft,
		};

		gpDevice->SetVertexShader(NULL);
		gpDevice->SetPixelShader(NULL);

		gpDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

		gpDevice->SetFVF(ScreenVertex::FVF);
		gpDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vertices, sizeof(ScreenVertex));
	}

	gpDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	gpDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	gpDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	gpDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	gpDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	gpDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	gpDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	TextureNode* pTextureNode = GetTexture(0);
	gpDevice->SetTexture(0, pTextureNode->pTexture);

	_pManager->_pSprite->Begin(D3DXSPRITE_DONOTSAVESTATE);

	// Render the caption if it's enabled.
	if (_showCaption)
	{
		// DrawSprite will offset the rect down by
		// m_nCaptionHeight, so adjust the rect higher
		// here to negate the effect.
		RECT rc = { 0, -_captionHeight, _width, 0 };
		DrawSprite(&_captionElement, &rc);
		rc.left += 5; // Make a left margin
		char output[256];
		strcpy(output, _captionText);
		if (_minimized)
			strcat(output, " (Minimized)");
		//DrawText9(output, &_captionElement, &rc, true);
	}

	// If the dialog is minimized, skip rendering
	// its controls.
	if (!_minimized)
	{
		for (int i = 0; i < _controls.size(); i++)
		{
			Control* pControl = _controls[i];

			// Focused control is drawn last
			if (pControl == _spControlFocus)
				continue;

			pControl->Render(deltaMS);
		}

		if (_spControlFocus != NULL && _spControlFocus->_pParent == this)
		{
			_spControlFocus->Render(deltaMS);
		}
	}

	_pManager->_pSprite->End();

	_pManager->_pStateBlock->Apply();

	return true;
}

void Dialog::SetNextDialog(Dialog * pDialog)
{
	if (pDialog == NULL)
	{
		pDialog = this;
	}

	_pNextDialog = pDialog;
	if (pDialog)
	{
		pDialog->_pPrevDialog = this;
	}
}

void Dialog::OnMouseMove(POINT point)
{
	Control *pControl = GetControlAtPoint(point);

	// If the mouse is still over the same control, nothing needs to be done
	if (pControl == _pControlMouseOver)
		return;

	// Handle mouse leaving the old control
	if (_pControlMouseOver)
		_pControlMouseOver->OnMouseLeave();

	// Handle mouse entering the new control
	_pControlMouseOver = pControl;
	if (pControl != nullptr)
		_pControlMouseOver->OnMouseEnter();
}

void Dialog::OnMouseUp(POINT point)
{
	_spControlPressed = nullptr;
	_pControlMouseOver = nullptr;
}

bool Dialog::OnCycleFocus(bool forward)
{
	Control* pControl = nullptr;
	Dialog* pDialog = nullptr;  //pDialog and pLastDialog are used to track wrapping of
	Dialog* pLastDialog = nullptr;    // focus from first control to last or vice versa.

	if (_spControlFocus == nullptr)
	{
		// If s_pControlFocus is NULL, we focus the first control of first dialog in
		// the case that bForward is true, and focus the last control of last dialog when
		// bForward is false.
		//
		if (forward)
		{
			// Search for the first control from the start of the dialog
			// array.
			for (int d = 0; d < _pManager->_dialogs.size(); ++d)
			{
				pDialog = pLastDialog = _pManager->_dialogs[d];
				if (pDialog && pDialog->_controls.size() > 0)
				{
					pControl = pDialog->_controls[0];
					break;
				}
			}

			if (!pDialog || !pControl)
			{
				// No dialog has been registered yet or no controls have been
				// added to the dialogs. Cannot proceed.
				return true;
			}
		}
		else
		{
			// Search for the first control from the end of the dialog
			// array.
			for (int d = _pManager->_dialogs.size() - 1; d >= 0; --d)
			{
				pDialog = pLastDialog = _pManager->_dialogs[d];
				if (pDialog && pDialog->_controls.size() > 0)
				{
					pControl = pDialog->_controls[(pDialog->_controls.size() - 1)];
					break;
				}
			}

			if (!pDialog || !pControl)
			{
				// No dialog has been registered yet or no controls have been
				// added to the dialogs. Cannot proceed.
				return true;
			}
		}
	}
	else if (_spControlFocus->_pParent != this)
	{
		// If a control belonging to another dialog has focus, let that other
		// dialog handle this event by returning false.
		//
		return false;
	}
	else
	{
		// Focused control belongs to this dialog. Cycle to the
		// next/previous control.
		pLastDialog = _spControlFocus->_pParent;
		pControl = (forward) ? GetNextControl(_spControlFocus) : GetPrevControl(_spControlFocus);
		pDialog = pControl->_pParent;
	}

	for (int i = 0; i < 0xffff; i++)
	{
		// If we just wrapped from last control to first or vice versa,
		// set the focused control to NULL. This state, where no control
		// has focus, allows the camera to work.

		//TODO
		int lastDialogIndex = 0;
		int dialogIndex = 0;
		for (int i = 0; i < _pManager->_dialogs.size(); ++i)
		{
			if (_pManager->_dialogs[i] == pLastDialog)
			{
				lastDialogIndex = i;
				break;
			}
		}

		for (int i = 0; i < _pManager->_dialogs.size(); ++i)
		{
			if (_pManager->_dialogs[i] == pDialog)
			{
				dialogIndex = i;
				break;
			}
		}

		if ((!forward && lastDialogIndex < dialogIndex) ||
			(forward && dialogIndex < lastDialogIndex))
		{
			if (_spControlFocus)
				_spControlFocus->OnFocusOut();
			_spControlFocus = NULL;
			return true;
		}

		// If we've gone in a full circle then focus doesn't change
		if (pControl == _spControlFocus)
			return true;

		// If the dialog accepts keybord input and the control can have focus then
		// move focus
		if (pControl->_pParent->_keyboardInput && pControl->CanHaveFocus())
		{
			if (_spControlFocus)
				_spControlFocus->OnFocusOut();
			_spControlFocus = pControl;
			_spControlFocus->OnFocusIn();
			return true;
		}

		pLastDialog = pDialog;
		pControl = (forward) ? GetNextControl(pControl) : GetPrevControl(pControl);
		pDialog = pControl->_pParent;
	}

	// If we reached this point, the chain of dialogs didn't form a complete loop
	return false;
}

