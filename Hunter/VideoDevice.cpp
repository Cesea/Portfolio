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
	_renderGroupHandlePool(VIDEO_CONFIG_RENDER_GROUP_MAX_NUM),
	_staticXMeshHandlePool(VIDEO_CONFIG_STATIC_XMESH_MAX_NUM),
	_skinnedXMeshHandlePool(VIDEO_CONFIG_SKINNED_XMESH_MAX_NUM)
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

	LoadDefaultTextures();
	LoadDefaultEffects();
	MakeDefaultVertexDecls();

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

//NOTE : Render 함수는 RenderView를 그리는 역할을 한다
// 만약 RenderView의 RenderState에 Effect가 설정 되어있다면 Effect를 사용하여 렌더 할 것이고 아니라면 
// 현제 Device에 설정된 값으로 렌더 될 것이다...
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

	gpDevice->SetTransform(D3DTS_VIEW, &sendingMatrix[1]);
	gpDevice->SetTransform(D3DTS_PROJECTION, &sendingMatrix[2]);

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

				renderView._commandBuffer.Read<uint32>(sendingState._drawData._startVertex);
				renderView._commandBuffer.Read<uint32>(sendingState._drawData._numPrim);
			} break;

			case video::CommandBuffer::eSetIndexBuffer:
			{
				IndexBufferHandle handle;
				renderView._commandBuffer.Read<IndexBufferHandle>(handle);
				sendingState._indexBuffer= handle;

				renderView._commandBuffer.Read<uint32>(sendingState._drawData._startIndex);
				renderView._commandBuffer.Read<uint32>(sendingState._drawData._numVertices);
				renderView._commandBuffer.Read<uint32>(sendingState._drawData._numPrim);
			} break;

			case video::CommandBuffer::eSetRenderGroup :
			{
				RenderGroupHandle handle;
				renderView._commandBuffer.Read<RenderGroupHandle>(handle);

				const RenderGroup &refRenderGroup = _renderGroups[handle.index];

				sendingState._vertexBuffer = refRenderGroup._vertexBuffer;
				sendingState._indexBuffer = refRenderGroup._indexBuffer;

				sendingState._material = refRenderGroup._materialRange._material;
				
				sendingState._drawData._startIndex = refRenderGroup._materialRange._startIndex;
				sendingState._drawData._numVertices = refRenderGroup._materialRange._numVertices;
				sendingState._drawData._numPrim = refRenderGroup._materialRange._numPrim;
			}break;

			case video::CommandBuffer::eSetMaterial:
			{
				MaterialHandle handle;
				renderView._commandBuffer.Read<MaterialHandle>(handle);
				sendingState._material= handle;

			}break;

			case video::CommandBuffer::eChangeFillMode :
			{
				RenderState::FillMode fillMode;
				renderView._commandBuffer.Read<RenderState::FillMode>(fillMode);
				
				sendingState._fillMode = fillMode;
			}break;

			case video::CommandBuffer::eChangePrimitiveType :
			{
				D3DPRIMITIVETYPE type;
				renderView._commandBuffer.Read<D3DPRIMITIVETYPE>(type);
				
				sendingState._drawData._primitiveType = type;
			}break;

			case video::CommandBuffer::eDraw:
			{
				drawCount++;

				if (sendingState._vertexBuffer.IsValid())
				{
					//NOTE : FillMode 처리해준다 Solid가 default 값
					//if (sendingState._fillMode != RenderState::FillMode::eFillSolid)
					//{
					//	if (sendingState._fillMode == RenderState::FillMode::eFillPoint)
					//	{
					//		gpDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_POINT);
					//	}
					//	else if (sendingState._fillMode == RenderState::FillMode::eFillWireFrame)
					//	{
					//		gpDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
					//	}
					//}
					if (sendingState._effect.IsValid())
					{
						DrawWithEffect(sendingState, sendingMatrix);
					}
					else
					{
						DrawWithoutEffect(sendingState, sendingMatrix);
					}
				}
				//한번의 렌더를 끝내면 RenderState를 Default 값으로 되돌린다.
				sendingState.ResetDefault();
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
	return result;
}

