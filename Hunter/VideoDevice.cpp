#include "stdafx.h"
#include "VideoDevice.h"

#include "stdafx.h"
#include "VideoDevice.h"

IDirect3DDevice9 *gpDevice;

using namespace video;

VideoDevice::VideoDevice()
	: _vertexBufferPool(VIDEO_CONFIG_VERTEXBUFFER_MAX_NUM),
	_indexBufferPool(VIDEO_CONFIG_VERTEXBUFFER_MAX_NUM),
	_textureHandlePool(VIDEO_CONFIG_VERTEXBUFFER_MAX_NUM),
	_effectHandlePool(VIDEO_CONFIG_VERTEXBUFFER_MAX_NUM),
	_vertexDeclHandlePool(VIDEO_CONFIG_VERTEXBUFFER_MAX_NUM),
	_renderViewHandlePool(VIDEO_CONFIG_RENDER_VIEW_MAX_NUM),
	_materialHandlePool(VIDEO_CONFIG_MATERIAL_MAX_NUM),
	_staticXMeshHandlePool(VIDEO_CONFIG_STATIC_XMESH_MAX_NUM),
	_skinnedXMeshHandlePool(VIDEO_CONFIG_SKINNED_XMESH_MAX_NUM),
	_animationInstanceHandlePool(VIDEO_CONFIG_ANIMATION_MAX_NUM),
	_fontHandlePool(VIDEO_CONFIG_FONT_MAX_NUM)
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

	//im::GuiRenderer::GetInstance()->Init(gpDevice, "consolas", WINSIZEX, WINSIZEY);

	LoadDefaultTextures();
	LoadDefaultEffects();
	MakeDefaultVertexDecls();

	DEBUG_DRAWER->Init();

	return true;
}

void VideoDevice::ShutDown()
{
	DEBUG_DRAWER->Shutdown();
	DEBUG_DRAWER->ReleaseInstance();

	COM_RELEASE(gpDevice);
	COM_RELEASE(_pD3D);
}

void VideoDevice::Update(float deltaTime)
{


}

//NOTE : Render 함수는 RenderView를 그리는 역할을 한다
// 만약 RenderView의 RenderState에 Effect가 설정 되어있다면 Effect를 사용하여 렌더 할 것이고 아니라면 
// 현제 Device에 설정된 값으로 렌더 될 것이다...
void video::VideoDevice::Render(RenderView & renderView) 
{
	static int32 counter = 0;

	int32 drawCount = 0;

	Matrix sendingMatrix[3];
	MatrixIdentity(&sendingMatrix[0]);
	sendingMatrix[1] = renderView._pCamera->GetViewMatrix();
	sendingMatrix[2] = renderView._pCamera->GetViewProjectionMatrix();

	for (uint32 i = 0; i < renderView._commandCount; ++i)
	{
		const RenderCommand &command = renderView._renderCommands[i];

		VertexBuffer &vBuffer = *GetVertexBuffer(command._vHandle);
		const Effect &effect = *GetEffect(command._effectHandle);
		const Material &material = *GetMaterial(command._materialHandle);
		const VertexDecl &decl = *VIDEO->GetVertexDecl(vBuffer._decl);

		effect.SetMaterial(material);
		effect.SetMatrix("matViewProjection", sendingMatrix[2]);

		if (command._drawType == RenderCommand::DrawType::eAnimated)
		{
			//메트릭스 행렬을 주자....
			const MatrixCache::CacheRange &range = command._cacheRange;
			if (range._end != -1)
			{
				effect.SetMatrices("FinalTransforms", &renderView._matrixCache._cache[range._start], range._end - range._start + 1);
				effect.SetInt("NumVertInfluences", command._vertInfluence);
			}
			effect.SetMatrix("matWorld", sendingMatrix[0]);
		}
		else
		{
			if (command._cacheRange._start == -1)
			{
				effect.SetMatrix("matWorld", sendingMatrix[0]);
			}
			else
			{
				effect.SetMatrix("matWorld", renderView._matrixCache._cache[command._cacheRange._start]);
			}
		}

		if (!command._iHandle.IsValid())
		{
			gpDevice->SetVertexDeclaration(decl._ptr);
			gpDevice->SetStreamSource(0, vBuffer._ptr, 0, decl._stride);

			D3DPRIMITIVETYPE primType = (command._primType == RenderCommand::PrimType::eTriangleList) ?
				(D3DPT_TRIANGLELIST) : (D3DPT_LINELIST);

			uint32 primCount = 0;
			if (primType == D3DPT_TRIANGLELIST)
			{
				primCount = (command._numPrim == 0) ? (vBuffer._size / decl._stride) / 3 : (command._numPrim);
			}
			else if(primType == D3DPT_LINELIST)
			{
				primCount = (command._numPrim == 0) ? (vBuffer._size / decl._stride) / 2 : (command._numPrim);
			}

			uint32 numPass = effect.BeginEffect();
			for (uint32 j = 0; j < numPass; ++j)
			{
				effect.BeginPass(j);
				gpDevice->DrawPrimitive(primType,
					command._startVertex, primCount);
				effect.EndPass();
			}
			effect.EndEffect();
		}
		else
		{
			const IndexBuffer &iBuffer = *GetIndexBuffer(command._iHandle);

			gpDevice->SetVertexDeclaration(decl._ptr);
			gpDevice->SetStreamSource(0, vBuffer._ptr, 0, decl._stride);
			gpDevice->SetIndices(iBuffer._ptr);

			uint32 numPass = effect.BeginEffect();

			for (uint32 j = 0; j < numPass; ++j)
			{
				effect.BeginPass(j);
				gpDevice->DrawIndexedPrimitive((command._primType == RenderCommand::PrimType::eTriangleList) ? (D3DPT_TRIANGLELIST) : (D3DPT_LINELIST),
					0, 0, 
					(command._numVertices == 0) ? (vBuffer._size / decl._stride) : (command._numVertices),
					(command._startIndex == 0) ? (0) : (command._startIndex),
					(command._numPrim == 0) ? (iBuffer._size / iBuffer._stride / 3) : (command._numPrim));
				effect.EndPass();
			}
			effect.EndEffect();
		}
		drawCount++;
	}
	#if defined (DEBUG) || defined (_DEBUG) 
	if (counter % 60 == 0)
	{
		Console::Log("Draw Count : % d\n", drawCount);
	}
	#endif
	counter++;
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

	_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	_pDevice->SetRenderState(D3DRS_DITHERENABLE, TRUE );

	_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE );
	_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE );
	_pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);

	return result;
}

