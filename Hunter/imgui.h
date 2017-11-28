#ifndef IM_GUI_H
#define IM_GUI_H

#include "VideoResource.h"

enum ImguiMouseButton 
{
	eImguiMButtonLeft = 0x01,
	ImguiMButtonRight = 0x02,
};


enum ImguiTextAlign 
{
	eImguiAlignLeft = 0,
	eImguiAlignCenter,
	eImguiAlignRight
};

inline uint32 ImguiRGBA(uint8 r, uint8 g, uint8 b, uint8 a = 255) 
{
	return (a << 24) | (r << 16) | (g << 8) | (b);
}

void ImguiBeginFrame(int32 mx, int32 my, uint8 mbut, int32 scroll, uint32 keyCode, bool32 shiftDown);

void ImguiEndFrame();

bool ImguiBeginScrollArea(const char* name, int32 x, int32 y, int32 w, int32 h, int32 *pOutScroll);
void ImguiEndScrollArea();

void ImguiIndent();
void ImguiUnindent();
void ImguiSeparator();
void ImguiSeparatorLine();

bool ImguiButton(const char* text, bool enabled = true);
bool ImguiItem(const char* text, bool enabled = true);
bool ImguiCheck(const char* text, bool checked, bool enabled = true);
bool ImguiCollapse(const char* text, const char* subtext, bool checked, bool enabled = true);
void ImguiLabel(const char* text);
void ImguiValue(const char* text);
bool ImguiSlider(const char* text, float* val, float vmin, float vmax, float vinc, bool enabled = true);

void ImguiDrawTexture( int16 xOffset, int16 yOffset, int16 width, int16 height, video::TextureHandle handle);

bool ImguiEdit(char *text, int32 width, bool enable = true);

void ImguiDrawText(int32 x, int32 y, int32 align, const char* text, uint32 color);

void ImguiDrawLine(int32 x0, int32 y0, int32 x1, int32 y1, int32 r, uint32 color);
void ImguiDrawRect(int32 x, int32 y, int32 w, int32 h, uint32 color);

// Pull render interface.
enum ImguiCommandType 
{
	eCommandRect,
	eCommandTriangle,
	eCommandLine,
	eCommandText,
	eCommandTexture,
	eCommandScissor,
};

struct ImguiRect 
{
	int16 _x, _y, _w, _h, _r;
};

struct ImguiText 
{
	int16 _x, _y, _align;
	const char* _text;
};

struct ImguiLine 
{
	int16 _x0, _y0, _x1, _y1, _r;
};

struct ImguiTexture
{
	int16 _x, _y, _w, _h;
	video::TextureHandle _handle{};
};

struct ImguiCommand 
{
	ImguiCommand() {}
	int8 _type;
	int8 _flags;
	uint32 _color;

	union 
	{
		ImguiLine _line;
		ImguiRect _rect;
		ImguiText _text;
		ImguiTexture _texture;
	};
};

const ImguiCommand* ImguiGetRenderQueue();
int32 ImguiGetRenderQueueSize();

#endif
