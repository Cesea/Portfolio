#include "stdafx.h"
#include "Control.h"

#include "Dialog.h"

using namespace gui;

void BlendColor::Init(D3DCOLOR defaultColor, D3DCOLOR disabledColor, D3DCOLOR hiddenColor)
{
	for (int32 i = 0; i < ControlState::STATE_NUM; ++i)
	{
		states[i] = defaultColor;
	}
	states[ControlState::STATE_DISABLED] = disabledColor;
	states[ControlState::STATE_HIDDEN] = hiddenColor;
	current = hiddenColor;
}

void BlendColor::Blend(ControlState state, uint32 deltaMS, float rate)
{
	D3DXCOLOR destColor = states[state];
	D3DXColorLerp(&current, &current, &destColor, 1.0f - powf(rate, 30 * deltaMS * 0.001f));

}

void Element::SetTexture(uint32 textureIndex, RECT *pRect, D3DCOLOR defaultTextureColor)
{
	this->textureIndex = textureIndex;

	if (pRect)
		this->textureRect = *pRect;
	else
		SetRectEmpty(&this->textureRect);

	this->textureColor.Init(defaultTextureColor);
}
void Element::SetFont(uint32 fontIndex, D3DCOLOR defaultFontColor, DWORD textFormat)
{
	this->fontIndex = fontIndex;
	this->fontFormat = textFormat;

	this->fontColor.Init(defaultFontColor);
}

void Element::Refresh()
{
	textureColor.current = textureColor.states[ControlState::STATE_HIDDEN];
	fontColor.current = fontColor.states[ControlState::STATE_HIDDEN];
}

Control::Control(Dialog * pParent)
{
	_type = CONTROL_INVALID;
	_pParent = pParent;
	_id = 0;
	_index = 0;
	_pUserData = NULL;

	_enabled = true;
	_visible = true;
	_mouseOver = false;
	_hasFocus = false;
	_isDefault = false;

	_pParent = pParent;

	_x = 0;
	_y = 0;
	_width = 0;
	_height = 0;

	ZeroMemory(&_boundingRect, sizeof(_boundingRect));
}

Control::~Control()
{
}

bool Control::Init()
{
	return true;
}

void Control::Refresh()
{
	_mouseOver = false;
	_hasFocus = false;

	for (int32 i = 0; i < _elements.size(); i++)
	{
		Element* pElement = _elements[i];
		pElement->Refresh();
	}
}

void Control::Render(uint32 deltaMS)
{
}

bool Control::MessageProc(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return false;
}

bool Control::HandleKeyboard(UINT msg, WPARAM wParam, LPARAM lParam)
{
	return false;
}

bool Control::HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam )
{
	return false;
}

bool Control::CanHaveFocus()
{
	return false;
}

void Control::OnFocusIn()
{
	_hasFocus = true;
}

void Control::OnFocusOut()
{
	_hasFocus = false;
}

void Control::OnMouseEnter()
{
	_mouseOver = true;
}

void Control::OnMouseLeave()
{
	_mouseOver = false;
}

void Control::OnHotkey()
{
}

void Control::SetEnabled(bool enable)
{
	_enabled = enable;

}

bool Control::GetEnabled()
{
	return _enabled;
}

void Control::SetVisible(bool visible)
{
	_visible = visible;
}

bool Control::GetVisible()
{
	return _visible;
}

ControlType Control::GetType() const
{
	return _type;
}

uint32 Control::GetID()
{
	return _id;
}

void Control::SetID(uint32 id)
{
	_id = id;
}

void Control::SetLocation(int32 x, int32 y)
{
	_x = x;
	_y = y;
}

void Control::SetSize(int32 width, int32 height)
{
	_width = width;
	_height = height;
	UpdateRect();
}

void Control::SetHotkey(uint32 hotkey)
{
	_hotkey = hotkey;
}

uint32 Control::GetHotKey()
{
	return _hotkey;
}

void * Control::GetUserData() const
{
	return _pUserData;
}

void Control::SetUserData(void *pData)
{
	_pUserData = pData;
}

void Control::SetTextColor(D3DCOLOR color)
{
	Element *pElement = _elements[0];
	if (pElement)
	{
		pElement->fontColor.states[ControlState::STATE_NORMAL] = color;
	}
}

Element * Control::GetElement(uint32 index)
{
	return _elements[index];
}

bool Control::SetElement(uint32 index, Element * pElement)
{
	if (_elements.size() > index)
	{
		_elements[index] = pElement;
		return true;
	}
	return false;
}

void Control::UpdateRect()
{
	SetRect(&_boundingRect, _x, _y, _x + _width, _y + _height);
}
