#ifndef STD_AFX_H
#define STD_AFX_H

#define SAFE_DELETE(p) if(p) {delete p; p = nullptr;}
#define SAFE_DELETE_ARRAY(p) if(p) {delete [] p; p = nullptr;}
#define COM_RELEASE(p) if(p) {p->Release(); p = nullptr;}

#define DEFINE_WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <strsafe.h>

#include <iostream>

#include <d3d9.h>
#include <d3dx9.h>

#include <algorithm>

#include "Typedefs.h"


#define WINSIZEX (1600)
#define WINSIZEY (900)

#include "CommonUtils.h"
#include "Engine.h"

extern Engine *gEngine;

#endif