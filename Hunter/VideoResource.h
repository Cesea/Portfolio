#ifndef VIDEO_RESOURCE_H
#define VIDEO_RESOURCE_H

#include "VideoDefines.h"
#include "VideoResourceHandles.h"

#include "VideoVertexFormat.h"

#include <map>
#include <string>

//TODO : Dynamic buffer 만들어라

namespace animation
{
	struct AnimationComponent;
	struct AnimationComponentHandle;
}

class Camera;


struct Memory
{
	void *_data;
	uint32 _size;
};

namespace video
{
	struct StaticXMesh;
	struct SkinnedXMesh;
	struct AnimationInstance;

	struct CommandBuffer
	{
		CommandBuffer() : _pos(0), _size(VIDEO_CONFIG_MAX_COMMAND_BUFFER_SIZE)
		{
			Finish();
		}

		//Change는 Device의 상태를 변경시키는 것이다
		//ChangePrimitive는 TRIANGLELIST와 LineStrip4
		enum Enum
		{
			eRendererInit,
			eSetTransform,
			eSetEffect,
			eSetRenderGroup,
			eSetVertexBuffer,
			eSetIndexBuffer,
			eSetMaterial,
			eChangeFillMode,
			eChangePrimitiveType,
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
		bool Create(uint32 size, void *data, uint32 stride);
		void Update(uint32 size, void *data);
		void Destroy();

		void PreReset();
		void PostReset();

		IDirect3DIndexBuffer9 *_ptr;
		uint32 _size;
		uint32 _stride;
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
		uint32 _stride{};
		DWORD _fvf{};
	};


	//Texture
	struct Texture
	{
		bool Create(const std::string &fileName);
		bool Create(int32 width, int32 height, D3DFORMAT fmt, D3DPOOL pool);
		void Destroy();

		bool Save(const std::string &fileName);

		IDirect3DTexture9 *_ptr{};
		D3DFORMAT _format{};
		uint32 _width{};
		uint32 _height{};
	};

	//머테리얼은 단순히 여러 텍스쳐의 집합이다.
	struct Material
	{
		bool Create();
		void Destroy();

		TextureHandle _textureHandles[VIDEO_CONFIG_MATERIAL_TEXTURE_MAX_NUM];

		void AddTexture(uint32 textureSlot, TextureHandle handle);
	};

	//디바이스에 실질적으로 저장되는 것은 model이 아니라 이거다
	//struct RenderGroup
	//{
	//	//머테리얼의 범위...
	//	struct MaterialRange
	//	{
	//		MaterialHandle _material;
	//		uint32 _startIndex;
	//		uint32 _numPrim;
	//		uint32 _startVertex;
	//		uint32 _numVertices;
	//		MaterialRange &operator== (const MaterialRange &other)
	//		{
	//			_material = other._material;
	//			_startIndex = other._startIndex;
	//			_numPrim = other._numPrim;
	//			_startVertex = other._startVertex;
	//			_numVertices = other._numVertices;
	//		}
	//	};
	//	bool Create(video::VertexBufferHandle vHandle, video::IndexBufferHandle iHandle, const RenderGroup::MaterialRange &materialRange);
	//	void Destroy();
	//	video::VertexBufferHandle _vertexBuffer;
	//	video::IndexBufferHandle _indexBuffer;
	//	//Sphere _sphere;
	//	//AABB _aabb;
	//	MaterialRange _materialRange;
	//};

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

		uint32 BeginEffect() const;

		void BeginPass(uint32 passNum) const;
		void EndPass() const;

		void EndEffect() const;

		void SetTechnique(LPCSTR name) const;

		void SetUniform(PredefinedUniform::Enum uniform, void *data);

		void SetMatrix(LPCSTR name, const Matrix &matrix) const;
		void SetMatrices(LPCSTR name, const Matrix *matrix, uint32 numMatrices) const;
		void SetTexture(const std::string &name, const Texture &texture) const;
		void SetMaterial(const Material &material) const;
		void SetInt(LPCSTR name, const int32 value) const;
		void SetFloat(LPCSTR name, const float value) const;
		void SetVector(LPCSTR name, const Vector4 &value) const;

		void SetValue(LPCSTR name, void *value, size_t size);
		void CommitChanges() const;

		//void DrawPrimitive(const video::VertexBuffer &vBuffer, const video::Material &mHandle) const;
		//void DrawPrimitiveIndex(const video::VertexBuffer &vBuffer, const video::IndexBuffer &iBuffer, const video::Material &mHandle) const;
		//void DrawStaticMesh(const StaticXMesh &mesh, LPCSTR technique = nullptr) const;
		//void DrawSkinnedMesh(const SkinnedXMesh &mesh, animation::AnimationComponent &animation, LPCSTR technique = nullptr) const;

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

	//TODO : Matrix Cache에서 여러개의 행렬을 넣을 수 있게끔 지원하라
	struct MatrixCache
	{
		//NOTE : end가 -1이면 하나만 쓰는거...
		struct CacheRange
		{
			int32 _start{-1};
			int32 _end{-1};
		};

		MatrixCache() 
			: _num(0)
		{
		}

		void Reset()
		{
			_num = 0;
		}

