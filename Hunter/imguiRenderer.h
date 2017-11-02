#ifndef IM_GUI_RENDERER_H
#define IM_GUI_RENDERER_H


namespace im
{
	// Pull render interface.
	enum GfxCommandType
	{
		GFXCOMMAND_RECT,
		GFXCOMMAND_TRIANGLE,
		GFXCOMMAND_LINE,
		GFXCOMMAND_TEXT,
		GFXCOMMAND_STENCIL,
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
		uint32 color;
		union
		{
			GfxLine line;
			GfxRect rect;
			GfxText text;
		};
	};

	constexpr uint32 GFXCOMMAND_QUEUE_SIZE = 5000;

	struct Vertex
	{
		D3DXVECTOR3 position;
		uint32 color;
		enum
		{
			FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE,
		};
	};

	struct UVVertex
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 uv;
		uint32 color;

		enum
		{
			FVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_DIFFUSE,
		};
	};

	struct State
	{
		State() :
			left(false), leftPressed(false), leftReleased(false),
			mx(-1), my(-1), scroll(0),
			active(0), hot(0), hotToBe(0), isHot(false), isActive(false), wentActive(false),
			dragX(0), dragY(0), dragOrig(0), widgetX(0), widgetY(0), widgetW(100),
			insideCurrentScroll(false), areaId(0), widgetId(0)
		{
		}

		bool left;
		bool leftPressed, leftReleased;
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
		int32 widgetX, widgetY, widgetW;
		bool insideCurrentScroll;

		uint32 areaId;
		uint32 widgetId;
	};
	static State gState;

	inline bool AnyActive()
	{
		return gState.active != 0;
	}

	inline bool IsActive(uint32 id)
	{
		return gState.active == id;
	}

	inline bool IsHot(uint32 id)
	{
		return gState.hot == id;
	}

	inline bool InRect(int32 x, int32 y, int32 w, int32 h, bool checkScroll = true)
	{
		return (!checkScroll || gState.insideCurrentScroll) && gState.mx >= x && gState.mx <= x + w && gState.my >= y && gState.my <= y + h;
	}

	inline void ClearInput()
	{
		gState.leftPressed = false;
		gState.leftReleased = false;
		gState.scroll = 0;
	}

	inline void ClearActive()
	{
		gState.active = 0;
		// mark all UI for this frame as processed
		ClearInput();
	}

	inline void SetActive(uint32 id)
	{
		gState.active = id;
		gState.wentActive = true;
	}

	inline void SetHot(uint32 id)
	{
		gState.hotToBe = id;
	}


	static bool ButtonLogic(uint32 id, bool over)
	{
		bool res = false;
		// process down
		if (!AnyActive())
		{
			if (over)
				SetHot(id);
			if (IsHot(id) && gState.leftPressed)
				SetActive(id);
		}

		// if button is active, then react on left up
		if (IsActive(id))
		{
			gState.isActive = true;
			if (over)
				SetHot(id);
			if (gState.leftReleased)
			{
				if (IsHot(id))
					res = true;
				ClearActive();
			}
		}

		if (IsHot(id))
			gState.isHot = true;

		return res;
	}

	static void UpdateInput(int32 mx, int32 my, uint8 mouseButton, int32 scroll)
	{
		bool left = (mouseButton & MOUSE_BUTTON_LEFT) != 0;

		gState.mx = mx;
		gState.my = my;
		gState.leftPressed = !gState.left && left;
		gState.leftReleased = gState.left && !left;
		gState.left = left;

		gState.scroll = scroll;
	}


	enum MouseButton
	{
		MOUSE_BUTTON_LEFT = 0x01,
		MOUSE_BUTTON_RIGHT = 0x02,
	};

	enum TextAlign
	{
		ALIGN_LEFT,
		ALIGN_CENTER,
		ALIGN_RIGHT,
	};

	inline unsigned int RGBA(uint8 r, uint8 g, uint8 b, uint8 a = 255)
	{
		return (r) | (g << 8) | (b << 16) | (a << 24);
	}

	void BeginFrame(int32 mx, int32 my, uint8 mbut, int32 scroll);
	void EndFrame();

	bool BeginScrollArea(const char* name, int32 x, int32 y, int32 w, int32 h, int32* scroll);
	void EndScrollArea();

	void Indent();
	void Unindent();
	void imguiSeparator();
	void SeparatorLine();

	bool Button(const char* text, bool enabled = true);
	bool Item(const char* text, bool enabled = true);
	bool Check(const char* text, bool checked, bool enabled = true);
	bool Collapse(const char* text, const char* subtext, bool checked, bool enabled = true);
	void Label(const char* text);
	void Value(const char* text);
	bool Slider(const char* text, float* val, float vmin, float vmax, float vinc, bool enabled = true);

	void DrawFont(int32 x, int32 y, int32 align, const char* text, uint32 color);
	void DrawLine(float x0, float y0, float x1, float y1, float r, uint32 color);
	void DrawRoundedRect(float x, float y, float w, float h, float r, uint32 color);
	void DrawRect(float x, float y, float w, float h, uint32 color);

	class GuiRenderer
	{
	public :
		GuiRenderer();
		~GuiRenderer();

		bool Init(IDirect3DDevice9 *pDevice, const char *fontPath, int32 clientWidth, int32 clientHeight);
		void Destroy();

		void Draw();

		static GfxCommand gfxCommandQueue[GFXCOMMAND_QUEUE_SIZE];
		static unsigned gfxCommandQueueSize;

	private :
		void DrawRect(int32 x0, int32 y0, int32 x1, int32 y1, uint32 color);
		void DrawLine(int32 x0, int32 y0, int32 x1, int32 y1, uint32 color);
		void DrawFont(int32 id, int32 x, int32 y, uint32 color, const char *str, DWORD format);

		IDirect3DDevice9 *_pDevice;
		IDirect3DStateBlock9 *_pStateBlock;

		int32 _clientWidth;
		int32 _clientHeight;

		D3DXMATRIXA16 _projection;
		D3DXMATRIXA16 _view;
		LPD3DXFONT _fonts[3];

	};
	//bool GuiInit(const char* fontpath);
	//void GuiDestroy();
	//void GuiDraw(int32 width, int32 height);
}

#endif