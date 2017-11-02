#include "stdafx.h"
#include "imguiRenderer.h"

#include "imgui.h"

using namespace im;

GfxCommand GuiRenderer::gfxCommandQueue[GFXCOMMAND_QUEUE_SIZE];
unsigned GuiRenderer::gfxCommandQueueSize;

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
	_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
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

	//glDisable(GL_SCISSOR_TEST);
	for (int i = 0; i < numQueue; ++i)
	{
		const GfxCommand& cmd = queue[i];
		if (cmd.type == GFXCOMMAND_RECT)
		{
			DrawRect((int32)cmd.rect.x, (int32)cmd.rect.y,
				(int32)cmd.rect.x + cmd.rect.w, (int32)cmd.rect.y + cmd.rect.h, cmd.color);
		}
		else if (cmd.type == GFXCOMMAND_LINE)
		{
			DrawLine(cmd.line.x0, cmd.line.y0, cmd.line.x1, cmd.line.y1, cmd.color);
		}
		else if (cmd.type == GFXCOMMAND_TRIANGLE)
		{
			//if (cmd.flags == 1)
			//{
			//	const float verts[3 * 2] =
			//	{
			//		(float)cmd.rect.x + 0.5f, (float)cmd.rect.y + 0.5f,
			//		(float)cmd.rect.x + 0.5f + (float)cmd.rect.w - 1, (float)cmd.rect.y + 0.5f + (float)cmd.rect.h / 2 - 0.5f,
			//		(float)cmd.rect.x + 0.5f, (float)cmd.rect.y + 0.5f + (float)cmd.rect.h - 1,
			//	};
			//	drawPolygon(verts, 3, 1.0f, cmd.col);
			//}
			//if (cmd.flags == 2)
			//{
			//	const float verts[3 * 2] =
			//	{
			//		(float)cmd.rect.x + 0.5f, (float)cmd.rect.y + 0.5f + (float)cmd.rect.h - 1,
			//		(float)cmd.rect.x + 0.5f + (float)cmd.rect.w / 2 - 0.5f, (float)cmd.rect.y + 0.5f,
			//		(float)cmd.rect.x + 0.5f + (float)cmd.rect.w - 1, (float)cmd.rect.y + 0.5f + (float)cmd.rect.h - 1,
			//	};
			//	drawPolygon(verts, 3, 1.0f, cmd.col);
			//}
		}
		else if (cmd.type == GFXCOMMAND_TEXT)
		{
			DrawFont(0, cmd.text.x, cmd.text.y, cmd.color, cmd.text.text, cmd.text.align);
		}
		//else if (cmd.type == GFXCMD_STENCIL)
		//{
		//	if (cmd.flags)
		//	{
		//		glEnable(GL_SCISSOR_TEST);
		//		glScissor(cmd.rect.x, cmd.rect.y, cmd.rect.w, cmd.rect.h);
		//	}
		//	else
		//	{
		//		glDisable(GL_SCISSOR_TEST);
		//	}
		//}
	}
	//glDisable(GL_SCISSOR_TEST);
}


void im::GuiRenderer::DrawRect(int32 x0, int32 y0, int32 x1, int32 y1, uint32 color)
{
	Vertex vertices[4];
	vertices[0].color = color;
	vertices[1].color = color;
	vertices[2].color = color;
	vertices[3].color = color;

	vertices[0].position[0] = x0;
	vertices[0].position[1] = y0;
	vertices[0].position[2] = 0.0f;

	vertices[1].position[0] = x1;
	vertices[1].position[1] = y0;
	vertices[1].position[2] = 0.0f;

	vertices[2].position[0] = x1;
	vertices[2].position[1] = y1;
	vertices[2].position[2] = 0.0f;

	vertices[3].position[0] = x0;
	vertices[3].position[1] = y1;
	vertices[3].position[2] = 0.0f;

	uint16 indices[6];
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;

	_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
	_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	_pDevice->SetFVF(Vertex::FVF);
	_pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, indices, D3DFMT_INDEX16, vertices, sizeof(Vertex));
}

