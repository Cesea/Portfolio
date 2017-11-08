#ifndef VIDEO_SYSTEM_H
#define VIDEO_SYSTEM_H

#include "imguiRenderer.h"
#include "DebugDraw.h"

#include "Command.h"
#include "CommandBucket.h"

//defines 
#define VIDEO (gEngine->GetVideo())

constexpr uint16 INVALID_BUFFER = 0;

namespace video
{
	constexpr uint32 VERTEXBUFFER_MAX_NUM = 1024;
	constexpr uint32 INDEXBUFFER_MAX_NUM = 1024;
	constexpr uint32 TEXTURE_MAX_NUM = 256;
	constexpr uint32 EFFECT_MAX_NUM = 64;
	constexpr uint32 EFFECT_MAX_NUM = 32;

	struct PredefinedUniforms
	{
		
		enum ParamType
		{
			eWorld = 0,
			eView,
			eViewProjection,
			eWorldViewProjection,
			eDirectionalLight0,
			eDirectionalLight1,
			eDirectionalLight22,
			ePointLight0,
			ePointLight1,
			ePointLight2,
			eSpotLight0,
			eSpotLight1,
			eSpotLight2,
			eMatrixPallete,
			eTexture0,
			eTexture1,
			eTexture2,
			eTexture3,
			eTexture4,
			eTexture5,
			eTexture6,
			eTime,
		};

		enum ValueType
		{
			eFloat,
			eFloat4,
			eInt,
			eInt4,
			eMatrix,
			eMatrices,
			eAny
		};
		struct Uniform
		{
			PredefinedUniforms::ParamType paramType;
			PredefinedUniforms::ValueType valueType;
			std::string name;
		};

		static std::vector<Uniform> _uniforms;
		static void Init();
	};

	struct VertexDecl
	{
		void Begin(int32 numElement)
		{
			_elements = new D3DVERTEXELEMENT9[numElement + 1];
			Assert(_elements);
		}
		void Add(D3DVERTEXELEMENT9 &element)
		{
			_elements[_count++] = element;
		}
		void End()
		{
			_elements[_count] = D3DDECL_END();
			HRESULT_CHECK(gpDevice->CreateVertexDeclaration(_elements, &_pDecl));
			Assert(_pDecl);
		}

		D3DVERTEXELEMENT9 *_elements{nullptr};
		IDirect3DVertexDeclaration9 *_pDecl{nullptr};
		int32 _count;
	};

	struct IndexBuffer
	{
		IndexBuffer()
			: _ptr(NULL)
			, _dynamic(NULL)
			, _size(0)
			, _flags(INVALID_BUFFER)
		{
		}

		void Create(uint32 size, void* data, uint16 flags);
		//void Update(uint32 offset, uint32 size, void* data, bool discard = false)
		//{
		//	if (nullptr != data &&  !_dynamic)
		//	{
		//		bx::memCopy(&_dynamic[offset], data, size);
		//	}
		//	void* buffer;
		//	HRESULT_CHECK(_ptr->Lock(offset, size, &buffer,
		//		discard || (_dynamic && 0 == offset && size == _size) ? D3DLOCK_DISCARD : 0));
		//	bx::memCopy(buffer, data, size);
		//	HRESULT_CHECK(_ptr->Unlock());
		//}

		void Destroy()
		{
			if (NULL != _ptr)
			{
				COM_RELEASE(_ptr);

				if (NULL != _dynamic)
				{
					free(_dynamic);
					_dynamic = NULL;
				}
			}
		}

		IDirect3DIndexBuffer9* _ptr;
		uint8* _dynamic;
		uint32 _size;
		uint16 _flags;
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
		//void Update(uint32 offset, uint32 size, void* data, bool discard = false)
		//{
		//	if (NULL != _dynamic
		//		&&  data != _dynamic)
		//	{
		//		bx::memCopy(&m_dynamic[offset], data, size);
		//	}
		//	void* buffer;
		//	HRESULT_CHECK(_ptr->Lock(offset , size
		//		, &buffer
		//		, discard || (_dynamic && 0 == offset && _size == size) ? D3DLOCK_DISCARD : 0
		//	));
		//	bx::memCopy(buffer, data, size);
		//	HRESULT_CHECK(_ptr->Unlock());
		//}

		void Destroy()
		{
			if (NULL != _ptr)
			{
				COM_RELEASE(_ptr);

				if (NULL != _dynamic)
				{
					free(_dynamic);
					_dynamic = NULL;
				}
			}
		}

