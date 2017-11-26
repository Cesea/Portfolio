#include "stdafx.h"
#include "imguiRenderer.h"

#include "imgui.h"

static IDirect3DStateBlock9 *gStateBlock = nullptr;

static Matrix gIMViewMatrix;
static Matrix gIMProjectionMatrix;

video::FontHandle gFontHandle;


struct UIVertex
{
	Vector3 _position;
	uint32 _color;

	enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };
};

struct UITextureVertex
{
	Vector3 _position;
	Vector2 _texcoord;
	uint32 _color;

	enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_DIFFUSE };

};


void DrawRect(int32 x0, int32 y0, int32 x1, int32 y1, D3DCOLOR color)
{
	UIVertex vertices[4];
	vertices[0]._color = color;
	vertices[1]._color = color;
	vertices[2]._color = color;
	vertices[3]._color = color;

	vertices[0]._position.x = (float)x0;
	vertices[0]._position.y = (float)y0;
	vertices[0]._position.z = 0.0f;

	vertices[1]._position.x = (float)x1;
	vertices[1]._position.y = (float)y0;
	vertices[1]._position.z = 0.0f;

	vertices[2]._position.x = (float)x1;
	vertices[2]._position.y = (float)y1;
	vertices[2]._position.z = 0.0f;

	vertices[3]._position.x = (float)x0;
	vertices[3]._position.y = (float)y1;
	vertices[3]._position.z = 0.0f;
	
	uint16 indices[6];
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;

	gpDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
	gpDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	gpDevice->SetFVF(UIVertex::FVF);
	HRESULT re = gpDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, indices, D3DFMT_INDEX16,
		vertices, sizeof(UIVertex));
}

void DrawLine(int32 x0, int32 y0, int32 x1, int32 y1, D3DCOLOR color)
{
	UIVertex vertices[2];
	vertices[0]._color = color;
	vertices[1]._color = color;

	vertices[0]._position[0] = (float)x0;
	vertices[0]._position[1] = (float)y0;
	vertices[0]._position[2] = 0.0f;

	vertices[1]._position[0] = (float)x1;
	vertices[1]._position[1] = (float)y1;
	vertices[1]._position[2] = 0.0f;

	gpDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
	gpDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	gpDevice->SetFVF(UIVertex::FVF);
	gpDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, vertices, sizeof(UIVertex));
}

void DrawTexture(int32 x0, int32 y0, int32 x1, int32 y1, video::TextureHandle handle)
{
	//UIVertex vertices[4];
	//vertices[0]._color = 0xffffffff;
	//vertices[1]._color = 0xffffffff;
	//vertices[2]._color = 0xffffffff;
	//vertices[3]._color = 0xffffffff;

	//vertices[0]._position.x = (float)x0;
	//vertices[0]._position.y = (float)y0;
	//vertices[0]._position.z = 0.0f;

	//vertices[1]._position.x = (float)x1;
	//vertices[1]._position.y = (float)y0;
	//vertices[1]._position.z = 0.0f;

	//vertices[2]._position.x = (float)x1;
	//vertices[2]._position.y = (float)y1;
	//vertices[2]._position.z = 0.0f;

	//vertices[3]._position.x = (float)x0;
	//vertices[3]._position.y = (float)y1;
	//vertices[3]._position.z = 0.0f;

	//uint16 indices[6];
	//indices[0] = 0; indices[1] = 1; indices[2] = 2;
	//indices[3] = 0; indices[4] = 2; indices[5] = 3;

	//gpDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
	//gpDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	//gpDevice->SetFVF(UIVertex::FVF);

	//gpDevice->SetTexture(0, VIDEO->GetTexture(handle)->_ptr);
	//HRESULT re = gpDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, indices, D3DFMT_INDEX16,
	//	vertices, sizeof(UIVertex));
	//gpDevice->SetTexture(0, nullptr);
}

void DrawFont(int32 id, int32 x, int32 y, uint32 color, const char *str, DWORD format)
{
	RECT rect = { x, y, 0, 0 };
	VIDEO->DrawFont(gFontHandle, str, x, y, color);
	//_fonts[id]->DrawTextA(nullptr, str, -1, &rect, format | DT_CALCRECT, 0);
	//_fonts[id]->DrawTextA(nullptr, str, -1, &rect, format, color);
}

void DrawTriangle(int32 x0, int32 y0, int32 x1, int32 y1, int32 x2, int32 y2, D3DCOLOR color)
{
	UIVertex vertices[3];
	vertices[0]._color = color;
	vertices[1]._color = color;
	vertices[2]._color = color;

	vertices[0]._position[0] = (float)x0;
	vertices[0]._position[1] = (float)y0;
	vertices[0]._position[2] = 0.0f;

	vertices[1]._position[0] = (float)x1;
	vertices[1]._position[1] = (float)y1;
	vertices[1]._position[2] = 0.0f;

	vertices[2]._position[0] = (float)x2;
	vertices[2]._position[1] = (float)y2;
	vertices[2]._position[2] = 0.0f;

	//gDevice->SetRenderState
	gpDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
	gpDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	gpDevice->SetFVF(UIVertex::FVF);
	gpDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, vertices, sizeof(UIVertex));
}