bool VideoDevice::InitDefaultRenderState()
{
	bool result = true;

	gpDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	gpDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	gpDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	gpDevice->SetRenderState(D3DRS_DITHERENABLE, TRUE );

	gpDevice->SetRenderState(D3DRS_ZENABLE, TRUE );
	gpDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE );
	gpDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);

 	gpDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE );
 	gpDevice->SetRenderState(D3DRS_SPECULARENABLE, FALSE );

	gpDevice->SetRenderState( D3DRS_COLORVERTEX ,   FALSE );
	
	return result;
}

//NOTE : 만약 이펙트로 렌더 한다면 항상 D3DPT_TRIANGLELIST라고 가정한다....
void video::VideoDevice::DrawWithEffect(const video::RenderState & renderState, Matrix * matrices)
{
	Effect *effectPtr{};
	VertexBuffer *vertexPtr{};
	IndexBuffer *indexPtr{};
	Material *materialPtr{};
	VertexDecl *declPtr{};

	DWORD fvf{};
	_pDevice->GetFVF(&fvf);

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
		declPtr = &_vertexDecls[_activeState._vertexDecl.index];
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
	//머테리얼까지 설정이 끝났다면 여기서 인덱스 버퍼가 있는지 없는지를 판별하여 분기를 나눈다.
	//인덱스 버퍼가 있다..
	if (renderState._indexBuffer.IsValid())
	{
		_activeState._indexBuffer = renderState._indexBuffer;
		indexPtr = &_indexBuffers[renderState._indexBuffer.index];
		_pDevice->SetIndices(indexPtr->_ptr);
		uint32 numVertices{};
		uint32 numPrim{};
		uint32 startIndex{};


		numVertices = (0 != renderState._drawData._numVertices) ? 
			(renderState._drawData._numVertices) : (vertexPtr->_size / declPtr->_stride);
		numPrim = (0 != renderState._drawData._numPrim) ? 
			(renderState._drawData._numPrim) : ((indexPtr->_size / 2) / 3);
		startIndex = (0 != renderState._drawData._startIndex) ? (renderState._drawData._startIndex) : 0;

		uint32 numPass = effectPtr->BeginEffect();
		for (uint32 i = 0; i < numPass; ++i)
		{
			effectPtr->BeginPass(i);
			_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, numVertices, startIndex, numPrim);
			effectPtr->EndPass();
		}
		effectPtr->EndEffect();

	}
	//인덱스 버퍼가 없다..
	else
	{
		_pDevice->SetIndices(nullptr);
		uint32 numPrim{};
		uint32 startVertex{};

		numPrim = (0 != renderState._drawData._numVertices) ? 
			(renderState._drawData._numPrim) : (vertexPtr->_size / declPtr->_stride);
		startVertex = (0 != renderState._drawData._startVertex) ? 
			(renderState._drawData._startVertex) : (0);

		uint32 numPass = effectPtr->BeginEffect();
		for (uint32 i = 0; i < numPass; ++i)
		{
			effectPtr->BeginPass(i);
			_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, startVertex, numPrim);

			effectPtr->EndPass();
		}
		effectPtr->EndEffect();
	}
}

