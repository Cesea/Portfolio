#ifndef IM_GUI_RENDERER_H
#define IM_GUI_RENDERER_H

#include "InputManager.h"
#include "SingletonBase.h"

namespace im
{
	enum TextAlign
	{
		ALIGN_LEFT = 0x00000000,
		ALIGN_CENTER = 0x00000001,
		ALIGN_RIGHT = 0x00000002,
		ALIGN_VCENTER = 0x00000004,
	};

	struct WindowPos
	{
		int32 x;
		int32 y;
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
		float z;
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
		static int32 CAPTION_HEIGHT;
		static int32 CAPTION_TEXT_OFFSET;
		static int32 BUTTON_GAP;
		static int32 BUTTON_SIZE;
		static int32 MINIMIZE_BUTTON_RIGHT_OFFSET;
		static int32 CLOSE_BUTTON_RIGHT_OFFSET;
		static int32 WIDGET_YOFFSET_VALUE;
		static int32 WIDGET_XOFFSET_VALUE;
		static int32 WIDGET_INDENT_VALUE;
		static int32 SPACING_VALUE;

		static int32 AREA_HEADER;

		static const D3DCOLOR DEFAULT_COLOR[4];

		int32 x;
		int32 y;
		int32 width;
		int32 height;

		float z{0.0f};

		int32 top;
		bool32 opened{true};
		bool32 mouseInside{ false };

		bool32 wentDrag{ false };
		bool32 canDrag{false};

		int32 prevDragX{-1};
		int32 prevDragY{-1};

		int32 widgetX{0};
		int32 widgetY{0};

		int32 internalWidgetID;
	};

	struct Button
	{
		static int32 BUTTON_WIDTH;
		static int32 BUTTON_HEIGHT;
	};

	struct Font
	{
		static int32 FONT_HEIGHT;
		static const D3DCOLOR DEFAULT_FONT_COLOR[4];
	};

	struct Slider
	{
		static int32 SLIDER_WIDTH;
		static int32 SLIDER_HEIGHT;
		static int32 MARKER_WIDTH;
	};

	struct Edit
	{
		static int32 EDIT_WIDTH;
		static int32 EDIT_MAX_CHAR;
		static int32 MARKET_WIDTH;
	};

	struct Check
	{
		static int32 CHECK_SIZE;
	};

	constexpr uint32 MAX_NUM_WINDOW = 10;

	struct State
	{
		State() :
			left(false), leftPressed(false), leftReleased(false),
			mx(-1), my(-1), scroll(0),
			active(0), hot(0), hotToBe(0), isHot(false), isActive(false), wentActive(false),
			dragX(0), dragY(0), dragOrig(0), 
			insideCurrentScroll(false), vkCode(0), keyboardFocusID(0),
			globalWidgetID(0), currentWindowID(0)
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

		uint8 vkCode;
		bool32 shiftDown;
	};

	bool imMessageProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	constexpr uint32 GFXCOMMAND_QUEUE_SIZE = 1000;

	class GuiRenderer : public SingletonBase<GuiRenderer>
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
		GuiRenderer::gState.vkCode = 0;
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
	static bool FrameLogic(uint32 windowID, bool over);
	static bool KeyLogic(uint32 id);

	static void UpdateInput(Mouse &refMouse, uint8 vkCode, bool32 shiftDown);

	void BeginFrame(Mouse &refMouse, uint8 inputChar, bool32 shiftDown);
	void EndFrame();


	bool BeginScrollArea(const char* name, WindowPos &pos, int32 w, int32 h, int32* scroll);
	void EndScrollArea();

	void Indent();
	void Unindent();
	void imguiSeparator();
	void SeparatorLine();

	bool Scissor(int32 x, int32 y, int32 width, int32 height, bool flag);

	bool Button(const char* text, int32 width = 0, int32 xPos = 0, bool enabled = true);
	//bool Item(const char* text, int32 width = 0, bool enabled = true);
	bool Check(const char* text, bool checked, int32 width = 0, int32 xPos = 0, bool enabled = true);
	bool Collapse(const char* text, bool checked, int32 width = 0, int32 xPos = 0, bool enabled = true);
	void Label(const char* text);
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