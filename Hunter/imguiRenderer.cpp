#include "stdafx.h"
#include "imguiRenderer.h"

using namespace im;

GfxCommand GuiRenderer::gfxCommandQueue[GFXCOMMAND_QUEUE_SIZE];
unsigned GuiRenderer::gfxCommandQueueSize;
GuiRenderer::State GuiRenderer::gState;

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

	_pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
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
				_pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, true);
			}
		}
	}
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

static void AddGfxCmdStencil(int32 x, int32 y, int32 w, int32 h, bool flag)
{
	if (GuiRenderer::gfxCommandQueueSize >= GFXCOMMAND_QUEUE_SIZE)
		return;
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
		return;
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
			SetHot(id);
		if (GuiRenderer::gState.leftReleased)
		{
			if (IsHot(id))
				res = true;
			ClearActive();
		}
	}

	if (IsHot(id))
		GuiRenderer::gState.isHot = true;

	return res;
}

bool im::KeyLogic(uint32 id)
{
	bool result = false;
	result = (GuiRenderer::gState.keyboardFocusID == id);
	return result;
}

void im::UpdateInput(Mouse &refMouse, uint8 inputChar)
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

	GuiRenderer::gState.charInput = inputChar;

}

void im::BeginFrame(Mouse &refMouse, uint8 inputChar)
{
	UpdateInput(refMouse, inputChar);

	GuiRenderer::gState.hot = GuiRenderer::gState.hotToBe;
	GuiRenderer::gState.hotToBe = 0;

	GuiRenderer::gState.wentActive = false;
	GuiRenderer::gState.isActive = false;
	GuiRenderer::gState.isHot = false;

	GuiRenderer::gState.widgetX = 0;
	GuiRenderer::gState.widgetY = 0;
	GuiRenderer::gState.widgetW = 60;

	GuiRenderer::gState.areaId = 1;
	GuiRenderer::gState.widgetId = 1;

	ResetGfxCmdQueue();
}

