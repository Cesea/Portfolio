#include "stdafx.h"
#include "imguiRenderer.h"

using namespace im;


GfxCommand GuiRenderer::gfxCommandQueue[GFXCOMMAND_QUEUE_SIZE];
unsigned GuiRenderer::gfxCommandQueueSize;
State GuiRenderer::gState;

int32 Window::CAPTION_HEIGHT = 28;
int32 Window::CAPTION_TEXT_OFFSET = 4;
int32 Window::BUTTON_GAP = 4;
int32 Window::BUTTON_SIZE = Window::CAPTION_HEIGHT - 2 * Window::BUTTON_GAP;
int32 Window::MINIMIZE_BUTTON_RIGHT_OFFSET = (Window::BUTTON_GAP + Window::BUTTON_SIZE) * 2;
int32 Window::CLOSE_BUTTON_RIGHT_OFFSET = Window::BUTTON_GAP + Window::BUTTON_SIZE;
int32 Window::WIDGET_YOFFSET_VALUE = 4;
int32 Window::WIDGET_XOFFSET_VALUE = 8;
int32 Window::WIDGET_INDENT_VALUE = 36;
int32 Window::SPACING_VALUE = 6;
int32 Window::AREA_HEADER = 24;

int32 Button::BUTTON_WIDTH = 60;
int32 Button::BUTTON_HEIGHT = 20;

int32 Font::FONT_HEIGHT = 10;

int32 Check::CHECK_SIZE = 8;

int32 Slider::SLIDER_WIDTH = 100;
int32 Slider::SLIDER_HEIGHT = 20;
int32 Slider::MARKER_WIDTH = 10;

int32 Edit::EDIT_WIDTH = 200;
int32 Edit::EDIT_MAX_CHAR = 64;
int32 Edit::MARKET_WIDTH = 5;

const D3DCOLOR Window::DEFAULT_COLOR[4] =
{
	D3DCOLOR_ARGB(180, 120, 120, 120),
	D3DCOLOR_ARGB(200, 150, 150, 150),
	D3DCOLOR_ARGB(230, 180, 180, 180),
	D3DCOLOR_ARGB(100, 100, 100, 100),
};

const D3DCOLOR Font::DEFAULT_FONT_COLOR[4] =
{
	D3DCOLOR_ARGB(230, 170, 170, 170),
	D3DCOLOR_ARGB(230, 210, 210, 210),
	D3DCOLOR_ARGB(230, 255, 255, 255),
	D3DCOLOR_ARGB(140, 100, 100, 100),
};

im::GuiRenderer::GuiRenderer()
{
	_pDevice = nullptr;
	_pStateBlock = nullptr;

	_clientHeight = 0;
	_clientWidth = 0;

	D3DXMatrixIdentity(&_projection);
	D3DXMatrixIdentity(&_view);

	for (int i = 0; i < 3; ++i)
	{
		_fonts[i] = nullptr;
	}
}

im::GuiRenderer::~GuiRenderer()
{
}

bool im::GuiRenderer::Init(IDirect3DDevice9 *pDevice, const char *fontPath, int32 clientWidth, int32 clientHeight)
{
	_pDevice = pDevice;

	_clientWidth = clientWidth;
	_clientHeight = clientHeight;

	D3DXMatrixIdentity(&_projection);
	D3DXMatrixIdentity(&_view);

	_projection._11 = 2.0f / (float)(_clientWidth);
	_projection._22 = 2.0f / (float)(_clientHeight);
	_projection._33 = 1.0f / (float)(0.1f - 100.0f);
	_projection._43 = 0.1f * (0.1f / (100.0f - 0.1f));
	_projection._44 = 1.0f;

	float tx, ty, tz;
	tx = (_clientWidth)-((float)_clientWidth / 2.0f);
	ty = (_clientHeight)-((float)_clientHeight / 2.0f);
	tz = 0.1f + 0.1f;

	_view._22 = -1.0f;
	_view._41 -= tx;
	_view._42 = ty;
	_view._43 = -tz;

	D3DXFONT_DESCA fontDesc;

	fontDesc.Height = 10;
	fontDesc.Width = 6;
	fontDesc.Weight = 10;
	fontDesc.MipLevels = 0;
	fontDesc.Italic = false;
	fontDesc.CharSet = DEFAULT_CHARSET;
	fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
	fontDesc.Quality = DEFAULT_QUALITY;
	fontDesc.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;

	strncpy(fontDesc.FaceName, fontPath, strlen(fontPath));

	for (int i = 0; i < 3; ++i)
	{
		fontDesc.Height += i * 5;
		fontDesc.Width += i * 5;
		fontDesc.Weight += i * 5;
		if (FAILED(D3DXCreateFontIndirectA(_pDevice, &fontDesc, &_fonts[i])))
		{
			//TODO : Logging
			return false;
		}
	}
}

void im::GuiRenderer::Destroy()
{
	for (int i = 0; i < 3; ++i)
	{
		COM_RELEASE(_fonts[i]);
	}
}