		CacheRange Add(const Matrix *matrix, int32 numMatrix = 1)
		{
			CacheRange result;
			if (nullptr != matrix)
			{
				Assert(_num + numMatrix < VIDEO_CONFIG_MAX_MATRIX_CACHE); 
				uint32 first = _num;
				memcpy(&_cache[_num], matrix, sizeof(Matrix) * numMatrix);
				_num += numMatrix;

				result._start = first;
				result._end = (numMatrix == 1) ? (-1) : (_num - 1);
				return result;
			}
			else
			{
				return result;
			}
		}

		uint32 Top()
		{
			return _num;
		}

		Matrix _cache[VIDEO_CONFIG_MAX_MATRIX_CACHE];
		uint32 _num;
	};

	struct RenderCommand
	{
		enum DrawType
		{
			eStatic,
			eAnimated,
		};

		enum PrimType
		{
			eTriangleList,
			eLineList
		};

		VertexBufferHandle _vHandle;
		IndexBufferHandle _iHandle;

		int16 _vertInfluence{};

		//버텍스 버퍼로만 그릴때 사용된다
		uint32 _startVertex{};

		//인덱스 버퍼로 그릴 때 사용된다
		uint32 _startIndex{};
		uint32 _numVertices{};
		uint32 _numPrim{};

		PrimType _primType;
		DrawType _drawType;

		EffectHandle _effectHandle;
		MaterialHandle _materialHandle;

		MatrixCache::CacheRange _cacheRange{};
	};

	struct RenderView
	{
		bool Create();
		void Destroy();

		RenderCommand &GetCommand() { return _renderCommands[_commandCount++]; }
		void PreRender();
		void ExecCommands();
		void PostRender();

		//void Sort();

		uint32 _clearColor{};

		Camera *_pCamera{};

		RenderCommand _renderCommands[VIDEO_CONFIG_MAX_RENDER_COMMAND_SIZE];
		uint32 _commandCount{};

		MatrixCache _matrixCache;

		FrameBufferHandle _colorFBHandle;
		FrameBufferHandle _depthFBHandle;;
	};
	
	struct Font
	{
		bool Create(const D3DXFONT_DESC &fontDesc);
		void Destroy();

		void GetBoundingRect(const std::string &str, int32 x, int32 y, RECT *pOutRect);
		void PrintText(const std::string &str, int32 x, int32 y, uint32 color);
		void PrintTextShadow(const std::string &str, int32 x, int32 y, uint32 color, uint32 shadow = 0xFF000000);

		ID3DXFont *_ptr{};
	};

	//디버그 용도로 사용한다....
	struct DebugBuffer
	{
		VertexBufferHandle _vHandle;
		void Add(const DebugVertex &vertex);
		void Reset();
		void Update();

		DebugVertex _vertices[1024];
		uint32 _count{};

		static video::EffectHandle sDefaultEffectHandle;
	};

	struct ImguiBuffer
	{


	};
}

template <typename HandleType>
class ResourceHandlePool
{
	typedef std::map<uint16, std::string> HandleTable;
	typedef std::map<std::string, HandleType> NameTable;
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
	HandleType Get(const std::string &name) const;

	bool32 IsValid(HandleType id) const;
	uint32 GetSize() const;
	void Resize(uint32 amount);
	void Clear();

	const HandleType BuildHandle(uint16 index, uint16 count) const;
	HandleTable &GetHandleTable() { return _handleTable; }
	NameTable &GetNametable() { return _nameTable; }

private:
	uint32 _defaultPoolSize;
	uint32 _nextID;

	std::vector<HandleType> _freeList;
	std::vector<uint32> _counts;
	HandleTable _handleTable;
	NameTable _nameTable;
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
		auto &handleIter = _handleTable.find(result.index);
		if (handleIter == _handleTable.end())
		{
			_handleTable[result.index] = name;
		}
		auto &nameIter = _nameTable.find(name);
		if (nameIter == _nameTable.end())
		{
			_nameTable[name] = result;
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

	auto handleFound = _handleTable.find(handle.index);
	if (handleFound != _handleTable.end())
	{
		auto nameFound = _nameTable.find(handleFound->second);
		if (nameFound != _nameTable.end())
		{
			_nameTable.erase(nameFound);
		}
		_handleTable.erase(handleFound);
	}
}

template<typename HandleType>
inline void ResourceHandlePool<HandleType>::SetName(HandleType handle, const std::string & name)
{
	auto handleFound = _handleTable.find(handle.index);
	_handleTable[handle.index] = name;

	auto nameFound = _nameTable.find(name);
	_nameTable[name] = handle;
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
		return BuildHandle(0, 0);
	}
}

template<typename HandleType>
inline const std::string & ResourceHandlePool<HandleType>::GetName(HandleType handle) const
{
	auto &find = _handleTable.find(handle.index);
	if (find != _handleTable.end())
	{
		return find->second;
	}
	else
	{
		return std::string();
	}
}

template<typename HandleType>
inline HandleType ResourceHandlePool<HandleType>::Get(const std::string & name) const
{
	auto &find = _nameTable.find(name);
	if (find != _nameTable.end())
	{
		return find->second;
	}
	else
	{
		return BuildHandle(0, 0);
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

	_handleTable.clear();
	_nameTable.clear();
	_nextID = 0;
}

template<typename HandleType>
inline const HandleType ResourceHandlePool<HandleType>::BuildHandle(uint16 index, uint16 count) const
{
	HandleType result(index, count);
	return result;
}

#include "VideoResource_Mesh.h"

#endif