#include "stdafx.h"
#include "VideoDevice9.h"

#include "VideoPlatform.h"

using namespace im;

namespace video
{
	static const D3DPRIMITIVETYPE sPrimType[] =
	{
		D3DPT_TRIANGLELIST,
		D3DPT_LINELIST,
	};

	static const uint32_t sPrimNumVerts[] =
	{
		3,
		2,
	};

	static const D3DMULTISAMPLE_TYPE sMsaa[] =
	{
		D3DMULTISAMPLE_NONE,
		D3DMULTISAMPLE_2_SAMPLES,
		D3DMULTISAMPLE_4_SAMPLES,
		D3DMULTISAMPLE_8_SAMPLES,
		D3DMULTISAMPLE_16_SAMPLES,
	};

	static const D3DBLEND sBlendFactor[] =
	{
		(D3DBLEND)0, // ignored
		D3DBLEND_ZERO,
		D3DBLEND_ONE,
		D3DBLEND_SRCCOLOR,
		D3DBLEND_INVSRCCOLOR,
		D3DBLEND_SRCALPHA,
		D3DBLEND_INVSRCALPHA,
		D3DBLEND_DESTALPHA,
		D3DBLEND_INVDESTALPHA,
		D3DBLEND_DESTCOLOR,
		D3DBLEND_INVDESTCOLOR,
		D3DBLEND_SRCALPHASAT,
	};

	static const D3DCMPFUNC sDepthFunc[] =
	{
		(D3DCMPFUNC)0, // ignored
		D3DCMP_LESS,
		D3DCMP_LESSEQUAL,
		D3DCMP_EQUAL,
		D3DCMP_GREATEREQUAL,
		D3DCMP_GREATER,
		D3DCMP_NOTEQUAL,
		D3DCMP_NEVER,
		D3DCMP_ALWAYS,
	};

	static const D3DCULL sCullMode[] =
	{
		D3DCULL_NONE,
		D3DCULL_CW,
		D3DCULL_CCW,
	};

	static const D3DFORMAT sColorFormat[] =
	{
		D3DFMT_UNKNOWN, // ignored
		D3DFMT_A8R8G8B8,
		D3DFMT_R32F,
	};

	static const D3DFORMAT sDepthFormat[] =
	{
		D3DFMT_UNKNOWN, // ignored
		D3DFMT_D24S8,
	};

	static const D3DTEXTUREADDRESS sTextureAddress[] =
	{
		D3DTADDRESS_WRAP,
		D3DTADDRESS_MIRROR,
		D3DTADDRESS_CLAMP,
	};

	static const D3DTEXTUREFILTERTYPE sTextureFilter[] =
	{
		D3DTEXF_LINEAR,
		D3DTEXF_POINT,
		D3DTEXF_ANISOTROPIC,
	};


	struct RendererContext
	{
		RendererContext()
			: _initialized(false)
			, _fmtNULL(false)
			, _fmtDF16(false)
			, _fmtDF24(false)
			, _fmtINTZ(false)
			, _fmtRAWZ(false)
			, _flags(VIDEO_RESET_NONE)
		{
		}

		void Init()
		{
			memset(&_params, 0, sizeof(_params));
			_params.BackBufferWidth = VIDEO_DEFAULT_WIDTH;
			_params.BackBufferHeight = VIDEO_DEFAULT_HEIGHT;
			_params.BackBufferFormat = D3DFMT_X8R8G8B8;
			_params.BackBufferCount = 1;
			_params.MultiSampleType = sMsaa[ (_flags & VIDEO_RESET_MSAA_MASK) >> VIDEO_RESET_MSAA_SHIFT ];
			_params.MultiSampleQuality = 0;
			_params.EnableAutoDepthStencil = TRUE;
			_params.AutoDepthStencilFormat = D3DFMT_D24S8;
			_params.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
			_params.FullScreen_RefreshRateInHz = 0;
			_params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
			_params.SwapEffect = D3DSWAPEFFECT_DISCARD;
			_params.hDeviceWindow = gWindowHandle;
			_params.Windowed = true;

			RECT rect;
			GetWindowRect(gWindowHandle, &rect);
			_params.BackBufferWidth = rect.right - rect.left;
			_params.BackBufferHeight = rect.bottom - rect.top;

			_d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

			uint32 behaviorFlags[] =
			{
				D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE,
				D3DCREATE_MIXED_VERTEXPROCESSING,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			};

			for (uint32 i = 0; i < ARRAY_COUNT(behaviorFlags) && nullptr == _device; ++i)
			{
				HRESULT_CHECK(_d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, gWindowHandle, 
					behaviorFlags[i], &_params, &_device ));
			}
			//_fmtNULL = SUCCEEDED(_d3d9->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
			//	D3DFMT_X8R8G8B8, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_TEXTURE, D3DFMT_NULL));
			//_fmtDF16 = SUCCEEDED(_d3d9->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
			//	D3DFMT_X8R8G8B 8, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_TEXTURE, D3DFMT_DF16));
			//_fmtDF24 = SUCCEEDED(_d3d9->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
			//	D3DFMT_X8R8G8B8, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_TEXTURE, D3DFMT_DF24));
			//_fmtINTZ = SUCCEEDED(_d3d9->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
			//	D3DFMT_X8R8G8B8, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_TEXTURE, D3DFMT_INTZ));
			//_fmtRAWZ = SUCCEEDED(_d3d9->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
			//	D3DFMT_X8R8G8B8, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_TEXTURE, D3DFMT_RAWZ));

			_fmtDepth = D3DFMT_D24S8;

			PostReset();

			_initialized = true;
		}

		void Shutdown()
		{
			COM_RELEASE(_device);
			COM_RELEASE(_d3d9);
		}


