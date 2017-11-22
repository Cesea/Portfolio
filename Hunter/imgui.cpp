#include "stdafx.h"
#include "imgui.h"

static const unsigned TEXT_POOL_SIZE = 8000;
static char gTextPool[TEXT_POOL_SIZE];
static uint32 gTextPoolSize = 0;
static const char *AllocText(const char *text) 
{
	uint32 len = strlen(text) + 1;
	if (gTextPoolSize + len >= TEXT_POOL_SIZE)
	{
		return 0;
	}
	char* dst = &gTextPool[gTextPoolSize];
	memcpy(dst, text, len);
	gTextPoolSize += len;
	return dst;
}

static const unsigned IMGUI_COMMAND_QUEUE_SIZE = 5000;
static ImguiCommand gIMCommandQueue[IMGUI_COMMAND_QUEUE_SIZE];
static unsigned gIMCommandQueueSize = 0;

static void ResetIMCommandQueue()
{
	gIMCommandQueueSize = 0;
	gTextPoolSize = 0;
}

static void AddCommandScissor(int32 x, int32 y, int32 w, int32 h) 
{
	if (gIMCommandQueueSize >= IMGUI_COMMAND_QUEUE_SIZE)
		return;
	ImguiCommand& cmd = gIMCommandQueue[gIMCommandQueueSize++];
	cmd._type = ImguiCommandType::eCommandScissor;
	cmd._flags = (x < 0) ? (0) : (1);      // On/off flag.
	cmd._color = 0;
	cmd._rect._x = (int16)x;
	cmd._rect._y = (int16)y;
	cmd._rect._w = (int16)w;
	cmd._rect._h = (int16)h;
}

static void AddCommandRect(float x, float y, float w, float h, uint32 color) 
{
	if (gIMCommandQueueSize >= IMGUI_COMMAND_QUEUE_SIZE)
	{
		return;
	}
	ImguiCommand& cmd = gIMCommandQueue[gIMCommandQueueSize++];
	cmd._type = ImguiCommandType::eCommandRect;
	cmd._flags = 0;
	cmd._color = color;
	cmd._rect._x = (int16)(x);
	cmd._rect._y = (int16)(y);
	cmd._rect._w = (int16)(w);
	cmd._rect._h = (int16)(h);
	cmd._rect._r = 0;
}

static void AddCommandLine(float x0, float y0, float x1, float y1, float r, uint32 color) 
{
	if (gIMCommandQueueSize >= IMGUI_COMMAND_QUEUE_SIZE)
	{
		return;
	}
	ImguiCommand& cmd = gIMCommandQueue[gIMCommandQueueSize++];
	cmd._type = ImguiCommandType::eCommandLine;
	cmd._flags = 0;
	cmd._color = color;
	cmd._line._x0 = (int16)(x0);
	cmd._line._y0 = (int16)(y0);
	cmd._line._x1 = (int16)(x1);
	cmd._line._y1 = (int16)(y1);
	cmd._line._r = (int16)(r);
}

static void AddCommandTriangle(int32 x, int32 y, int32 w, int32 h, int32 flags, uint32 color) 
{
	if (gIMCommandQueueSize >= IMGUI_COMMAND_QUEUE_SIZE)
	{
		return;
	}
	ImguiCommand& cmd = gIMCommandQueue[gIMCommandQueueSize++];
	cmd._type = ImguiCommandType::eCommandTriangle;
	cmd._flags = (char)flags;
	cmd._color = color;
	cmd._rect._x = (int16)(x);
	cmd._rect._y = (int16)(y);
	cmd._rect._w = (int16)(w);
	cmd._rect._h = (int16)(h);
}

