#ifndef VIDEO_RESOURCE_H
#define VIDEO_RESOURCE_H

#include "VideoDefines.h"
#include "VideoResourceHandles.h"

#include <map>
#include <string>

//TODO : Dynamic buffer 만들어라

struct Memory
{
	void *_data;
	uint32 _size;
};

namespace video
{

	struct CommandBuffer
	{
		CommandBuffer() : _pos(0), _size(VIDEO_CONFIG_MAX_COMMAND_BUFFER_SIZE)
		{
			Finish();
		}

		enum Enum
		{
			eRendererInit,
			eSetTransform,
			eSetEffect,
			eSetVertexBuffer,
			eSetIndexBuffer,
			eSetMaterial,
			eDraw,
			eEnd
		};

		void Write(const void* data, uint32 size)
		{
			Assert(_pos < _size);
			memcpy(&_buffer[_pos], data, size);
			_pos += size;
		}

		template<typename Type>
		void Write(const Type &in)
		{
			Write(reinterpret_cast<const uint8 *>(&in), sizeof(Type));
		}

		void Read(void* data, uint32_t size)
		{
			Assert(_pos < _size);
			memcpy(data, &_buffer[_pos], size);
			_pos += size;
		}

		template<typename Type>
		void Read(Type &in)
		{
			Read(reinterpret_cast<uint8 *>(&in), sizeof(Type));
		}

		void Reset()
		{
			_pos = 0;
		}

		void Finish()
		{
			uint8 cmd = eEnd;
			Write(cmd);
			_pos = 0;
		}

		uint32 _pos;
		uint32 _size;
		uint8 _buffer[VIDEO_CONFIG_MAX_COMMAND_BUFFER_SIZE];

	private:
		CommandBuffer(const CommandBuffer&)
		{
		}
		void operator=(const CommandBuffer&)
		{
		}
	};


	template <typename HandleType>
	class ResourceHandlePool
	{
		//Handle의 Index값이 map에 들어간다..
		typedef std::map<uint16, std::string> NameTable;
	public:
		explicit ResourceHandlePool(uint32 poolSize);

		ResourceHandlePool(const ResourceHandlePool&) = delete;
		ResourceHandlePool(ResourceHandlePool&&) = delete;
		ResourceHandlePool& operator=(const ResourceHandlePool&) = delete;
		ResourceHandlePool& operator=(ResourceHandlePool&&) = delete;

		HandleType Create(const std::string &name = "");
		void Remove(HandleType handle);

		void SetName(HandleType id, const std::string &name);

		HandleType Get(uint32 index) const;
		const std::string &GetName(HandleType id) const;

		bool32 IsValid(HandleType id) const;
		uint32 GetSize() const;
		void Resize(uint32 amount);
		void Clear();

		const HandleType BuildHandle(uint16 index, uint16 count) const;

	private:

		uint32 _defaultPoolSize;

		uint32 _nextID;

		std::vector<HandleType> _freeList;
		std::vector<uint32> _counts;
		NameTable _nameTable;
	};

	//Vertex Buffer
	struct VertexBuffer
	{
		bool Create(uint32 size, void *data, VertexDeclHandle decl);
		void Update(uint32 size, void *data);
		void Destroy();

		void PreReset();
		void PostReset();

		IDirect3DVertexBuffer9 *_ptr;
		uint32 _size;
		bool32 _dynamic;

		VertexDeclHandle _decl;
	};

	//Index Buffer
	struct IndexBuffer
	{
		bool Create(uint32 size, void *data);
		void Update(uint32 size, void *data);
		void Destroy();

		void PreReset();
		void PostReset();

		IDirect3DIndexBuffer9 *_ptr;
		uint32 _size;
		bool32 _dynamic;
	};

	inline D3DVERTEXELEMENT9 VertexElement(WORD stream, WORD offset, BYTE type, BYTE method, BYTE usage, BYTE usageIndex)
	{
		D3DVERTEXELEMENT9 result;
		result.Stream = stream;
		result.Offset = offset;
		result.Type = type;
		result.Method = method;
		result.Usage = usage;
		result.UsageIndex = usageIndex;
		return result;
	}

	//Vertex Decl
	struct VertexDecl
	{
		void Begin();
		void Add(D3DVERTEXELEMENT9 &element);
		void End(uint32 stride);

		void Destroy();

		D3DVERTEXELEMENT9 _elements[VIDEO_CONFIG_MAX_NUM_VERTEX_ELEMENTS];
		IDirect3DVertexDeclaration9 *_ptr{};
		uint32 _count{};
		uint32 _stride;
	};


	//Texture
	struct Texture
	{
		bool Create(const std::string fileName);
		void Destroy();

		IDirect3DTexture9 *_ptr{};
		D3DFORMAT _format{};
		uint32 _width{};
		uint32 _height{};
	};

	struct Material
	{
		D3DMATERIAL9 _d3dMateirla{};
		TextureHandle _textureHandles[VIDEO_CONFIG_MATERIAL_TEXTURE_MAX_NUM];
	};

