#include "stdafx.h"
#include "VideoDevice.h"

IDirect3DDevice9 *gpDevice;

using namespace im;

using namespace video;

VideoDevice::VideoDevice()
{
}

VideoDevice::~VideoDevice()
{
}

bool VideoDevice::Init()
{
	if (!InitD3D(gEngine->GetWindowHandle()))
	{
		return false;
	}

	im::GuiRenderer::GetInstance()->Init(gpDevice, "consolas", WINSIZEX, WINSIZEY);

	Matrix view;
	Matrix projection;


	MatrixLookAtLH(&view, &Vector3(0.0f, 0.0f, -2.0f), &Vector3(0.0f, 0.0f, 0.0f), &Vector3(0.0f, 1.0f, 0.0f));
	MatrixPerspectiveFovLH(&projection, D3DX_PI * 0.5f, (float)WINSIZEX / (float)WINSIZEY, 0.1f, 1000.0f);

	_commandBucket.Init(1000, view, projection);

	return true;
}

void VideoDevice::ShutDown()
{
	COM_RELEASE(_pDevice, 0);
	COM_RELEASE(_pD3D, 0);

}

void VideoDevice::Update(float deltaTime)
{


}

void VideoDevice::Render()
{
	_commandBucket.Sort();

	_pDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, 0xff303030, 1.0f, 0);
	_pDevice->BeginScene();

	_commandBucket.Submit();

	im::GuiRenderer::GetInstance()->Draw();

	_pDevice->EndScene();
	_pDevice->Present(nullptr, nullptr, NULL, nullptr);

	_commandBucket.Clear();
}

bool VideoDevice::InitD3D(HWND windowHandle)
{
	bool result = true;

	RECT windowRect = {};
	GetClientRect(windowHandle, &windowRect);
	_renderWindow.width = windowRect.right - windowRect.left;
	_renderWindow.height = windowRect.bottom - windowRect.top;
	_renderWindow.backBufferFormat = D3DFMT_A8R8G8B8;
	_renderWindow.depthStencilFormat = D3DFMT_D24S8;

	//DirectX Init Start
	_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!_pD3D)
	{
		return false;
	}

	D3DCAPS9 deviceCaps;
	_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &deviceCaps);

	DWORD behaviorFlag{};
	if (deviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		behaviorFlag = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		behaviorFlag = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	_presentParams.BackBufferCount = 1;
	_presentParams.BackBufferWidth = _renderWindow.width;
	_presentParams.BackBufferHeight = _renderWindow.height;
	_presentParams.BackBufferFormat = _renderWindow.backBufferFormat;
	_presentParams.EnableAutoDepthStencil = true;
	_presentParams.AutoDepthStencilFormat = _renderWindow.depthStencilFormat;
	_presentParams.hDeviceWindow = windowHandle;
	_presentParams.Windowed = true;
	_presentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	_presentParams.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	_presentParams.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	_presentParams.MultiSampleType = D3DMULTISAMPLE_NONE;
	_presentParams.MultiSampleQuality = 0;
	_presentParams.Flags = 0;

	HRESULT hresult = _pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, windowHandle,
		behaviorFlag, &_presentParams, &_pDevice);
	if (FAILED(hresult))
	{
		return false;
	}

	InitDefaultRenderState();

	gpDevice = _pDevice;

	return result;
}

bool VideoDevice::InitDefaultRenderState()
{
	bool result = true;

	_pDevice->SetRenderState(D3DRS_LIGHTING, false);
	//_pDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);

	return result;
}

bool VideoDevice::PerformRender()
{
	return false;
}
