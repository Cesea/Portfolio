#ifndef IMGUI_RENDERER_H
#define IMGUI_RENDERER_H

#include "imgui.h"

bool imguiWindowProc(UINT msg, WPARAM wParam, LPARAM lParam);

bool imguiRenderInit();

void imguiRenderDestroy();

void imguiRenderDraw();
//void imguiRenderBegin();
//void imguiRenderEnd();

#endif