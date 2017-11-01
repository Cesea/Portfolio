#include "stdafx.h"
#include "StaticControl.h"

	using namespace gui;

	StaticControl::StaticControl(Dialog * pParent)
	{
		_type = ControlType::CONTROL_STATIC;
		_pParent = pParent;

		ZeroMemory(_text, sizeof(char) * MAX_PATH);
	}

	StaticControl::~StaticControl()
	{
	}

	void StaticControl::Render(float deltaMS)
	{
		if (_visible == false)
			return;

		ControlState state = ControlState::STATE_NORMAL;

		if (_enabled == false)
			state = STATE_DISABLED;

		Element* pElement = _elements[0];

		pElement->fontColor.Blend(state, Uint32ToFloat(deltaMS));

		//_pParent->DrawText(_text, pElement, &_boundingRect, true);
	}

	bool StaticControl::GetTextCopy(char * toCopy)
	{
		return false;
	}

	bool StaticControl::SetText(char * text)
	{
		return false;
	}