void im::GuiRenderer::DrawLine(int32 x0, int32 y0, int32 x1, int32 y1, uint32 color)
{
	Vertex vertices[2];
	vertices[0].color = color;
	vertices[1].color = color;
	vertices[0].position[0] = x0;
	vertices[0].position[1] = y0;
	vertices[0].position[2] = 0.0f;

	vertices[1].position[0] = x1;
	vertices[1].position[1] = y1;
	vertices[1].position[2] = 0.0f;

	//gDevice->SetRenderState
	_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
	_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	_pDevice->SetFVF(Vertex::FVF);
	_pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, vertices, sizeof(Vertex));
}

void im::GuiRenderer::DrawFont(int32 id, int32 x, int32 y, uint32 color, const char *str, DWORD format)
{
	RECT rect = { x, y, 0, 0 };
	_fonts[id]->DrawText(nullptr, str, -1, &rect, format | DT_CALCRECT, 0);
	_fonts[id]->DrawText(nullptr, str, -1, &rect, format, color);
}


static void ResetGfxCmdQueue()
{
	GuiRenderer::gfxCommandQueueSize = 0;
}

static void AddGfxCmdStencil(int32 x, int32 y, int32 w, int32 h)
{
	if (GuiRenderer::gfxCommandQueueSize >= GFXCOMMAND_QUEUE_SIZE)
		return;
	GfxCommand& cmd = GuiRenderer::gfxCommandQueue[GuiRenderer::gfxCommandQueueSize++];
	cmd.type = GFXCOMMAND_STENCIL;
	cmd.flags = x < 0 ? 0 : 1;      // on/off flag.
	cmd.color = 0;
	cmd.rect.x = (int16)x;
	cmd.rect.y = (int16)y;
	cmd.rect.w = (int16)w;
	cmd.rect.h = (int16)h;
}

static void AddGfxCmdRect(float x, float y, float w, float h, uint32 color)
{
	if (GuiRenderer::gfxCommandQueueSize >= GFXCOMMAND_QUEUE_SIZE)
		return;
	GfxCommand& cmd = GuiRenderer::gfxCommandQueue[GuiRenderer::gfxCommandQueueSize++];
	cmd.type = GFXCOMMAND_RECT;
	cmd.flags = 0;
	cmd.color = color;
	cmd.rect.x = (int16)(x*8.0f);
	cmd.rect.y = (int16)(y*8.0f);
	cmd.rect.w = (int16)(w*8.0f);
	cmd.rect.h = (int16)(h*8.0f);
}

static void AddGfxCmdLine(float x0, float y0, float x1, float y1, float r, uint32 color)
{
	if (GuiRenderer::gfxCommandQueueSize >= GFXCOMMAND_QUEUE_SIZE)
		return;
	GfxCommand& cmd = GuiRenderer::gfxCommandQueue[GuiRenderer::gfxCommandQueueSize++];
	cmd.type = GFXCOMMAND_LINE;
	cmd.flags = 0;
	cmd.color = color;
	cmd.line.x0 = (int16)(x0*8.0f);
	cmd.line.y0 = (int16)(y0*8.0f);
	cmd.line.x1 = (int16)(x1*8.0f);
	cmd.line.y1 = (int16)(y1*8.0f);
}


static void AddGfxCmdRoundedRect(float x, float y, float w, float h, float r, uint32 color)
{
	if (GuiRenderer::gfxCommandQueueSize >= GFXCOMMAND_QUEUE_SIZE)
		return;
	GfxCommand& cmd = GuiRenderer::gfxCommandQueue[GuiRenderer::gfxCommandQueueSize++];
	cmd.type = GFXCOMMAND_RECT;
	cmd.flags = 0;
	cmd.color = color;
	cmd.rect.x = (int16)(x*8.0f);
	cmd.rect.y = (int16)(y*8.0f);
	cmd.rect.w = (int16)(w*8.0f);
	cmd.rect.h = (int16)(h*8.0f);
}

static void AddGfxCmdTriangle(int32 x, int32 y, int32 w, int32 h, int32 flags, uint32 color)
{
	
	if (GuiRenderer::gfxCommandQueueSize >= GFXCOMMAND_QUEUE_SIZE)
		return;
	GfxCommand& cmd = GuiRenderer::gfxCommandQueue[GuiRenderer::gfxCommandQueueSize++];
	cmd.type = GFXCOMMAND_TRIANGLE;
	cmd.flags = (char)flags;
	cmd.color = color;
	cmd.rect.x = (int16)(x*8.0f);
	cmd.rect.y = (int16)(y*8.0f);
	cmd.rect.w = (int16)(w*8.0f);
	cmd.rect.h = (int16)(h*8.0f);
}

