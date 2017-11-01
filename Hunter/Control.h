#ifndef CONTROL_H
#define CONTROL_H


namespace gui
{
	class Dialog;

	enum ControlType
	{
		CONTROL_INVALID,
		CONTROL_BUTTON,
		CONTROL_STATIC,
		CONTROL_CHECKBOX,
		CONTROL_RADIOBUTTON,
		CONTROL_COMBOBOX,
		CONTROL_SLIDER,
		CONTROL_EDITBOX,
		CONTROL_IMEEDITBOX,
		CONTROL_LISTBOX,
		CONTROL_SCROLLBAR,
	};

	enum ControlState
	{
		STATE_NORMAL = 0,
		STATE_DISABLED,
		STATE_HIDDEN,
		STATE_FOCUS,
		STATE_MOUSEOVER,
		STATE_PRESSED,
		STATE_NUM,
	};

	struct BlendColor
	{
		void Init(D3DCOLOR defaultColor, D3DCOLOR disabledColor = D3DCOLOR_ARGB(200, 128, 128, 128), D3DCOLOR hiddenColor = 0);
		void Blend(ControlState state, uint32 deltaMS, float rate = 0.7f);

		D3DCOLOR states[ControlState::STATE_NUM];
		D3DXCOLOR current;
	};


	class Element
	{
	public:
		void SetTexture(uint32 textureIndex, RECT *pRect, D3DCOLOR defaultTextureColor = D3DCOLOR_ARGB(255, 255, 255, 255));
		void SetFont(uint32 fontIndex, D3DCOLOR defaultTextureColor = D3DCOLOR_ARGB(255, 255, 255, 255),
			DWORD textFormat = DT_CENTER | DT_VCENTER);

		void Refresh();

		uint32 textureIndex;
		uint32 fontIndex;
		DWORD fontFormat;

		RECT textureRect;

		BlendColor textureColor;
		BlendColor fontColor;
	};

	struct ElementHolder
	{
		ControlType type;
		uint32 elementIndex;

		Element element;
	};

	class Control
	{
	public:
		Control(Dialog *pParent = nullptr);
		virtual ~Control();

		virtual bool Init();
		virtual void Refresh();
		virtual void Render(uint32 deltaMS);

		virtual bool MessageProc(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam);
		virtual bool HandleKeyboard(UINT msg, WPARAM wParam, LPARAM lParam);
		virtual bool HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam );

		virtual bool CanHaveFocus();
		virtual void OnFocusIn();
		virtual void OnFocusOut();
		virtual void OnMouseEnter();
		virtual void OnMouseLeave();
		virtual void OnHotkey();

		virtual bool ContainsPoint(POINT point)
		{
			return PtInRect(&_boundingRect, point);
		}

		virtual bool GetEnabled();
		virtual void SetEnabled(bool enable);
		virtual void SetVisible(bool visible);
		virtual bool GetVisible();

		ControlType GetType() const;

		uint32 GetID();
		void SetID(uint32 id);

		uint32 GetIndex() { return _index; }
		void SetIndex(uint32 index) { _index = index; }

		void SetLocation(int32 x, int32 y);
		void SetSize(int32 width, int32 height);
		void SetHotkey(uint32 hotkey);
		uint32 GetHotKey();

		void *GetUserData() const;
		void SetUserData(void *pData);

		virtual void SetTextColor(D3DCOLOR color);

		Element *GetElement(uint32 index);
		bool SetElement(uint32 index, Element *pElement);

		bool _visible;
		bool _mouseOver;
		bool _hasFocus;
		bool _isDefault;

		Dialog *_pParent;

	protected:

		int32 _x;
		int32 _y;

		int32 _width;
		int32 _height;

		uint32 _index;

		std::vector<Element *> _elements;

		virtual void UpdateRect();

		uint32 _id;

		ControlType _type;
		uint32 _hotkey;

		bool _enabled;

		void *_pUserData;

		RECT _boundingRect;
	};

}


#endif