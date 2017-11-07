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

	_vertexBufferManager.Init(256);
	_indexBufferManager.Init(256);

	_pimguiRenderer = new im::GuiRenderer();
	_pimguiRenderer->Init(gpDevice, "consolas", WINSIZEX, WINSIZEY);


	Matrix view;
	Matrix projection;


	MatrixLookAtLH(&view, &Vector3(0.0f, 0.0f, -2.0f), &Vector3(0.0f, 0.0f, 0.0f), &Vector3(0.0f, 1.0f, 0.0f));
	MatrixPerspectiveFovLH(&projection, D3DX_PI * 0.5f, (float)WINSIZEX / (float)WINSIZEY, 0.1f, 1000.0f);

	_commandBucket.Init(1000, view, projection);


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
	_commandBucket.Sort();

	_pDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, 0xff303030, 1.0f, 0);
	_pDevice->BeginScene();

	_commandBucket.Submit();

	_pimguiRenderer->Draw();

	_pDevice->EndScene();
	_pDevice->Present(nullptr, nullptr, NULL, nullptr);

	_commandBucket.Clear();
	_matrixCache.Clear();
}

bool32 VideoSystem::Draw(VertexBufferHandle vertexHandle,
		uint32 startVertex, uint32 primitiveCount, uint32 matrixIndex)
{
	_pDevice->SetTransform(D3DTS_WORLD, &_matrixCache.GetAt(matrixIndex));

	_pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	const VertexBuffer &refVertexBuffer = _vertexBufferManager.GetMember(vertexHandle);
	_pDevice->SetStreamSource(0, refVertexBuffer.Buffer(), 0, refVertexBuffer.GetStride());
	_pDevice->SetIndices(nullptr);
	if (FAILED(_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, startVertex, primitiveCount)))
	{
		return false;
	}
	return true;
}

bool32 VideoSystem::DrawIndexed(VertexBufferHandle vertexHandle, IndexBufferHandle indexHandle, 
	uint32 numVertex, uint32 startIndex, uint32 primitiveCount, uint32 matrixIndex)
{
	_pDevice->SetTransform(D3DTS_WORLD, &_matrixCache.GetAt(matrixIndex));

	_pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	const VertexBuffer &refVertexBuffer = _vertexBufferManager.GetMember(vertexHandle);
	const IndexBuffer &refIndexBuffer = _indexBufferManager.GetMember(vertexHandle);
	_pDevice->SetStreamSource(0, refVertexBuffer.Buffer(), 0, refVertexBuffer.GetStride());
	_pDevice->SetIndices(refIndexBuffer.Buffer());
	if (FAILED(_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, numVertex, startIndex, primitiveCount)))
	{
		return false;
	}
	return true;
}

//bool32 VideoSystem::DrawIndexed(IDirect3DVertexBuffer9 * pVertexBuffer, 
//	IDirect3DIndexBuffer9 * pIndexBuffer,
//	IDirect3DVertexDeclaration9 * pVertexDecl, 
//	uint32 indexCount, uint32 vertexCount, uint32 startIndex, uint32 baseVertex, uint32 stride)
//{
//	_pDevice->SetVertexDeclaration(pVertexDecl);
//	_pDevice->SetStreamSource(0, pVertexBuffer, 0, stride);
//	_pDevice->SetIndices(pIndexBuffer);
//	if (FAILED(_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, baseVertex, 0, 
//		vertexCount, startIndex, indexCount / 3)))
//	{
//		return false;
//	}
//	return true;
//}

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