static void AddGfxCmdText(int32 x, int32 y, int32 align, const char* text, uint32 color)
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


void im::BeginFrame(int32 mx, int32 my, uint8 mbut, int32 scroll)
{
	UpdateInput(mx, my, mbut, scroll);

	gState.hot = gState.hotToBe;
	gState.hotToBe = 0;

	gState.wentActive = false;
	gState.isActive = false;
	gState.isHot = false;

	gState.widgetX = 0;
	gState.widgetY = 0;
	gState.widgetW = 0;

	gState.areaId = 1;
	gState.widgetId = 1;

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
	gState.areaId++;
	gState.widgetId = 0;
	gScrollId = (gState.areaId << 16) | gState.widgetId;

	gState.widgetX = x + SCROLL_AREA_PADDING;
	gState.widgetY = y + h - AREA_HEADER + (*scroll);
	gState.widgetW = w - SCROLL_AREA_PADDING * 4;
	gScrollTop = y - AREA_HEADER + h;
	gScrollBottom = y + SCROLL_AREA_PADDING;
	gScrollRight = x + w - SCROLL_AREA_PADDING * 3;
	gScrollVal = scroll;

	gScrollAreaTop = gState.widgetY;

	gFocusTop = y - AREA_HEADER;
	gFocusBottom = y - AREA_HEADER + h;

	gInsideScrollArea = InRect(x, y, w, h, false);
	gState.insideCurrentScroll = gInsideScrollArea;

	AddGfxCmdRoundedRect((float)x, (float)y, (float)w, (float)h, 6, RGBA(0, 0, 0, 192));

	//AddGfxCmdText(x + AREA_HEADER / 2, y + h - AREA_HEADER / 2 - TEXT_HEIGHT / 2, IMGUI_ALIGN_LEFT, name, RGBA(255, 255, 255, 128));

	AddGfxCmdStencil(x + SCROLL_AREA_PADDING, y + SCROLL_AREA_PADDING, w - SCROLL_AREA_PADDING * 4, h - AREA_HEADER - SCROLL_AREA_PADDING);

	return gInsideScrollArea;
}

void im::EndScrollArea()
{
	// Disable scissoring.
	AddGfxCmdStencil(-1, -1, -1, -1);

	// Draw scroll bar
	int x = gScrollRight + SCROLL_AREA_PADDING / 2;
	int y = gScrollBottom;
	int w = SCROLL_AREA_PADDING * 2;
	int h = gScrollTop - gScrollBottom;

	int stop = gScrollAreaTop;
	int sbot = gState.widgetY;
	int sh = stop - sbot; // The scrollable area height.

	float barHeight = (float)h / (float)sh;

	if (barHeight < 1)
	{
		float barY = (float)(y - sbot) / (float)sh;
		if (barY < 0) barY = 0;
		if (barY > 1) barY = 1;

		// Handle scroll bar logic.
		unsigned int hid = gScrollId;
		int hx = x;
		int hy = y + (int)(barY*h);
		int hw = w;
		int hh = (int)(barHeight*h);

		const int range = h - (hh - 1);
		bool over = InRect(hx, hy, hw, hh);
		ButtonLogic(hid, over);
		if (IsActive(hid))
		{
			float u = (float)(hy - y) / (float)range;
			if (gState.wentActive)
			{
				gState.dragY = gState.my;
				gState.dragOrig = u;
			}
			if (gState.dragY != gState.my)
			{
				u = gState.dragOrig + (gState.my - gState.dragY) / (float)range;
				if (u < 0) u = 0;
				if (u > 1) u = 1;
				*gScrollVal = (int)((1 - u) * (sh - h));
			}
		}

		// BG
		AddGfxCmdRoundedRect((float)x, (float)y, (float)w, (float)h, (float)w / 2 - 1, RGBA(0, 0, 0, 196));
		// Bar
		if (IsActive(hid))
			AddGfxCmdRoundedRect((float)hx, (float)hy, (float)hw, (float)hh, (float)w / 2 - 1, RGBA(255, 196, 0, 196));
		else
			AddGfxCmdRoundedRect((float)hx, (float)hy, (float)hw, (float)hh, (float)w / 2 - 1, IsHot(hid) ? RGBA(255, 196, 0, 96) : RGBA(255, 255, 255, 64));

		// Handle mouse scrolling.
		if (gInsideScrollArea) // && !anyActive())
		{
			if (gState.scroll)
			{
				*gScrollVal += 20 * gState.scroll;
				if (*gScrollVal< 0) *gScrollVal = 0;
				if (*gScrollVal >(sh - h)) *gScrollVal = (sh - h);
			}
		}
	}
	gState.insideCurrentScroll = false;
}