void im::GuiRenderer::Draw()
{
	if (_pStateBlock == nullptr)
	{
		_pDevice->CreateStateBlock(D3DSBT_ALL, &_pStateBlock);
		_pStateBlock->Capture();
	}
	else
	{
		_pStateBlock->Capture();
	}

	//_pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	_pDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, 0);
	_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	_pDevice->SetRenderState(D3DRS_LASTPIXEL, FALSE);
	_pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
	_pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	_pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0000000F);
	_pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
	_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);
	_pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	_pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	_pDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_PASSTHRU);
	_pDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	_pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	_pDevice->SetVertexShader(NULL);
	_pDevice->SetPixelShader(NULL);

	_pDevice->SetTransform(D3DTS_VIEW, &_view);
	_pDevice->SetTransform(D3DTS_PROJECTION, &_projection);


	const GfxCommand* queue = gfxCommandQueue;
	int numQueue = gfxCommandQueueSize;

	for (int i = 0; i < numQueue; ++i)
	{
		const GfxCommand& cmd = queue[i];
		if (cmd.type == GFXCOMMAND_RECT)
		{
			this->DrawRect((int32)cmd.rect.x, (int32)cmd.rect.y,
				(int32)cmd.rect.x + cmd.rect.w, (int32)cmd.rect.y + cmd.rect.h, cmd.color);
		}
		else if (cmd.type == GFXCOMMAND_LINE)
		{
			this->DrawLine(cmd.line.x0, cmd.line.y0, cmd.line.x1, cmd.line.y1, cmd.color);
		}
		else if (cmd.type == GFXCOMMAND_TRIANGLE)
		{
			if (cmd.flags == 1)
			{
				this->DrawTriangle(cmd.rect.x + cmd.rect.w / 2, cmd.rect.y,
					cmd.rect.x + cmd.rect.w, cmd.rect.y + cmd.rect.h,
					cmd.rect.x, cmd.rect.y + cmd.rect.h, 
					cmd.color);
			}
			if (cmd.flags == 2)
			{
				this->DrawTriangle(cmd.rect.x + cmd.rect.w / 2, cmd.rect.y + cmd.rect.h,
					cmd.rect.x, cmd.rect.y,
					cmd.rect.x + cmd.rect.w, cmd.rect.y, 
					cmd.color);
			}
		}
		else if (cmd.type == GFXCOMMAND_TEXT)
		{
			this->DrawFont(0, cmd.text.x, cmd.text.y, cmd.color, cmd.text.text, cmd.text.align);
		}
		else if (cmd.type == GFXCOMMAND_SCISSOR)
		{
			if (cmd.flags)
			{
				_pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, true);
				_pDevice->SetScissorRect(&RECTMake(cmd.rect.x, cmd.rect.y, cmd.rect.x + cmd.rect.w, cmd.rect.y + cmd.rect.h));
			}
			else
			{
				_pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
			}
		}
	}
	_pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
	_pStateBlock->Apply();
}



void im::GuiRenderer::DrawRect(int32 x0, int32 y0, int32 x1, int32 y1, D3DCOLOR color)
{
	UIVertex vertices[4];
	vertices[0].color = color;
	vertices[1].color = color;
	vertices[2].color = color;
	vertices[3].color = color;

	vertices[0].position.x = x0;
	vertices[0].position.y = y0;
	vertices[0].position.z = 0.0f;

	vertices[1].position.x = x1;
	vertices[1].position.y = y0;
	vertices[1].position.z = 0.0f;

	vertices[2].position.x = x1;
	vertices[2].position.y = y1;
	vertices[2].position.z = 0.0f;

	vertices[3].position.x = x0;
	vertices[3].position.y = y1;
	vertices[3].position.z = 0.0f;

	uint16 indices[6];
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;

	_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
	_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	_pDevice->SetFVF(UIVertex::FVF);
	_pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, indices, D3DFMT_INDEX16, 
		vertices, sizeof(UIVertex));
}

void im::GuiRenderer::DrawLine(int32 x0, int32 y0, int32 x1, int32 y1, D3DCOLOR color)
{
	UIVertex vertices[2];
	vertices[0].color = color;
	vertices[1].color = color;

	vertices[0].position[0] = x0;
	vertices[0].position[1] = y0;
	vertices[0].position[2] = 0.0f;

	vertices[1].position[0] = x1;
	vertices[1].position[1] = y1;
	vertices[1].position[2] = 0.0f;

	_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
	_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	_pDevice->SetFVF(UIVertex::FVF);
	_pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, vertices, sizeof(UIVertex));
}

void im::GuiRenderer::DrawFont(int32 id, int32 x, int32 y, D3DCOLOR color, const char *str, DWORD format)
{
	RECT rect = { x, y, 0, 0 };
	_fonts[id]->DrawTextA(nullptr, str, -1, &rect, format | DT_CALCRECT, 0);
	_fonts[id]->DrawTextA(nullptr, str, -1, &rect, format, color);
}

void im::GuiRenderer::DrawTriangle(int32 x0, int32 y0, int32 x1, int32 y1, int32 x2, int32 y2, D3DCOLOR color)
{
	UIVertex vertices[3];
	vertices[0].color = color;
	vertices[1].color = color;
	vertices[2].color = color;

	vertices[0].position[0] = x0;
	vertices[0].position[1] = y0;
	vertices[0].position[2] = 0.0f;

	vertices[1].position[0] = x1;
	vertices[1].position[1] = y1;
	vertices[1].position[2] = 0.0f;

	vertices[2].position[0] = x2;
	vertices[2].position[1] = y2;
	vertices[2].position[2] = 0.0f;

	//gDevice->SetRenderState
	_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
	_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	_pDevice->SetFVF(UIVertex::FVF);
	_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, vertices, sizeof(UIVertex));
}


