#include "stdafx.h"
#include "VideoDevice.h"

#include "stdafx.h"
#include "VideoDevice.h"

IDirect3DDevice9 *gpDevice;

using namespace im;
using namespace video;

VideoDevice::VideoDevice()
	: _vertexBufferPool(VIDEO_CONFIG_VERTEXBUFFER_MAX_NUM),
	_indexBufferPool(VIDEO_CONFIG_VERTEXBUFFER_MAX_NUM),
	_textureHandlePool(VIDEO_CONFIG_VERTEXBUFFER_MAX_NUM),
	_effectHandlePool(VIDEO_CONFIG_VERTEXBUFFER_MAX_NUM),
	_vertexDeclHandlePool(VIDEO_CONFIG_VERTEXBUFFER_MAX_NUM),
	_renderViewHandlePool(VIDEO_CONFIG_RENDER_VIEW_MAX_NUM),
	_materialHandlePool(VIDEO_CONFIG_MATERIAL_MAX_NUM),
	_modelHandlePool(VIDEO_CONFIG_MODEL_MAX_NUM)
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

void video::VideoDevice::Render(RenderView & renderView)
{
	static uint32 count = 0;

	uint32 drawCount = 0;

	renderView.PreRender();

	_pDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, renderView._clearColor, 1.0f, 0);
	_pDevice->BeginScene();

	CommandBuffer::Enum command;
	renderView._commandBuffer.Read<CommandBuffer::Enum>(command);


	RenderState sendingState{};
	Matrix sendingMatrix[3];
	MatrixIdentity(&sendingMatrix[0]);
	sendingMatrix[1] = renderView._viewMatrix;
	sendingMatrix[2] = renderView._projectionMatrix;

	if (command == CommandBuffer::Enum::eRendererInit)
	{
		while (command != CommandBuffer::Enum::eEnd)
		{
			renderView._commandBuffer.Read<CommandBuffer::Enum>(command);

			switch (command)
			{
			case video::CommandBuffer::eSetTransform:
			{
				uint32 cacheIndex{};
				renderView._commandBuffer.Read<uint32>(cacheIndex);
				sendingMatrix[0] = renderView._matrixCache._cache[cacheIndex];

			} break;
			case video::CommandBuffer::eSetEffect:
			{
				EffectHandle handle;
				renderView._commandBuffer.Read<EffectHandle>(handle);

				sendingState._effect= handle;
			} break;

			case video::CommandBuffer::eSetVertexBuffer:
			{
				VertexBufferHandle handle;
				renderView._commandBuffer.Read<VertexBufferHandle>(handle);
				sendingState._vertexBuffer= handle;

				renderView._commandBuffer.Read<uint32>(sendingState._startVertex);
				renderView._commandBuffer.Read<uint32>(sendingState._numPrim);
			} break;

			case video::CommandBuffer::eSetIndexBuffer:
			{
				IndexBufferHandle handle;
				renderView._commandBuffer.Read<IndexBufferHandle>(handle);
				sendingState._indexBuffer= handle;

				renderView._commandBuffer.Read<uint32>(sendingState._startIndex);
				renderView._commandBuffer.Read<uint32>(sendingState._numVertices);
				renderView._commandBuffer.Read<uint32>(sendingState._numPrim);

			} break;

			//case video::CommandBuffer::eSetMesh:
			//{
			//} break;
			case video::CommandBuffer::eSetMaterial:
			{
				MaterialHandle handle;
				renderView._commandBuffer.Read<MaterialHandle>(handle);
				sendingState._material= handle;

			} break;
			case video::CommandBuffer::eDraw:
			{
				drawCount++;
				if (sendingState._vertexBuffer.IsValid())
				{
					if (sendingState._indexBuffer.IsValid())
					{
						DrawIndexPrimitive(sendingState, sendingMatrix);
					}
					else
					{
						DrawPrimitive(sendingState, sendingMatrix);
					}
				}
				sendingState._indexBuffer.MakeInvalid();
				MatrixIdentity(&sendingMatrix[0]);
			} break;
			default:
			{
			} break;
			}
		}
	}
	renderView.PostRender();
	
	_pDevice->EndScene();
	_pDevice->Present(nullptr, nullptr, NULL, nullptr);

#if defined (DEBUG) || defined (_DEBUG) 
	count++;
	if (count % 60 == 0)
	{
		Console::Log("Draw Count : % d\n", drawCount);
	}