bool imguiRenderInit()
{
	gStateBlock = nullptr;

	MatrixIdentity(&gIMViewMatrix);
	MatrixIdentity(&gIMProjectionMatrix);

	gIMProjectionMatrix._11 = 2.0f / (float)(WINSIZEX);
	gIMProjectionMatrix._22 = 2.0f / (float)(WINSIZEY);
	gIMProjectionMatrix._33 = 1.0f / (float)(0.1f - 100.0f);
	gIMProjectionMatrix._43 = 0.1f * (0.1f / (100.0f - 0.1f));
	gIMProjectionMatrix._44 = 1.0f;

	float tx, ty, tz;
	tx = (WINSIZEX)-((float)WINSIZEX / 2.0f);
	ty = (WINSIZEY)-((float)WINSIZEY / 2.0f);
	tz = 0.1f + 0.1f;

	gIMViewMatrix._22 = -1.0f;
	gIMViewMatrix._41 -= tx;
	gIMViewMatrix._42 = ty;
	gIMViewMatrix._43 = -tz;

	D3DXFONT_DESC fontDesc{};

	fontDesc.Height = 10;
	fontDesc.Width = 6;
	fontDesc.Weight = 10;
	fontDesc.MipLevels = 0;
	fontDesc.Italic = false;
	fontDesc.CharSet = DEFAULT_CHARSET;
	fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
	fontDesc.Quality = DEFAULT_QUALITY;
	fontDesc.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;

	strncpy(fontDesc.FaceName, "Consolas", strlen("Consolas"));

	gFontHandle = VIDEO->CreateFont(fontDesc, "guiDefaultFont");

	return true;
}

void imguiRenderDestroy()
{
	VIDEO->DestroyFont(gFontHandle);
	COM_RELEASE(gStateBlock);
}

void imguiRenderDraw()
{
	if (nullptr == gStateBlock )
	{
		gpDevice->CreateStateBlock(D3DSBT_ALL, &gStateBlock);
		gStateBlock->Capture();
	}
	else
	{
		gStateBlock->Capture();
	}

	gpDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	gpDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	gpDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	gpDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	gpDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	gpDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	gpDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	gpDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, 0);
	gpDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	gpDevice->SetRenderState(D3DRS_LASTPIXEL, FALSE);
	gpDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
	gpDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	gpDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0000000F);
	gpDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
	gpDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);
	gpDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	gpDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	gpDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	gpDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	gpDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_PASSTHRU);
	gpDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	gpDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	gpDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	gpDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	gpDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	gpDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	gpDevice->SetVertexShader(NULL);
	gpDevice->SetPixelShader(NULL);

	gpDevice->SetTransform(D3DTS_VIEW, &gIMViewMatrix);
	gpDevice->SetTransform(D3DTS_PROJECTION, &gIMProjectionMatrix);


	const ImguiCommand *queue = ImguiGetRenderQueue();
	int32 numQueue = ImguiGetRenderQueueSize();

	for (int32 i = 0; i < numQueue; ++i)
	{
		const ImguiCommand &cmd = queue[i];
		if (cmd._type == ImguiCommandType::eCommandRect )
		{
			DrawRect((int32)cmd._rect._x, (int32)cmd._rect._y,
				(int32)cmd._rect._x + cmd._rect._w, (int32)cmd._rect._y + cmd._rect._h, cmd._color);
		}
		else if (cmd._type == ImguiCommandType::eCommandLine)
		{
			DrawLine(cmd._line._x0, cmd._line._y0, cmd._line._x1, cmd._line._y1, cmd._color);
		}
		else if (cmd._type == ImguiCommandType::eCommandTriangle)
		{
			if (cmd._flags == 1)
			{
				DrawTriangle(cmd._rect._x + cmd._rect._w / 2, cmd._rect._y,
					cmd._rect._x + cmd._rect._w, cmd._rect._y + cmd._rect._h,
					cmd._rect._x, cmd._rect._y + cmd._rect._h,
					cmd._color);
			}
			if (cmd._flags == 2)
			{
				DrawTriangle(cmd._rect._x + cmd._rect._w / 2, cmd._rect._y + cmd._rect._h,
					cmd._rect._x, cmd._rect._y,
					cmd._rect._x + cmd._rect._w, cmd._rect._y,
					cmd._color);
			}
		}
		else if (cmd._type == ImguiCommandType::eCommandText)
		{
			DrawFont(0, cmd._text._x, cmd._text._y, cmd._color, cmd._text._text, cmd._text._align);
		}
		//else if (cmd._type == ImguiCommandType::eCommandTexture)
		//{
		//	DrawTexture(cmd._texture._x, cmd._texture._y, 
		//		cmd._texture._x + cmd._texture._w, cmd._texture._y + cmd._texture._h, 
		//		cmd._texture._handle);
		//}
		else if (cmd._type == ImguiCommandType::eCommandScissor)
		{
			if (cmd._flags)
			{
				gpDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, true);
				gpDevice->SetScissorRect(&RECTMake(cmd._rect._x, cmd._rect._y, cmd._rect._x + cmd._rect._w, cmd._rect._y + cmd._rect._h));
			}
			else
			{
				gpDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
			}
		}
	}
	gpDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
	gStateBlock->Apply();
}