static void ResetGfxCmdQueue()
{
	GuiRenderer::gfxCommandQueueSize = 0;
}

static void AddGfxCommandScissor(int32 x, int32 y, int32 w, int32 h, bool flag)
{
	if (GuiRenderer::gfxCommandQueueSize >= GFXCOMMAND_QUEUE_SIZE)
	{
		return;
	}
	GfxCommand& cmd = GuiRenderer::gfxCommandQueue[GuiRenderer::gfxCommandQueueSize++];
	cmd.type = GFXCOMMAND_SCISSOR;
	cmd.flags = flag;
	cmd.color = 0;
	cmd.rect.x = (int16)x;
	cmd.rect.y = (int16)y;
	cmd.rect.w = (int16)w;
	cmd.rect.h = (int16)h;
}

static void AddGfxCommandRect(float x, float y, float w, float h, D3DCOLOR color)
{
	if (GuiRenderer::gfxCommandQueueSize >= GFXCOMMAND_QUEUE_SIZE)
	{
		return;
	}
	GfxCommand& cmd = GuiRenderer::gfxCommandQueue[GuiRenderer::gfxCommandQueueSize++];
	cmd.type = GFXCOMMAND_RECT;
	cmd.flags = 0;
	cmd.color = color;
	cmd.rect.x = (int16)(x);
	cmd.rect.y = (int16)(y);
	cmd.rect.w = (int16)(w);
	cmd.rect.h = (int16)(h);
}

static void AddGfxCmdLine(float x0, float y0, float x1, float y1, float r, D3DCOLOR color)
{
	if (GuiRenderer::gfxCommandQueueSize >= GFXCOMMAND_QUEUE_SIZE)
		return;
	GfxCommand& cmd = GuiRenderer::gfxCommandQueue[GuiRenderer::gfxCommandQueueSize++];
	cmd.type = GFXCOMMAND_LINE;
	cmd.flags = 0;
	cmd.color = color;
	cmd.line.x0 = (int16)(x0);
	cmd.line.y0 = (int16)(y0);
	cmd.line.x1 = (int16)(x1);
	cmd.line.y1 = (int16)(y1);
}


static void AddGfxCommandTriangle(int32 x, int32 y, int32 w, int32 h, int32 flags, D3DCOLOR color)
{
	if (GuiRenderer::gfxCommandQueueSize >= GFXCOMMAND_QUEUE_SIZE)
		return;
	GfxCommand& cmd = GuiRenderer::gfxCommandQueue[GuiRenderer::gfxCommandQueueSize++];
	cmd.type = GFXCOMMAND_TRIANGLE;
	cmd.flags = (char)flags;
	cmd.color = color;
	cmd.rect.x = (int16)(x);
	cmd.rect.y = (int16)(y);
	cmd.rect.w = (int16)(w);
	cmd.rect.h = (int16)(h);
}

static void AddGfxCommandText(int32 x, int32 y, int32 align, const char* text, D3DCOLOR color)
{
	if (GuiRenderer::gfxCommandQueueSize >= GFXCOMMAND_QUEUE_SIZE)
		return;
	GfxCommand& cmd = GuiRenderer::gfxCommandQueue[GuiRenderer::gfxCommandQueueSize++];
	cmd.type = GFXCOMMAND_TEXT;
	cmd.flags = 0;
	cmd.color = color;
	cmd.text.x = (short)x;
	cmd.text.y = (short)y;
	cmd.text.align = (short)align;
	cmd.text.text = text;
}

bool im::imMessageProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	bool result = false;
	return result;
}

bool im::ButtonLogic(uint32 id, bool over)
{
	bool res = false;
	// process down
	if (!AnyActive())
	{
		if (over)
			SetHot(id);
		if (IsHot(id) && GuiRenderer::gState.leftPressed)
			SetActive(id);
	}

	// if button is active, then react on left up
	if (IsActive(id))
	{
		GuiRenderer::gState.isActive = true;
		if (over)
		{
			SetHot(id);
		}
		if (GuiRenderer::gState.leftReleased)
		{
			if (IsHot(id))
			{
				res = true;
			}
			ClearActive();
		}
	}

	if (IsHot(id))
	{
		GuiRenderer::gState.isHot = true;
	}

	return res;
}

bool im::FrameLogic(uint32 windowID, bool over)
{
	return false;
}

bool im::KeyLogic(uint32 id)
{
	bool result = false;
	result = (GuiRenderer::gState.keyboardFocusID == id);
	return result;
}

