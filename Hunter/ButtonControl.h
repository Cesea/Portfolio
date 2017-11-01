#ifndef BUTTON_CONTROL_H
#define BUTTON_CONTROL_H

#include "StaticControl.h"

namespace gui
{
	class ButtonControl : public StaticControl
	{
	public :
		ButtonControl(Dialog *pParent);
		virtual ~ButtonControl();

		virtual bool HandleKeyboard(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual bool HandleMouse(UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam);
		virtual void OnHotkey()
		{
			if (m_pDialog->IsKeyboardInputEnabled()) m_pDialog->RequestFocus(this);
			m_pDialog->SendEvent(EVENT_BUTTON_CLICKED, true, this);
		}

		virtual BOOL ContainsPoint(POINT pt)
		{
			return PtInRect(&_boundingBox, pt);
		}
		virtual bool CanHaveFocus()
		{
			return (_visible && _fastcall);
		}

		virtual void Render(uint32 deltaMS);

	protected:
		bool _pressed;
	};
}

#endif