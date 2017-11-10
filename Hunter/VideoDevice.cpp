#include "stdafx.h"
#include "VideoDevice.h"

#include "VideoPlatform.h"

IDirect3DDevice9 *gpDevice;

using namespace im;

using namespace video;

VideoDevice sVideoDevice;

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

	//_commandBucket.Init(1000, view, projection);

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

//void VideoDevice::Render()
//{
//	_commandBucket.Sort();
//
//	_pDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, 0xff303030, 1.0f, 0);
//	_pDevice->BeginScene();
//
//	_commandBucket.Submit();
//
//	im::GuiRenderer::GetInstance()->Draw();
//
//	_pDevice->EndScene();
//	_pDevice->Present(nullptr, nullptr, NULL, nullptr);
//
//	_commandBucket.Clear();
//}

void video::VideoDevice::UpdateResolution(const Resolution & resolution)
{
}

void video::VideoDevice::SetRenderTarget(RenderTargetHandle handle)
{
	if (handle.index != INVALID_HANDLE)
	{
		HRESULT_CHECK(_pDevice->SetRenderTarget(0, _backBufferColor));
		HRESULT_CHECK(_pDevice->SetDepthStencilSurface(_backBufferDepthStencil));
	}
	else
	{
		RenderTarget &refRenderTarget = _renderTargets[handle.index];
		HRESULT_CHECK(_pDevice->SetRenderTarget(0, refRenderTarget._color));
		HRESULT_CHECK(_pDevice->SetDepthStencilSurface(
			(nullptr != refRenderTarget._depth) ? refRenderTarget._depth : _backBufferDepthStencil));
	}
}

void video::VideoDevice::Flip()
{
	_pDevice->Present(nullptr, nullptr, NULL, nullptr);
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

void video::IndexBuffer::Create(uint32 size, void * data)
{
	_size = _size;
	_dynamic = (nullptr == data);

	uint32_t usage = D3DUSAGE_WRITEONLY;
	D3DPOOL pool = D3DPOOL_MANAGED;

	if (_dynamic)
	{
		usage |= D3DUSAGE_DYNAMIC;
		pool = D3DPOOL_DEFAULT;
	}

	HRESULT_CHECK(gpDevice->CreateIndexBuffer(_size, usage, D3DFMT_INDEX16, pool, &_ptr, nullptr ));
}

void video::VertexBuffer::Create(uint32 size, void * data, VertexDeclHandle declHandle)
{
	_size = _size;
	_decl = declHandle;
	_dynamic = (nullptr == data);

	uint32_t usage = D3DUSAGE_WRITEONLY;
	D3DPOOL pool = D3DPOOL_MANAGED;

	if (_dynamic)
	{
		usage |= D3DUSAGE_DYNAMIC;
		pool = D3DPOOL_DEFAULT;
	}

	HRESULT_CHECK(gpDevice->CreateVertexBuffer(_size, usage, 0, pool, &_ptr, nullptr));
}

void video::RenderTarget::Create(uint32 width, uint32 height, uint32 flags)
{
	_width = _width;
	_height = _height;
	_flags = _flags;

	CreateTextures();
}

void video::RenderTarget::CreateTextures()
{
	if (0 != _flags)
	{
		uint32_t colorFormat = (_flags & VIDEO_RENDER_TARGET_COLOR_MASK) >> VIDEO_RENDER_TARGET_COLOR_SHIFT;
		uint32_t depthFormat = (_flags & VIDEO_RENDER_TARGET_DEPTH_MASK) >> VIDEO_RENDER_TARGET_DEPTH_SHIFT;

		if (0 < colorFormat)
		{
			HRESULT_CHECK(gpDevice->CreateTexture(_width, _height, 1, D3DUSAGE_RENDERTARGET,
				sColorFormat[colorFormat], D3DPOOL_DEFAULT, &_colorTexture, NULL ));

			HRESULT_CHECK(_colorTexture->GetSurfaceLevel(0, m_color));
		}

		if (0 < depthFormat)
		{
			HRESULT_CHECK(gpDevice->CreateTexture(_width , _height , 1 , D3DUSAGE_DEPTHSTENCIL, 
				sDepthFormat[depthFormat], s_renderCtx.m_fmtDepth , D3DPOOL_DEFAULT , &m_depthTexture , NULL ));

			HRESULT_CHECK(_depthTexture->GetSurfaceLevel(0, &_depth));
		}
	}
}

void video::RenderTarget::DestroyTextures()
{
	if (0 != _flags)
	{
		uint32_t colorFormat = (_flags & VIDEO_RENDER_TARGET_COLOR_MASK) >> VIDEO_RENDER_TARGET_COLOR_SHIFT;
		uint32_t depthFormat = (_flags & VIDEO_RENDER_TARGET_DEPTH_MASK) >> VIDEO_RENDER_TARGET_DEPTH_SHIFT;

		if (0 < colorFormat)
		{
			COM_RELEASE(_color);
			COM_RELEASE(_colorTexture);
		}

		if (0 < depthFormat)
		{
			COM_RELEASE(_depth);
			COM_RELEASE(_depthTexture);
		}
	}
}

void video::RenderTarget::Commit(uint8 stage)
{
	HRESULT_CHECK(gpDevice->SetSamplerState(stage, D3DSAMP_MINFILTER, D3DTEXF_LINEAR));
	HRESULT_CHECK(gpDevice->SetSamplerState(stage, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR));
	HRESULT_CHECK(gpDevice->SetSamplerState(stage, D3DSAMP_MIPFILTER, D3DTEXF_POINT));
	HRESULT_CHECK(gpDevice->SetSamplerState(stage, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP));
	HRESULT_CHECK(gpDevice->SetSamplerState(stage, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP));
	HRESULT_CHECK(gpDevice->SetTexture(stage, _colorTexture));
}

DWORD WINAPI RenderThread(LPVOID arg)
{
	while (!sExit)
	{
		RenderFrame();
	}
	sExit = false;
	return EXIT_SUCCESS;
}