void im::UpdateInput(Mouse &refMouse, uint8 vkCode, bool32 shiftDown)
{
	//TODO : DEBUG

	GuiRenderer::gState.mx = refMouse.GetCurrentPoint().x;
	GuiRenderer::gState.my = refMouse.GetCurrentPoint().y;

	GuiRenderer::gState.leftPressed = refMouse.IsPressed(MOUSE_BUTTON_LEFT);
	GuiRenderer::gState.leftReleased = refMouse.IsReleased(MOUSE_BUTTON_LEFT);

	GuiRenderer::gState.rightPressed = refMouse.IsPressed(MOUSE_BUTTON_RIGHT);
	GuiRenderer::gState.rightReleased = refMouse.IsReleased(MOUSE_BUTTON_RIGHT);

	GuiRenderer::gState.middlePressed = refMouse.IsPressed(MOUSE_BUTTON_MIDDLE);
	GuiRenderer::gState.middleReleased = refMouse.IsReleased(MOUSE_BUTTON_MIDDLE);

	GuiRenderer::gState.left = refMouse.IsDown(MOUSE_BUTTON_LEFT);
	GuiRenderer::gState.right = refMouse.IsDown(MOUSE_BUTTON_RIGHT);
	GuiRenderer::gState.middle = refMouse.IsDown(MOUSE_BUTTON_MIDDLE);

	GuiRenderer::gState.scroll = refMouse.GetWheelDelta();

	GuiRenderer::gState.vkCode = vkCode;
	GuiRenderer::gState.shiftDown = shiftDown;
}

void im::BeginFrame(Mouse &refMouse, uint8 inputChar, bool32 shiftDown)
{
	UpdateInput(refMouse, inputChar, shiftDown);

	GuiRenderer::gState.hot = GuiRenderer::gState.hotToBe;
	GuiRenderer::gState.hotToBe = 0;

	GuiRenderer::gState.wentActive = false;
	GuiRenderer::gState.isActive = false;
	GuiRenderer::gState.isHot = false;

	GuiRenderer::gState.currentWindowID = 0;
	for (int32 i = 0; i < MAX_NUM_WINDOW; ++i)
	{
		GuiRenderer::gState.windows[i].widgetX = 0;
		GuiRenderer::gState.windows[i].widgetY = 0;
		GuiRenderer::gState.windows[i].internalWidgetID = 0;
		GuiRenderer::gState.windows[i].wentDrag = false;
	}

	GuiRenderer::gState.globalWidgetID = 0;

	ResetGfxCmdQueue();
}

void im::EndFrame()
{
	ClearInput();
}


constexpr int32 NORMAL_COLOR = 0;
constexpr int32 HOVER_COLOR = 1;
constexpr int32 HOT_COLOR = 2;
constexpr int32 DISABLE_COLOR = 3;

static const D3DCOLOR DEFAULT_SCROLL_COLOR[4] =
{
	D3DCOLOR_ARGB(140, 100, 100, 100),
	D3DCOLOR_ARGB(170, 130, 130, 130),
	D3DCOLOR_ARGB(200, 160, 160, 160),
	D3DCOLOR_ARGB(70, 100, 100, 100),
};

static const D3DCOLOR DEFAULT_BUTTON_COLOR[4] = 
{
	D3DCOLOR_ARGB(180, 120, 120, 120),
	D3DCOLOR_ARGB(200, 150, 150, 150),
	D3DCOLOR_ARGB(230, 180, 180, 180),
	D3DCOLOR_ARGB(100, 100, 100, 100),
};



//static int32 gScrollTop = 0;
//static int32 gScrollBottom = 0;
//static int32 gScrollRight = 0;
//static int32 gScrollAreaTop = 0;
//static int32* gScrollVal = 0;
//static int32 gFocusTop = 0;
//static int32 gFocusBottom = 0;
//static uint32 gScrollId = 0;
//static bool gInsideScrollArea = false;

static Window &GetCurrentWindowRef()
{
	Window &result = GuiRenderer::gState.windows[GuiRenderer::gState.currentWindowID];
	return result;
}