//TODO : Default texture들을 로드 하자
void video::VideoDevice::LoadDefaultTextures()
{
	TextureHandle diffuse = VIDEO->CreateTexture("../resources/Textures/diffuseDefault.png", "diffuseDefault.png");
	TextureHandle normal = VIDEO->CreateTexture("../resources/Textures/normalDefault.png", "normalDefault.png");
	TextureHandle specular = VIDEO->CreateTexture("../resources/Textures/specularDefault.png", "specularDefault.png");
	TextureHandle emission = VIDEO->CreateTexture("../resources/Textures/emissionDefault.png", "emissionDefault.png");

	MaterialHandle material = VIDEO->CreateMaterial("Default");
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

	//DebugVertex Decl
	VertexDecl debugVertex;
	debugVertex.Begin();
	debugVertex.Add(VertexElement(0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0));
	debugVertex.Add(VertexElement(0, sizeof(Vector3), D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0));
	debugVertex.End(sizeof(DebugVertex));
	VIDEO->CreateVertexDecl(&debugVertex, DebugVertex::_name);

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
	VIDEO->CreateEffect("../resources/Shaders/StaticMesh.fx", "StaticMesh.fx");
	VIDEO->CreateEffect("../resources/Shaders/StaticTestMesh.fx", "StaticTestMesh.fx");
	VIDEO->CreateEffect("../resources/Shaders/SkinnedMesh.fx", "SkinnedMesh.fx");
	VIDEO->CreateEffect("../resources/Shaders/TerrainBase.fx", "TerrainBase.fx");

	VIDEO->CreateEffect("../resources/Shaders/DebugShader.fx", "DebugShader.fx");
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
		_vertexBufferPool.Remove(result);
		Assert(false);//Creation failed
	}
	return result;
}

VertexBufferHandle video::VideoDevice::GetVertexBufferFromXMesh(ID3DXMesh *pMesh, const std::string &name)
{
	Assert(pMesh);

	VertexBufferHandle result = _vertexBufferPool.Create(name);

	IDirect3DVertexBuffer9 *meshVBuffer;
	D3DVERTEXBUFFER_DESC bufferDesc{};
	HRESULT_CHECK(pMesh->GetVertexBuffer(&meshVBuffer));
	meshVBuffer->GetDesc(&bufferDesc);

	D3DVERTEXELEMENT9 elements[MAX_FVF_DECL_SIZE];
	pMesh->GetDeclaration(elements);

	uint32 vertexStride = D3DXGetDeclVertexSize(elements, 0);

	VertexDecl decl;
	decl.Begin();
	for (int32 i = 0; i < MAX_FVF_DECL_SIZE; ++i)
	{
		if (elements[i].Stream == 255)
		{
			break;
		}
		decl.Add(elements[i]);
	}
	decl.End(vertexStride);

	VertexDeclHandle declHandle = VIDEO->CreateVertexDecl(&decl);

	VertexBuffer &refBuffer = _vertexBuffers[result.index];
	refBuffer._ptr = meshVBuffer;
	refBuffer._size = bufferDesc.Size;
	refBuffer._dynamic = false;
	refBuffer._decl = declHandle;

	return result;
}