static void AddCommandText(int32 x, int32 y, int32 align, const char* text, uint32 color) 
{
	if (gIMCommandQueueSize >= IMGUI_COMMAND_QUEUE_SIZE)
	{
		return;
	}
	ImguiCommand& cmd = gIMCommandQueue[gIMCommandQueueSize++];
	cmd._type = ImguiCommandType::eCommandText;
	cmd._flags = 0;
	cmd._color = color;
	cmd._text._x = (short)x;
	cmd._text._y = (short)y;
	cmd._text._align = (short)align;
	cmd._text._text = AllocText(text);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct GuiState 
{

	GuiState() :
		_left(false), _leftPressed(false), _leftReleased(false),
		_mx(-1), _my(-1), _scroll(0),
		_active(0), _hot(0), _hotToBe(0), IsHot(false), IsActive(false), _wentActive(false),
		_dragX(0), _dragY(0), _dragOrig(0), _widgetX(0), _widgetY(0), _widgetW(100),
		_insideCurrentScroll(false), _areaID(0), _widgetID(0), _keyboardFocus(0), _vkCode(0)
	{
	}

	bool _left;
	bool _leftPressed, _leftReleased;
	int32 _mx, _my;
	int32 _scroll;
	uint32 _active;
	uint32 _hot;
	uint32 _hotToBe;
	uint32 _keyboardFocus;
	bool IsHot;
	bool IsActive;
	bool _wentActive;
	int32 _dragX, _dragY;
	float _dragOrig;
	int32 _widgetX, _widgetY, _widgetW;
	bool _insideCurrentScroll;

	uint32 _areaID;
	uint32 _widgetID;

	uint32 _vkCode;
};

static GuiState gState;

inline bool AnyActive() 
{
	return gState._active != 0;
}

inline bool IsActive(uint32 id) 
{
	return gState._active == id;
}

inline bool IsHot(uint32 id) 
{
	return gState._hot == id;
}

inline bool InRect(int32 x, int32 y, int32 w, int32 h, bool checkScroll = true) {
	return /*(!checkScroll || gState._insideCurrentScroll) && */
		(gState._mx >= x) && 
		(gState._mx <= x + w) && 
		(gState._my >= y) && 
		(gState._my <= y + h);
}

inline void ClearInput() 
{
	gState._leftPressed = false;
	gState._leftReleased = false;
	gState._scroll = 0;
	gState._vkCode = 0;
}

inline void ClearActive() {
	gState._active = 0;
	ClearInput();
}

inline void SetActive(uint32 id) 
{
	gState._active = id;
	gState._wentActive = true;
}

inline void SetHot(uint32 id) 
{
	gState._hotToBe = id;
}


static bool ButtonLogic(uint32 id, bool over) 
{
	bool result = false;
	// Process down
	if (!AnyActive()) 
	{
		if (over)
		{
			SetHot(id);
		}
		if (IsHot(id) && gState._leftPressed)
		{
			SetActive(id);
		}
	}
	// If button is active, then react on left up
	if (IsActive(id)) 
	{
		gState.IsActive = true;
		if (over)
		{
			SetHot(id);
		}
		if (gState._leftReleased) 
		{
			if (IsHot(id))
			{
				result = true;
			}
			ClearActive();
		}
	}

	if (IsHot(id))
	{
		gState.IsHot = true;
	}
	return result;
}

static bool KeyLogic(uint32 id)
{
	bool result = false;
	result = (gState._keyboardFocus == id);
	return result;
}

static void UpdateInput(int32 mx, int32 my, unsigned char mbut, int32 scroll, uint32 keyCode) 
{
	bool left = (mbut & ImguiMouseButton::eImguiMButtonLeft) != 0;

	gState._mx = mx;
	gState._my = my;
	gState._leftPressed = !gState._left && left;
	gState._leftReleased = gState._left && !left;
	gState._left = left;

	gState._scroll = scroll;
	gState._vkCode = keyCode;
}


void ImguiBeginFrame(int32 mx, int32 my, uint8 mbut, int32 scroll, uint32 keyCode)
{
	UpdateInput(mx, my, mbut, scroll, keyCode);

	gState._hot = gState._hotToBe;
	gState._hotToBe = 0;

	gState._wentActive = false;
	gState.IsActive = false;
	gState.IsHot = false;

	gState._widgetX = 0;
	gState._widgetY = 0;
	gState._widgetW = 0;

	gState._areaID = 1;
	gState._widgetID = 1;

	ResetIMCommandQueue();
}

void ImguiEndFrame()
{
	ClearInput();
}

static const int BUTTON_HEIGHT = 20;
static const int SLIDER_HEIGHT = 20;
static const int SLIDER_MARKER_WIDTH = 10;
static const int CHECK_SIZE = 8;
static const int DEFAULT_SPACING = 4;
static const int TEXT_HEIGHT = 8;
static const int SCROLL_AREA_PADDING = 6;
static const int INDENT_SIZE = 16;
static const int AREA_HEADER = 28;

static int g_scrollTop = 0;
static int g_scrollBottom = 0;
static int g_scrollRight = 0;
static int g_scrollAreaTop = 0;
static int* g_scrollVal = 0;
static int g_focusTop = 0;
static int g_focusBottom = 0;
static unsigned int g_scrollId = 0;
static bool g_insideScrollArea = false;



bool ImguiBeginScrollArea(const char * name, int32 x, int32 y, int32 w, int32 h, int32 * pOutScroll)
{
	gState._areaID++;
	gState._widgetID = 0;
	g_scrollId = (gState._areaID << 16) | gState._widgetID;

	gState._widgetX = x + SCROLL_AREA_PADDING;
	gState._widgetY = y + AREA_HEADER + (*pOutScroll);
	gState._widgetW = w - SCROLL_AREA_PADDING * 4;
	g_scrollTop = y + AREA_HEADER;
	g_scrollBottom = y + SCROLL_AREA_PADDING + h;
	g_scrollRight = x + w - SCROLL_AREA_PADDING * 3;
	g_scrollVal = pOutScroll;

	g_scrollAreaTop = gState._widgetY;

	g_focusTop = y - AREA_HEADER;
	g_focusBottom = y - AREA_HEADER + h;

	g_insideScrollArea = InRect(x, y, w, h, false);
	gState._insideCurrentScroll = g_insideScrollArea;

	AddCommandRect((float)x, (float)y, (float)w, (float)h, ImguiRGBA(20, 20, 20, 100));

	AddCommandText(x + AREA_HEADER / 2, y + h - AREA_HEADER / 2 - TEXT_HEIGHT / 2, ImguiTextAlign::eImguiAlignLeft, name, ImguiRGBA(255, 255, 255, 128));

	AddCommandScissor(x + SCROLL_AREA_PADDING, y + SCROLL_AREA_PADDING, w - SCROLL_AREA_PADDING * 4, h - AREA_HEADER - SCROLL_AREA_PADDING);

	return g_insideScrollArea;
}

void ImguiEndScrollArea()
{
	// Disable scissoring.
	AddCommandScissor(-1, -1, -1, -1);

	// Draw _scroll bar
	//int x = g_scrollRight + SCROLL_AREA_PADDING / 2;
	//int y = g_scrollTop;
	//int w = SCROLL_AREA_PADDING * 2;
	//int h = g_scrollBottom - g_scrollTop - SCROLL_AREA_PADDING * 3;

	//int stop = g_scrollAreaTop;
	//int sbot = gState._widgetY;
	//int sh = sbot - stop; // The scrollable area height.

	//float barHeight = (float)sh / (float)h;

	//if (barHeight > 1.0) 
	//{
	//	float barY = (float)(y - stop) / (float)sh;
	//	if (barY < 0) barY = 0;
	//	if (barY > 1) barY = 1;

	//	// Handle _scroll bar logic.
	//	unsigned int hid = g_scrollId;
	//	int hx = x;
	//	int hy = y + (int)(barY*h);
	//	int hw = w;
	//	int hh = (int)(barHeight*h);

	//	const int range = h - (hh - 1);
	//	bool over = InRect(hx, hy, hw, hh);
	//	ButtonLogic(hid, over);
	//	if (IsActive(hid)) 
	//	{
	//		float u = (float)(hy - y) / (float)range;
	//		if (gState._wentActive) 
	//		{
	//			gState._dragY = gState._my;
	//			gState._dragOrig = u;
	//		}
	//		if (gState._dragY != gState._my) 
	//		{
	//			u = gState._dragOrig + (gState._my - gState._dragY) / (float)range;
	//			if (u < 0) u = 0;
	//			if (u > 1) u = 1;
	//			*g_scrollVal = (int)((1 - u) * (h - sh));
	//		}
	//	}

	//	// BG
	//	AddCommandRect((float)x, (float)y, (float)w, (float)h, ImguiRGBA(0, 200, 200, 196));
	//	// Bar
	//	if (IsActive(hid))
	//	{
	//		AddCommandRect((float)hx, (float)hy, (float)hw, (float)hh, ImguiRGBA(255, 196, 0, 196));
	//	}
	//	else
	//	{
	//		AddCommandRect((float)hx, (float)hy, (float)hw, (float)hh, 
	//			IsHot(hid) ? ImguiRGBA(255, 196, 0, 96) : ImguiRGBA(255, 255, 255, 64));
	//	}

	//	// Handle mouse scrolling.
	//	if (g_insideScrollArea) 
	//	{ // && !anyActive())
	//		if (gState._scroll != 0) 
	//		{
	//			*g_scrollVal += gState._scroll;

	//			if (*g_scrollVal < 0)
	//			{
	//				*g_scrollVal = 0;
	//			}
	//			if (*g_scrollVal > (h - sh))
	//			{
	//				*g_scrollVal = (h - sh);
	//			}
	//		}
	//	}
	//}
	gState._insideCurrentScroll = false;
}

void ImguiIndent()
{
	gState._widgetX += INDENT_SIZE;
	gState._widgetW -= INDENT_SIZE;
}

void ImguiUnindent()
{
	gState._widgetX -= INDENT_SIZE;
	gState._widgetW += INDENT_SIZE;
}

void ImguiSeparator()
{
	gState._widgetY += DEFAULT_SPACING * 3;
}

void ImguiSeparatorLine()
{
	int x = gState._widgetX;
	int y = gState._widgetY - DEFAULT_SPACING * 2;
	int w = gState._widgetW;
	int h = 1;
	gState._widgetY += DEFAULT_SPACING * 4;

	AddCommandRect((float)x, (float)y, (float)w, (float)h, ImguiRGBA(255, 255, 255, 32));
}

bool ImguiButton(const char * text, bool enabled)
{
	gState._widgetID++;
	unsigned int id = (gState._areaID << 16) | gState._widgetID;

	int x = gState._widgetX;
	int y = gState._widgetY;
	int w = 80;
	int h = BUTTON_HEIGHT;
	gState._widgetY += BUTTON_HEIGHT + DEFAULT_SPACING;

	bool over = enabled && InRect(x, y, w, h);
	bool res = ButtonLogic(id, over);

	if (over)
	{
		int a = 0;
	}

	AddCommandRect((float)x, (float)y, (float)w, (float)h, ImguiRGBA(255, 200, 200, IsActive(id) ? 230 : 150));
	if (enabled)
	{
		AddCommandText(x + BUTTON_HEIGHT / 2, y + BUTTON_HEIGHT / 2 - TEXT_HEIGHT / 2, 
			ImguiTextAlign::eImguiAlignLeft, text, IsHot(id) ? ImguiRGBA(255, 196, 0, 255) : ImguiRGBA(255, 255, 255, 200));
	}
	else
	{
		AddCommandText(x + BUTTON_HEIGHT / 2, y + BUTTON_HEIGHT / 2 - TEXT_HEIGHT / 2, 
			ImguiTextAlign::eImguiAlignLeft, text, ImguiRGBA(128, 128, 128, 200));
	}


	return res;
}

bool ImguiItem(const char * text, bool enabled)
{
	gState._widgetID++;
	unsigned int id = (gState._areaID << 16) | gState._widgetID;

	int x = gState._widgetX;
	int y = gState._widgetY - BUTTON_HEIGHT;
	int w = gState._widgetW;
	int h = BUTTON_HEIGHT;
	gState._widgetY += BUTTON_HEIGHT + DEFAULT_SPACING;

	bool over = enabled && InRect(x, y, w, h);
	bool res = ButtonLogic(id, over);

	if (IsHot(id))
	{
		AddCommandRect((float)x, (float)y, (float)w, (float)h, ImguiRGBA(255, 196, 0, IsActive(id) ? 196 : 96));
	}

	if (enabled)
	{
		AddCommandText(x + BUTTON_HEIGHT / 2, y + BUTTON_HEIGHT / 2 - TEXT_HEIGHT / 2, 
			ImguiTextAlign::eImguiAlignLeft, text, ImguiRGBA(255, 255, 255, 200));
	}
	else
	{
		AddCommandText(x + BUTTON_HEIGHT / 2, y + BUTTON_HEIGHT / 2 - TEXT_HEIGHT / 2, 
			ImguiTextAlign::eImguiAlignLeft, text, ImguiRGBA(128, 128, 128, 200));
	}
	return res;
}

bool ImguiCheck(const char * text, bool checked, bool enabled)
{
	gState._widgetID++;
	unsigned int id = (gState._areaID << 16) | gState._widgetID;

	int x = gState._widgetX;
	int y = gState._widgetY - BUTTON_HEIGHT;
	int w = gState._widgetW;
	int h = BUTTON_HEIGHT;
	gState._widgetY += BUTTON_HEIGHT + DEFAULT_SPACING;

	bool over = enabled && InRect(x, y, w, h);
	bool res = ButtonLogic(id, over);

	const int cx = x + BUTTON_HEIGHT / 2 - CHECK_SIZE / 2;
	const int cy = y + BUTTON_HEIGHT / 2 - CHECK_SIZE / 2;
	AddCommandRect((float)cx - 3, (float)cy - 3, (float)CHECK_SIZE + 6, (float)CHECK_SIZE + 6, ImguiRGBA(128, 128, 128, IsActive(id) ? 196 : 96));
	if (checked) 
	{
		if (enabled)
		{
			AddCommandRect((float)cx, (float)cy, (float)CHECK_SIZE, (float)CHECK_SIZE, ImguiRGBA(255, 255, 255, IsActive(id) ? 255 : 200));
		}
		else
		{
			AddCommandRect((float)cx, (float)cy, (float)CHECK_SIZE, (float)CHECK_SIZE, ImguiRGBA(128, 128, 128, 200));
		}
	}

	if (enabled)
	{
		AddCommandText(x + BUTTON_HEIGHT, y + BUTTON_HEIGHT / 2 - TEXT_HEIGHT / 2, ImguiTextAlign::eImguiAlignLeft, text, 
			IsHot(id) ? ImguiRGBA(255, 196, 0, 255) : ImguiRGBA(255, 255, 255, 200));
	}
	else
	{
		AddCommandText(x + BUTTON_HEIGHT, y + BUTTON_HEIGHT / 2 - TEXT_HEIGHT / 2, ImguiTextAlign::eImguiAlignLeft, text, ImguiRGBA(128, 128, 128, 200));
	}
	return res;
}

bool ImguiCollapse(const char * text, const char * subtext, bool checked, bool enabled)
{
	gState._widgetID++;
	unsigned int id = (gState._areaID << 16) | gState._widgetID;

	int x = gState._widgetX;
	int y = gState._widgetY;
	int w = gState._widgetW;
	int h = BUTTON_HEIGHT;
	gState._widgetY += BUTTON_HEIGHT; // + DEFAULT_SPACING;

	const int cx = x + BUTTON_HEIGHT / 2 - CHECK_SIZE / 2;
	const int cy = y + BUTTON_HEIGHT / 2 - CHECK_SIZE / 2;

	bool over = enabled && InRect(x, y, w, h);
	bool res = ButtonLogic(id, over);

	if (checked)
	{
		AddCommandTriangle(cx, cy, CHECK_SIZE, CHECK_SIZE, 2, ImguiRGBA(255, 255, 255, IsActive(id) ? 255 : 200));
	}
	else
	{
		AddCommandTriangle(cx, cy, CHECK_SIZE, CHECK_SIZE, 1, ImguiRGBA(255, 255, 255, IsActive(id) ? 255 : 200));
	}

	if (enabled)
	{
		AddCommandText(x + BUTTON_HEIGHT, y + BUTTON_HEIGHT / 2 - TEXT_HEIGHT / 2, ImguiTextAlign::eImguiAlignLeft, text, 
			IsHot(id) ? ImguiRGBA(255, 196, 0, 255) : ImguiRGBA(255, 255, 255, 200));
	}
	else
	{
		AddCommandText(x + BUTTON_HEIGHT, y + BUTTON_HEIGHT / 2 - TEXT_HEIGHT / 2, ImguiTextAlign::eImguiAlignLeft, text, ImguiRGBA(128, 128, 128, 200));
	}

	if (subtext)
	{
		AddCommandText(x + w - BUTTON_HEIGHT / 2, y + BUTTON_HEIGHT / 2 - TEXT_HEIGHT / 2, 
			ImguiTextAlign::eImguiAlignLeft, subtext, ImguiRGBA(255, 255, 255, 128));
	}

	return res;
}

void ImguiLabel(const char * text)
{
	int x = gState._widgetX;
	int y = gState._widgetY;
	gState._widgetY += BUTTON_HEIGHT;
	AddCommandText(x, y + BUTTON_HEIGHT / 2 - TEXT_HEIGHT / 2, ImguiTextAlign::eImguiAlignLeft, text, ImguiRGBA(255, 255, 255, 255));
}

void ImguiValue(const char * text)
{
	const int x = gState._widgetX;
	const int y = gState._widgetY;
	const int w = gState._widgetW;
	gState._widgetY += BUTTON_HEIGHT;

	AddCommandText(x + w - BUTTON_HEIGHT / 2, y + BUTTON_HEIGHT / 2 - TEXT_HEIGHT / 2, 
		ImguiTextAlign::eImguiAlignLeft, text, ImguiRGBA(255, 255, 255, 200));
}

bool ImguiSlider(const char * text, float * val, float vmin, float vmax, float vinc, bool enabled)
{
	gState._widgetID++;
	unsigned int id = (gState._areaID << 16) | gState._widgetID;

	int x = gState._widgetX;
	int y = gState._widgetY;
	int w = gState._widgetW;
	int h = SLIDER_HEIGHT;
	gState._widgetY += SLIDER_HEIGHT + DEFAULT_SPACING;

	AddCommandRect((float)x, (float)y, (float)w, (float)h, ImguiRGBA(0, 0, 0, 128));

	const int range = w - SLIDER_MARKER_WIDTH;

	float u = (*val - vmin) / (vmax - vmin);
	if (u < 0) u = 0;
	if (u > 1) u = 1;
	int m = (int)(u * range);

	bool over = enabled && InRect(x + m, y, SLIDER_MARKER_WIDTH, SLIDER_HEIGHT);
	bool res = ButtonLogic(id, over);
	bool valChanged = false;

	if (IsActive(id)) 
	{
		if (gState._wentActive) 
		{
			gState._dragX = gState._mx;
			gState._dragOrig = u;
		}
		if (gState._dragX != gState._mx) 
		{
			u = gState._dragOrig + (float)(gState._mx - gState._dragX) / (float)range;
			if (u < 0) u = 0;
			if (u > 1) u = 1;
			*val = vmin + u*(vmax - vmin);
			*val = floorf(*val / vinc + 0.5f)*vinc; // Snap to vinc
			m = (int)(u * range);
			valChanged = true;
		}
	}

	if (IsActive(id))
	{
		AddCommandRect((float)(x + m), (float)y, (float)SLIDER_MARKER_WIDTH, (float)SLIDER_HEIGHT, ImguiRGBA(255, 255, 255, 255));
	}
	else
	{
		AddCommandRect((float)(x + m), (float)y, (float)SLIDER_MARKER_WIDTH, (float)SLIDER_HEIGHT,
			IsHot(id) ? ImguiRGBA(255, 196, 0, 128) : ImguiRGBA(255, 255, 255, 64));
	}


	// TODO: fix this, take a look at 'nicenum'.
	int digits = (int)(ceilf(log10f(vinc)));
	char fmt[16];
	snprintf(fmt, 16, "%%.%df", digits >= 0 ? 0 : -digits);
	char msg[128];
	snprintf(msg, 128, fmt, *val);

	if (enabled) 
	{
		AddCommandText(x + SLIDER_HEIGHT / 2, y + SLIDER_HEIGHT / 2 - TEXT_HEIGHT / 2, ImguiTextAlign::eImguiAlignLeft, text, 
			IsHot(id) ? ImguiRGBA(255, 196, 0, 255) : ImguiRGBA(255, 255, 255, 200));
		AddCommandText(x + w - SLIDER_HEIGHT / 2, y + SLIDER_HEIGHT / 2 - TEXT_HEIGHT / 2, ImguiTextAlign::eImguiAlignLeft, msg, 
			IsHot(id) ? ImguiRGBA(255, 196, 0, 255) : ImguiRGBA(255, 255, 255, 200));
	}
	else 
	{
		AddCommandText(x + SLIDER_HEIGHT / 2, y + SLIDER_HEIGHT / 2 - TEXT_HEIGHT / 2, 
			ImguiTextAlign::eImguiAlignLeft, text, ImguiRGBA(128, 128, 128, 200));
		AddCommandText(x + w - SLIDER_HEIGHT / 2, y + SLIDER_HEIGHT / 2 - TEXT_HEIGHT / 2, 
			ImguiTextAlign::eImguiAlignLeft, msg, ImguiRGBA(128, 128, 128, 200));
	}

	return res || valChanged;
}

bool imguiEdit(char * text, int32 width, bool enable)
{
	gState._widgetID++;
	unsigned int id = (gState._areaID << 16) | gState._widgetID;

	int x = gState._widgetX;
	int y = gState._widgetY;
	int w = gState._widgetW;
	int h = BUTTON_HEIGHT;
	gState._widgetY += BUTTON_HEIGHT; // + DEFAULT_SPACING;

	bool over = enable && InRect(x, y, w, h);
	bool mouseResult = ButtonLogic(id, over);
	bool keyResult = KeyLogic(id);

	if (mouseResult)
	{
		gState._keyboardFocus = id;
	}

	if (gState._vkCode && keyResult)
	{
		int32 index = strlen(text);
		if (gState._vkCode == VK_BACK)
		{
			if (index > 0)
			{
				text[index - 1] = '\0';
			}
		}
		else
		{
			char mappedChar = MapVirtualKey(gState._vkCode, MAPVK_VK_TO_CHAR);
			if (IsLetter(mappedChar))
			{
				//if (!gState.shiftDown)
				//{
				//	mappedChar += 32;
				//}
			}
			text[index] = mappedChar;
		}
	}

	AddCommandRect((float)x, (float)y, (float)w, (float)h,
		IsActive(id) ? ImguiRGBA(255, 196, 0, 255) : ImguiRGBA(255, 255, 255, 200));

	if (enable)
	{
		AddCommandText(x + 5, y + 2,
			ImguiTextAlign::eImguiAlignLeft, text,
			IsHot(id) ? ImguiRGBA(0, 0, 0, 255) : ImguiRGBA(0, 0, 0, 200));
	}
	else
	{
		AddCommandText(x + 5, y + 2,
			ImguiTextAlign::eImguiAlignLeft, text,
			ImguiRGBA(60, 60, 60, 60));
	}

	return mouseResult;
}

void ImguiDrawText(int32 x, int32 y, int32 align, const char * text, uint32 color)
{
	AddCommandText(x, y, align, text, color);
}

void ImguiDrawLine(int32 x0, int32 y0, int32 x1, int32 y1, int32 r, uint32 color)
{
	AddCommandLine((float)x0, (float)y0, (float)x1, (float)y1, (float)r, color);
}


void ImguiDrawRect(int32 x, int32 y, int32 w, int32 h, uint32 color)
{
	AddCommandRect((float)x, (float)y, (float)w, (float)h, color);
}

const ImguiCommand * ImguiGetRenderQueue()
{
	return gIMCommandQueue;
}

int32 ImguiGetRenderQueueSize()
{
	return gIMCommandQueueSize;
}