bool im::BeginScrollArea(const char * name, WindowPos &pos,  int32 w, int32 h, int32 * scroll)
{
	Window &refWindow = GetCurrentWindowRef();

	refWindow.x = pos.x;
	refWindow.y = pos.y;
	refWindow.width = w;
	refWindow.height = h;

	//gScrollTop = y + BUTTON_HEIGHT + DEFAULT_SPACING;
	//gScrollBottom = gScrollTop + h - BUTTON_HEIGHT - DEFAULT_SPACING;
	//gScrollRight = x + w - SCROLL_AREA_PADDING * 3;
	//gScrollVal = scroll;

	//gScrollAreaTop = GuiRenderer::gState.widgetY;

	//gFocusTop = y - AREA_HEADER;
	//gFocusBottom = y - AREA_HEADER + h;

	//gInsideScrollArea = InRect(x, y, w, h, false);
	//GuiRenderer::gState.insideCurrentScroll = gInsideScrollArea;
	refWindow.mouseInside = InRect(refWindow.x, refWindow.y, refWindow.width, refWindow.height, false);

	//배경 그리기
	AddGfxCommandRect((float)refWindow.x, (float)refWindow.y, (float)w, (float)h, 
		DEFAULT_SCROLL_COLOR[NORMAL_COLOR]);

	//캡션 그리기
	AddGfxCommandRect((float)refWindow.x, (float)refWindow.y, (float)w, (float)Window::CAPTION_HEIGHT,
		DEFAULT_SCROLL_COLOR[HOVER_COLOR]);

	refWindow.canDrag = InRect(refWindow.x, refWindow.y, w, Window::CAPTION_HEIGHT, false);

	AddGfxCommandText(refWindow.x + (Window::AREA_HEADER / 2), refWindow.y + (Font::FONT_HEIGHT), ALIGN_LEFT, 
		name, Font::DEFAULT_FONT_COLOR[HOT_COLOR]);

	AddGfxCommandScissor(refWindow.x, refWindow.y + Window::CAPTION_HEIGHT, w, h - Window::CAPTION_HEIGHT, true);

	refWindow.widgetX += Window::SPACING_VALUE * 2;
	refWindow.widgetY += Window::CAPTION_HEIGHT + Window::SPACING_VALUE;

	//if (GuiRenderer::gState.wentActive)
	//{
	//	GuiRenderer::gState.dragX = GuiRenderer::gState.mx;
	//	GuiRenderer::gState.dragOrig = normalizedValue;
	//}
	//if (GuiRenderer::gState.dragX != GuiRenderer::gState.mx)
	//{
	//	normalizedValue = GuiRenderer::gState.dragOrig +
	//		(float)(GuiRenderer::gState.mx - GuiRenderer::gState.dragX) / (float)range;
	//	if (normalizedValue < 0) normalizedValue = 0;
	//	if (normalizedValue > 1) normalizedValue = 1;
	//	*val = vmin + normalizedValue * (vmax - vmin);
	//	*val = floorf(*val / vinc + 0.5f) * vinc; // Snap to vinc
	//	m = (int32)(normalizedValue * range);
	//	valChanged = true;
	//}
	if(refWindow.canDrag && GuiRenderer::gState.leftPressed)
	{
		refWindow.wentDrag = true;
	}

	if (refWindow.wentDrag)
	{
		refWindow.prevDragX = GuiRenderer::gState.mx;
		refWindow.prevDragY = GuiRenderer::gState.my;
	}

	if (refWindow.prevDragX != -1 || refWindow.prevDragY != -1)
	{
		int32 xDiff = refWindow.prevDragX - GuiRenderer::gState.mx;
		int32 yDiff = refWindow.prevDragY - GuiRenderer::gState.my;
		refWindow.x -= xDiff;
		refWindow.y -= yDiff;

		//refWindow.x -= refWindow.x - GuiRenderer::gState.mx;
		//refWindow.y -= refWindow.y - GuiRenderer::gState.my;

		refWindow.prevDragX = GuiRenderer::gState.mx;
		refWindow.prevDragY = GuiRenderer::gState.my;

		if (GuiRenderer::gState.leftReleased)
		{
			refWindow.prevDragX = -1;
			refWindow.prevDragY = -1;
		}

		pos.x = refWindow.x;
		pos.y = refWindow.y;
	}


	return refWindow.mouseInside;
}

void im::EndScrollArea()
{
	// Disable scissoring.
	//AddGfxCmdStencil(-1, -1, -1, -1);

	// Draw scroll bar
	//int x = gScrollRight + SCROLL_AREA_PADDING / 2;
	//int y = gScrollTop;
	//int w = SCROLL_AREA_PADDING * 2;
	//int h = gScrollBottom - gScrollTop;

	//int sTop = gScrollAreaTop;
	//GuiRenderer::gState.widgetY = gScrollBottom + DEFAULT_SPACING * 2;
	//int sBottom = GuiRenderer::gState.widgetY;
	//int sh = sTop - sBottom; // The scrollable area height.


	//float barHeight = (float)h / (float)sh;

	//if (barHeight < 1)
	//{
	//	float barY = (float)(y - sBottom) / (float)sh;
	//	if (barY < 0) barY = 0;
	//	if (barY > 1) barY = 1;
	//	// Handle scroll bar logic.
	//	unsigned int hid = gScrollId;
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
	//		if (GuiRenderer::gState.wentActive)
	//		{
	//			GuiRenderer::gState.dragY = GuiRenderer::gState.my;
	//			GuiRenderer::gState.dragOrig = u;
	//		}
	//		if (GuiRenderer::gState.dragY != GuiRenderer::gState.my)
	//		{
	//			u = GuiRenderer::gState.dragOrig + (GuiRenderer::gState.my - GuiRenderer::gState.dragY) / (float)range;
	//			if (u < 0) u = 0;
	//			if (u > 1) u = 1;
	//			*gScrollVal = (int)((1 - u) * (sh - h));
	//		}
	//	}
	//	// BG
	//	AddGfxCommandRect((float)x, (float)y, (float)w, (float)h, D3DCOLOR_ARGB(200, 100, 100, 196));
	//	// Bar
	//	if (IsActive(hid))
	//	{
	//		AddGfxCommandRect((float)hx, (float)hy, (float)hw, (float)hh, 
	//		DEFAULT_SCROLL_COLOR[HOT_COLOR]);
	//	}
	//	else
	//	{ 
	//		AddGfxCommandRect((float)hx, (float)hy, (float)hw, (float)hh, 
	//			IsHot(hid) ? DEFAULT_SCROLL_COLOR[HOVER_COLOR] : DEFAULT_SCROLL_COLOR[NORMAL_COLOR]);
	//	}
	//	

	//	// Handle mouse scrolling.
	//	if (gInsideScrollArea) // && !anyActive())
	//	{
	//		if (GuiRenderer::gState.scroll != 0)
	//		{
	//			*gScrollVal += (float)GuiRenderer::gState.scroll * 0.1f;
	//			if (*gScrollVal< 0) *gScrollVal = 0;
	//			if (*gScrollVal >(sh - h)) *gScrollVal = (sh - h);
	//		}
	//	}
	//}
	GuiRenderer::gState.insideCurrentScroll = false;

	AddGfxCommandScissor(0, 0, 0, 0, false);
	GuiRenderer::gState.currentWindowID++;
}

