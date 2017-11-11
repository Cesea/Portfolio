#ifndef VIDEO_DEVICE_9_H
#define VIDEO_DEVICE_9_H

#include "VideoPlatform.h"
#include "Video.h"

#include "imguiRenderer.h"
#include "DebugDraw.h"

//defines 
namespace video
{
	struct IndexBuffer
	{
		IndexBuffer() 
			: _ptr(NULL), _dynamic(NULL), _size(0)
		{
		}

		void PreReset();
		void PostReset();

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
			: _ptr(NULL), _dynamic(NULL), _size(0)
		{
		}

		void PreReset();
		void PostReset();

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

	struct VertexDeclaration
	{
		void Create(const VertexDecl& decl);

		void Destroy()
		{
			COM_RELEASE(_ptr);
		}

		IDirect3DVertexDeclaration9* _ptr;
		VertexDecl _decl;
	};

	struct Shader
	{
		Shader()
			: _ptr(nullptr)
		{
		}

		void Create(bool fragment, const Memory* mem);
		DWORD* GetShaderCode(uint8_t fragmentBit, const Memory* mem);

		void Destroy()
		{
			ConstantBuffer::Destroy(_constantBuffer);
			_constantBuffer = nullptr;
			_numPredefined = 0;

			COM_RELEASE(_ptr);
		}

		IUnknown* _ptr;
		ConstantBuffer* _constantBuffer;
		PredefinedUniform _predefined[PredefinedUniform::Count];
		uint8_t _numPredefined;
	};

	struct SurfaceMaterial
	{
		void Create(const Shader& vsh, const Shader& fsh)
		{
			Assert(vsh._ptr); //Vertex Shader is Valid
			_vsh = &vsh;

			Assert(fsh._ptr); //Fragment Shader is Valid
			_fsh = &fsh;

			memcpy(&_predefined[0], vsh._predefined, vsh._numPredefined * sizeof(PredefinedUniform));
			memcpy(&_predefined[vsh._numPredefined], fsh._predefined, 
				fsh._numPredefined * sizeof(PredefinedUniform));
			_numPredefined = vsh._numPredefined + fsh._numPredefined;
		}

		void Destroy()
		{
			_numPredefined = 0;
			_vsh = NULL;
			_fsh = NULL;
		}

		const Shader* _vsh;
		const Shader* _fsh;

		PredefinedUniform _predefined[PredefinedUniform::Count * 2];
		uint8_t _numPredefined;
	};

	struct Texture
	{
		Texture()
			: _ptr(NULL)
		{
		}

		void Create(const Memory *mem, uint32 flag);

		void Destroy(bool resize = false)
		{
			COM_RELEASE(_ptr);
		}

		void Commit(uint8 stage);

		IDirect3DTexture9* _ptr;

		uint32 _width;
		uint32 _height;

		D3DTEXTUREFILTERTYPE _minFilter;
		D3DTEXTUREFILTERTYPE _magFilter;
		D3DTEXTUREFILTERTYPE _mipFilter;
		D3DTEXTUREADDRESS _tau;
		D3DTEXTUREADDRESS _tav;
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

		//void PreReset();
		//void PostReset();

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


	//class VideoDevice
	//{
	//	VideoDevice();
	//	virtual ~VideoDevice();
	//	bool Init();
	//	void ShutDown();
	//	void UpdateResolution(const Resolution &resolution);
	//	void SetRenderTarget(RenderTargetHandle handle);
	//	void Flip();
	//	EventChannel GetChannel() { return _channel; }
	//	bool InitD3D(HWND windowHandle);
	//	bool InitDefaultRenderState();
	//	bool PerformRender();
	//	LPDIRECT3D9 _pD3D;
	//	LPDIRECT3DDEVICE9 _pDevice;

	//	D3DPRESENT_PARAMETERS _presentParams;
	//	IDirect3DSwapChain9 *_additionalSwapChain;
	//	EventChannel _channel;
	//	IDirect3DSurface9 *_backBufferColor;
	//	IDirect3DSurface9 *_backBufferDepthStencil;
	//	D3DPRESENT_PARAMETERS m_params;
	//	uint32 _flags;
	//	bool _initialized;
	//	bool _fmtNULL;
	//	bool _fmtDF16;
	//	bool _fmtDF24;
	//	bool _fmtINTZ;
	//	bool _fmtRAWZ;
	//	D3DFORMAT _fmtDepth;
	//	IndexBuffer _indexBuffers[VIDEO_MAX_INDEX_BUFFERS];
	//	VertexBuffer _vertexBuffers[VIDEO_MAX_VERTEX_BUFFERS];
	//	SurfaceMaterial _materials[VIDEO_MAX_MATERIALS];
	//	Texture _textures[VIDEO_MAX_TEXTURES];
	//	VertexDecl _vertexDecls[VIDEO_MAX_VERTEX_DECLS];
	//	RenderTarget _renderTargets[VIDEO_MAX_RENDER_TARGETS];
	//	UniformRegistry _uniformReg;
	//	//void* m_uniforms[BGFX_CONFIG_MAX_UNIFORMS];

	//	//TextVideoMem m_textVideoMem;
	//};
}

#endif