void im::Indent()
{
	gState.widgetX += INDENT_SIZE;
	gState.widgetW -= INDENT_SIZE;
}

void im::Unindent()
{
	gState.widgetX -= INDENT_SIZE;
	gState.widgetW += INDENT_SIZE;
}

void im::imguiSeparator()
{
	gState.widgetY -= DEFAULT_SPACING * 3;
}

void im::SeparatorLine()
{
	int x = gState.widgetX;
	int y = gState.widgetY - DEFAULT_SPACING * 2;
	int w = gState.widgetW;
	int h = 1;
	gState.widgetY -= DEFAULT_SPACING * 4;

	AddGfxCmdRect((float)x, (float)y, (float)w, (float)h, RGBA(255, 255, 255, 32));
}

bool im::Button(const char * text, bool enabled)
{
	gState.widgetId++;
	uint32 id = (gState.areaId << 16) | gState.widgetId;

	int x = gState.widgetX;
	int y = gState.widgetY - BUTTON_HEIGHT;
	int w = gState.widgetW;
	int h = BUTTON_HEIGHT;
	gState.widgetY -= BUTTON_HEIGHT + DEFAULT_SPACING;

	bool over = enabled && InRect(x, y, w, h);
	bool res = ButtonLogic(id, over);

	AddGfxCmdRoundedRect((float)x, (float)y, (float)w, (float)h, (float)BUTTON_HEIGHT / 2 - 1, RGBA(128, 128, 128, IsActive(id) ? 196 : 96));
	if (enabled)
	{
		AddGfxCmdText(x + BUTTON_HEIGHT / 2, y + BUTTON_HEIGHT / 2 - TEXT_HEIGHT / 2,
			ALIGN_LEFT, text, IsHot(id) ? RGBA(255, 196, 0, 255) : RGBA(255, 255, 255, 200));
	}
	else
	{
		AddGfxCmdText(x + BUTTON_HEIGHT / 2, y + BUTTON_HEIGHT / 2 - TEXT_HEIGHT / 2,
			ALIGN_LEFT, text, RGBA(128, 128, 128, 200));
	}

	return res;
}

bool im::Item(const char * text, bool enabled)
{
	gState.widgetId++;
	unsigned int id = (gState.areaId << 16) | gState.widgetId;

	int32 x = gState.widgetX;
	int32 y = gState.widgetY - BUTTON_HEIGHT;
	int32 w = gState.widgetW;
	int32 h = BUTTON_HEIGHT;
	gState.widgetY -= BUTTON_HEIGHT + DEFAULT_SPACING;

	bool over = enabled && InRect(x, y, w, h);
	bool res = ButtonLogic(id, over);

	if (IsHot(id))
	{
		AddGfxCmdRoundedRect((float)x, (float)y, (float)w, (float)h, 2.0f, RGBA(255, 196, 0, IsActive(id) ? 196 : 96));
	}

	if (enabled)
		AddGfxCmdText(x + BUTTON_HEIGHT / 2, y + BUTTON_HEIGHT / 2 - TEXT_HEIGHT / 2,
			ALIGN_LEFT, text, RGBA(255, 255, 255, 200));
	else
		AddGfxCmdText(x + BUTTON_HEIGHT / 2, y + BUTTON_HEIGHT / 2 - TEXT_HEIGHT / 2,
			ALIGN_LEFT, text, RGBA(128, 128, 128, 200));

	return res;
}

