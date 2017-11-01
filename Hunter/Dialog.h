#ifndef DIALOG_H
#define DIALOG_H

#include "GuiResource.h"
#include "Control.h"
#include "StaticControl.h"

#include <vector>

namespace gui
{
	struct ScreenVertex
	{
		float x, y, z, w;
		D3DCOLOR color;
		float u, v;

		enum
		{
			FVF = D3DFVF_XYZW | D3DFVF_DIFFUSE | D3DFVF_TEX1,
		};
	};

	struct ScreenVertexUnTex
	{
		float x, y, z, w;
		D3DCOLOR color;

		enum
		{
			FVF = D3DFVF_XYZW | D3DFVF_DIFFUSE,
		};
	};

	typedef void (CALLBACK *GUICALLBACKEVENT)(UINT event, int32 controlID, Control *pControl, void *pUserContext);

	class Dialog
	{
	public:
		Dialog();
		virtual ~Dialog();

		void Init(DialogResourceManager *pResourceManager);

		bool InternalMessageProc(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam);

		bool AddStatic(int32 id, const char *text, int32 x, int32 y, int32 width, int32 height, bool isDefault = false,
                                   StaticControl** ppCreated=NULL );

		void RemoveControl(int32 ID);
		void RemoveAllControls();

		void EnableNonUserEvents(bool enable) { _nonUserEvent = enable; }
		void EnableKeyboardInput(bool enable) { _keyboardInput = enable; }
		bool IsKeyboardInputEnabled() { return _keyboardInput; }
		void EnableMouseInput(bool enable) { _mouseInput = enable; }


		bool DrawRect(RECT* pRect, D3DCOLOR color);
		bool DrawPolyLine(POINT* apPoints, UINT nNumPoints, D3DCOLOR color);
		bool DrawSprite(Element* pElement, RECT* prcDest);
		bool CalcTextRect(const char *strText, Element* pElement, RECT* pDestRect, int nCount = -1);
		bool DrawFont(const char *text, Element* pElement, RECT *pDestRect, bool shadow = false,
			int count = -1);

		Control *GetControl(uint32 id);
		Control *GetControl(uint32 id, ControlType type);
		Control *GetControlAtPoint(POINT pt);

		bool GetControlEnabled(uint32 id);
		void SetControlEnabled(uint32 id, bool enable);

		void ClearFocus();
		void FocusDefaultControl();

		bool SetDefaultElement(ControlType type, uint32 elementIndex, Element *pElement);
		Element *GetDefaultElement(ControlType type, uint32 elementIndex);
		void InitDefaultElement();

		void SendEvent(UINT event, bool triggeredByUser, Control *pControl);
		void RequestFocus(Control *pControl);

		bool _nonUserEvent;
		bool _mouseInput;
		bool _keyboardInput;

		bool GetVisible() { return _visible; }
		void SetVisible(bool b) { _visible = b; }

		bool GetMinimized() { return _minimized; }
		void SetMinimized(bool b) { _minimized = b; }

		//void SetBackgroundColors(D3DCOLOR color) {SetBackgrou}

		void SetEnableCaption(bool enable) { _showCaption = enable; }
		int32 GetCaptionHeight() const { return _captionHeight; }
		void SetCaptionHeight(int32 height) { _captionHeight = height; }

		void SetCaptionText(const char *text) { strncpy(_captionText, text, strlen(text)); }

		POINT GetLocation() const { return PointMake(_x, _y); }
		POINT GetSize() const { return PointMake(_width, _height); }
		int32 GetWidth() { return _width; }
		int32 GetHeight() { return _height; }


		void SetCallback(GUICALLBACKEVENT pCallback, void *pUserContext = nullptr);
		void EnableNonUserEvent(bool enable);

		bool SetFont(uint32 index, const char *faceName, int32 height, int32 width);
		FontNode *GetFont(uint32 index);
		
		bool SetTexture(uint32 index, const char *fileName);
		TextureNode *GetTexture(uint32 index);

		DialogResourceManager *GetManager() { return _pManager; }

		void SetBackgroundColors(D3DCOLOR colorAllCorners) { SetBackgroundColors(colorAllCorners, colorAllCorners, colorAllCorners, colorAllCorners); }
		void SetBackgroundColors(D3DCOLOR colorTopLeft, D3DCOLOR colorTopRight, 
			D3DCOLOR colorBottomLeft, D3DCOLOR colorBottomRight);

		static Control *GetNextControl(Control *pControl);
		static Control *GetPrevControl(Control *pControl);

		void Refresh();
		bool OnRender(uint32 deltaMS);

	private:

		int32 _defaultControlID;
		
		void SetNextDialog(Dialog *pDialog);

		void OnMouseMove(POINT point);
		void OnMouseUp(POINT point);

		bool OnCycleFocus(bool forward);

		Control *_pControlMouseOver;
		static Control *_spControlFocus;
		static Control *_spControlPressed;

		D3DCOLOR _colorTopLeft;
		D3DCOLOR _colorTopRight;
		D3DCOLOR _colorBottomLeft;
		D3DCOLOR _colorBottomRight;

		GUICALLBACKEVENT _callbackEvent;
		void *_pUserContext;

		int _x;
		int _y;
		int _width;
		int _height;
		int _captionHeight;

		bool _visible;
		bool _showCaption;
		bool _minimized;
		bool _drag;
		char _captionText[MAX_PATH];

		std::vector<int32> _textures;
		std::vector<int32> _fonts;

		DialogResourceManager *_pManager;

		std::vector<gui::Control *> _controls;
		std::vector<gui::ElementHolder *> _defaultElements;

		Element _captionElement;

		Dialog *_pNextDialog;
		Dialog *_pPrevDialog;
	};

}


#endif