	struct PredefinedUniform
	{
		enum Enum
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
			Count,
		};
		static LPCSTR ParamName[Enum::Count];
	};


	//Effect
	struct Effect
	{
		bool Create(const std::string fileName);
		void Destroy();

		uint32 BeginEffect();

		void BeginPass(uint32 passNum);
		void EndPass();

		void EndEffect();

		void SetUniform(PredefinedUniform::Enum uniform, void *data);

		void SetMatrix(LPCSTR name, Matrix &matrix);
		void SetTexture(LPCSTR name, Texture &texture);
		void SetMaterial(Material &material);

		void SetValue(LPCSTR name, void *value, size_t size);


		ID3DXEffect *_ptr{};
	};

	

	//FrameBuffer
	struct FrameBuffer
	{
		FrameBuffer()
			: _num(0), _needResolve(0)
		{
			_depthHandle.index = INVALID_INDEX;
		}

		bool Create(uint8 num, const TextureHandle *handles);
		void Destroy();
		void Resolve() const;
		void PreReset();
		void PostReset();
		void CreateNullColorRT();

		IDirect3DSurface9* _color;
		IDirect3DSurface9* _depthStencil;
		TextureHandle _colorHandle;
		TextureHandle _depthHandle;
		uint8_t _num;
		bool _needResolve;
	};

	//이거 만들어야 함...
	struct MatrixCache
	{
		MatrixCache() 
			: _num(0)
		{
		}

		void Reset()
		{
			_num = 0;
		}

		uint32 Add(const Matrix *matrix)
		{
			if (nullptr != matrix)
			{
				//Assert(_num + _num < VIDEO_CONFIG_MAX_MATRIX_CACHE); /*"Matrix cache overflow. %d (max: %d)", m_num + _num, BGFX_CONFIG_MAX_MATRIX_CACHE);*/
				//uint32 num = Uint32Min(VIDEO_CONFIG_MAX_MATRIX_CACHE - _num, num);
				uint32 first = _num;
				memcpy(&_cache[_num], matrix, sizeof(Matrix));
				_num += 1;
				return first;
			}
			return 0;
		}

		Matrix _cache[VIDEO_CONFIG_MAX_MATRIX_CACHE];
		uint32 _num;
	};


	//NOTE : 만약 deffered rendering을 하게된다면  RenderView의 FrameBuffer에 이미지들을 렌더 한 후에 여러 RenderView의 
	// FrameBuffer를 합성하여 그린다??
	//Context와 같은 역할

	struct RenderState
	{
		VertexBufferHandle _vertexBuffer{};
		VertexDeclHandle _vertexDecl{};
		IndexBufferHandle _indexBuffer{};
		MaterialHandle _material{};
		EffectHandle _effect{};
	};

	struct RenderView
	{
		bool Create();

		void Begin();
		void End();

		void SetViewProjection(const Matrix &viewMatrix, const Matrix &projectionMatrix);
		void SetTransform(const Matrix &matrix);

		void SetEffect(EffectHandle handle);

		void Submit(VertexBufferHandle handle);
		void Submit(IndexBufferHandle iHandle);

		void Draw();

		void PreRender();
		void PostRender();

		void Destroy();

		Matrix _viewMatrix;
		Matrix _projectionMatrix;

		uint32 _clearColor{};

		MatrixCache _matrixCache;
		CommandBuffer _commandBuffer;
	};

	//ResourceHandlePool
	template<typename HandleType>
	inline ResourceHandlePool<HandleType>::ResourceHandlePool(uint32 poolSize)
		:_defaultPoolSize(poolSize), _counts(poolSize), _nextID(1)
	{
	}

	template<typename HandleType>
	inline HandleType ResourceHandlePool<HandleType>::Create(const std::string &name)
	{
		HandleType result;
		if (!_freeList.empty())
		{
			result = _freeList.back();
			_freeList.pop_back();
		}
		else
		{
			result.index = _nextID++;
			_counts[result.index] = 1;
		}

		//이름을 지정 해 주었다면 이름 추가한다
		if (name.length() > 0)
		{
			auto &iter = _nameTable.find(result.index);
			if (iter != _nameTable.end())
			{
				_nameTable[result.index] = name;
			}
		}

		return result;
	}

	template<typename HandleType>
	inline void ResourceHandlePool<HandleType>::Remove(HandleType handle)
	{
		auto &counter = _counts[handle.index];
		++counter; // increment the counter in the cache
		_freeList.push_back(BuildHandle(handle.index, handle.count)); // add the ID to the freeList

		auto found = _nameTable.find(handle.index);
		if (found != _nameTable.end())
		{
			_nameTable.erase(found);
		}
	}

	template<typename HandleType>
	inline void ResourceHandlePool<HandleType>::SetName(HandleType handle, const std::string & name)
	{
		auto find = _nameTable.find(handle.index);
		if (find != _nameTable.end())
		{
			_nameTable[handle.index] = name;
		}
	}

	template<typename HandleType>
	inline HandleType ResourceHandlePool<HandleType>::Get(uint32 index) const
	{
		if (index < _counts.size())
		{
			return BuildHandle(index);
		}
		else
		{
			return BuildHandle(index);
		}
	}

	template<typename HandleType>
	inline const std::string & ResourceHandlePool<HandleType>::GetName(HandleType handle) const
	{
		auto &find = _nameTable.find(handle.index);
		if (find != _nameTable.end())
		{
			return find.second;
		}
		else
		{
			return std::string();
		}
	}

	template<typename HandleType>
	inline bool32 ResourceHandlePool<HandleType>::IsValid(HandleType id) const
	{
		if (id.index > _counts.size())
		{
			return false;
		}
		else
		{
			return (id.counter == _counts[id.index]) && (id.counter > 0);
		}
	}

	template<typename HandleType>
	inline uint32 ResourceHandlePool<HandleType>::GetSize() const
	{
		return _counts.size();
	}

	template<typename HandleType>
	inline void ResourceHandlePool<HandleType>::Resize(uint32 amount)
	{
		_counts.resize(amount);
	}

	template<typename HandleType>
	inline void ResourceHandlePool<HandleType>::Clear()
	{
		_counts.clear();
		_freeList.clear();
		_nextID = 0;
	}

	template<typename HandleType>
	inline const HandleType ResourceHandlePool<HandleType>::BuildHandle(uint16 index, uint16 count) const
	{
		HandleType result(index, count);
		return result;
	}

}
#endif