#endif

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

	gpDevice = _pDevice;

	InitDefaultRenderState();

	gpDevice = _pDevice;

	return result;
}

bool VideoDevice::InitDefaultRenderState()
{
	bool result = true;
	gpDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	gpDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	gpDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	gpDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	gpDevice->SetRenderState(D3DRS_DITHERENABLE, TRUE );

	gpDevice->SetRenderState(D3DRS_ZENABLE, TRUE );
	gpDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE );
	gpDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);

 	gpDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE );
 	gpDevice->SetRenderState(D3DRS_SPECULARENABLE, FALSE );

	gpDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	gpDevice->SetRenderState(D3DRS_AMBIENT, 0x02020202);

	gpDevice->SetRenderState( D3DRS_COLORVERTEX ,   FALSE );

	gpDevice->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);	
	gpDevice->SetRenderState( D3DRS_SPECULARMATERIALSOURCE , D3DMCS_MATERIAL);	
	gpDevice->SetRenderState( D3DRS_AMBIENTMATERIALSOURCE  , D3DMCS_MATERIAL);	
	gpDevice->SetRenderState( D3DRS_EMISSIVEMATERIALSOURCE , D3DMCS_MATERIAL);
	
	// initialize the world transform matrix as the identity
	Matrix matrix;
	MatrixIdentity(&matrix);
	gpDevice->SetTransform(D3DTS_WORLD , &matrix);
	gpDevice->SetTransform(D3DTS_VIEW , &matrix);
	gpDevice->SetTransform(D3DTS_PROJECTION, &matrix);	
	return result;
}


void video::VideoDevice::DrawPrimitive(const video::RenderState & renderState, Matrix *matrices)
{
	Effect *effectPtr{};
	VertexBuffer *vertexPtr {};
	Material *materialPtr {};
	VertexDecl *declPtr{};

	//머테리얼 설정 여부 확인
	if (renderState._material != _activeState._material)
	{
		_activeState._material = renderState._material;
	}

	if (_activeState._material.IsValid())
	{
		materialPtr = &_materials[_activeState._material.index];
	}

	if (renderState._vertexBuffer != _activeState._vertexBuffer)
	{
		_activeState._vertexBuffer = renderState._vertexBuffer;
		vertexPtr = &_vertexBuffers[_activeState._vertexBuffer.index];
		//Vertex Decl설정여부 확인
		if (_activeState._vertexDecl != vertexPtr->_decl)
		{
			_activeState._vertexDecl = vertexPtr->_decl;
			declPtr = &_vertexDecls[_activeState._vertexDecl.index];
			_pDevice->SetVertexDeclaration(declPtr->_ptr);
		}
		else
		{
			declPtr = &_vertexDecls[_activeState._vertexDecl.index];
		}
		_pDevice->SetStreamSource(0, vertexPtr->_ptr, 0, declPtr->_stride);
	}
	else
	{
		vertexPtr = &_vertexBuffers[_activeState._vertexBuffer.index];
		declPtr = &_vertexDecls[_activeState._vertexBuffer.index];
	}

	if (renderState._effect != _activeState._effect)
	{
		_activeState._effect = renderState._effect;
	}
	effectPtr = &_effects[_activeState._effect.index];

	effectPtr->SetMatrix("gWorld", matrices[0]);
	effectPtr->SetMatrix("gView", matrices[1]);
	effectPtr->SetMatrix("gProjection", matrices[2]);

	if (materialPtr)
	{
		for (uint32 i = 0; i < VIDEO_CONFIG_MATERIAL_TEXTURE_MAX_NUM; ++i)
		{
			if (materialPtr->_textureHandles[i].IsValid())
			{
				effectPtr->SetTexture(PredefinedUniform::ParamName[PredefinedUniform::Enum::eTexture0 + i], 
					_textures[materialPtr->_textureHandles[i].index]);
			}
		}
	}
	uint32 numPrim{};
	uint32 startVertex{};

	numPrim = (0 != renderState._numVertices) ? (renderState._numPrim) : (vertexPtr->_size / declPtr->_stride);
	startVertex = (0 != renderState._startVertex) ? (renderState._startVertex) : (0);

	uint32 numPass = effectPtr->BeginEffect();
	for (uint32 i = 0; i < numPass; ++i)
	{
		effectPtr->BeginPass(i);
		_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, startVertex, numPrim);
		effectPtr->EndPass();
	}
	effectPtr->EndEffect();
}