//NOTE : 만약 이펙트로 렌더 한다면 항상 D3DPT_TRIANGLELIST라고 가정한다....
void video::VideoDevice::DrawWithoutEffect(const video::RenderState & renderState, Matrix * matrices)
{
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

	if (renderState._vertexBuffer != _activeState._vertexBuffer)
	{
		_activeState._vertexBuffer = renderState._vertexBuffer;
		vertexPtr = &_vertexBuffers[_activeState._vertexBuffer.index];
		//Vertex Decl설정여부 확인
		//effect를 사용하지 않기때문에 device의 fvf를 설정 해 주어야 한다.
		if (_activeState._vertexDecl != vertexPtr->_decl)
		{
			_activeState._vertexDecl = vertexPtr->_decl;
			declPtr = &_vertexDecls[_activeState._vertexDecl.index];
		}
		else
		{
			declPtr = &_vertexDecls[_activeState._vertexDecl.index];
		}
		//NOTE : 기본적으로 렌더러는 이펙트를 사용하여 렌더하도록 하기때문에 항상 
		// FVF를 설정 해 주고 reset부분에서 꺼 주도록 하자...
		_pDevice->SetStreamSource(0, vertexPtr->_ptr, 0, declPtr->_stride);
	}
	else
	{
		vertexPtr = &_vertexBuffers[_activeState._vertexBuffer.index];
		declPtr = &_vertexDecls[_activeState._vertexDecl.index];
	}
	_pDevice->SetFVF(declPtr->_fvf);

	_pDevice->SetTransform(D3DTS_WORLD, &matrices[0]);

	if (materialPtr)
	{
		for (uint32 i = 0; i < VIDEO_CONFIG_MATERIAL_TEXTURE_MAX_NUM; ++i)
		{
			if (materialPtr->_textureHandles[i].IsValid())
			{
				//NOTE : SetTexture의 첫번째 인자에 어떤 값이 들어가야 맞는걸까?
				_pDevice->SetTexture(i, _textures[materialPtr->_textureHandles[i].index]._ptr);
			}
		}
	}

	//머테리얼까지 설정이 끝났다면 여기서 인덱스 버퍼가 있는지 없는지를 판별하여 분기를 나눈다.
	//인덱스 버퍼가 있다..
	if (renderState._indexBuffer.IsValid())
	{
		indexPtr = &_indexBuffers[renderState._indexBuffer.index];
		_pDevice->SetIndices(indexPtr->_ptr);

		uint32 numVertices{};
		uint32 numPrim{};
		uint32 startIndex{};

		//TriangleList일때...
		if ( D3DPT_TRIANGLELIST == renderState._drawData._primitiveType)
		{
			numVertices = (0 != renderState._drawData._numVertices) ?
				(renderState._drawData._numVertices) : (vertexPtr->_size / declPtr->_stride);
			numPrim = (0 != renderState._drawData._numPrim) ?
				(renderState._drawData._numPrim) : ((indexPtr->_size / 2) / 3);
			startIndex = (0 != renderState._drawData._startIndex) ? (renderState._drawData._startIndex) : 0;

			_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, numVertices, startIndex, numPrim);
		}
		//LineList일때
		else if(D3DPT_LINELIST == renderState._drawData._primitiveType)
		{
			numVertices = (0 != renderState._drawData._numVertices) ?
				(renderState._drawData._numVertices) : (vertexPtr->_size / declPtr->_stride);
			numPrim = (0 != renderState._drawData._numPrim) ?
				(renderState._drawData._numPrim) : ((indexPtr->_size / 2) / 2);
			startIndex = (0 != renderState._drawData._startIndex) ? (renderState._drawData._startIndex) : 0;
			_pDevice->DrawIndexedPrimitive(D3DPT_LINELIST, 0, 0, numVertices, startIndex, numPrim);
		}

	}
	//인덱스 버퍼가 없다..
	else
	{
		uint32 numPrim{};
		uint32 startVertex{};

		if ( D3DPT_TRIANGLELIST == renderState._drawData._primitiveType)
		{
			numPrim = (0 != renderState._drawData._numVertices) ?
				(renderState._drawData._numPrim) : (vertexPtr->_size / declPtr->_stride);
			startVertex = (0 != renderState._drawData._startVertex) ?
				(renderState._drawData._startVertex) : (0);
			_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, startVertex, numPrim);
		}
		//LineList일때
		else if (D3DPT_LINELIST == renderState._drawData._primitiveType)
		{
			numPrim = (0 != renderState._drawData._numVertices) ?
				(renderState._drawData._numPrim) : (vertexPtr->_size / declPtr->_stride);
			startVertex = (0 != renderState._drawData._startVertex) ?
				(renderState._drawData._startVertex) : (0);
			_pDevice->DrawPrimitive(D3DPT_LINELIST, startVertex, numPrim);
		}
	}

}


