#ifndef IM_GUI_RENDERER_H
#define IM_GUI_RENDERER_H

#include "InputSystem.h"

namespace im
{
	enum TextAlign
	{
		ALIGN_LEFT = 0x00000000,
		ALIGN_CENTER = 0x00000001,
		ALIGN_RIGHT = 0x00000002,
		ALIGN_VCENTER = 0x00000004,
	};


	enum GfxCommandType
	{
		GFXCOMMAND_RECT,
		GFXCOMMAND_TRIANGLE,
		GFXCOMMAND_LINE,
		GFXCOMMAND_TEXT,
		GFXCOMMAND_SCISSOR,
	};

	struct GfxRect
	{
		int16 x, y, w, h;
	};

	struct GfxText
	{
		int16 x, y, align;
		const char* text;
	};

	struct GfxLine
	{
		int16 x0, y0, x1, y1;
	};

	struct GfxCommand
	{
		char type;
		char flags;
		char pad[2];
		D3DCOLOR color;
		union
		{
			GfxLine line;
			GfxRect rect;
			GfxText text;
		};
	};

	struct Window
	{
		int32 x;
		int32 y;
		int32 width;
		int32 height;

		int32 scroll;
		int32 top;
		bool32 opened;

		int32 widgetX;
		int32 widgetY;

		int32 internalWidgetID;
	};
	constexpr uint32 MAX_NUM_WINDOW = 10;

	struct State
	{
		State() :
			left(false), leftPressed(false), leftReleased(false),
			mx(-1), my(-1), scroll(0),
			active(0), hot(0), hotToBe(0), isHot(false), isActive(false), wentActive(false),
			dragX(0), dragY(0), dragOrig(0), widgetX(0), widgetY(0), widgetW(100),
			insideCurrentScroll(false), areaId(0), widgetId(0), charInput(0), keyboardFocusID(0)
		{
		}

		bool left, right, middle;
		bool leftPressed, leftReleased;
		bool rightPressed, rightReleased;
		bool middlePressed, middleReleased;
		int32 mx, my;
		int32 scroll;
		uint32 active;
		uint32 hot;
		uint32 hotToBe;
		bool isHot;
		bool isActive;
		bool wentActive;
		int32 dragX, dragY;
		float dragOrig;
		bool insideCurrentScroll;

		Window windows[im::MAX_NUM_WINDOW];
		uint32 currentWindowID;

		uint32 globalWidgetID;

		uint32 keyboardFocusID;

		uint8 charInput;
	};

	bool imMessageProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	constexpr uint32 GFXCOMMAND_QUEUE_SIZE = 5000;

	class GuiRenderer
	{
	public :
	
	public:
		GuiRenderer();
		~GuiRenderer();

		bool Init(IDirect3DDevice9 *pDevice, const char *fontPath, int32 clientWidth, int32 clientHeight);
		void Destroy();

		void Draw();

		static State gState;

		static GfxCommand gfxCommandQueue[GFXCOMMAND_QUEUE_SIZE];
		static unsigned gfxCommandQueueSize;

		ID3DXFont *GetFont() { return _fonts[0]; }


	private :
		void DrawRect(int32 x0, int32 y0, int32 x1, int32 y1, D3DCOLOR color);
		void DrawLine(int32 x0, int32 y0, int32 x1, int32 y1, D3DCOLOR color);
		void DrawFont(int32 id, int32 x, int32 y, D3DCOLOR color, const char *str, DWORD format);
		void DrawTriangle(int32 x0, int32 y0, int32 x1, int32 y1, int32 x2, int32 y2, D3DCOLOR color);

		IDirect3DDevice9 *_pDevice;
		IDirect3DStateBlock9 *_pStateBlock;

		int32 _clientWidth;
		int32 _clientHeight;

		D3DXMATRIXA16 _projection;
		D3DXMATRIXA16 _view;
		LPD3DXFONT _fonts[3];
	};

	struct UIVertex
	{
		D3DXVECTOR3 position;
		D3DCOLOR color;
		enum
		{
			FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE
		};
	};

	struct UVVertex
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 uv;
		D3DCOLOR color;

		enum
		{
			FVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_DIFFUSE
		};
	};

	inline bool AnyActive()
	{
		return GuiRenderer::gState.active != 0;
	}

	inline bool IsActive(uint32 id)
	{
		return GuiRenderer::gState.active == id;
	}

	inline bool IsHot(uint32 id)
	{
		return GuiRenderer::gState.hot == id;
	}

	inline bool InRect(int32 x, int32 y, int32 w, int32 h, bool checkScroll = true)
	{
		bool result = false;
		result = (GuiRenderer::gState.mx >= x) && (GuiRenderer::gState.mx <= x + w) && (GuiRenderer::gState.my >= y) && (GuiRenderer::gState.my <= y + h);
		return result;
	}

	inline void ClearInput()
	{
		GuiRenderer::gState.leftPressed = false;
		GuiRenderer::gState.leftReleased = false;
		GuiRenderer::gState.leftPressed = false;
		GuiRenderer::gState.leftReleased = false;
		GuiRenderer::gState.rightPressed = false;
		GuiRenderer::gState.rightReleased = false;
		GuiRenderer::gState.middlePressed = false;
		GuiRenderer::gState.middleReleased = false;

		GuiRenderer::gState.scroll = 0;
		GuiRenderer::gState.charInput = 0;
	}

	inline void ClearActive()
	{
		GuiRenderer::gState.active = 0;
		// mark all UI for this frame as processed
		ClearInput();
	}

	inline void SetActive(uint32 id)
	{
		GuiRenderer::gState.active = id;
		GuiRenderer::gState.wentActive = true;
	}

	inline void SetHot(uint32 id)
	{
		GuiRenderer::gState.hotToBe = id;
	}


	static bool ButtonLogic(uint32 id, bool over);
	static bool KeyLogic(uint32 id);

	static void UpdateInput(Mouse &refMouse, uint8 inputChar);

	void BeginFrame(Mouse &refMouse, uint8 inputChar);
	void EndFrame();


	bool BeginWindow(const char* name, int32 x, int32 y, int32 w, int32 h, int32* scroll);
	void EndWindow();
	
	bool BeginScrollArea(const char* name, int32 x, int32 y, int32 w, int32 h, int32* scroll);
	void EndScrollArea();

	void Indent();
	void Unindent();
	void imguiSeparator();
	void SeparatorLine();

	bool Scissor(int32 x, int32 y, int32 width, int32 height, bool flag);

	bool Button(const char* text, int32 width = 0, bool enabled = true);
	bool Item(const char* text, int32 width = 0, bool enabled = true);
	bool Check(const char* text, bool checked, int32 width = 0, bool enabled = true);
	bool Collapse(const char* text, const char* subtext, bool checked, int32 width = 0, bool enabled = true);
	void Label(const char* text);
	void Value(const char* text);
	bool Slider(const char* text, float* val, float vMin, float vMax, float vInc, 
		int32 width = 0, bool enabled = true);
	bool Edit(char *text, int32 width = 0, bool enabled = true);


	//void DrawFontCommand(int32 x, int32 y, int32 align, const char* text, D3DCOLOR color);
	//void DrawLineCommand(float x0, float y0, float x1, float y1, float r, D3DCOLOR color);
	//void DrawRectCommand(float x, float y, float w, float h, D3DCOLOR color);

	class EditorWindow
	{
	public :

	private :

	};

}

#endif