void video::VideoDevice::DrawIndexPrimitive(const video::RenderState & renderState, Matrix *matrices)
{
	Effect *effectPtr{};
	VertexBuffer *vertexPtr{};
	IndexBuffer *indexPtr{};
	Material *materialPtr{};
	VertexDecl *declPtr{};

	//머테리얼 설정 여부 확인
	if (renderState._material != _activeState._material)
	{
		_activeState._material = renderState._material;
	}
	if (_activeState._material.IsValid())
	{
		materialPtr = &_materials[_activeState._material.index];
	}

	if (renderState._vertexBuffer!= _activeState._vertexBuffer)
	{
		_activeState._vertexBuffer = renderState._vertexBuffer;
		vertexPtr = &_vertexBuffers[_activeState._vertexBuffer.index];
		//Vertex Decl설정여부 확인
		if (_activeState._vertexDecl != vertexPtr->_decl)
		{
			_activeState._vertexDecl = vertexPtr->_decl;
			declPtr = &_vertexDecls[_activeState._vertexDecl.index];
			_pDevice->SetVertexDeclaration(declPtr->_ptr);
		}
		else
		{
			declPtr = &_vertexDecls[_activeState._vertexDecl.index];
		}
		_pDevice->SetStreamSource(0, vertexPtr->_ptr, 0, declPtr->_stride);
	}
	else
	{
		vertexPtr = &_vertexBuffers[_activeState._vertexBuffer.index];
		declPtr = &_vertexDecls[_activeState._vertexBuffer.index];
	}

	if (renderState._indexBuffer != _activeState._indexBuffer)
	{
		_activeState._indexBuffer = renderState._indexBuffer;
	}
	indexPtr = &_indexBuffers[_activeState._indexBuffer.index];
	_pDevice->SetIndices(indexPtr->_ptr);

	if (renderState._effect != _activeState._effect)
	{
		_activeState._effect = renderState._effect;
	}
	effectPtr = &_effects[_activeState._effect.index];

	effectPtr->SetMatrix("gWorld", matrices[0]);
	effectPtr->SetMatrix("gView", matrices[1]);
	effectPtr->SetMatrix("gProjection", matrices[2]);

	if (materialPtr)
	{
		for (uint32 i = 0; i < VIDEO_CONFIG_MATERIAL_TEXTURE_MAX_NUM; ++i)
		{
			if (materialPtr->_textureHandles[i].IsValid())
			{
				effectPtr->SetTexture(PredefinedUniform::ParamName[PredefinedUniform::Enum::eTexture0 + i],
					_textures[materialPtr->_textureHandles[i].index]);
			}
		}
	}

	uint32 numVertices{};
	uint32 numPrim{};
	uint32 startIndex{};

	numVertices = (0 != renderState._numVertices) ? (renderState._numVertices) : (vertexPtr->_size / declPtr->_stride);
	numPrim = (0 != renderState._numPrim) ? (renderState._numPrim) : ((indexPtr->_size / 2 ) / 3);
	startIndex = (0 != renderState._startIndex) ? (renderState._startIndex) : 0;

	uint32 numPass = effectPtr->BeginEffect();
	for (uint32 i = 0; i < numPass; ++i)
	{
		effectPtr->BeginPass(i);
		_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, numVertices, startIndex, numPrim);
		effectPtr->EndPass();
	}
	effectPtr->EndEffect();
}

RenderView *video::VideoDevice::GetRenderView(RenderViewHandle handle)
{
	if (handle.IsValid())
	{
		return &_renderViews[handle.index];
	}
	return nullptr;
}

VertexBufferHandle video::VideoDevice::CreateVertexBuffer(Memory * memory, VertexDeclHandle declHandle, const std::string & name)
{
	VertexBufferHandle result = _vertexBufferPool.Create(name);
	if (!_vertexBuffers[result.index].Create(memory->_size, memory->_data, declHandle))
	{
		Assert(false);//Creation failed
	}
	return result;
}

VertexBufferHandle video::VideoDevice::GetVertexBuffer(const std::string & name)
{
	return _vertexBufferPool.Get(name);
}

void video::VideoDevice::DestroyVertexBuffer(VertexBufferHandle handle)
{
	_vertexBuffers[handle.index].Destroy();
	_vertexBufferPool.Remove(handle);
}