//void video::VideoDevice::DrawPrimitive(const video::RenderState & renderState, Matrix *matrices)
//{
//	Effect *effectPtr{};
//	VertexBuffer *vertexPtr {};
//	Material *materialPtr {};
//	VertexDecl *declPtr{};
//
//	//머테리얼 설정 여부 확인
//	if (renderState._material != _activeState._material)
//	{
//		_activeState._material = renderState._material;
//	}
//
//	if (_activeState._material.IsValid())
//	{
//		materialPtr = &_materials[_activeState._material.index];
//	}
//
//	if (renderState._vertexBuffer != _activeState._vertexBuffer)
//	{
//		_activeState._vertexBuffer = renderState._vertexBuffer;
//		vertexPtr = &_vertexBuffers[_activeState._vertexBuffer.index];
//		//Vertex Decl설정여부 확인
//		if (_activeState._vertexDecl != vertexPtr->_decl)
//		{
//			_activeState._vertexDecl = vertexPtr->_decl;
//			declPtr = &_vertexDecls[_activeState._vertexDecl.index];
//			_pDevice->SetVertexDeclaration(declPtr->_ptr);
//		}
//		else
//		{
//			declPtr = &_vertexDecls[_activeState._vertexDecl.index];
//		}
//		_pDevice->SetStreamSource(0, vertexPtr->_ptr, 0, declPtr->_stride);
//	}
//	else
//	{
//		vertexPtr = &_vertexBuffers[_activeState._vertexBuffer.index];
//		declPtr = &_vertexDecls[_activeState._vertexBuffer.index];
//	}
//
//	if (renderState._effect != _activeState._effect)
//	{
//		_activeState._effect = renderState._effect;
//	}
//	effectPtr = &_effects[_activeState._effect.index];
//
//	effectPtr->SetMatrix("gWorld", matrices[0]);
//	effectPtr->SetMatrix("gView", matrices[1]);
//	effectPtr->SetMatrix("gProjection", matrices[2]);
//
//	if (materialPtr)
//	{
//		for (uint32 i = 0; i < VIDEO_CONFIG_MATERIAL_TEXTURE_MAX_NUM; ++i)
//		{
//			if (materialPtr->_textureHandles[i].IsValid())
//			{
//				effectPtr->SetTexture(PredefinedUniform::ParamName[PredefinedUniform::Enum::eTexture0 + i], 
//					_textures[materialPtr->_textureHandles[i].index]);
//			}
//		}
//	}
//	uint32 numPrim{};
//	uint32 startVertex{};
//
//	numPrim = (0 != renderState._drawData._numVertices) ? (renderState._drawData._numPrim) : (vertexPtr->_size / declPtr->_stride);
//	startVertex = (0 != renderState._drawData._startVertex) ? (renderState._drawData._startVertex) : (0);
//
//	uint32 numPass = effectPtr->BeginEffect();
//	for (uint32 i = 0; i < numPass; ++i)
//	{
//		effectPtr->BeginPass(i);
//		_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, startVertex, numPrim);
//		effectPtr->EndPass();
//	}
//	effectPtr->EndEffect();
//}
//
//void video::VideoDevice::DrawIndexPrimitive(const video::RenderState & renderState, Matrix *matrices)
//{
//	Effect *effectPtr{};
//	VertexBuffer *vertexPtr{};
//	IndexBuffer *indexPtr{};
//	Material *materialPtr{};
//	VertexDecl *declPtr{};
//
//	//머테리얼 설정 여부 확인
//	if (renderState._material != _activeState._material)
//	{
//		_activeState._material = renderState._material;
//	}
//	if (_activeState._material.IsValid())
//	{
//		materialPtr = &_materials[_activeState._material.index];
//	}
//
//	if (renderState._vertexBuffer!= _activeState._vertexBuffer)
//	{
//		_activeState._vertexBuffer = renderState._vertexBuffer;
//		vertexPtr = &_vertexBuffers[_activeState._vertexBuffer.index];
//		//Vertex Decl설정여부 확인
//		if (_activeState._vertexDecl != vertexPtr->_decl)
//		{
//			_activeState._vertexDecl = vertexPtr->_decl;
//			declPtr = &_vertexDecls[_activeState._vertexDecl.index];
//			_pDevice->SetVertexDeclaration(declPtr->_ptr);
//		}
//		else
//		{
//			declPtr = &_vertexDecls[_activeState._vertexDecl.index];
//		}
//		_pDevice->SetStreamSource(0, vertexPtr->_ptr, 0, declPtr->_stride);
//	}
//	else
//	{
//		vertexPtr = &_vertexBuffers[_activeState._vertexBuffer.index];
//		declPtr = &_vertexDecls[_activeState._vertexBuffer.index];
//	}
//
//	if (renderState._indexBuffer != _activeState._indexBuffer)
//	{
//		_activeState._indexBuffer = renderState._indexBuffer;
//	}
//	indexPtr = &_indexBuffers[_activeState._indexBuffer.index];
//	_pDevice->SetIndices(indexPtr->_ptr);
//
//	if (renderState._effect != _activeState._effect)
//	{
//		_activeState._effect = renderState._effect;
//	}
//	effectPtr = &_effects[_activeState._effect.index];
//
//	effectPtr->SetMatrix("gWorld", matrices[0]);
//	effectPtr->SetMatrix("gView", matrices[1]);
//	effectPtr->SetMatrix("gProjection", matrices[2]);
//
//	if (materialPtr)
//	{
//		for (uint32 i = 0; i < VIDEO_CONFIG_MATERIAL_TEXTURE_MAX_NUM; ++i)
//		{
//			if (materialPtr->_textureHandles[i].IsValid())
//			{
//				effectPtr->SetTexture(PredefinedUniform::ParamName[PredefinedUniform::Enum::eTexture0 + i],
//					_textures[materialPtr->_textureHandles[i].index]);
//			}
//		}
//	}
//
//	uint32 numVertices{};
//	uint32 numPrim{};
//	uint32 startIndex{};
//
//	numVertices = (0 != renderState._drawData._numVertices) ? (renderState._drawData._numVertices) : (vertexPtr->_size / declPtr->_stride);
//	numPrim = (0 != renderState._drawData._numPrim) ? (renderState._drawData._numPrim) : ((indexPtr->_size / 2 ) / 3);
//	startIndex = (0 != renderState._drawData._startIndex) ? (renderState._drawData._startIndex) : 0;
//
//	uint32 numPass = effectPtr->BeginEffect();
//	for (uint32 i = 0; i < numPass; ++i)
//	{
//		effectPtr->BeginPass(i);
//		_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, numVertices, startIndex, numPrim);
//		effectPtr->EndPass();
//	}
//	effectPtr->EndEffect();
//}