void im::Indent()
{
	Window &refWindow = GetCurrentWindowRef();
	refWindow.widgetX += Window::WIDGET_INDENT_VALUE;
}

void im::Unindent()
{
	Window &refWindow = GetCurrentWindowRef();
	refWindow.widgetX -= Window::WIDGET_INDENT_VALUE;
}

void im::imguiSeparator()
{
	Window &refWindow = GetCurrentWindowRef();
	refWindow.widgetY += Window::SPACING_VALUE * 3;
}

void im::SeparatorLine()
{
	Window &refWindow = GetCurrentWindowRef();
	refWindow.widgetY += Window::SPACING_VALUE * 3;

	int x = refWindow.widgetX;
	int y = refWindow.widgetY + Window::SPACING_VALUE * 2;
	int w = refWindow.width;
	int h = Window::SPACING_VALUE;

	refWindow.widgetY += Window::SPACING_VALUE * 4;

	AddGfxCommandRect((float)x, (float)y, (float)w, (float)h, D3DCOLOR_ARGB(180, 255, 255, 255));
}

bool im::Scissor(int32 x, int32 y, int32 width, int32 height, bool flag)
{
	AddGfxCommandScissor(x, y, width, height, flag);
	return true;
}

bool im::Button(const char * text, int32 width, int32 xPos, bool enabled)
{
	Window &refWindow = GetCurrentWindowRef();
	refWindow.internalWidgetID++;
	uint32 id = ++GuiRenderer::gState.globalWidgetID;

	int32 x = (xPos) ? (xPos) : (refWindow.widgetX + refWindow.x);
	int32 y = refWindow.widgetY + refWindow.y;
	int32 w = (width == 0) ? Button::BUTTON_WIDTH : width;
	int32 h = Button::BUTTON_HEIGHT;
	refWindow.widgetY += Button::BUTTON_HEIGHT + Window::SPACING_VALUE;

	bool over = enabled && InRect(x, y, w, h);

	bool res = ButtonLogic(id, over);

	AddGfxCommandRect((float)x, (float)y, (float)w, (float)h, 
		 IsActive(id) ? DEFAULT_BUTTON_COLOR[HOT_COLOR] : DEFAULT_BUTTON_COLOR[NORMAL_COLOR]);

	if (enabled)
	{
		AddGfxCommandText(x + Button::BUTTON_HEIGHT / 2, y + Button::BUTTON_HEIGHT / 2 - Font::FONT_HEIGHT / 2,
			ALIGN_LEFT, text, 
			IsHot(id) ? Font::DEFAULT_FONT_COLOR[HOT_COLOR] : Font::DEFAULT_FONT_COLOR[NORMAL_COLOR]);
	}
	else
	{
		AddGfxCommandText(x + Button::BUTTON_HEIGHT / 2, y + Button::BUTTON_HEIGHT / 2 - Font::FONT_HEIGHT / 2,
			ALIGN_LEFT, text, Font::DEFAULT_FONT_COLOR[DISABLE_COLOR]);
	}

	return res;
}

bool im::Check(const char * text, bool checked, int32 width, int32 xPos,  bool enabled)
{
	Window &refWindow = GetCurrentWindowRef();
	refWindow.internalWidgetID++;
	uint32 id = ++GuiRenderer::gState.globalWidgetID;

	int32 x = (xPos) ? (xPos) : (refWindow.widgetX + refWindow.x);
	int32 y = refWindow.widgetY + refWindow.y;
	int32 w = (width == 0) ? Button::BUTTON_WIDTH : width;
	int32 h = Button::BUTTON_HEIGHT;
	refWindow.widgetY += Button::BUTTON_HEIGHT + Window::SPACING_VALUE;

	bool over = enabled && InRect(x, y, w, h);
	bool res = ButtonLogic(id, over);

	const int cx = x + (Button::BUTTON_HEIGHT / 2) - (Check::CHECK_SIZE / 2);
	const int cy = y + (Button::BUTTON_HEIGHT / 2) - (Check::CHECK_SIZE / 2);
	AddGfxCommandRect((float)cx - 2, (float)cy - 2, (float)Check::CHECK_SIZE + 4, (float)Check::CHECK_SIZE + 4,
		IsActive(id) ? DEFAULT_BUTTON_COLOR[HOT_COLOR] : DEFAULT_BUTTON_COLOR[NORMAL_COLOR]);

	//checkbox color
	if (checked)
	{
		if (enabled)
		{
			AddGfxCommandRect((float)cx, (float)cy, (float)Check::CHECK_SIZE, (float)Check::CHECK_SIZE,
				D3DCOLOR_ARGB(255, 255, 255, IsActive(id) ? 255 : 200));
		}
		else
		{
			AddGfxCommandRect((float)cx, (float)cy, (float)Check::CHECK_SIZE, (float)Check::CHECK_SIZE,
				D3DCOLOR_ARGB(128, 128, 128, 200));
		}
	}

	if (enabled)
	{
		AddGfxCommandText(x + Button::BUTTON_HEIGHT, y + (Button::BUTTON_HEIGHT / 2) - (Font::FONT_HEIGHT / 2),
			ALIGN_LEFT, text, 
			IsHot(id) ? Font::DEFAULT_FONT_COLOR[HOT_COLOR] : Font::DEFAULT_FONT_COLOR[NORMAL_COLOR]);
	}
	else
	{
		AddGfxCommandText(x + Button::BUTTON_HEIGHT, y + (Button::BUTTON_HEIGHT / 2) - (Font::FONT_HEIGHT / 2),
			ALIGN_LEFT, text, 
			Font::DEFAULT_FONT_COLOR[DISABLE_COLOR]);
	}
	return res;
}