void im::EndFrame()
{
	ClearInput();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const int32 BUTTON_HEIGHT = 20;
static const int32 SLIDER_HEIGHT = 20;
static const int32 SLIDER_MARKER_WIDTH = 10;
static const int32 CHECK_SIZE = 8;
static const int32 DEFAULT_SPACING = 4;
static const int32 TEXT_HEIGHT = 8;
static const int32 SCROLL_AREA_PADDING = 6;
static const int32 INDENT_SIZE = 16;
static const int32 AREA_HEADER = 28;

static const int32 DEFAULT_BUTTON_WIDTH = 60;
static const int32 DEFAULT_SLIDER_WIDTH = 120;

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

static const D3DCOLOR DEFAULT_FONT_COLOR[4] = 
{
	D3DCOLOR_ARGB(230, 170, 170, 170),
	D3DCOLOR_ARGB(230, 210, 210, 210),
	D3DCOLOR_ARGB(230, 255, 255, 255),
	D3DCOLOR_ARGB(140, 100, 100, 100),
};

static int32 gScrollTop = 0;
static int32 gScrollBottom = 0;
static int32 gScrollRight = 0;
static int32 gScrollAreaTop = 0;
static int32* gScrollVal = 0;
static int32 gFocusTop = 0;
static int32 gFocusBottom = 0;
static uint32 gScrollId = 0;
static bool gInsideScrollArea = false;

bool im::BeginScrollArea(const char * name, int32 x, int32 y, int32 w, int32 h, int32 * scroll)
{
	GuiRenderer::gState.areaId++;
	GuiRenderer::gState.widgetId = 0;
	gScrollId = (GuiRenderer::gState.areaId << 16) | GuiRenderer::gState.widgetId;

	GuiRenderer::gState.widgetX = x + SCROLL_AREA_PADDING;
	//GuiRenderer::gState.widgetY = y + h + (*scroll) + DEFAULT_SPACING;
	GuiRenderer::gState.widgetY = y + BUTTON_HEIGHT + DEFAULT_SPACING;
	GuiRenderer::gState.widgetW = w - SCROLL_AREA_PADDING * 4;
	gScrollTop = y + BUTTON_HEIGHT + DEFAULT_SPACING;
	gScrollBottom = gScrollTop + h - BUTTON_HEIGHT - DEFAULT_SPACING;
	gScrollRight = x + w - SCROLL_AREA_PADDING * 3;
	gScrollVal = scroll;

	gScrollAreaTop = GuiRenderer::gState.widgetY;

	gFocusTop = y - AREA_HEADER;
	gFocusBottom = y - AREA_HEADER + h;

	gInsideScrollArea = InRect(x, y, w, h, false);
	GuiRenderer::gState.insideCurrentScroll = gInsideScrollArea;

	AddGfxCommandRect((float)x, (float)y, (float)w, (float)h, DEFAULT_SCROLL_COLOR[NORMAL_COLOR]);

	AddGfxCommandRect((float)x, (float)y, (float)w, (float)BUTTON_HEIGHT, DEFAULT_SCROLL_COLOR[HOVER_COLOR]);

	AddGfxCommandText(x + AREA_HEADER / 2, y + TEXT_HEIGHT / 2, ALIGN_LEFT, 
		name, DEFAULT_FONT_COLOR[HOT_COLOR]);

	//AddGfxCmdStencil(x + SCROLL_AREA_PADDING, y + SCROLL_AREA_PADDING, w - SCROLL_AREA_PADDING * 4, h - AREA_HEADER - SCROLL_AREA_PADDING);

	return gInsideScrollArea;
}

void im::EndScrollArea()
{
	// Disable scissoring.
	//AddGfxCmdStencil(-1, -1, -1, -1);

	// Draw scroll bar
	int x = gScrollRight + SCROLL_AREA_PADDING / 2;
	int y = gScrollTop;
	int w = SCROLL_AREA_PADDING * 2;
	int h = gScrollBottom - gScrollTop;

	int sTop = gScrollAreaTop;
	GuiRenderer::gState.widgetY = gScrollBottom + DEFAULT_SPACING * 2;
	int sBottom = GuiRenderer::gState.widgetY;
	int sh = sTop - sBottom; // The scrollable area height.


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
}

void im::Indent()
{
	GuiRenderer::gState.widgetX += INDENT_SIZE;
	GuiRenderer::gState.widgetW -= INDENT_SIZE;
}

void im::Unindent()
{
	GuiRenderer::gState.widgetX -= INDENT_SIZE;
	GuiRenderer::gState.widgetW += INDENT_SIZE;
}

void im::imguiSeparator()
{
	GuiRenderer::gState.widgetY -= DEFAULT_SPACING * 3;
}

void im::SeparatorLine()
{
	int x = GuiRenderer::gState.widgetX;
	int y = GuiRenderer::gState.widgetY - DEFAULT_SPACING * 2;
	int w = GuiRenderer::gState.widgetW;
	int h = 1;
	GuiRenderer::gState.widgetY -= DEFAULT_SPACING * 4;

	AddGfxCommandRect((float)x, (float)y, (float)w, (float)h, D3DCOLOR_ARGB(255, 255, 255, 32));
}

bool im::Button(const char * text, int32 width, bool enabled)
{
	GuiRenderer::gState.widgetId++;
	uint32 id = (GuiRenderer::gState.areaId << 16) | GuiRenderer::gState.widgetId;

	int32 x = GuiRenderer::gState.widgetX;
	int32 y = GuiRenderer::gState.widgetY/* - BUTTON_HEIGHT*/;
	int32 w = (width == 0) ? DEFAULT_BUTTON_WIDTH : width;
	int32 h = BUTTON_HEIGHT;
	GuiRenderer::gState.widgetY += BUTTON_HEIGHT + DEFAULT_SPACING;

	bool over = enabled && InRect(x, y, w, h);

	bool res = ButtonLogic(id, over);

	AddGfxCommandRect((float)x, (float)y, (float)w, (float)h, 
		 IsActive(id) ? DEFAULT_BUTTON_COLOR[HOT_COLOR] : DEFAULT_BUTTON_COLOR[NORMAL_COLOR]);

	if (enabled)
	{
		AddGfxCommandText(x + BUTTON_HEIGHT / 2, y + BUTTON_HEIGHT / 2 - TEXT_HEIGHT / 2,
			ALIGN_LEFT, text, IsHot(id) ? DEFAULT_FONT_COLOR[HOT_COLOR] : DEFAULT_FONT_COLOR[NORMAL_COLOR]);
	}
	else
	{
		AddGfxCommandText(x + BUTTON_HEIGHT / 2, y + BUTTON_HEIGHT / 2 - TEXT_HEIGHT / 2,
			ALIGN_LEFT, text, DEFAULT_FONT_COLOR[DISABLE_COLOR]);
	}

	return res;
}

bool im::Item(const char * text, int32 width, bool enabled)
{
	GuiRenderer::gState.widgetId++;
	unsigned int id = (GuiRenderer::gState.areaId << 16) | GuiRenderer::gState.widgetId;

	int32 x = GuiRenderer::gState.widgetX;
	int32 y = GuiRenderer::gState.widgetY;
	int32 w = (width == 0) ? DEFAULT_BUTTON_WIDTH : width;
	int32 h = BUTTON_HEIGHT;
	GuiRenderer::gState.widgetY += BUTTON_HEIGHT + DEFAULT_SPACING;

	bool over = enabled && InRect(x, y, w, h);
	bool res = ButtonLogic(id, over);

	if (IsHot(id))
	{
		AddGfxCommandRect((float)x, (float)y, (float)w, (float)h,
			IsActive(id) ? DEFAULT_BUTTON_COLOR[HOT_COLOR] : DEFAULT_BUTTON_COLOR[NORMAL_COLOR]);
	}

	if (enabled)
	{
		AddGfxCommandText(x + BUTTON_HEIGHT / 2, y + BUTTON_HEIGHT / 2 - TEXT_HEIGHT / 2,
			ALIGN_LEFT, text, IsHot(id) ? DEFAULT_FONT_COLOR[HOT_COLOR] : DEFAULT_FONT_COLOR[NORMAL_COLOR]);
	}
	else
	{
		AddGfxCommandText(x + BUTTON_HEIGHT / 2, y + BUTTON_HEIGHT / 2 - TEXT_HEIGHT / 2,
			ALIGN_LEFT, text, DEFAULT_FONT_COLOR[DISABLE_COLOR]);
	}
	
	return res;
}

bool im::Check(const char * text, bool checked, int32 width, bool enabled)
{
	GuiRenderer::gState.widgetId++;
	unsigned int id = (GuiRenderer::gState.areaId << 16) | GuiRenderer::gState.widgetId;

	int x = GuiRenderer::gState.widgetX;
	int y = GuiRenderer::gState.widgetY;
	int w = (width == 0) ? DEFAULT_BUTTON_WIDTH : width;
	int h = BUTTON_HEIGHT;
	GuiRenderer::gState.widgetY += BUTTON_HEIGHT + DEFAULT_SPACING;

	bool over = enabled && InRect(x, y, w, h);
	bool res = ButtonLogic(id, over);

	const int cx = x + BUTTON_HEIGHT / 2 - CHECK_SIZE / 2;
	const int cy = y + BUTTON_HEIGHT / 2 - CHECK_SIZE / 2;
	AddGfxCommandRect((float)cx - 3, (float)cy - 3, (float)CHECK_SIZE + 6, (float)CHECK_SIZE + 6,
		IsActive(id) ? DEFAULT_BUTTON_COLOR[HOT_COLOR] : DEFAULT_BUTTON_COLOR[NORMAL_COLOR]);

	//checkbox color
	if (checked)
	{
		if (enabled)
		{
			AddGfxCommandRect((float)cx, (float)cy, (float)CHECK_SIZE, (float)CHECK_SIZE,
				D3DCOLOR_ARGB(255, 255, 255, IsActive(id) ? 255 : 200));
		}
		else
		{
			AddGfxCommandRect((float)cx, (float)cy, (float)CHECK_SIZE, (float)CHECK_SIZE,
				D3DCOLOR_ARGB(128, 128, 128, 200));
		}
	}

	if (enabled)
	{
		AddGfxCommandText(x + BUTTON_HEIGHT, y + BUTTON_HEIGHT / 2 - TEXT_HEIGHT / 2,
			ALIGN_LEFT, text, IsHot(id) ? DEFAULT_FONT_COLOR[HOT_COLOR] : DEFAULT_FONT_COLOR[NORMAL_COLOR]);
	}
	else
	{
		AddGfxCommandText(x + BUTTON_HEIGHT, y + BUTTON_HEIGHT / 2 - TEXT_HEIGHT / 2,
			ALIGN_LEFT, text, DEFAULT_FONT_COLOR[DISABLE_COLOR]);
	}

	return res;
}

bool im::Collapse(const char * text, const char * subtext, bool checked, int32 width, bool enabled)
{
	GuiRenderer::gState.widgetId++;
	unsigned int id = (GuiRenderer::gState.areaId << 16) | GuiRenderer::gState.widgetId;

	int x = GuiRenderer::gState.widgetX;
	int y = GuiRenderer::gState.widgetY;
	int w = (width == 0) ? DEFAULT_BUTTON_WIDTH : width;
	int h = BUTTON_HEIGHT;
	GuiRenderer::gState.widgetY += BUTTON_HEIGHT + DEFAULT_SPACING;

	const int cx = x + BUTTON_HEIGHT / 2 - CHECK_SIZE / 2;
	const int cy = y + BUTTON_HEIGHT / 2 - CHECK_SIZE / 2;

	bool over = enabled && InRect(x, y, w, h);
	bool res = ButtonLogic(id, over);

	//color for checkbox
	if (checked)
	{
		AddGfxCommandTriangle(cx, cy, CHECK_SIZE, CHECK_SIZE, 2, 
			D3DCOLOR_ARGB(255, 255, 255, IsActive(id) ? 255 : 200));
	}
	else
	{
		AddGfxCommandTriangle(cx, cy, CHECK_SIZE, CHECK_SIZE, 1, 
			D3DCOLOR_ARGB(255, 255, 255, IsActive(id) ? 255 : 200));
	}

	if (enabled)
	{
		AddGfxCommandText(x + BUTTON_HEIGHT, y + BUTTON_HEIGHT / 2 - TEXT_HEIGHT / 2, ALIGN_LEFT,
			text, IsHot(id) ? DEFAULT_FONT_COLOR[HOT_COLOR] : DEFAULT_FONT_COLOR[NORMAL_COLOR]);
	}
	else
	{
		AddGfxCommandText(x + BUTTON_HEIGHT, y + BUTTON_HEIGHT / 2 - TEXT_HEIGHT / 2, ALIGN_LEFT, 
			text, DEFAULT_FONT_COLOR[DISABLE_COLOR]);
	}

	if (subtext)
	{
		AddGfxCommandText(x + w - BUTTON_HEIGHT / 2, y + BUTTON_HEIGHT / 2 - TEXT_HEIGHT / 2, ALIGN_RIGHT, 
			subtext, DEFAULT_FONT_COLOR[NORMAL_COLOR]);
	}
	return res;
}

void im::Label(const char * text)
{
	int x = GuiRenderer::gState.widgetX;
	int y = GuiRenderer::gState.widgetY;
	GuiRenderer::gState.widgetY += BUTTON_HEIGHT;
	AddGfxCommandText(x + DEFAULT_SPACING, y + BUTTON_HEIGHT / 2 - TEXT_HEIGHT / 2, ALIGN_LEFT, text, 
		DEFAULT_FONT_COLOR[NORMAL_COLOR]);
}

void im::Value(const char * text)
{
	const int x = GuiRenderer::gState.widgetX;
	const int y = GuiRenderer::gState.widgetY;
	const int w = DEFAULT_BUTTON_WIDTH;
	GuiRenderer::gState.widgetY += BUTTON_HEIGHT;

	AddGfxCommandText(x + w - BUTTON_HEIGHT / 2, y + BUTTON_HEIGHT / 2 - TEXT_HEIGHT / 2,
		ALIGN_LEFT, text, DEFAULT_FONT_COLOR[NORMAL_COLOR]);
}

bool im::Slider(const char * text, float * val, float vmin, float vmax, float vinc, int32 width, bool enabled)
{
	GuiRenderer::gState.widgetId++;
	unsigned int id = (GuiRenderer::gState.areaId << 16) | GuiRenderer::gState.widgetId;

	int x = GuiRenderer::gState.widgetX;
	int y = GuiRenderer::gState.widgetY;
	int w = (width == 0) ? DEFAULT_SLIDER_WIDTH : width;
	int h = SLIDER_HEIGHT;
	GuiRenderer::gState.widgetY += SLIDER_HEIGHT + DEFAULT_SPACING;

	AddGfxCommandRect((float)x, (float)y, (float)w, (float)h, DEFAULT_BUTTON_COLOR[NORMAL_COLOR]);

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
		if (GuiRenderer::gState.wentActive)
		{
			GuiRenderer::gState.dragX = GuiRenderer::gState.mx;
			GuiRenderer::gState.dragOrig = u;
		}
		if (GuiRenderer::gState.dragX != GuiRenderer::gState.mx)
		{
			u = GuiRenderer::gState.dragOrig + (float)(GuiRenderer::gState.mx - GuiRenderer::gState.dragX) / (float)range;
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
		AddGfxCommandRect((float)(x + m), (float)y, (float)SLIDER_MARKER_WIDTH, (float)SLIDER_HEIGHT, 
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	else
	{
		AddGfxCommandRect((float)(x + m), (float)y, (float)SLIDER_MARKER_WIDTH, (float)SLIDER_HEIGHT, 
			IsHot(id) ? D3DCOLOR_ARGB(210, 180, 180, 180) : D3DCOLOR_ARGB(180, 150, 150, 150));
	}

	// TODO: fix this, take a look at 'nicenum'.
	//int digits = (int)(ceilf(log10f(vinc)));
	//char fmt[16];
	//snprintf(fmt, 16, "%%.%df", digits >= 0 ? 0 : -digits);
	//char msg[128];
	//snprintf(msg, 128, fmt, *val);

	if (enabled)
	{
		AddGfxCommandText(x + SLIDER_HEIGHT / 2, y + SLIDER_HEIGHT / 2 - TEXT_HEIGHT / 2, ALIGN_LEFT, text, 
			IsHot(id) ? DEFAULT_FONT_COLOR[HOT_COLOR] : DEFAULT_FONT_COLOR[NORMAL_COLOR]);
		//AddGfxCommandText(x + SLIDER_HEIGHT / 2, y + SLIDER_HEIGHT / 2 - TEXT_HEIGHT / 2, ALIGN_LEFT, msg, 
		//	IsHot(id) ? DEFAULT_FONT_COLOR[HOT_COLOR] : DEFAULT_FONT_COLOR[NORMAL_COLOR]);
	}
	else
	{
		AddGfxCommandText(x + SLIDER_HEIGHT / 2, y + SLIDER_HEIGHT / 2 - TEXT_HEIGHT / 2, ALIGN_LEFT, 
			text, DEFAULT_FONT_COLOR[DISABLE_COLOR]);
		//AddGfxCommandText(x + w - SLIDER_HEIGHT / 2, y + SLIDER_HEIGHT / 2 - TEXT_HEIGHT / 2, ALIGN_LEFT, 
		//	msg, DEFAULT_FONT_COLOR[DISABLE_COLOR]);
	}

	return res || valChanged;
}

bool im::Edit(char * text, int32 width, bool enabled)
{
	GuiRenderer::gState.widgetId++;
	uint32 id = (GuiRenderer::gState.areaId << 16) | GuiRenderer::gState.widgetId;

	int32 x = GuiRenderer::gState.widgetX;
	int32 y = GuiRenderer::gState.widgetY/* - BUTTON_HEIGHT*/;
	int32 w = (width == 0) ? DEFAULT_BUTTON_WIDTH : width;
	int32 h = BUTTON_HEIGHT;
	GuiRenderer::gState.widgetY += BUTTON_HEIGHT + DEFAULT_SPACING;

	bool over = enabled && InRect(x, y, w, h);

	bool mouseResult = ButtonLogic(id, over);
	if (mouseResult)
	{
		GuiRenderer::gState.keyboardFocusID = id;
	}

	bool keyResult = KeyLogic(id);

	if (GuiRenderer::gState.charInput && keyResult)
	{
		int32 index = strlen(text);
		if (GuiRenderer::gState.charInput == VK_BACK)
		{
			if (index > 0)
			{
				text[index - 1] = '\0';
			}
		}
		else
		{
			text[index] = GuiRenderer::gState.charInput;
		}
	}

	AddGfxCommandRect((float)x, (float)y, (float)w, (float)h,
		IsActive(id) ? DEFAULT_BUTTON_COLOR[HOT_COLOR] : DEFAULT_BUTTON_COLOR[NORMAL_COLOR]);

	if (enabled)
	{
		AddGfxCommandText(x + BUTTON_HEIGHT / 2, y + BUTTON_HEIGHT / 2 - TEXT_HEIGHT / 2,
			ALIGN_LEFT, text, IsHot(id) ? DEFAULT_FONT_COLOR[HOT_COLOR] : DEFAULT_FONT_COLOR[NORMAL_COLOR]);
	}
	else
	{
		AddGfxCommandText(x + BUTTON_HEIGHT / 2, y + BUTTON_HEIGHT / 2 - TEXT_HEIGHT / 2,
			ALIGN_LEFT, text, DEFAULT_FONT_COLOR[DISABLE_COLOR]);
	}

	return keyResult;
}

//void im::DrawFontCommand(int32 x, int32 y, int32 align, const char * text, D3DCOLOR color)
//{
//	AddGfxCommandText(x, y, align, text, color);
//}
//
//void im::DrawLineCommand(float x0, float y0, float x1, float y1, float r, D3DCOLOR color)
//{
//	AddGfxCmdLine(x0, y0, x1, y1, r, color);
//}
//
//void im::DrawRectCommand(float x, float y, float w, float h, D3DCOLOR color)
//{
//	AddGfxCommandRect(x, y, w, h, color);
//}