//TODO : Default texture들을 로드 하자
void video::VideoDevice::LoadDefaultTextures()
{
	TextureHandle diffuse = VIDEO->CreateTexture("../resources/textures/diffuseDefault.png", "diffuseDefault.png");
	TextureHandle normal = VIDEO->CreateTexture("../resources/textures/normalDefault.png", "normalDefault.png");
	TextureHandle specular = VIDEO->CreateTexture("../resources/textures/specularDefault.png", "specularDefault.png");
	TextureHandle emission = VIDEO->CreateTexture("../resources/textures/emissionDefault.png", "emissionDefault.png");

	MaterialHandle material = VIDEO->CreateMaterial("default");
	VIDEO->MaterialAddTexture(material, VIDEO_TEXTURE0, diffuse);
	VIDEO->MaterialAddTexture(material, VIDEO_TEXTURE1, normal);
	VIDEO->MaterialAddTexture(material, VIDEO_TEXTURE2, specular);
	VIDEO->MaterialAddTexture(material, VIDEO_TEXTURE3, emission);
}

void video::VideoDevice::MakeDefaultVertexDecls()
{
	//StaticVertex Decl
	VertexDecl staticVertex;
	staticVertex.Begin();
	staticVertex.Add(VertexElement(0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0));
	staticVertex.Add(VertexElement(0, sizeof(Vector3), D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0));
	staticVertex.Add(VertexElement(0, sizeof(Vector3) * 2, 
		D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0));
	staticVertex.Add(VertexElement(0, sizeof(Vector3) * 3, 
		D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0));
	staticVertex.Add(VertexElement(0, sizeof(Vector3) * 4, 
		D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0));
	staticVertex.End(sizeof(StaticMeshVertex));
	VIDEO->CreateVertexDecl(&staticVertex, StaticMeshVertex::_name);

	//LineVertex Decl
	VertexDecl lineVertex;
	lineVertex.Begin();
	lineVertex.Add(VertexElement(0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0));
	lineVertex.Add(VertexElement(0, sizeof(Vector3), D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0));
	lineVertex.End(sizeof(LineVertex));
	VIDEO->CreateVertexDecl(&lineVertex, LineVertex::_name);

	//StaticTestVertex Decl
	VertexDecl staticTestVertex;
	staticTestVertex.Begin();
	staticTestVertex.Add(VertexElement(0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0));
	staticTestVertex.Add(VertexElement(0, sizeof(Vector3), D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0));
	staticTestVertex.Add(VertexElement(0, sizeof(Vector3) * 2, D3DDECLTYPE_FLOAT2, 
		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0));
	staticTestVertex.End(sizeof(StaticTestVertex));
	VIDEO->CreateVertexDecl(&staticTestVertex, StaticTestVertex::_name);

}