IndexBufferHandle video::VideoDevice::CreateIndexBuffer(Memory * memory, const std::string & name)
{
	IndexBufferHandle result = _indexBufferPool.Create(name);
	if (!_indexBuffers[result.index].Create(memory->_size, memory->_data))
	{
		Assert(false);//Creation failed
	}
	return result;
}

IndexBufferHandle video::VideoDevice::GetIndexBuffer(const std::string & name)
{
	return _indexBufferPool.Get(name);
}

void video::VideoDevice::DestroyIndexBuffer(IndexBufferHandle handle)
{
	_indexBuffers[handle.index].Destroy();
	_indexBufferPool.Remove(handle);
}

VertexDeclHandle video::VideoDevice::CreateVertexDecl(const VertexDecl *decl)
{
	VertexDeclHandle result = _vertexDeclHandlePool.Create();
	memcpy(&_vertexDecls[result.index], decl, sizeof(VertexDecl));
	return result;
}

VertexDeclHandle video::VideoDevice::GetVertexDecl(const std::string & name)
{
	return _vertexDeclHandlePool.Get(name);
}

void video::VideoDevice::DestroyVertexDecl(VertexDeclHandle handle)
{
	_vertexDecls[handle.index].Destroy();
	_vertexDeclHandlePool.Remove(handle);
}

TextureHandle video::VideoDevice::CreateTexture(const std::string &fileName, const std::string &name)
{
	TextureHandle result = _textureHandlePool.Create(name);
	if (!_textures[result.index].Create(fileName))
	{
		Assert(false);
	}
	return result;
}

TextureHandle video::VideoDevice::GetTexture(const std::string & name)
{
	return _textureHandlePool.Get(name);
}

void video::VideoDevice::DestroyTexture(TextureHandle handle)
{
	_textures[handle.index].Destroy();
	_textureHandlePool.Remove(handle);
}

EffectHandle video::VideoDevice::CreateEffect(const std::string &fileName, const std::string &name)
{
	EffectHandle result = _effectHandlePool.Create(name);
	if (!_effects[result.index].Create(fileName))
	{
		Assert(false);
	}
	return result;
}

EffectHandle video::VideoDevice::GetEffect(const std::string & name)
{
	return _effectHandlePool.Get(name);
}

void video::VideoDevice::DestroyEffect(EffectHandle handle)
{
	_effects[handle.index].Destroy();
	_effectHandlePool.Remove(handle);
}

RenderViewHandle video::VideoDevice::CreateRenderView(const std::string & name)
{
	RenderViewHandle result = _renderViewHandlePool.Create(name);
	_renderViews[result.index].Create();
	return result;
}

RenderViewHandle video::VideoDevice::GetRenderView(const std::string & name)
{
	return _renderViewHandlePool.Get(name);
}

void video::VideoDevice::DestroyRenderView(RenderViewHandle handle)
{
	_renderViews[handle.index].Destroy();
	_renderViewHandlePool.Remove(handle);
}

MaterialHandle video::VideoDevice::CreateMaterial(const std::string & name)
{
	MaterialHandle result = _materialHandlePool.Create(name);
	_materials[result.index].Create();
	return result;
}

MaterialHandle video::VideoDevice::GetMaterial(const std::string & name)
{
	return _materialHandlePool.Get(name);
}

void video::VideoDevice::DestroyMaterial(MaterialHandle handle)
{
	_materials[handle.index].Destroy();
	_materialHandlePool.Remove(handle);
}

void video::VideoDevice::AddTextureToMaterial(MaterialHandle material, uint32 textureSlot, TextureHandle texture)
{
	_materials[material.index].AddTexture(textureSlot, texture);
}

void video::VideoDevice::SetCurrentRenderView(RenderViewHandle handle)
{
	_pCurrentView = &_renderViews[handle.index];
}

void video::VideoDevice::SetRenderViewProjectionMatrix(RenderViewHandle handle, const Matrix & view, const Matrix & projection)
{
	RenderView *pRenderView = &_renderViews[handle.index];
	pRenderView->_viewMatrix = view;
	pRenderView->_projectionMatrix = projection;
}

void video::VideoDevice::SetCurrentRenderViewProjectionMatrix(const Matrix & view, const Matrix & projection)
{
	Assert(_pCurrentView);
	_pCurrentView->_viewMatrix = view;
	_pCurrentView->_projectionMatrix = projection;
}

