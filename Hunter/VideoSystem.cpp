#include "stdafx.h"
#include "VideoSystem.h"

IDirect3DDevice9 *gpDevice;

using namespace im;

VideoSystem::VideoSystem()
{
}

VideoSystem::~VideoSystem()
{
}

bool VideoSystem::Init(const std::string & name, const SystemSetting & setting)
{
	_name = name;

	if (!InitD3D(gEngine->GetWindowHandle()))
	{
		return false;
	}

	_pimguiRenderer = new im::GuiRenderer();
	_pimguiRenderer->Init(gpDevice, "consolas", WINSIZEX, WINSIZEY);


	return true;
}

void VideoSystem::ShutDown()
{
}

void VideoSystem::Update(float deltaTime)
{


}

void VideoSystem::Render()
{
	_pDevice->BeginScene();

	//PerformRender();

	_pimguiRenderer->Draw();
	_pDevice->EndScene();

	_pDevice->Present(nullptr, nullptr, NULL, nullptr);
}

bool VideoSystem::InitD3D(HWND windowHandle)
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

bool VideoSystem::InitDefaultRenderState()
{
	bool result = true;

	_pDevice->SetRenderState(D3DRS_LIGHTING, false);
	//_pDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);

	return result;
}

bool VideoSystem::PerformRender()
{
	return false;
}