bool im::Collapse(const char * text, bool checked, int32 width, int32 xPos, bool enabled)
{
	Window &refWindow = GetCurrentWindowRef();
	refWindow.internalWidgetID++;
	uint32 id = ++GuiRenderer::gState.globalWidgetID;

	int32 x = (xPos) ? (xPos) : (refWindow.widgetX + refWindow.x);
	int32 y = refWindow.widgetY + refWindow.y;
	int32 w = (width == 0) ? Button::BUTTON_WIDTH : width;
	int32 h = Button::BUTTON_HEIGHT;
	refWindow.widgetY += Button::BUTTON_HEIGHT + Window::SPACING_VALUE;

	bool over = enabled && InRect(x, y, w, h);
	bool res = ButtonLogic(id, over);

	const int cx = x + (Button::BUTTON_HEIGHT / 2) - (Check::CHECK_SIZE / 2);
	const int cy = y + (Button::BUTTON_HEIGHT / 2) - (Check::CHECK_SIZE / 2);

	////color for checkbox
	if (checked)
	{
		AddGfxCommandTriangle(cx, cy, Check::CHECK_SIZE, Check::CHECK_SIZE, 2, 
			D3DCOLOR_ARGB(255, 255, 255, IsActive(id) ? 255 : 200));
	}
	else
	{
		AddGfxCommandTriangle(cx, cy, Check::CHECK_SIZE, Check::CHECK_SIZE, 1, 
			D3DCOLOR_ARGB(255, 255, 255, IsActive(id) ? 255 : 200));
	}

	if (enabled)
	{
		AddGfxCommandText(x + Button::BUTTON_HEIGHT, y + (Button::BUTTON_HEIGHT / 2) - (Font::FONT_HEIGHT / 2),
			ALIGN_LEFT, text,
			IsHot(id) ? Font::DEFAULT_FONT_COLOR[HOT_COLOR] : Font::DEFAULT_FONT_COLOR[NORMAL_COLOR]);
	}
	else
	{
		AddGfxCommandText(x + Button::BUTTON_HEIGHT, y + (Button::BUTTON_HEIGHT / 2) - (Font::FONT_HEIGHT / 2),
			ALIGN_LEFT, text,
			Font::DEFAULT_FONT_COLOR[DISABLE_COLOR]);
	}

	return res;
}

void im::Label(const char * text)
{
	Window &refWindow = GetCurrentWindowRef();
	refWindow.internalWidgetID++;
	uint32 id = ++GuiRenderer::gState.globalWidgetID;

	int32 x = refWindow.widgetX + refWindow.x;
	int32 y = refWindow.widgetY + refWindow.y;
	refWindow.widgetY += Button::BUTTON_HEIGHT + Window::SPACING_VALUE;

	AddGfxCommandText(x + Window::SPACING_VALUE, y + Button::BUTTON_HEIGHT / 2 - Font::FONT_HEIGHT / 2, 
		ALIGN_LEFT, text, 
		Font::DEFAULT_FONT_COLOR[NORMAL_COLOR]);
}

