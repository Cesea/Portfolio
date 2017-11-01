#ifndef STATIC_CONTROL_H
#define STATIC_CONTROL_H

#include "Control.h"

namespace gui
{
	class StaticControl : public Control
	{
	public:
		StaticControl(Dialog *pParent);
		virtual ~StaticControl();

		virtual void Render(float deltaMS);
		virtual bool ContainsPoint(POINT pt)
		{
			return false;
		}

		bool GetTextCopy(char *toCopy);
		char *GetText()
		{
			return _text;
		}
		bool SetText(char * text);


	protected:
		char _text[MAX_PATH];      // Window text  

	};
}

#endif