bool im::Check(const char * text, bool checked, bool enabled)
{
	gState.widgetId++;
	unsigned int id = (gState.areaId << 16) | gState.widgetId;

	int x = gState.widgetX;
	int y = gState.widgetY - BUTTON_HEIGHT;
	int w = gState.widgetW;
	int h = BUTTON_HEIGHT;
	gState.widgetY -= BUTTON_HEIGHT + DEFAULT_SPACING;

	bool over = enabled && InRect(x, y, w, h);
	bool res = ButtonLogic(id, over);

	const int cx = x + BUTTON_HEIGHT / 2 - CHECK_SIZE / 2;
	const int cy = y + BUTTON_HEIGHT / 2 - CHECK_SIZE / 2;
	AddGfxCmdRoundedRect((float)cx - 3, (float)cy - 3, (float)CHECK_SIZE + 6, (float)CHECK_SIZE + 6, 4, RGBA(128, 128, 128, IsActive(id) ? 196 : 96));
	if (checked)
	{
		if (enabled)
		{
			AddGfxCmdRoundedRect((float)cx, (float)cy, (float)CHECK_SIZE, (float)CHECK_SIZE, (float)CHECK_SIZE / 2 - 1,
				RGBA(255, 255, 255, IsActive(id) ? 255 : 200));
		}
		else
		{
			AddGfxCmdRoundedRect((float)cx, (float)cy, (float)CHECK_SIZE, (float)CHECK_SIZE, (float)CHECK_SIZE / 2 - 1,
				RGBA(128, 128, 128, 200));
		}
	}

	if (enabled)
	{
		AddGfxCmdText(x + BUTTON_HEIGHT, y + BUTTON_HEIGHT / 2 - TEXT_HEIGHT / 2,
			ALIGN_LEFT, text, IsHot(id) ? RGBA(255, 196, 0, 255) : RGBA(255, 255, 255, 200));
	}
	else
	{
		AddGfxCmdText(x + BUTTON_HEIGHT, y + BUTTON_HEIGHT / 2 - TEXT_HEIGHT / 2,
			ALIGN_LEFT, text, RGBA(128, 128, 128, 200));
	}

	return res;
}

bool im::Collapse(const char * text, const char * subtext, bool checked, bool enabled)
{
	gState.widgetId++;
	unsigned int id = (gState.areaId << 16) | gState.widgetId;

	int x = gState.widgetX;
	int y = gState.widgetY - BUTTON_HEIGHT;
	int w = gState.widgetW;
	int h = BUTTON_HEIGHT;
	gState.widgetY -= BUTTON_HEIGHT; // + DEFAULT_SPACING;

	const int cx = x + BUTTON_HEIGHT / 2 - CHECK_SIZE / 2;
	const int cy = y + BUTTON_HEIGHT / 2 - CHECK_SIZE / 2;

	bool over = enabled && InRect(x, y, w, h);
	bool res = ButtonLogic(id, over);

	if (checked)
		AddGfxCmdTriangle(cx, cy, CHECK_SIZE, CHECK_SIZE, 2, RGBA(255, 255, 255, IsActive(id) ? 255 : 200));
	else
		AddGfxCmdTriangle(cx, cy, CHECK_SIZE, CHECK_SIZE, 1, RGBA(255, 255, 255, IsActive(id) ? 255 : 200));

	if (enabled)
		AddGfxCmdText(x + BUTTON_HEIGHT, y + BUTTON_HEIGHT / 2 - TEXT_HEIGHT / 2, ALIGN_LEFT, text, IsHot(id) ? RGBA(255, 196, 0, 255) : RGBA(255, 255, 255, 200));
	else
		AddGfxCmdText(x + BUTTON_HEIGHT, y + BUTTON_HEIGHT / 2 - TEXT_HEIGHT / 2, ALIGN_LEFT, text, RGBA(128, 128, 128, 200));

	if (subtext)
		AddGfxCmdText(x + w - BUTTON_HEIGHT / 2, y + BUTTON_HEIGHT / 2 - TEXT_HEIGHT / 2, ALIGN_RIGHT, subtext, RGBA(255, 255, 255, 128));

	return res;
}

void im::Label(const char * text)
{
	int x = gState.widgetX;
	int y = gState.widgetY - BUTTON_HEIGHT;
	gState.widgetY -= BUTTON_HEIGHT;
	AddGfxCmdText(x, y + BUTTON_HEIGHT / 2 - TEXT_HEIGHT / 2, ALIGN_LEFT, text, RGBA(255, 255, 255, 255));
}