//TODO : Default Shaders들을 로드하자
void video::VideoDevice::LoadDefaultEffects()
{
	VIDEO->CreateEffect("../resources/shaders/staticMesh.fx", "staticMesh.fx");
	VIDEO->CreateEffect("../resources/shaders/staticTestMesh.fx", "staticTestMesh.fx");
	VIDEO->CreateEffect("../resources/shaders/skinnedMesh.fx", "skinnedMesh.fx");
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

const VertexBuffer * video::VideoDevice::GetVertexBuffer(VertexBufferHandle handle)
{
	if (handle.IsValid())
	{
		return &_vertexBuffers[handle.index];
	}
	return nullptr;
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

const IndexBuffer * video::VideoDevice::GetIndexBuffer(IndexBufferHandle handle)
{
	if (handle.IsValid())
	{
		return &_indexBuffers[handle.index];
	}
	return nullptr;
}

void video::VideoDevice::DestroyIndexBuffer(IndexBufferHandle handle)
{
	_indexBuffers[handle.index].Destroy();
	_indexBufferPool.Remove(handle);
}

VertexDeclHandle video::VideoDevice::CreateVertexDecl(const VertexDecl *decl, const std::string &name)
{
	VertexDeclHandle result = _vertexDeclHandlePool.Create(name);
	memcpy(&_vertexDecls[result.index], decl, sizeof(VertexDecl));
	return result;
}

VertexDeclHandle video::VideoDevice::GetVertexDecl(const std::string & name)
{
	return _vertexDeclHandlePool.Get(name);
}

const VertexDecl & video::VideoDevice::GetVertexDecl(VertexDeclHandle handle)
{
	Assert(handle.IsValid());
	return _vertexDecls[handle.index];
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
		//텍스쳐의 생성에 실패했다면 만든 핸들은 지우고 빈 핸들을 반환한다
		_textureHandlePool.Remove(result);
		return TextureHandle();
	}
	return result;
}

TextureHandle video::VideoDevice::GetTexture(const std::string & name)
{
	return _textureHandlePool.Get(name);
}

const Texture * video::VideoDevice::GetTexture(TextureHandle handle)
{
	if (handle.IsValid())
	{
		return &_textures[handle.index];
	}
	return nullptr;
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

const Effect * video::VideoDevice::GetEffect(EffectHandle handle)
{
	if (handle.IsValid())
	{
		return &_effects[handle.index];
	}
	return nullptr;
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

const Material * video::VideoDevice::GetMaterial(MaterialHandle handle)
{
	if (handle.IsValid())
	{
		return &_materials[handle.index];
	}
	return nullptr;
}

void video::VideoDevice::DestroyMaterial(MaterialHandle handle)
{
	_materials[handle.index].Destroy();
	_materialHandlePool.Remove(handle);
}

void video::VideoDevice::MaterialAddTexture(MaterialHandle material, uint32 textureSlot, TextureHandle texture)
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

RenderGroupHandle video::VideoDevice::CreateRenderGroup(video::VertexBufferHandle vHandle, video::IndexBufferHandle iHandle,
	const RenderGroup::MaterialRange &materialRange, const std::string &name)
{
	RenderGroupHandle result = _renderGroupHandlePool.Create(name);
	_renderGroups[result.index].Create(vHandle, iHandle, materialRange);
	return result;
}

RenderGroupHandle video::VideoDevice::GetRenderGroup(const std::string & name)
{
	return _renderGroupHandlePool.Get(name);
}

void video::VideoDevice::DestroyRenderGroup(RenderGroupHandle handle)
{
	_renderGroups[handle.index].Destroy();
	_renderGroupHandlePool.Remove(handle);
}

void video::VideoDevice::RenderGroupSetEffect(RenderGroupHandle group, EffectHandle effect)
{
}

StaticXMeshHandle video::VideoDevice::CreateStaticXMesh(const std::string fileName, const Matrix * pCorrection, const std::string &name)
{
	StaticXMeshHandle result = _staticXMeshHandlePool.Create(name);
	if (!_staticMeshes[result.index].Create(fileName, pCorrection))
	{
		Console::Log("StaticMesh %s create failed\n", fileName);
		return StaticXMeshHandle();
	}
	return result;
}

StaticXMeshHandle video::VideoDevice::GetStaticXMesh(const std::string & name)
{
	return _staticXMeshHandlePool.Get(name);
}

const StaticXMesh *video::VideoDevice::GetStaticXMesh(StaticXMeshHandle handle)
{
	if (handle.IsValid())
	{
		return &_staticMeshes[handle.index];
	}
	return nullptr;
}

void video::VideoDevice::DestroyStaticXMesh(StaticXMeshHandle handle)
{
	_staticMeshes[handle.index].Destroy();
	_staticXMeshHandlePool.Remove(handle);
}

SkinnedXMeshHandle video::VideoDevice::CreateSkinnedXMesh(const std::string fileName, const Matrix * pCorrection, const std::string & name)
{
	SkinnedXMeshHandle result = _skinnedXMeshHandlePool.Create(name);
	if (!_skinnedMeshes[result.index].Create(fileName, pCorrection))
	{
		Console::Log("SkinnedMesh %s create failed\n", fileName);
		return SkinnedXMeshHandle();
	}
	return result;
}

SkinnedXMeshHandle video::VideoDevice::GetSkinnedXMesh(const std::string & name)
{
	return _skinnedXMeshHandlePool.Get(name);
}

const SkinnedXMesh * video::VideoDevice::GetSkinnedXMesh(SkinnedXMeshHandle handle)
{
	if (handle.IsValid())
	{
		return &_skinnedMeshes[handle.index];
	}
	return nullptr;
}

void video::VideoDevice::DestroySkinnedMesh(SkinnedXMeshHandle handle)
{
	if (handle.IsValid())
	{
		_skinnedMeshes[handle.index].Destroy();
	}
	_skinnedXMeshHandlePool.Remove(handle);
}