bool im::Slider(const char * text, float * val, float vmin, float vmax, float vinc, int32 width, bool enabled)
{
	Window &refWindow = GetCurrentWindowRef();
	refWindow.internalWidgetID++;
	uint32 id = ++GuiRenderer::gState.globalWidgetID;

	int32 x = refWindow.widgetX + refWindow.x;
	int32 y = refWindow.widgetY + refWindow.y;
	int32 w = (width == 0) ? Slider::SLIDER_WIDTH : width;
	int32 h = Slider::SLIDER_HEIGHT;
	refWindow.widgetY += Slider::SLIDER_HEIGHT + Window::SPACING_VALUE;

	//bool over = enabled && InRect(x, y, w, h);
	//bool res = ButtonLogic(id, over);

	AddGfxCommandRect((float)x, (float)y, (float)w, (float)h, DEFAULT_BUTTON_COLOR[NORMAL_COLOR]);

	const int range = w - Slider::MARKER_WIDTH;

	float normalizedValue = (*val - vmin) / (vmax - vmin);
	if (normalizedValue < 0.0f)
	{
		normalizedValue = 0.0f;
	}
	if (normalizedValue > 1.0f)
	{
		normalizedValue = 1.0f;
	}
	int m = (int)(normalizedValue * range);

	bool over = enabled && InRect(x + m, y, Slider::MARKER_WIDTH, Slider::SLIDER_HEIGHT);
	bool res = ButtonLogic(id, over);
	bool valChanged = false;

	if (IsActive(id))
	{
		if (GuiRenderer::gState.wentActive)
		{
			GuiRenderer::gState.dragX = GuiRenderer::gState.mx;
			GuiRenderer::gState.dragOrig = normalizedValue;
		}
		if (GuiRenderer::gState.dragX != GuiRenderer::gState.mx)
		{
			normalizedValue = GuiRenderer::gState.dragOrig + 
				(float)(GuiRenderer::gState.mx - GuiRenderer::gState.dragX) / (float)range;
			if (normalizedValue < 0) normalizedValue = 0;
			if (normalizedValue > 1) normalizedValue = 1;
			*val = vmin + normalizedValue * (vmax - vmin);
			*val = floorf(*val / vinc + 0.5f) * vinc; // Snap to vinc
			m = (int32)(normalizedValue * range);
			valChanged = true;
		}
	}

	if (IsActive(id))
	{
		AddGfxCommandRect((float)(x + m), (float)y, (float)Slider::MARKER_WIDTH, (float)Slider::SLIDER_HEIGHT, 
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	else
	{
		AddGfxCommandRect((float)(x + m), (float)y, (float)Slider::MARKER_WIDTH, (float)Slider::SLIDER_HEIGHT, 
			IsHot(id) ? D3DCOLOR_ARGB(210, 180, 180, 180) : D3DCOLOR_ARGB(180, 150, 150, 150));
	}

	if (enabled)
	{
		AddGfxCommandText(x + (Slider::SLIDER_WIDTH / 2),
			y + Slider::SLIDER_HEIGHT / 2 - Font::FONT_HEIGHT / 2, ALIGN_LEFT, text, 
			IsHot(id) ? Font::DEFAULT_FONT_COLOR[HOT_COLOR] : Font::DEFAULT_FONT_COLOR[NORMAL_COLOR]);
	}
	else
	{
		AddGfxCommandText(x + Slider::SLIDER_HEIGHT / 2, 
			y + Slider::SLIDER_HEIGHT / 2 - Font::FONT_HEIGHT / 2, ALIGN_LEFT, 
			text, Font::DEFAULT_FONT_COLOR[DISABLE_COLOR]);
	}

	return res || valChanged;
}

bool im::Edit(char * text, int32 width, bool enabled)
{
	Window &refWindow = GetCurrentWindowRef();
	refWindow.internalWidgetID++;
	uint32 id = ++GuiRenderer::gState.globalWidgetID;

	int32 x = refWindow.widgetX + refWindow.x;
	int32 y = refWindow.widgetY + refWindow.y;
	int32 w = (width == 0) ? Edit::EDIT_WIDTH : width;
	int32 h = Button::BUTTON_HEIGHT;
	refWindow.widgetY += Button::BUTTON_HEIGHT + Window::SPACING_VALUE;

	RECT textRect = { 0, 0, 0, 0 };
	GuiRenderer::GetInstance()->GetFont()->
		DrawTextA(nullptr, text, -1, &textRect, ALIGN_LEFT | DT_CALCRECT, 0xffffffff);
	int32 textWidth = textRect.right - textRect.left;

	bool over = enabled && InRect(x, y, w, h);
	bool mouseResult = ButtonLogic(id, over);

	if (mouseResult)
	{
		GuiRenderer::gState.keyboardFocusID = id;
	}

	bool keyResult = KeyLogic(id);

	if (GuiRenderer::gState.vkCode && keyResult)
	{
		int32 index = strlen(text);
		if (GuiRenderer::gState.vkCode == VK_BACK)
		{
			if (index > 0)
			{
				text[index - 1] = '\0';
			}
		}
		else
		{
			char mappedChar = MapVirtualKey(GuiRenderer::gState.vkCode, MAPVK_VK_TO_CHAR);
			if (IsLetter(mappedChar))
			{
				if (!GuiRenderer::gState.shiftDown)
				{
					mappedChar += 32;
				}
			}
			text[index] = mappedChar;
		}
	}

	AddGfxCommandRect((float)x, (float)y, (float)w, (float)h,
		IsActive(id) ? DEFAULT_BUTTON_COLOR[HOT_COLOR] : DEFAULT_BUTTON_COLOR[NORMAL_COLOR]);

	if (enabled)
	{
		AddGfxCommandText(x + Button::BUTTON_HEIGHT / 2, y + Button::BUTTON_HEIGHT / 2 - Font::FONT_HEIGHT / 2,
			ALIGN_LEFT, text, 
			IsHot(id) ? Font::DEFAULT_FONT_COLOR[HOT_COLOR] : Font::DEFAULT_FONT_COLOR[NORMAL_COLOR]);
	}
	else
	{
		AddGfxCommandText(x + Button::BUTTON_HEIGHT / 2, y + Button::BUTTON_HEIGHT / 2 - Font::FONT_HEIGHT / 2,
			ALIGN_LEFT, text, Font::DEFAULT_FONT_COLOR[DISABLE_COLOR]);
	}

	static uint32 tick = 0;
	static bool on = false;

	if (tick % 60 == 0)
	{
		on = !on;
	}

	if (keyResult && on)
	{
		AddGfxCommandRect((float)x + textWidth + Button::BUTTON_HEIGHT / 2, (float)y, (float)4, (float)h, DEFAULT_BUTTON_COLOR[HOT_COLOR]);
	}

	tick++;

	bool entered = false;
	if (id == GuiRenderer::gState.keyboardFocusID &&
		(GuiRenderer::gState.vkCode == VK_RETURN))
	{
		entered = true;
	}

	return entered;
}

