#ifndef IMGUI_RENDERER_H
#define IMGUI_RENDERER_H

#include "imgui.h"

extern video::FontHandle gFontHandle;

bool imguiRenderInit();

void imguiRenderDestroy();

void imguiRenderDraw();
//void imguiRenderBegin();
//void imguiRenderEnd();

#endif