		IDirect3DVertexBuffer9* _ptr;
		uint8* _dynamic;
		uint32 _size;
		VertexDeclHandle _decl;
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
			if (_ptr != nullptr)
			{
				COM_RELEASE(_ptr);
			}
		}
		ID3DXEffect *_ptr;
		//UniformBuffer *_constantsBuffer;
	};

	struct Texture
	{
		Texture()
			: _ptr(NULL)
			, _surface(NULL)
			, _textureFormat(D3DFMT_UNKNOWN)
		{
		}

		void CreateFromFile(const std::string &fileName)
		{
			D3DXIMAGE_INFO imageInfo;

			HRESULT_CHECK(D3DXGetImageInfoFromFile(fileName.c_str(), &imageInfo));
			_width = imageInfo.Width;
			_height = imageInfo.Height;
			_textureFormat = imageInfo.Format;

			HRESULT_CHECK(D3DXCreateTextureFromFile(gpDevice, fileName.c_str(), &_ptr));
		}
		void Create(uint32 width, uint32 height)
		{
			HRESULT_CHECK(D3DXCreateTexture(gpDevice, width, height, 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &_ptr));
			_width = width;
			_height = height;
			_textureFormat = D3DFMT_A8R8G8B8;
		}
		IDirect3DSurface9* GetSurface(uint8 side = 0, uint8 mip = 0) const
		{
			return _surface;
		}

		void Destroy(bool resize = false)
		{
			COM_RELEASE(_ptr);
			COM_RELEASE(_surface);
		}

		IDirect3DTexture9* _ptr;
		IDirect3DSurface9* _surface;

		uint32 _width;
		uint32 _height;
		D3DFORMAT _textureFormat;
	};


	struct RenderWindow
	{
		int32 width;
		int32 height;

		D3DFORMAT backBufferFormat;
		D3DFORMAT depthStencilFormat;
		HWND windowHandle;
	};


	class VideoDevice
	{
	public:
		VideoDevice();
		virtual ~VideoDevice();

		bool Init();
		void ShutDown();

		void Update(float deltaTime);
		void Render();

		LPDIRECT3DDEVICE9 GetDevice()
		{
			return _pDevice;
		}
		EventChannel GetChannel() { return _channel; }

		bool32 Draw(VertexBufferHandle vertexHandle,
			uint32 startVertex, uint32 primitiveCount, uint32 matrixIndex);
		bool32 DrawIndexed(VertexBufferHandle vertexHandle, IndexBufferHandle indexHandle,
			uint32 numVertex, uint32 startIndex, uint32 primitiveCount, uint32 matrixIndex);

		CommandBucket<uint32> &GetCommandBucket() { return _commandBucket; }

		void CreateIndexBuffer(IndexBufferHandle handle, DataPackage* mem, uint16 flags)
		{
			_indexBuffers[handle.index].Create(mem->Size(), mem->Data(), flags);
		}

		void DestroyIndexBuffer(IndexBufferHandle handle)
		{
			_indexBuffers[handle.index].Destroy();
		}

		void CreateVertexDecl(VertexDeclHandle handle, const VertexDecl& decl)
		{
			VertexDecl& refDecl = _vertexDecls[handle.index];
			memcpy(&decl, )
			bx::memCopy(&refDecl, &decl, sizeof(VertexDecl));
		}

		void DestroyVertexDecl(VertexDeclHandle /*handle*/)
		{
		}

		void CreateVertexBuffer(VertexBufferHandle handle, DataPackage* mem,
			VertexDeclHandle declHandle, uint16 /*_flags*/)
		{
			_vertexBuffers[handle.index].Create(mem->Size(), mem->Data(), declHandle);
		}

		void DestroyVertexBuffer(VertexBufferHandle handle)
		{
			_vertexBuffers[handle.index].Destroy();
		}

		void CreateDynamicIndexBuffer(IndexBufferHandle handle, uint32_t size, uint16 flags)
		{
			_indexBuffers[handle.index].Create(size, NULL, flags);
		}

		//void UpdateDynamicIndexBuffer(IndexBufferHandle handle, uint32_t _offset,
		//	uint32_t _size, DataPackage* _mem)
		//{
		//	_indexBuffers[handle.index].Update(_offset, bx::uint32_min(_size, _mem->size), _mem->data);
		//}

		void DestroyDynamicIndexBuffer(IndexBufferHandle handle)
		{
			_indexBuffers[handle.index].Destroy();
		}

		void CreateDynamicVertexBuffer(VertexBufferHandle handle, uint32_t _size, uint16 /*_flags*/)
		{
			VertexDeclHandle decl = INVALID_HANDLE;
			_vertexBuffers[handle.index].Create(_size, NULL, decl);
		}

		//void UpdateDynamicVertexBuffer(VertexBufferHandle handle, uint32_t _offset,
		//	uint32_t _size, DataPackage* _mem)
		//{
		//	_vertexBuffers[handle.index].Update(_offset, bx::uint32_min(_size, _mem->size), _mem->data);
		//}

		void DestroyDynamicVertexBuffer(VertexBufferHandle handle)
		{
			_vertexBuffers[handle.index].Destroy();
		}

		void CreateEffect(EffectHandle handle, const std::string &fileName)
		{
			_effects[handle.index].Create(fileName);
		}

		void DestroyEffect(EffectHandle handle)
		{
			_effects[handle.index].Destroy();
		}

		//void CreateTexture(TextureHandle handle, DataPackage* mem, uint32_t flags, uint8_t skip)
		//{
		//	_textures[handle.index].Create(mem, flags, skip);
		//}

		void CreateTextureFromFile(TextureHandle handle, const std::string &fileName)
		{
			_textures[handle.index].CreateFromFile(fileName);
		}

	private:
		bool InitD3D(HWND windowHandle);

		bool InitDefaultRenderState();

		bool PerformRender();

		LPDIRECT3D9 _pD3D;
		LPDIRECT3DDEVICE9 _pDevice;

		D3DPRESENT_PARAMETERS _presentParams;

		IDirect3DSwapChain9 *_additionalSwapChain;

		RenderWindow _renderWindow;

		EventChannel _channel;

		CommandBucket<uint32> _commandBucket;

		VertexBuffer _vertexBuffers[VERTEXBUFFER_MAX_NUM];
		IndexBuffer _indexBuffers[INDEXBUFFER_MAX_NUM];
		Texture _textures[TEXTURE_MAX_NUM];
		Effect _effects[EFFECT_MAX_NUM];
		VertexDecl _vertexDecls[DECL_MAX_NUM];
	};
}

#endif
