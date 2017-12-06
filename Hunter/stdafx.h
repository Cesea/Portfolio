#ifndef STD_AFX_H
#define STD_AFX_H



#define SAFE_DELETE(p) if(p) {delete p; p = nullptr;}
#define SAFE_DELETE_ARRAY(p) if(p) {delete [] p; p = nullptr;}
#define COM_RELEASE(p) if(p) {p->Release(); p = nullptr;}
#define SAFE_RELEASE(p) if(p) {p->Release(); p = nullptr;}

#define HRESULT_CHECK(expression)  \
{ \
	if(FAILED(expression)) \
	{ \
	Assert(false); \
	} \
}

#define ARRAY_COUNT(arr) (sizeof(arr) / sizeof(arr[0]))

#if defined (DEBUG) || defined (_DEBUG)
	#define Assert(expression)  if(!(expression)) { *(int *)0 = 0;}
#else
	#define Assert(expression) 
#endif

#define DEFINE_WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <strsafe.h>

#include <iostream>
#include <vector>

#include <d3d9.h>
#include <d3dx9.h>
#include <rmxfguid.h>
#include <rmxftmpl.h>

#include <math.h>

#include <memory>
#include <array>
#include <algorithm>
#include <type_traits>
#include <stack>

#include <assert.h>

#define WINSTARTX (300)
#define WINSTARTY (100)

#define WINSIZEX (1600)
#define WINSIZEY (900)

#define EDITORX (0)
#define EDITORY (0)
#define EDITORSIZEX (400)
#define EDITORSIZEY (900)

constexpr int CLIENTCENTERX = WINSTARTX +  WINSIZEX / 2;
constexpr int CLIENTCENTERY = WINSTARTY + WINSIZEY / 2;

#define MAX_FILE_NAME 256

#include "Typedefs.h"
#include "Memory.h"
#include "Reflection.h"
#include "EventChannel.h"
#include "EntityComponentSystem.h"


#include "Math.h"
#include "CommonUtils.h"


#include "Engine.h"
#include "GameUtils.h"

extern HWND gWindowHandle;
extern IDirect3DDevice9 *gpDevice;
extern Engine *gEngine;
extern bool32 gEditorOn;

#endif