void im::Value(const char * text)
{
	const int x = gState.widgetX;
	const int y = gState.widgetY - BUTTON_HEIGHT;
	const int w = gState.widgetW;
	gState.widgetY -= BUTTON_HEIGHT;

	AddGfxCmdText(x + w - BUTTON_HEIGHT / 2, y + BUTTON_HEIGHT / 2 - TEXT_HEIGHT / 2, ALIGN_RIGHT, text, RGBA(255, 255, 255, 200));
}

bool im::Slider(const char * text, float * val, float vmin, float vmax, float vinc, bool enabled)
{
	gState.widgetId++;
	unsigned int id = (gState.areaId << 16) | gState.widgetId;

	int x = gState.widgetX;
	int y = gState.widgetY - BUTTON_HEIGHT;
	int w = gState.widgetW;
	int h = SLIDER_HEIGHT;
	gState.widgetY -= SLIDER_HEIGHT + DEFAULT_SPACING;

	AddGfxCmdRoundedRect((float)x, (float)y, (float)w, (float)h, 4.0f, RGBA(0, 0, 0, 128));

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
		if (gState.wentActive)
		{
			gState.dragX = gState.mx;
			gState.dragOrig = u;
		}
		if (gState.dragX != gState.mx)
		{
			u = gState.dragOrig + (float)(gState.mx - gState.dragX) / (float)range;
			if (u < 0) u = 0;
			if (u > 1) u = 1;
			*val = vmin + u*(vmax - vmin);
			*val = floorf(*val / vinc + 0.5f)*vinc; // Snap to vinc
			m = (int)(u * range);
			valChanged = true;
		}
	}

	if (IsActive(id))
		AddGfxCmdRoundedRect((float)(x + m), (float)y, (float)SLIDER_MARKER_WIDTH, (float)SLIDER_HEIGHT, 4.0f, RGBA(255, 255, 255, 255));
	else
		AddGfxCmdRoundedRect((float)(x + m), (float)y, (float)SLIDER_MARKER_WIDTH, (float)SLIDER_HEIGHT, 4.0f, IsHot(id) ? RGBA(255, 196, 0, 128) : RGBA(255, 255, 255, 64));

	// TODO: fix this, take a look at 'nicenum'.
	int digits = (int)(ceilf(log10f(vinc)));
	char fmt[16];
	snprintf(fmt, 16, "%%.%df", digits >= 0 ? 0 : -digits);
	char msg[128];
	snprintf(msg, 128, fmt, *val);

	if (enabled)
	{
		AddGfxCmdText(x + SLIDER_HEIGHT / 2, y + SLIDER_HEIGHT / 2 - TEXT_HEIGHT / 2, ALIGN_LEFT, text, IsHot(id) ? RGBA(255, 196, 0, 255) : RGBA(255, 255, 255, 200));
		AddGfxCmdText(x + w - SLIDER_HEIGHT / 2, y + SLIDER_HEIGHT / 2 - TEXT_HEIGHT / 2, ALIGN_RIGHT, msg, IsHot(id) ? RGBA(255, 196, 0, 255) : RGBA(255, 255, 255, 200));
	}
	else
	{
		AddGfxCmdText(x + SLIDER_HEIGHT / 2, y + SLIDER_HEIGHT / 2 - TEXT_HEIGHT / 2, ALIGN_LEFT, text, RGBA(128, 128, 128, 200));
		AddGfxCmdText(x + w - SLIDER_HEIGHT / 2, y + SLIDER_HEIGHT / 2 - TEXT_HEIGHT / 2, ALIGN_RIGHT, msg, RGBA(128, 128, 128, 200));
	}

	return res || valChanged;
}

void im::DrawFont(int32 x, int32 y, int32 align, const char * text, uint32 color)
{
	AddGfxCmdText(x, y, align, text, color);
}

void im::DrawLine(float x0, float y0, float x1, float y1, float r, uint32 color)
{
	AddGfxCmdLine(x0, y0, x1, y1, r, color);
}

void im::DrawRoundedRect(float x, float y, float w, float h, float r, uint32 color)
{
	AddGfxCmdRoundedRect(x, y, w, h, r, color);
}

void im::DrawRect(float x, float y, float w, float h, uint32 color)
{
	AddGfxCmdRect(x, y, w, h, color);
}