		void UpdateResolution(const Resolution& resolution)
		{
			if (_params.BackBufferWidth != resolution._width || 
				_params.BackBufferHeight != resolution._height || 
				_flags != resolution._flags)
			{
				_flags = resolution._flags;

				//TODO : Implement this
				//_textVideoMem.Resize(false, resolution._width, resolution._height);

				D3DDEVICE_CREATION_PARAMETERS dcp;
				HRESULT_CHECK(_device->GetCreationParameters(&dcp));

				D3DDISPLAYMODE dm;
				HRESULT_CHECK(_d3d9->GetAdapterDisplayMode(dcp.AdapterOrdinal, &dm));

				_params.BackBufferFormat = dm.Format;

				_params.BackBufferWidth = resolution._width;
				_params.BackBufferHeight = resolution._height;
				_params.FullScreen_RefreshRateInHz =
					VIDEO_RESET_FULLSCREEN == (_flags & VIDEO_RESET_FULLSCREEN_MASK) ? 60 : 0;
				_params.PresentationInterval = !!(_flags & VIDEO_RESET_VSYNC) ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;

				D3DMULTISAMPLE_TYPE msaa = sMsaa[(_flags & VIDEO_RESET_MSAA_MASK) >> VIDEO_RESET_MSAA_SHIFT];
				HRESULT hr = _d3d9->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
					D3DDEVTYPE_HAL, _params.BackBufferFormat, _params.Windowed, msaa, NULL);

				_params.MultiSampleType = SUCCEEDED(hr) ? msaa : D3DMULTISAMPLE_NONE;

				PreReset();
				HRESULT_CHECK(_device->Reset(&_params));
				PostReset();
			}
		}

		void SetRenderTarget(RenderTargetHandle renderTargetHandle)
		{
			if (renderTargetHandle.index == video::invalidHandle)
			{
				HRESULT_CHECK(_device->SetRenderTarget(0, _backBufferColor));
				HRESULT_CHECK(_device->SetDepthStencilSurface(_backBufferDepthStencil));
			}
			else
			{
				RenderTarget& renderTargetRef = _renderTargets[renderTargetHandle.index];
				HRESULT_CHECK(_device->SetRenderTarget(0, renderTargetRef._color));
				HRESULT_CHECK(_device->SetDepthStencilSurface((NULL != renderTargetRef._depth) ?
					renderTargetRef._depth : _backBufferDepthStencil));
			}
		}

		void SetShaderConstantF(uint8_t flags, uint16_t regIndex, const float* val, uint16_t numRegs)
		{
			if (_flags & VIDEO_UNIFORM_FRAGMENTBIT)
			{
				HRESULT_CHECK(_device->SetPixelShaderConstantF(regIndex, val, numRegs));
			}
			else
			{
				HRESULT_CHECK(_device->SetVertexShaderConstantF(regIndex, val, numRegs));
			}
		}

		void Reset()
		{
			PreReset();

			HRESULT hr;

			do
			{
				hr = _device->Reset(&_params);
			} while (FAILED(hr));

			PostReset();
		}

		bool IsLost(HRESULT hr) const
		{
			return (D3DERR_DEVICELOST == hr) ||
				(D3DERR_DRIVERINTERNALERROR == hr);
		}

		void Flip()
		{
			HRESULT hr;
			hr = _device->Present(NULL, NULL, NULL, NULL);

			if (IsLost(hr))
			{
				do
				{
					do
					{
						hr = _device->TestCooperativeLevel();
					} while (D3DERR_DEVICENOTRESET != hr);

					Reset();
					hr = _device->TestCooperativeLevel();
				} while (FAILED(hr));
			}
			else if (FAILED(hr))
			{
				Console::Log("Present failed with err 0x%08x.", hr);
			}
		}

		void PreReset()
		{
			for (uint32_t stage = 0; stage < VIDEO_STATE_TEX_COUNT; ++stage)
			{
				HRESULT_CHECK(_device->SetTexture(stage, NULL));
			}

			HRESULT_CHECK(_device->SetRenderTarget(0, _backBufferColor));
			HRESULT_CHECK(_device->SetDepthStencilSurface(_backBufferDepthStencil));
			HRESULT_CHECK(_device->SetVertexShader(nullptr));
			HRESULT_CHECK(_device->SetPixelShader(nullptr));
			HRESULT_CHECK(_device->SetStreamSource(0, nullptr, 0, 0));
			HRESULT_CHECK(_device->SetIndices(nullptr));

			COM_RELEASE(_backBufferColor);
			COM_RELEASE(_backBufferDepthStencil);

			for (uint32_t i = 0; i < ARRAY_COUNT(_indexBuffers); ++i)
			{
				_indexBuffers[i].PreReset();
			}

			for (uint32_t i = 0; i < ARRAY_COUNT(_vertexBuffers); ++i)
			{
				_vertexBuffers[i].PreReset();
			}

			//TODO : Implement this
			//for (uint32_t i = 0; i < ARRAY_COUNT(_renderTargets); ++i)
			//{
			//	_renderTargets[i].PreReset();
			//}
		}

		void PostReset()
		{
			HRESULT_CHECK(_device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &_backBufferColor));
			HRESULT_CHECK(_device->GetDepthStencilSurface(&_backBufferDepthStencil));

			for (uint32_t i = 0; i < ARRAY_COUNT(_indexBuffers); ++i)
			{
				_indexBuffers[i].PostReset();
			}

			for (uint32_t i = 0; i < ARRAY_COUNT(_vertexBuffers); ++i)
			{
				_vertexBuffers[i].PostReset();
			}

			//TODO : Implement this
			//for (uint32_t i = 0; i < ARRAY_COUNT(_renderTargets); ++i)
			//{
			//	_renderTargets[i].PostReset();
			//}
		}

		void SaveScreenShot(Memory* mem)
		{
			#if BX_PLATFORM_WINDOWS
			IDirect3DSurface9* surface;
			D3DDEVICE_CREATION_PARAMETERS dcp;
			HRESULT_CHECK(_device->GetCreationParameters(&dcp));

			D3DDISPLAYMODE dm;
			HRESULT_CHECK(m_d3d9->GetAdapterDisplayMode(dcp.AdapterOrdinal, &dm));

			HRESULT_CHECK(_device->CreateOffscreenPlainSurface(dm.Width
				, dm.Height
				, D3DFMT_A8R8G8B8
				, D3DPOOL_SCRATCH
				, &surface
				, NULL
			));

			HRESULT_CHECK(_device->GetFrontBufferData(0, surface));

			D3DLOCKED_RECT rect;
			HRESULT_CHECK(surface->LockRect(&rect
				, NULL
				, D3DLOCK_NO_DIRTY_UPDATE | D3DLOCK_NOSYSLOCK | D3DLOCK_READONLY
			));

			RECT rc;
			GetClientRect(bgfxHwnd, &rc);
			POINT point;
			point.x = rc.left;
			point.y = rc.top;
			ClientToScreen(bgfxHwnd, &point);
			uint8_t* data = (uint8_t*)rect.pBits;
			uint32_t bpp = rect.Pitch / dm.Width;
			saveTga((const char*)_mem->data, m_params.BackBufferWidth, m_params.BackBufferHeight, rect.Pitch, &data[point.y*rect.Pitch + point.x*bpp]);

			HRESULT_CHECK(surface->UnlockRect());
			DX_RELEASE(surface, 0);
			#endif // BX_PLATFORM_WINDOWS
		}

		#if BX_PLATFORM_WINDOWS
		D3DPERF_SetMarkerFunc m_D3DPERF_SetMarker;
		D3DPERF_BeginEventFunc  m_D3DPERF_BeginEvent;
		D3DPERF_EndEventFunc m_D3DPERF_EndEvent;
		#endif // BX_PLATFORM_WINDOWS

		IDirect3D9* _d3d9;
		IDirect3DDevice9* _device;

		IDirect3DSurface9* _backBufferColor;
		IDirect3DSurface9* _backBufferDepthStencil;

		D3DPRESENT_PARAMETERS _params;
		uint32_t _flags;

		bool _initialized;
		bool _fmtNULL;
		bool _fmtDF16;
		bool _fmtDF24;
		bool _fmtINTZ;
		bool _fmtRAWZ;

		D3DFORMAT _fmtDepth;

		IndexBuffer _indexBuffers[VIDEO_MAX_INDEX_BUFFERS];
		VertexBuffer _vertexBuffers[VIDEO_MAX_VERTEX_BUFFERS];
		Shader _vertexShaders[VIDEO_MAX_VERTEX_SHADERS];
		Shader _fragmentShaders[VIDEO_MAX_FRAGMENT_SHADERS];
		SurfaceMaterial _surfaceMaterials[VIDEO_MAX_SURFACE_MATERIALS];
		Texture _textures[VIDEO_MAX_TEXTURES];
		VertexDeclaration _vertexDecls[VIDEO_MAX_VERTEX_DECLS];
		RenderTarget _renderTargets[VIDEO_MAX_RENDER_TARGETS];
		UniformRegistry _uniformReg;
		void* _uniforms[VIDEO_MAX_UNIFORMS];

		TextVideoMem m_textVideoMem;
	};

	static RendererContext sRendererContext;

	void ConstantBuffer::Commit(bool force)
	{
		Reset();

		do
		{
			uint32 opcode = Read();

			if (ConstantType::eEnd == opcode)
			{
				break;
			}

			ConstantType::Enum type;
			uint16_t loc;
			uint16_t num;
			uint16_t copy;
			DecodeOpcode(opcode, type, loc, num, copy);

			const char* data;
			if (copy)
			{
				data = Read(gConstantTypeSize[type] * num);
			}
			else
			{
				memcpy(&data, Read(sizeof(void*)), sizeof(void*));
			}

			#define CASE_IMPLEMENT_UNIFORM(uniform, glsuffix, dxsuffix, type) \
				case ConstantType::uniform: \
				{ \
					type* value = (type*)data; \
					sRendererContext._device->SetVertexShaderConstant##dxsuffix(loc, value, num); \
				} break; \
				\
				case ConstantType::uniform | VIDEO_UNIFORM_FRAGMENTBIT: \
				{ \
					type* value = (type*)data; \
					sRendererContext._device->SetPixelShaderConstant##dxsuffix(loc, value, num); \
				} break;

			switch ((int32)type)
			{
				CASE_IMPLEMENT_UNIFORM(eUniform1i, 1iv, I, int);
				CASE_IMPLEMENT_UNIFORM(eUniform1f, 1fv, F, float);
				CASE_IMPLEMENT_UNIFORM(eUniform1iv, 1iv, I, int);
				CASE_IMPLEMENT_UNIFORM(eUniform1fv, 1fv, F, float);
				CASE_IMPLEMENT_UNIFORM(eUniform2fv, 2fv, F, float);
				CASE_IMPLEMENT_UNIFORM(eUniform3fv, 3fv, F, float);
				CASE_IMPLEMENT_UNIFORM(eUniform4fv, 4fv, F, float);
				CASE_IMPLEMENT_UNIFORM(eUniform3x3fv, Matrix3fv, F, float);
				CASE_IMPLEMENT_UNIFORM(eUniform4x4fv, Matrix4fv, F, float);

			case ConstantType::eEnd:
				break;

			default:
				//BX_TRACE("%4d: INVALID 0x%08x, t %d, l %d, n %d, c %d", m_pos, opcode, type, loc, num, copy);
				break;
			}

			#undef CASE_IMPLEMENT_UNIFORM

		} while (true);
	}

	void IndexBuffer::PreReset()
	{
		if (_dynamic)
		{
			COM_RELEASE(_ptr);
		}
	}

	void IndexBuffer::PostReset()
	{
		if (_dynamic)
		{
			HRESULT_CHECK(sRendererContext._device->CreateIndexBuffer(_size, 
				D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, D3DFMT_INDEX16, 
				D3DPOOL_DEFAULT, &_ptr ,nullptr));
		}
	}

	void IndexBuffer::Create(uint32 size, void * data)
	{
		_size = size;
		_dynamic = (nullptr == data);

		uint32 usage = D3DUSAGE_WRITEONLY;
		D3DPOOL pool = D3DPOOL_MANAGED;

		if (_dynamic)
		{
			usage |= D3DUSAGE_DYNAMIC;
			pool = D3DPOOL_DEFAULT;
		}

		HRESULT_CHECK(sRendererContext._device->CreateIndexBuffer(
			_size, usage, D3DFMT_INDEX16, pool, &_ptr, nullptr));
	}

	void VertexBuffer::PreReset()
	{
		if (_dynamic)
		{
			COM_RELEASE(_ptr);
		}
	}

	void VertexBuffer::PostReset()
	{
		if (_dynamic)
		{
			HRESULT_CHECK(sRendererContext._device->CreateVertexBuffer(_size,
				D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, 0,
				D3DPOOL_DEFAULT, &_ptr, nullptr));
		}
	}

	void VertexBuffer::Create(uint32 size, void * data, VertexDeclHandle declHandle)
	{
		_size = size;
		_decl = declHandle;
		_dynamic = (nullptr == data);

		uint32 usage = D3DUSAGE_WRITEONLY;
		D3DPOOL pool = D3DPOOL_MANAGED;

		if (_dynamic)
		{
			usage |= D3DUSAGE_DYNAMIC;
			pool = D3DPOOL_DEFAULT;
		}

		HRESULT_CHECK(sRendererContext._device->CreateVertexBuffer(
			_size, usage, 0, pool, &_ptr, nullptr));
	}

	//void RenderTarget::PreReset()
	//{
	//}

	//void RenderTarget::PostReset()
	//{
	//}

	void RenderTarget::Create(uint32 width, uint32 height, uint32 flags)
	{
		_width = _width;
		_height = _height;
		_flags = _flags;

		CreateTextures();
	}

	void RenderTarget::CreateTextures()
	{
		if (0 != _flags)
		{
			uint32 colorFormat = (_flags & VIDEO_RENDER_TARGET_COLOR_MASK) >> VIDEO_RENDER_TARGET_COLOR_SHIFT;
			uint32 depthFormat = (_flags & VIDEO_RENDER_TARGET_DEPTH_MASK) >> VIDEO_RENDER_TARGET_DEPTH_SHIFT;

			if (0 < colorFormat)
			{
				HRESULT_CHECK(sRendererContext._device->CreateTexture(_width, _height, 1, D3DUSAGE_RENDERTARGET,
					sColorFormat[colorFormat], D3DPOOL_DEFAULT, &_colorTexture, NULL));

				HRESULT_CHECK(_colorTexture->GetSurfaceLevel(0, &_color));
			}

			if (0 < depthFormat)
			{
				HRESULT_CHECK(sRendererContext._device->CreateTexture(_width, _height, 1, D3DUSAGE_DEPTHSTENCIL,
					sDepthFormat[depthFormat], D3DPOOL_DEFAULT, &_depthTexture, NULL));

				HRESULT_CHECK(_depthTexture->GetSurfaceLevel(0, &_depth));
			}
		}
	}

	void video::RenderTarget::DestroyTextures()
	{
		if (0 != _flags)
		{
			uint32 colorFormat = (_flags & VIDEO_RENDER_TARGET_COLOR_MASK) >> VIDEO_RENDER_TARGET_COLOR_SHIFT;
			uint32 depthFormat = (_flags & VIDEO_RENDER_TARGET_DEPTH_MASK) >> VIDEO_RENDER_TARGET_DEPTH_SHIFT;

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
		HRESULT_CHECK(sRendererContext._device->SetSamplerState(stage, D3DSAMP_MINFILTER, D3DTEXF_LINEAR));
		HRESULT_CHECK(sRendererContext._device->SetSamplerState(stage, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR));
		HRESULT_CHECK(sRendererContext._device->SetSamplerState(stage, D3DSAMP_MIPFILTER, D3DTEXF_POINT));
		HRESULT_CHECK(sRendererContext._device->SetSamplerState(stage, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP));
		HRESULT_CHECK(sRendererContext._device->SetSamplerState(stage, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP));
		HRESULT_CHECK(sRendererContext._device->SetTexture(stage, _colorTexture));
	}

static const D3DVERTEXELEMENT9 sAttrib[Attrib::Count+1] =
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,     0},
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,       0},
		{0, 0, D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,        0},
		{0, 0, D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,        1},
		{0, 0, D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0},
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT,  0},
		{0, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,     0},
		{0, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,     1},
		{0, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,     2},
		{0, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,     3},
		{0, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,     4},
		{0, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,     5},
		{0, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,     6},
		{0, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,     7},
		D3DDECL_END()
	};

	void VertexDeclaration::Create(const VertexDecl &decl)
	{
		memcpy(&_decl, &decl, sizeof(VertexDecl));
		//dump(_decl);

		D3DVERTEXELEMENT9 vertexElements[Attrib::Count + 1];
		D3DVERTEXELEMENT9* elem = vertexElements;

		for (uint32_t attr = 0; attr < Attrib::Count; ++attr)
		{
			if (0xff != _decl._attributes[attr])
			{
				uint8_t num;
				AttribType::Enum type;
				bool normalized;
				_decl.Decode(Attrib::Enum(attr), num, type, normalized);

				memcpy(elem, &sAttrib[attr], sizeof(D3DVERTEXELEMENT9));

				D3DDECLTYPE declType = D3DDECLTYPE(elem->Type);

				switch (type)
				{
				case AttribType::eUint8:
					if (normalized)
					{
						declType = D3DDECLTYPE_UBYTE4N;
					}
					else
					{
						declType = D3DDECLTYPE_UBYTE4;
					}
					break;

				case AttribType::eUint16:
					if (normalized)
					{
						switch (num)
						{
						default:
						case 2:
							declType = D3DDECLTYPE_SHORT2N;
							break;

						case 4:
							declType = D3DDECLTYPE_SHORT4N;
							break;
						}
					}
					else
					{
						switch (num)
						{
						default:
						case 2:
							declType = D3DDECLTYPE_SHORT2;
							break;

						case 4:
							declType = D3DDECLTYPE_SHORT4;
							break;
						}
					}
					break;

				case AttribType::eFloat:
					switch (num)
					{
					case 1:
						declType = D3DDECLTYPE_FLOAT1;
						break;

					case 2:
						declType = D3DDECLTYPE_FLOAT2;
						break;

					default:
					case 3:
						declType = D3DDECLTYPE_FLOAT3;
						break;

					case 4:
						declType = D3DDECLTYPE_FLOAT4;
						break;
					}

					break;

				default:
					Assert(false) //"Invalid attrib type."
					break;
				}

				elem->Type = declType;
				elem->Offset = _decl._offset[attr];
				++elem;

				//BX_TRACE("\tattr %d, num %d, type %d, norm %d, offset %d"
				//	, attr
				//	, num
				//	, type
				//	, normalized
				//	, _decl.m_offset[attr]
				//);
			}
		}

		memcpy(elem, &sAttrib[Attrib::Count], sizeof(D3DVERTEXELEMENT9));

		HRESULT_CHECK(sRendererContext._device->CreateVertexDeclaration(vertexElements, &_ptr));
	}

	void Shader::Create(bool fragment, const Memory * mem)
	{
		_constantBuffer = ConstantBuffer::Create(1024);

		StreamRead stream(mem->_data, mem->_size);
		uint16 count;
		stream.Read(count);

		_numPredefined = 0;

		//BX_TRACE("Shader consts %d", count);

		uint8 fragmentBit = fragment ? VIDEO_UNIFORM_FRAGMENTBIT : 0;

		for (uint32_t i = 0; i < count; ++i)
		{
			uint8_t nameSize;
			stream.Read(nameSize);

			char name[256];
			stream.Read(&name, nameSize);
			name[nameSize] = '\0';

			uint8_t type;
			stream.Read(type);

			uint8_t num;
			stream.Read(num);

			uint16_t regIndex;
			stream.Read(regIndex);

			uint16_t regCount;
			stream.Read(regCount);

			//BX_TRACE("\t%s, type %2d, num %2d, r.index %3d, r.count %2d", name, type, num, regIndex, regCount );

			const void* data = NULL;
			PredefinedUniform::Enum predefined = NameToPredefinedUniformEnum(name);
			if (PredefinedUniform::Count != predefined)
			{
				_predefined[_numPredefined]._loc = regIndex;
				_predefined[_numPredefined]._type = predefined | fragmentBit;
				_numPredefined++;
			}
			else
			{
				const UniformInfo* info = sRendererContext._uniformReg.Find(name);
				if (NULL != info)
				{
					data = info->_data;
					_constantBuffer->WriteUniformRef((ConstantType::Enum)(type | fragmentBit), regIndex, data, regCount);
					//BX_TRACE("store %s %p", name, data);
				}
			}
		}

		uint16 shaderSize;
		stream.Read(shaderSize);

		_constantBuffer->Finish();

		const DWORD* code = (const DWORD*)stream.GetDataPtr();

		if (fragment)
		{
			HRESULT_CHECK(sRendererContext._device->CreatePixelShader(code, (IDirect3DPixelShader9**)&_ptr));
		}
		else
		{
			HRESULT_CHECK(sRendererContext._device->CreateVertexShader(code, (IDirect3DVertexShader9**)&_ptr));
		}
	}

	DWORD * Shader::GetShaderCode(uint8_t fragmentBit, const Memory * mem)
	{
		return nullptr;
	}

	static bool sExit = false;

	DWORD WINAPI RenderThread(LPVOID arg)
	{
		while (!sExit)
		{
			RenderFrame();
		}
		sExit = false;
		return EXIT_SUCCESS;
	}

	void Context::RendererInit()
	{
		sRendererContext.Init();
	}

	void Context::RendererShutdown()
	{
		sRendererContext.Shutdown();
	}

	void Context::RendererCreateIndexBuffer(IndexBufferHandle handle, Memory* mem)
	{
		sRendererContext._indexBuffers[handle.index].Create(mem->_size, mem->_data);
	}
	
	void Context::RendererDestroyIndexBuffer(IndexBufferHandle handle)
	{
		sRendererContext._indexBuffers[handle.index].Destroy();
	}

	void Context::RendererCreateDynamicIndexBuffer(IndexBufferHandle handle, uint32 size)
	{
		sRendererContext._indexBuffers[handle.index].Create(size, nullptr);
	}

	void Context::RendererDestroyDynamicIndexBuffer(IndexBufferHandle handle)
	{
		sRendererContext._indexBuffers[handle.index].Destroy();
	}

	void Context::RendererCreateVertexBuffer(VertexBufferHandle handle, Memory* mem, VertexDeclHandle declHandle)
	{
		sRendererContext._vertexBuffers[handle.index].Create(mem->_size, mem->_data, declHandle);
	}

	void Context::RendererDestroyVertexBuffer(VertexBufferHandle handle)
	{
		sRendererContext._vertexBuffers[handle.index].Destroy();
	}

	void Context::RendererCreateDynamicVertexBuffer(VertexBufferHandle handle, uint32 size)
	{
		VertexDeclHandle declHandle{INVALID_HANDLE};
		sRendererContext._vertexBuffers[handle.index].Create(size, nullptr, declHandle);
	}

	void Context::RendererDestroyDynamicVertexBuffer(VertexBufferHandle handle)
	{
		sRendererContext._vertexBuffers[handle.index].Destroy();
	}

	void Context::RendererCreateVertexDecl(VertexDeclHandle handle, const VertexDecl& decl)
	{
		sRendererContext._vertexDecls[handle.index].Create(decl);
	}

	void Context::RendererDestroyVertexDecl(VertexDeclHandle handle)
	{
		sRendererContext._vertexDecls[handle.index].Destroy();
	}

	void Context::RendererCreateVertexShader(VertexShaderHandle handle, Memory* mem)
	{
		sRendererContext._vertexShaders[handle.index].Create(false, mem);
	}

	void Context::RendererDestroyVertexShader(VertexShaderHandle handle)
	{
		sRendererContext._vertexShaders[handle.index].Destroy();
	}

	void Context::RendererCreateFragmentShader(FragmentShaderHandle handle, Memory* mem)
	{
		sRendererContext._fragmentShaders[handle.index].Create(true, mem);
	}

	void Context::RendererDestroyFragmentShader(FragmentShaderHandle handle)
	{
		sRendererContext._fragmentShaders[handle.index].Destroy();
	}

	void Context::RendererCreateMaterial(SurfaceMaterialHandle handle,
		VertexShaderHandle vsh, FragmentShaderHandle fsh)
	{
		sRendererContext._surfaceMaterials[handle.index].Create(
			sRendererContext._vertexShaders[vsh.index],
			sRendererContext._fragmentShaders[fsh.index]);
	}

	void Context::RendererDestroyMaterial(FragmentShaderHandle handle)
	{
		sRendererContext._surfaceMaterials[handle.index].Destroy();
	}

	void Context::RendererCreateTexture(TextureHandle handle, Memory* mem, uint32 flags)
	{
		sRendererContext._textures[handle.index].Create(mem, flags);
	}

	void Context::RendererDestroyTexture(TextureHandle handle)
	{
		sRendererContext._textures[handle.index].Destroy();
	}

	void Context::RendererCreateRenderTarget(RenderTargetHandle handle, uint16 width, uint16 height, uint32 flags)
	{
		sRendererContext._renderTargets[handle.index].Create(width, height, flags);
	}

	void Context::RendererDestroyRenderTarget(RenderTargetHandle handle)
	{
		sRendererContext._renderTargets[handle.index].Destroy();
	}

	void Context::RendererCreateUniform(UniformHandle handle, ConstantType::Enum type, uint16 num, const char* name)
	{
		uint32 size = gConstantTypeSize[type] * num;
		void* data = gRealloc(NULL, size);
		sRendererContext._uniforms[handle.index] = data;
		sRendererContext._uniformReg.Reg(name, sRendererContext._uniforms[handle.index]);
	}

	void Context::RendererDestroyUniform(UniformHandle handle)
	{
		gFree(sRendererContext._uniforms[handle.index]);
	}

	void Context::RendererSaveScreenShot(Memory* mem)
	{
		sRendererContext.SaveScreenShot(mem);
	}

	void Context::RendererUpdateUniform(uint16 loc, const void* data, uint32 size)
	{
		memcpy(sRendererContext._uniforms[loc], data, size);
	}

	void Context::RendererSubmit()
	{
		//PIX_BEGINEVENT(D3DCOLOR_RGBA(0xff, 0x00, 0x00, 0xff), "rendererSubmit");

		sRendererContext.UpdateResolution(_render->_resolution);

		sRendererContext._device->BeginScene();

		if (0 < _render->_iboffset)
		{
			DynamicIndexBuffer* ib = _render->_dynamicIb;
			sRendererContext._indexBuffers[ib->_handle.index].Update(_render->_iboffset, ib->_data);
		}

		if (0 < _render->_vboffset)
		{
			DynamicVertexBuffer* vb = _render->_dynamicVb;
			sRendererContext._vertexBuffers[vb->_handle.index].Update(_render->_vboffset, vb->_data);
		}

		_render->Sort();

		RenderState currentState;
		currentState.Reset();
		currentState._flags = VIDEO_STATE_NONE;

		Matrix viewProjection[VIDEO_MAX_VIEWS];
		for (uint32_t i = 0; i < VIDEO_MAX_VIEWS; ++i)
		{
			MatrixMultiply(&viewProjection[i], &_render->_view[i], &_render->_projection[i]);
		}

		HRESULT_CHECK(sRendererContext._device->SetRenderState(
			D3DRS_FILLMODE, _render->_debug & VIDEO_DEBUG_WIREFRAME ? D3DFILL_WIREFRAME : D3DFILL_SOLID));

		uint16 materialIdx = video::invalidHandle;

		SortKey key;
		uint8_t view = 0xff;
		RenderTargetHandle rt;
		rt.index = INVALID_HANDLE;
		float alphaRef = 0.0f;
		D3DPRIMITIVETYPE primType = D3DPT_TRIANGLELIST;
		uint32_t primNumVerts = 3;

		uint32_t statsNumPrims = 0;
		uint32_t statsNumIndices = 0;

		//int64_t elapsed = -bx::getHPCounter();

		if (0 == (_render->_debug & VIDEO_DEBUG_IFH))
		{
			for (uint32_t item = 0, numItems = _render->_num; item < numItems; ++item)
			{
				key.Decode(_render->_sortKeys[item]);
				const RenderState& state = _render->_renderState[_render->_sortValues[item]];

				const uint64_t newFlags = state._flags;
				uint64_t changedFlags = currentState._flags ^ state._flags;
				currentState._flags = newFlags;

				if (key._view != view)
				{
					currentState.Clear();
					changedFlags = VIDEO_STATE_MASK;
					currentState._flags = newFlags;

					//PIX_ENDEVENT();
					//PIX_BEGINEVENT(D3DCOLOR_RGBA(0xff, 0x00, 0x00, 0xff), "view");

					view = key._view;
					materialIdx = video::invalidHandle;

					if (_render->_renderTarget[view].index != rt.index)
					{
						rt = _render->_renderTarget[view];
						sRendererContext.SetRenderTarget(rt);
					}

					Rect& rect = _render->_rect[view];

					D3DVIEWPORT9 vp;
					vp.X = rect._x;
					vp.Y = rect._y;
					vp.Width = rect._width;
					vp.Height = rect._height;
					vp.MinZ = 0.0f;
					vp.MaxZ = 1.0f;
					HRESULT_CHECK(sRendererContext._device->SetViewport(&vp));

					Clear& clear = _render->_clear[view];

					if (VIDEO_CLEAR_NONE != clear._flags)
					{
						D3DCOLOR color;
						DWORD flags = 0;

						if (VIDEO_CLEAR_COLOR_BIT & clear._flags)
						{
							flags |= D3DCLEAR_TARGET;
							uint32_t rgba = clear._rgba;
							color = D3DCOLOR_RGBA(rgba >> 24, (rgba >> 16) & 0xff, (rgba >> 8) & 0xff, rgba & 0xff);
							HRESULT_CHECK(sRendererContext._device->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA));
						}

						if (VIDEO_CLEAR_DEPTH_BIT & clear._flags)
						{
							flags |= D3DCLEAR_ZBUFFER;
							HRESULT_CHECK(sRendererContext._device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE));
						}

						if (VIDEO_CLEAR_STENCIL_BIT & clear._flags)
						{
							flags |= D3DCLEAR_STENCIL;
						}

						if (0 != flags)
						{
							RECT rc;
							rc.left = rect._x;
							rc.top = rect._y;
							rc.right = rect._x + rect._width;
							rc.bottom = rect._y + rect._height;
							HRESULT_CHECK(sRendererContext._device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE));
							HRESULT_CHECK(sRendererContext._device->SetScissorRect(&rc));
							HRESULT_CHECK(sRendererContext._device->Clear(0, NULL, flags, 
								color, clear._depth, clear._stencil));
							HRESULT_CHECK(sRendererContext._device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE));
						}
					}

					HRESULT_CHECK(sRendererContext._device->SetRenderState(D3DRS_ZENABLE, TRUE));
					HRESULT_CHECK(sRendererContext._device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS));
					HRESULT_CHECK(sRendererContext._device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE));
					HRESULT_CHECK(sRendererContext._device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE));
					HRESULT_CHECK(sRendererContext._device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER));
				}

				if ((VIDEO_STATE_CULL_MASK | VIDEO_STATE_DEPTH_WRITE | VIDEO_STATE_DEPTH_TEST_MASK |
					VIDEO_STATE_ALPHA_MASK | VIDEO_STATE_ALPHA_WRITE | VIDEO_STATE_RGB_WRITE |
					VIDEO_STATE_BLEND_MASK | VIDEO_STATE_ALPHA_REF_MASK | VIDEO_STATE_PT_MASK) & changedFlags)
				{
					if (VIDEO_STATE_CULL_MASK & changedFlags)
					{
						uint32_t cull = (newFlags & VIDEO_STATE_CULL_MASK) >> VIDEO_STATE_CULL_SHIFT;
						HRESULT_CHECK(sRendererContext._device->SetRenderState(D3DRS_CULLMODE, sCullMode[cull]));
					}

					if (VIDEO_STATE_DEPTH_WRITE & changedFlags)
					{
						HRESULT_CHECK(sRendererContext._device->SetRenderState(D3DRS_ZWRITEENABLE, 
							!!(VIDEO_STATE_DEPTH_WRITE & newFlags)));
					}

					if (VIDEO_STATE_DEPTH_TEST_MASK & changedFlags)
					{
						uint32_t func = (newFlags & VIDEO_STATE_DEPTH_TEST_MASK) >> VIDEO_STATE_DEPTH_TEST_SHIFT;
						HRESULT_CHECK(sRendererContext._device->SetRenderState(D3DRS_ZENABLE, 0 != func));

						if (0 != func)
						{
							HRESULT_CHECK(sRendererContext._device->SetRenderState(D3DRS_ZFUNC, sDepthFunc[func]));
						}
					}

					if ((VIDEO_STATE_ALPHA_TEST | VIDEO_STATE_ALPHA_REF_MASK) & changedFlags)
					{
						uint32_t ref = (newFlags&VIDEO_STATE_ALPHA_REF_MASK) >> VIDEO_STATE_ALPHA_REF_SHIFT;
						alphaRef = ref / 255.0f;
						HRESULT_CHECK(sRendererContext._device->SetRenderState(D3DRS_ALPHAREF, ref));
						HRESULT_CHECK(sRendererContext._device->SetRenderState(D3DRS_ALPHATESTENABLE, !!(VIDEO_STATE_ALPHA_TEST & newFlags)));
					}

					if ((VIDEO_STATE_ALPHA_WRITE | VIDEO_STATE_RGB_WRITE) & changedFlags)
					{
						uint32_t writeEnable = (newFlags&VIDEO_STATE_ALPHA_WRITE) ? D3DCOLORWRITEENABLE_ALPHA : 0;
						writeEnable |= (newFlags&VIDEO_STATE_RGB_WRITE) ? D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE : 0;
						HRESULT_CHECK(sRendererContext._device->SetRenderState(D3DRS_COLORWRITEENABLE, writeEnable));
					}

					if (VIDEO_STATE_BLEND_MASK & changedFlags)
					{
						bool alphaBlendEnabled = !!(VIDEO_STATE_BLEND_MASK & newFlags);
						HRESULT_CHECK(sRendererContext._device->SetRenderState(D3DRS_ALPHABLENDENABLE, alphaBlendEnabled));
						//						HRESULT_CHECK(sRendererContext._device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, alphaBlendEnabled) );

						if (alphaBlendEnabled)
						{
							uint32_t blend = (newFlags&VIDEO_STATE_BLEND_MASK) >> VIDEO_STATE_BLEND_SHIFT;
							uint32_t src = blend & 0xf;
							uint32_t dst = (blend >> 4) & 0xf;

							HRESULT_CHECK(sRendererContext._device->SetRenderState(D3DRS_SRCBLEND, sBlendFactor[src]));
							HRESULT_CHECK(sRendererContext._device->SetRenderState(D3DRS_DESTBLEND, sBlendFactor[dst]));
							//							HRESULT_CHECK(sRendererContext._device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_SRCALPHA) );
							//							HRESULT_CHECK(sRendererContext._device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_INVSRCALPHA) );
						}
					}

					uint8_t primIndex = uint8_t((newFlags & VIDEO_STATE_PT_MASK) >> VIDEO_STATE_PT_SHIFT);
					primType = sPrimType[primIndex];
					primNumVerts = sPrimNumVerts[primIndex];
				}

				bool materialChanged = false;
				bool constantsChanged = state._constBegin < state._constEnd;
				RendererUpdateUniforms(_render->_constantBuffer, state._constBegin, state._constEnd);

				if (key._material != materialIdx)
				{
					materialIdx = key._material;

					if (video::invalidHandle == materialIdx)
					{
						sRendererContext._device->SetVertexShader(NULL);
						sRendererContext._device->SetPixelShader(NULL);
					}
					else
					{
						SurfaceMaterial& material = sRendererContext._surfaceMaterials[materialIdx];
						sRendererContext._device->SetVertexShader((IDirect3DVertexShader9*)material._vsh->_ptr);
						sRendererContext._device->SetPixelShader((IDirect3DPixelShader9*)material._fsh->_ptr);
					}

					materialChanged = constantsChanged = true;
				}

				if (video::invalidHandle != materialIdx)
				{
					SurfaceMaterial &material = sRendererContext._surfaceMaterials[materialIdx];

					if (constantsChanged)
					{
						SurfaceMaterial &material = sRendererContext._surfaceMaterials[materialIdx];
						material._vsh->_constantBuffer->Commit(materialChanged);
						material._fsh->_constantBuffer->Commit(materialChanged);
					}

					for (uint32_t i = 0, num = material._numPredefined; i < num; ++i)
					{
						PredefinedUniform& predefined = material._predefined[i];
						uint8 flags = predefined._type & VIDEO_UNIFORM_FRAGMENTBIT;
						switch (predefined._type & (~VIDEO_UNIFORM_FRAGMENTBIT))
						{
						case PredefinedUniform::eViewRect:
						{
							float rect[4];
							rect[0] = _render->_rect[view]._x;
							rect[1] = _render->_rect[view]._y;
							rect[2] = _render->_rect[view]._width;
							rect[3] = _render->_rect[view]._height;

							sRendererContext.SetShaderConstantF(flags, predefined._loc, &rect[0], 1);
						}break;

						case PredefinedUniform::eViewTexel:
						{
							float rect[4];
							rect[0] = 1.0f / float(_render->_rect[view]._width);
							rect[1] = 1.0f / float(_render->_rect[view]._height);

							sRendererContext.SetShaderConstantF(flags, predefined._loc, &rect[0], 1);
						} break;

						case PredefinedUniform::eView:
						{
							sRendererContext.SetShaderConstantF(flags, predefined._loc, &_render->_view[view]._11, 4);
						} break;

						case PredefinedUniform::eViewProj:
						{
							sRendererContext.SetShaderConstantF(flags, predefined._loc, &viewProjection[view]._11, 4);
						} break;

						case PredefinedUniform::eModel :
						{
							const Matrix &model = _render->_matrixCache._cache[state._matrix];
							sRendererContext.SetShaderConstantF(flags, predefined._loc, &model._11, state._num * 4);
						} break;

						case PredefinedUniform::eModelViewProj :
						{
							Matrix modelViewProj;
							const Matrix& model = _render->_matrixCache._cache[state._matrix];
							MatrixMultiply(&modelViewProj, &model, &viewProjection[view]);
							sRendererContext.SetShaderConstantF(flags, predefined._loc, &modelViewProj._11, 4);
						} break;

						case PredefinedUniform::eModelViewProjX :
						{
						/*	const Matrix &model = _render->_matrixCache._cache[state._matrix];

							static const BX_ALIGN_STRUCT_16(float) s_bias[16] =
							{
								0.5f, 0.0f, 0.0f, 0.0f,
								0.0f, 0.5f, 0.0f, 0.0f,
								0.0f, 0.0f, 0.5f, 0.0f,
								0.5f, 0.5f, 0.5f, 1.0f,
							};

							uint8_t other = _render->m_other[view];
							Matrix viewProjBias;
							matrix_mul(viewProjBias.val, viewProj[other].val, s_bias);

							Matrix modelViewProj;
							matrix_mul(modelViewProj.val, model.val, viewProjBias.val);

							sRendererContext.SetShaderConstantF(flags, predefined._loc, modelViewProj.val, 4);*/
						} break;

						//TODO : Implement this
						case PredefinedUniform::eViewProjX:
						{
							//static const BX_ALIGN_STRUCT_16(float) s_bias[16] =
							//{
							//	0.5f, 0.0f, 0.0f, 0.0f,
							//	0.0f, 0.5f, 0.0f, 0.0f,
							//	0.0f, 0.0f, 0.5f, 0.0f,
							//	0.5f, 0.5f, 0.5f, 1.0f,
							//};

							////Matrix

							//uint8 other = _render->_other[view];
							//Matrix viewProjBias;
							////matrix_mul(viewProjBias.val, viewProj[other].val, s_bias);
							//MatrixMultiply(&viewProjBias, viewProjection)

							//sRendererContext.SetShaderConstantF(flags, predefined._loc, viewProjBias.val, 4);
						} break;

						case PredefinedUniform::eAlphaRef:
						{
							sRendererContext.SetShaderConstantF(flags, predefined._loc, &alphaRef, 1);
						} break;

						default:
						{
							Assert(false);//predefined %d not handled
						} break;
						}
					}
				}

				//				if (BGFX_STATE_TEX_MASK & changedFlags)
				{
					uint64_t flag = VIDEO_STATE_TEX0;
					for (uint32_t stage = 0; stage < VIDEO_STATE_TEX_COUNT; ++stage)
					{
						const Sampler& sampler = state._sampler[stage];
						Sampler& current = currentState._sampler[stage];
						if (current._index != sampler._index || 
							current._flags != sampler._flags || 
							materialChanged)
						{
							if (video::invalidHandle != sampler._index)
							{
								switch (sampler._flags & VIDEO_SAMPLER_TYPE_MASK)
								{
								case 0:
									sRendererContext._textures[sampler._index].Commit(stage);
									break;

								case 1:
									sRendererContext._renderTargets[sampler._index].Commit(stage);
									break;

								case 2:
									//									id = sRendererContext.m_renderTargets[sampler.m_idx].m_depth.m_id;
									break;
								}
							}
							else
							{
								HRESULT_CHECK(sRendererContext._device->SetTexture(stage, NULL));
							}
						}

						current = sampler;
						flag <<= 1;
					}
				}

				if (currentState._vertexBuffer.index != state._vertexBuffer.index || materialChanged)
				{
					currentState._vertexBuffer = state._vertexBuffer;

					uint16_t handle = state._vertexBuffer.index;
					if (video::invalidHandle != handle)
					{
						VertexBuffer& vb = sRendererContext._vertexBuffers[handle];

						uint16_t decl = (vb._decl.index == video::invalidHandle) ? 
							(state._vertexDecl.index) : (vb._decl.index);
						VertexDeclaration& vertexDecl = sRendererContext._vertexDecls[decl];
						HRESULT_CHECK(sRendererContext._device->SetStreamSource(0, vb._ptr, 0,
							vertexDecl._decl._stride));
						HRESULT_CHECK(sRendererContext._device->SetVertexDeclaration(vertexDecl._ptr));
					}
					else
					{
						HRESULT_CHECK(sRendererContext._device->SetStreamSource(0, NULL, 0, 0));
					}
				}

				if (currentState._indexBuffer.index != state._indexBuffer.index)
				{
					currentState._indexBuffer = state._indexBuffer;

					uint16_t handle = state._indexBuffer.index;
					if (video::invalidHandle != handle)
					{
						IndexBuffer& ib = sRendererContext._indexBuffers[handle];
						HRESULT_CHECK(sRendererContext._device->SetIndices(ib._ptr));
					}
					else
					{
						HRESULT_CHECK(sRendererContext._device->SetIndices(NULL));
					}
				}

				if (video::invalidHandle != state._indexBuffer.index)
				{
					uint32_t numVertices = state._numVertices;
					if (UINT32_C(0xffffffff) == numVertices)
					{
						VertexBuffer& vb = sRendererContext._vertexBuffers[currentState._vertexBuffer.index];
						uint16 decl = (vb._decl.index == video::invalidHandle) ?
							(state._vertexDecl.index) : (vb._decl.index);
						VertexDeclaration& vertexDecl = sRendererContext._vertexDecls[decl];
						numVertices = vb._size / vertexDecl._decl._stride;
					}

					uint32_t numIndices;
					uint32_t numPrims = 0;

					if (VIDEO_DRAW_WHOLE_INDEX_BUFFER == state._startIndex)
					{
						numIndices = sRendererContext._indexBuffers[state._indexBuffer.index]._size / 2;
						numPrims = numIndices / primNumVerts;

						HRESULT_CHECK(sRendererContext._device->DrawIndexedPrimitive(primType,
							state._startVertex, 0, numVertices, 0, numPrims ));
					}
					else if (primNumVerts <= state._numIndices)
					{
						numIndices = state._numIndices;
						numPrims = numIndices / primNumVerts;

						HRESULT_CHECK(sRendererContext._device->DrawIndexedPrimitive(primType, 
							state._startVertex, 0, numVertices, state._startIndex, numPrims));
					}

					statsNumPrims += numPrims;
					statsNumIndices += numIndices;
				}
			}
			//PIX_ENDEVENT();
		}

		//int64 now = bx::getHPCounter();
		//elapsed += now;

		//static int64 last = now;
		//int64 frameTime = now - last;
		//last = now;

		//if (_render->_debug & (VIDEO_DEBUG_IFH | VIDEO_DEBUG_STATS))
		//{
		//	PIX_BEGINEVENT(D3DCOLOR_RGBA(0x40, 0x40, 0x40, 0xff), "debug");

		//	TextVideoMem& tvm = sRendererContext.m_textVideoMem;

		//	static int64_t next = now;

		//	if (now >= next)
		//	{
		//		next = now + bx::getHPFrequency();
		//		double freq = double(bx::getHPFrequency());
		//		double toMs = 1000.0 / freq;

		//		tvm.clear();
		//		uint16_t pos = 10;
		//		tvm.printf(10, pos++, 0x8e, "      Frame: %3.4f [ms] / %3.2f", frameTime*toMs, freq / frameTime);
		//		tvm.printf(10, pos++, 0x8e, " Draw calls: %4d / %3.4f [ms]", _render->m_num, elapsed*toMs);
		//		tvm.printf(10, pos++, 0x8e, "      Prims: %7d", statsNumPrims);
		//		tvm.printf(10, pos++, 0x8e, "    Indices: %7d", statsNumIndices);
		//		tvm.printf(10, pos++, 0x8e, "   DVB size: %7d", _render->m_vboffset);
		//		tvm.printf(10, pos++, 0x8e, "   DIB size: %7d", _render->m_iboffset);

		//		uint8_t attr[2] = { 0x89, 0x8a };
		//		uint8_t attrIndex = _render->m_waitSubmit < _render->m_waitRender;

		//		tvm.printf(10, pos++, attr[attrIndex & 1], "Submit wait: %3.4f [ms]", _render->m_waitSubmit*toMs);
		//		tvm.printf(10, pos++, attr[(attrIndex + 1) & 1], "Render wait: %3.4f [ms]", _render->m_waitRender*toMs);
		//	}

		//	g_textVideoMemBlitter.blit(tvm);

		//	PIX_ENDEVENT();
		//}

		sRendererContext._device->EndScene();
	}

	void Texture::Create(const Memory *mem, uint32 flags)
	{
		_tau = sTextureAddress[(flags & VIDEO_TEXTURE_U_MASK) >> VIDEO_TEXTURE_U_SHIFT];
		_tav = sTextureAddress[(flags & VIDEO_TEXTURE_V_MASK) >> VIDEO_TEXTURE_V_SHIFT];
		_minFilter = sTextureFilter[(flags & VIDEO_TEXTURE_MIN_MASK) >> VIDEO_TEXTURE_MIN_SHIFT];
		_magFilter = sTextureFilter[(flags & VIDEO_TEXTURE_MAG_MASK) >> VIDEO_TEXTURE_MAG_SHIFT];
		_mipFilter = sTextureFilter[(flags & VIDEO_TEXTURE_MIP_MASK) >> VIDEO_TEXTURE_MIP_SHIFT];

		//Dds dds;

		//if (parseDds(dds, mem))
		//{
		//	D3DFORMAT typefmt[4] =
		//	{
		//		D3DFMT_X8R8G8B8,
		//		D3DFMT_DXT1,
		//		D3DFMT_DXT3,
		//		D3DFMT_DXT5,
		//	};

		//	D3DFORMAT fmt = typefmt[dds.m_type];

		//	bool decompress = false;

		//	if (decompress
		//		|| (0 == dds.m_type	&&  dds.m_hasAlpha))
		//	{
		//		fmt = D3DFMT_A8R8G8B8;
		//	}

		//	DX_CHECK(s_renderCtx.m_device->CreateTexture(dds.m_width
		//		, dds.m_height
		//		, dds.m_numMips
		//		, 0
		//		, fmt
		//		, D3DPOOL_MANAGED
		//		, &m_ptr
		//		, NULL
		//	));

		//	if (decompress
		//		|| 0 == dds.m_type)
		//	{
		//		uint32_t width = dds.m_width;
		//		uint32_t height = dds.m_height;

		//		for (uint32_t lod = 0, num = dds.m_numMips; lod < num; ++lod)
		//		{
		//			width = Uint32Max(1, width);
		//			height = Uint32Max(1, height);

		//			Mip mip;
		//			if (GetRawImageData(dds, lod, mem, mip))
		//			{
		//				D3DLOCKED_RECT rect;
		//				DX_CHECK(m_ptr->LockRect(lod, &rect, NULL, 0));
		//				uint8_t* bits = (uint8_t*)rect.pBits;

		//				if (width != mip.m_width
		//					|| height != mip.m_height)
		//				{
		//					uint8_t* temp = (uint8_t*)gRealloc(NULL, mip._width * mip._height * 4);
		//					mip.decode(temp);
		//					uint32_t srcpitch = mip.m_width * 4;
		//					uint32_t dstpitch = rect.Pitch;

		//					for (uint32_t yy = 0; yy < height; ++yy)
		//					{
		//						uint8_t* src = &temp[yy * srcpitch];
		//						uint8_t* dst = &bits[yy * dstpitch];

		//						for (uint32_t xx = 0; xx < width; ++xx)
		//						{
		//							memcpy(dst, src, 4);
		//							dst += 4;
		//							src += 4;
		//						}
		//					}

		//					gFree(temp);
		//				}
		//				else
		//				{
		//					mip.decode(bits);
		//				}

		//				DX_CHECK(m_ptr->UnlockRect(lod));
		//			}

		//			width >>= 1;
		//			height >>= 1;
		//		}
		//	}
		//	else
		//	{
		//		for (uint32_t ii = 0, num = dds.m_numMips; ii < num; ++ii)
		//		{
		//			Mip mip;
		//			if (GetRawImageData(dds, ii, mem, mip))
		//			{
		//				D3DLOCKED_RECT rect;
		//				DX_CHECK(m_ptr->LockRect(ii, &rect, NULL, 0));
		//				uint8_t* dst = (uint8_t*)rect.pBits;
		//				memcpy(dst, mip.m_data, mip.m_size);
		//				DX_CHECK(m_ptr->UnlockRect(ii));
		//			}
		//		}
		//	}
		//}
		//else
		//{
		//	StreamRead stream(mem->_data, mem->_size);

		//	uint32_t magic;
		//	stream.Read(magic);

		//	if (VIDEO_MAGIC == magic)
		//	{
		//		uint16_t width;
		//		stream.Read(width);

		//		uint16_t height;
		//		stream.Read(height);

		//		uint8_t bpp;
		//		stream.Read(bpp);

		//		uint8_t numMips;
		//		stream.Read(numMips);

		//		stream.Align(16);

		//		HRESULT_CHECK(sRendererContext._device->CreateTexture(width, height, numMips,
		//			0, (1 == bpp) ? D3DFMT_L8 : D3DFMT_A8R8G8B8, D3DPOOL_MANAGED , &_ptr , NULL ));

		//		for (uint8_t mip = 0; mip < numMips; ++mip)
		//		{
		//			width = Uint32Max(width, 1);
		//			height = Uint32Max(height, 1);

		//			D3DLOCKED_RECT rect;
		//			HRESULT_CHECK(_ptr->LockRect(mip, &rect, NULL, 0));
		//			uint8_t* dst = (uint8_t*)rect.pBits;
		//			stream.Read(dst, width*height*bpp);
		//			HRESULT_CHECK(_ptr->UnlockRect(mip));

		//			width >>= 1;
		//			height >>= 1;
		//		}
		//	}
		//	else
		//	{
		//		//
		//	}
		//}
	}

	void Texture::Commit(uint8 stage)
	{
		HRESULT_CHECK(sRendererContext._device->SetSamplerState(stage, D3DSAMP_MINFILTER, _minFilter));
		HRESULT_CHECK(sRendererContext._device->SetSamplerState(stage, D3DSAMP_MAGFILTER, _magFilter));
		HRESULT_CHECK(sRendererContext._device->SetSamplerState(stage, D3DSAMP_MIPFILTER, _mipFilter));
		HRESULT_CHECK(sRendererContext._device->SetSamplerState(stage, D3DSAMP_ADDRESSU, _tau));
		HRESULT_CHECK(sRendererContext._device->SetSamplerState(stage, D3DSAMP_ADDRESSV, _tav));
		HRESULT_CHECK(sRendererContext._device->SetTexture(stage, _ptr));
	}

}