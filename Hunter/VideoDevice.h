#ifndef VIDEO_SYSTEM_H
#define VIDEO_SYSTEM_H

#include "Video.h"

#include "imguiRenderer.h"
#include "DebugDraw.h"

//TODOs
 //1. DynamicVertexBuffer, DynamicIndexBuffer를 만들자... Update
 //2. Texture Update를 만들자 Lock, Unlock, UpdateContents
 //3. Texture CreateFromMemory를 만들자
 //4. PredefinedUniforms를 만들자


//defines 
#define VIDEO (gEngine->GetVideo())

constexpr uint16 INVALID_BUFFER = 0;

namespace video
{
	constexpr uint32 VERTEXBUFFER_MAX_NUM = 1024;
	constexpr uint32 INDEXBUFFER_MAX_NUM = 1024;
	constexpr uint32 TEXTURE_MAX_NUM = 256;
	constexpr uint32 EFFECT_MAX_NUM = 64;
	constexpr uint32 DECL_MAX_NUM = 32;


	struct IndexBuffer
	{
		IndexBuffer()
			: _ptr(NULL)
			, _dynamic(NULL)
			, _size(0)
		{
		}

		void Create(uint32 size, void* data);
		void Update(uint32 size, void* data)
		{
			void* buffer;
			HRESULT_CHECK(_ptr->Lock(0, _size, &buffer,
				_dynamic ? D3DLOCK_DISCARD/*|D3DLOCK_NOOVERWRITE*/ : 0 ));

			memcpy(buffer, data, _size);
			_ptr->Unlock();
		}

		void Destroy()
		{
			if (NULL != _ptr)
			{
				COM_RELEASE(_ptr);
			}
		}

		IDirect3DIndexBuffer9* _ptr;
		uint32 _size;
		bool32  _dynamic;
	};

	struct VertexBuffer
	{
		VertexBuffer()
			: _ptr(NULL)
			, _dynamic(NULL)
			, _size(0)
		{
		}

		void Create(uint32 size, void* data, VertexDeclHandle declHandle);
		void Update(uint32 size, void* data)
		{
			void* buffer;
			HRESULT_CHECK(_ptr->Lock(0, _size, &buffer,
				_dynamic ? D3DLOCK_DISCARD/*|D3DLOCK_NOOVERWRITE*/ : 0));

			memcpy(buffer, data, _size);
			_ptr->Unlock();
		}

		void Destroy()
		{
			COM_RELEASE(_ptr);
		}

		IDirect3DVertexBuffer9* _ptr;
		uint32 _size;
		VertexDeclHandle _decl;
		bool32 _dynamic;
	};

	struct Effect
	{
		Effect()
			:_ptr(nullptr)
			//_constantsBuffer(nullptr)
		{
		}
		void Create(const std::string &fileName)
		{
			DWORD shaderFlag = 0;
		#if defined (_DEBUG) || defined (DEBUG)
			shaderFlag |= D3DXSHADER_DEBUG;
		#endif
			HRESULT_CHECK(D3DXCreateEffectFromFile(gpDevice, fileName.c_str(), nullptr, nullptr, shaderFlag,
				nullptr, &_ptr, nullptr));
		}
		void Destroy()
		{
			COM_RELEASE(_ptr);
		}
		ID3DXEffect *_ptr;
		//UniformBuffer *_constantsBuffer;
	};

	struct Texture
	{
		Texture()
			: _ptr(NULL)
		{
		}

		void CreateFromFile(const std::string &fileName)
		{
			D3DXIMAGE_INFO imageInfo;

			HRESULT_CHECK(D3DXGetImageInfoFromFile(fileName.c_str(), &imageInfo));
			_width = imageInfo.Width;
			_height = imageInfo.Height;

			HRESULT_CHECK(D3DXCreateTextureFromFile(gpDevice, fileName.c_str(), &_ptr));
		}
		void Create(uint32 width, uint32 height)
		{
			HRESULT_CHECK(D3DXCreateTexture(gpDevice, width, height, 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &_ptr));
			_width = width;
			_height = height;
		}

		void Destroy(bool resize = false)
		{
			COM_RELEASE(_ptr);
		}

		IDirect3DTexture9* _ptr;

		uint32 _width;
		uint32 _height;
	};

	struct RenderTarget
	{
		RenderTarget()
			: _colorTexture(NULL)
			, _color(NULL)
			, _depthTexture(NULL)
			, _depth(NULL)
			, _width(0)
			, _height(0)
			, _flags(0)
		{
		}

		void Create(uint32 width, uint32 height, uint32 flags);
		void CreateTextures();
		void DestroyTextures();

		void Destroy()
		{
			DestroyTextures();
			_flags = 0;
		}

		void Commit(uint8 stage);

		IDirect3DTexture9* _colorTexture;
		IDirect3DSurface9* _color;
		IDirect3DTexture9* _depthTexture;
		IDirect3DSurface9* _depth;
		uint32 _width;
		uint32 _height;
		uint32 _flags;
	};


	class VideoDevice
	{
		VideoDevice();
		virtual ~VideoDevice();

		bool Init();
		void ShutDown();

		void UpdateResolution(const Resolution &resolution);
		void SetRenderTarget(RenderTargetHandle handle);
		void Flip();

		EventChannel GetChannel() { return _channel; }

		bool InitD3D(HWND windowHandle);
		bool InitDefaultRenderState();
		bool PerformRender();

		LPDIRECT3D9 _pD3D;
		LPDIRECT3DDEVICE9 _pDevice;

		D3DPRESENT_PARAMETERS _presentParams;

		IDirect3DSwapChain9 *_additionalSwapChain;

		EventChannel _channel;

		IDirect3DSurface9 *_backBufferColor;
		IDirect3DSurface9 *_backBufferDepthStencil;

		D3DPRESENT_PARAMETERS m_params;
		uint32 _flags;

		bool _initialized;
		bool _fmtNULL;
		bool _fmtDF16;
		bool _fmtDF24;
		bool _fmtINTZ;
		bool _fmtRAWZ;

		D3DFORMAT _fmtDepth;

		IndexBuffer _indexBuffers[VIDEO_MAX_INDEX_BUFFERS];
		VertexBuffer _vertexBuffers[VIDEO_MAX_VERTEX_BUFFERS];
 		Effect _effects[VIDEO_MAX_EFFECT];
 		SurfaceMaterial _materials[VIDEO_MAX_MATERIALS];
 		Texture _textures[VIDEO_MAX_TEXTURES];
		VertexDecl _vertexDecls[VIDEO_MAX_VERTEX_DECLS];
 		RenderTarget _renderTargets[VIDEO_MAX_RENDER_TARGETS];
		UniformRegistry _uniformReg;
		//void* m_uniforms[BGFX_CONFIG_MAX_UNIFORMS];

		//TextVideoMem m_textVideoMem;
	};

	static VideoDevice sVideoDevice;
}

#endif