VertexBufferHandle video::VideoDevice::GetVertexBuffer(const std::string & name)
{
	return _vertexBufferPool.Get(name);
}

VertexBuffer * video::VideoDevice::GetVertexBuffer(VertexBufferHandle handle)
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

IndexBufferHandle video::VideoDevice::CreateIndexBuffer(Memory * memory, uint32 stride, const std::string & name)
{
	IndexBufferHandle result = _indexBufferPool.Create(name);
	if (!_indexBuffers[result.index].Create(memory->_size, memory->_data, stride))
	{
		Assert(false);//Creation failed
	}
	return result;
}

IndexBufferHandle video::VideoDevice::GetIndexBufferFromXMesh(ID3DXMesh * pMesh, const std::string &name)
{
	Assert(pMesh);

	IndexBufferHandle result = _indexBufferPool.Create(name);

	IDirect3DIndexBuffer9 *meshIBuffer;
	D3DINDEXBUFFER_DESC bufferDesc{};
	HRESULT_CHECK(pMesh->GetIndexBuffer(&meshIBuffer));
	meshIBuffer->GetDesc(&bufferDesc);

	IndexBuffer &refBuffer = _indexBuffers[result.index];
	refBuffer._ptr = meshIBuffer;
	refBuffer._size = bufferDesc.Size;
	refBuffer._dynamic = false;
	refBuffer._stride = (bufferDesc.Type == D3DFMT_INDEX16) ? (2) : (4);

	return result;
}

IndexBufferHandle video::VideoDevice::GetIndexBuffer(const std::string & name)
{
	return _indexBufferPool.Get(name);
}

IndexBuffer * video::VideoDevice::GetIndexBuffer(IndexBufferHandle handle)
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

VertexDecl * video::VideoDevice::GetVertexDecl(VertexDeclHandle handle)
{
	Assert(handle.IsValid());
	return &_vertexDecls[handle.index];
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

Texture * video::VideoDevice::GetTexture(TextureHandle handle) 
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

Effect * video::VideoDevice::GetEffect(EffectHandle handle)
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

Material * video::VideoDevice::GetMaterial(MaterialHandle handle)
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

StaticXMesh *video::VideoDevice::GetStaticXMesh(StaticXMeshHandle handle)
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

SkinnedXMesh * video::VideoDevice::GetSkinnedXMesh(SkinnedXMeshHandle handle)
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

AnimationInstanceHandle video::VideoDevice::CreateAnimationInstance(SkinnedXMeshHandle xMesh, const std::string & name)
{
	AnimationInstanceHandle result = _animationInstanceHandlePool.Create(name);
	if (!_animationInstances[result.index].Create(xMesh))
	{
		Console::Log("Skinned Animation create failed\n");
		return AnimationInstanceHandle();
	}
	return result;
}

AnimationInstanceHandle video::VideoDevice::GetAnimationInstance(const std::string & name)
{
	return _animationInstanceHandlePool.Get(name);
}

AnimationInstance * video::VideoDevice::GetAnimationInstance(AnimationInstanceHandle handle)
{
	if (handle.IsValid())
	{
		return &_animationInstances[handle.index];
	}
	return nullptr;
}

void video::VideoDevice::DestroyAnimationInstance(AnimationInstanceHandle handle)
{
	if (handle.IsValid())
	{
		_animationInstances[handle.index].Destroy();
	}
	_animationInstanceHandlePool.Remove(handle);
}

FontHandle video::VideoDevice::CreateFont(const D3DXFONT_DESC & fontDesc, const std::string & name)
{
	FontHandle result = _fontHandlePool.Create(name);
	if (!_fonts[result.index].Create(fontDesc))
	{
		Console::Log("Skinned Animation create failed\n");
		return FontHandle();
	}
	return result;
}

FontHandle video::VideoDevice::GetFont(const std::string & name)
{
	return _fontHandlePool.Get(name);
}

Font * video::VideoDevice::GetFont(FontHandle handle)
{
	if (handle.IsValid())
	{
		return &_fonts[handle.index];
	}
	return nullptr;
}

void video::VideoDevice::DestroyFont(FontHandle handle)
{
	if (handle.IsValid())
	{
		_fonts[handle.index].Destroy();
	}
	_fontHandlePool.Remove(handle);
}

void video::VideoDevice::DrawFont(FontHandle handle, const std::string & str, int32 x, int32 y, uint32 color)
{
	_fonts[handle.index].PrintText(str, x, y, color);
}

void video::VideoDevice::DrawFontShadow(FontHandle handle, const std::string & str, int32 x, int32 y, uint32 color, uint32 shadow)
{
	_fonts[handle.index].PrintTextShadow(str, x, y, color);
}

void video::VideoDevice::GetBoundingRect(FontHandle handle, const std::string & str, int32 x, int32 y, RECT * pOutRect)
{
	_fonts[handle.index].GetBoundingRect(str, x, y, pOutRect);
}
