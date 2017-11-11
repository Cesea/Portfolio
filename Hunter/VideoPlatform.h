#ifndef VIDEO_PLATFORM_H
#define VIDEO_PLATFORM_H

#include "stdafx.h"

#include "Video.h"
#include <inttypes.h>
#include <stdarg.h>

#include <unordered_map>

	#define VIDEO_RENDERER_DIRECT3D (BX_PLATFORM_WINDOWS|BX_PLATFORM_XBOX360)
	#define VIDEO_MULTITHREADED  \
		( (VIDEO_PLATFORM_WINDOWS | VIDEOBX_PLATFORM_XBOX360 | VIDEOBX_PLATFORM_NACL | VIDEOBX_PLATFORM_LINUX) & (!BGFX_CONFIG_RENDERER_NULL) )
	#define VIDEO_MAX_DRAW_CALLS (8<<10)
	#define VIDEO_MAX_MATRIX_CACHE (16<<10)
	#define VIDEO_MAX_VIEWS 32
	#define VIDEO_MAX_INDEX_BUFFERS (4<<10)
	#define VIDEO_MAX_VERTEX_DECLS 64
	#define VIDEO_MAX_VERTEX_BUFFERS (4<<10)
	#define VIDEO_MAX_VERTEX_SHADERS 128
	#define VIDEO_MAX_FRAGMENT_SHADERS 128
	#define VIDEO_MAX_SURFACE_MATERIALS 512
	#define VIDEO_MAX_TEXTURES (4<<10)
	#define VIDEO_MAX_RENDER_TARGETS 64
	#define VIDEO_MAX_UNIFORMS 512
	#define VIDEO_MAX_COMMAND_BUFFER_SIZE (64<<10)
	#define VIDEO_DYNAMIC_VERTEX_BUFFER_SIZE (6<<20)
	#define VIDEO_DYNAMIC_INDEX_BUFFER_SIZE (2<<20)
	#define VIDEO_MAX_CONSTANT_BUFFER_SIZE (512<<10)

#if VIDEO_MULTITHREADED
#include "Semaphore.h"
#endif
	#define VIDEO_DRAW_WHOLE_INDEX_BUFFER 0xffffffff

	#define VIDEO_DEFAULT_WIDTH  1280
	#define VIDEO_DEFAULT_HEIGHT 720

	#define VIDEO_STATE_TEX0      UINT64_C(0x0100000000000000)
	#define VIDEO_STATE_TEX1      UINT64_C(0x0200000000000000)
	#define VIDEO_STATE_TEX2      UINT64_C(0x0400000000000000)
	#define VIDEO_STATE_TEX3      UINT64_C(0x0800000000000000)
	#define VIDEO_STATE_TEX4      UINT64_C(0x1000000000000000)
	#define VIDEO_STATE_TEX_MASK  UINT64_C(0x1f00000000000000)
	#define VIDEO_STATE_TEX_COUNT 5

	#define VIDEO_SAMPLER_NONE               UINT16_C(0x0000)
	#define VIDEO_SAMPLER_RENDERTARGET_COLOR UINT16_C(0x0001)
	#define VIDEO_SAMPLER_RENDERTARGET_DEPTH UINT16_C(0x0002)
	#define VIDEO_SAMPLER_TYPE_MASK          UINT16_C(0x0003)

namespace video
{

	extern const uint32 gConstantTypeSize[ConstantType::Count];
	extern ReallocFunction gRealloc;
	extern FreeFunction gFree;
	extern void Free(Memory* _mem);
	//extern void saveTga(const char* _filePath, uint32 _width, uint32 _height, uint32 _pitch, const void* _data);
	extern const char* GetAttribName(Attrib::Enum _attr);


	inline uint32 Uint16Min(uint16 _a, uint16 _b)
	{
		return _a > _b ? _b : _a;
	}

	inline uint32 Hash(const void* data, uint32 size)
	{
		HashMurmur2A murmur;
		murmur.Begin();
		murmur.Add(data, (int)size);
		return murmur.End();
	}

	inline uint32 GCD(uint32 a, uint32 b)
	{
		do
		{
			uint32 tmp = a % b;
			a = b;
			b = tmp;
		} while (b);

		return a;
	}

	inline uint32 LCM(uint32 a, uint32 b)
	{
		return a * (b / GCD(a, b));
	}


	inline uint32 StrideAlign(uint32 offset, uint32 stride)
	{
		const uint32 mod = offset % stride;

		const uint32 add = stride - mod;
		const uint32 mask = -(mod == 0);

		const uint32 tmp = Uint32Selb(mask, 0, add);

		const uint32 result = offset + tmp;
		return result;
	}

	inline uint32 StrideAlign16(uint32 offset, uint32 stride)
	{
		uint32 align = LCM(16, stride);
		return offset + align - (offset % align);
	}

	inline uint32 StrideAlign256(uint32 offset, uint32 stride)
	{
		uint32 align = LCM(256, stride);
		return offset + align - (offset % align);
	}

	void Dump(const VertexDecl &decl);

	struct TextVideoMem
	{
		TextVideoMem()
		{
			Resize();
		}

		~TextVideoMem()
		{
			gFree(_mem);
		}

		void Resize(bool sm = false, uint16 width = VIDEO_DEFAULT_WIDTH, uint16 height = VIDEO_DEFAULT_HEIGHT)
		{
			_small = sm;
			_width = Uint32Max(1, width / 8);
			_height = Uint32Max(1, height / (_small ? 8 : 16));
			_size = _width * _height * 2;

			_mem = (uint8*)gRealloc(_mem, _size);

			Clear();
		}

		void Clear()
		{
			memset(_mem, 0, _size);
		}

		void PrintfVargs(uint16 x, uint16 y, uint8 attr, const char* format, va_list argList)
		{
			if (x < _width && y < _height)
			{
				char* temp = (char*)alloca(_width);

				uint32 num = vsnprintf(temp, _width, format, argList);

				uint8* mem = &_mem[(y * _width + x) * 2];
				for (uint32 i = 0, xx = x; i < num && xx < _width; ++i, ++xx)
				{
					mem[0] = temp[i];
					mem[1] = attr;
					mem += 2;
				}
			}
		}

		void Printf(uint16 x, uint16 y, uint8 attr, const char* format, ...)
		{
			va_list argList;
			va_start(argList, format);
			PrintfVargs(x, y, attr, format, argList);
			va_end(argList);
		}

		uint8* _mem;
		uint32 _size;
		uint16 _width;
		uint16 _height;
		bool _small;
	};

	struct TextVideoMemBlitter
	{
		void Init();
		void Blit(const TextVideoMem& _mem);
		void Setup();
		void Render(uint32 _numIndices);

		video::TextureHandle _texture;
		DynamicVertexBuffer* _vb;
		DynamicIndexBuffer* _ib;
		video::VertexDecl _decl;
		video::SurfaceMaterialHandle _material;
	};

	extern TextVideoMemBlitter gTextVideoMemBlitter;

	struct PredefinedUniform
	{
		enum Enum
		{
			eViewRect,
			eViewTexel,
			eView,
			eViewProj,
			eViewProjX,
			eModel,
			eModelViewProj,
			eModelViewProjX,
			eAlphaRef,
			Count
		};

		uint8 _type;
		uint16 _loc;
	};
	PredefinedUniform::Enum NameToPredefinedUniformEnum(const char* name);

	class StreamRead
	{
	public:
		StreamRead(const void* data, uint32 size)
			: _data((uint8*)data)
			, _size(size)
			, _pos(0)
		{
		}

		~StreamRead()
		{
		}

		void Skip(uint32 size)
		{
			Assert(_size - _pos >= size); //용량 확인
			_pos += size;
		}

		void Read(void* data, uint32 size)
		{
			Assert(_size - _pos >= size); //용량 확인
			memcpy(data, &_data[_pos], size);
			_pos += size;
		}

		template<typename T>
		void Read(T& value)
		{
			Read(&value, sizeof(T));
		}

		const uint8* GetDataPtr() const
		{
			return &_data[_pos];
		}

		uint32 GetPos() const
		{
			return _pos;
		}

		void Align(uint16 align)
		{
			_pos = StrideAlign(_pos, align);
		}

	private:
		const uint8* _data;
		uint32 _size;
		uint32 _pos;
	};

	class StreamWrite
	{
	public:
		StreamWrite(void* data, uint32 size)
			: _data((uint8*)data)
			, _size(size)
			, _pos(0)
		{
		}

		~StreamWrite()
		{
		}

		void Write(void* data, uint32 size)
		{
			Assert(_size - _pos >= size); //용량 확인
			memcpy(&_data[_pos], data, size);
			_pos += size;
		}

		template<typename T>
		void Write(T &value)
		{
			Write(&value, sizeof(T));
		}

		uint8* GetDataPtr() const
		{
			return &_data[_pos];
		}

		uint32 GetPos() const
		{
			return _pos;
		}

		void Align(uint16 align)
		{
			_pos = StrideAlign(_pos, align);
		}

	private:
		uint8* _data;
		uint32 _size;
		uint32 _pos;
	};

	struct CommandBuffer
	{
		CommandBuffer()
			: _pos(0)
			, _size(VIDEO_MAX_COMMAND_BUFFER_SIZE)
		{
			Finish();
		}

		enum Enum
		{
			eRendererInit,
			eCreateVertexDecl,
			eCreateIndexBuffer,
			eCreateDynamicIndexBuffer,
			eCreateVertexBuffer,
			eCreateDynamicVertexBuffer,
			eCreateVertexShader,
			eCreateFragmentShader,
			eCreateMaterial,
			eCreateTexture,
			eCreateRenderTarget,
			eCreateUniform,
			eEnd,
			eRendererShutdown,
			eDestroyVertexDecl,
			eDestroyIndexBuffer,
			eDestroyDynamicIndexBuffer,
			eDestroyVertexBuffer,
			eDestroyDynamicVertexBuffer,
			eDestroyVertexShader,
			eDestroyFragmentShader,
			eDestroyMaterial,
			eDestroyTexture,
			eDestroyRenderTarget,
			eDestroyUniform,
			eSaveScreenShot,
		};

		void Write(const void* data, uint32 size)
		{
			Assert(_pos < _size);
			memcpy(&_buffer[_pos], data, size);
			_pos += size;
		}

		template<typename T>
		void Write(const T& in)
		{
			Write(reinterpret_cast<const uint8*>(&in), sizeof(T));
		}

		void Read(void* data, uint32 size)
		{
			Assert(_pos < _size, "");
			memcpy(data, &_buffer[_pos], size);
			_pos += size;
		}

		template<typename T>
		void Read(T& in)
		{
			Read(reinterpret_cast<uint8*>(&in), sizeof(T));
		}

		void Reset()
		{
			_pos = 0;
		}

		void Finish()
		{
			uint32 cmd = eEnd;
			Write(cmd);
			_pos = 0;
		}

		uint32 _pos;
		uint32 _size;
		uint8 _buffer[VIDEO_MAX_COMMAND_BUFFER_SIZE];

	private:
		CommandBuffer(const CommandBuffer&);
		void operator=(const CommandBuffer&);
	};

	struct SortKey
	{
		uint64 Encode()
		{
			// |               3               2               1               0|
			// |fedcba9876543210fedcba9876543210fedcba9876543210fedcba9876543210|
			// |             vvvvvsssssssssssttmmmmmmmmmdddddddddddddddddddddddd|
			// |                 ^          ^ ^        ^                       ^|
			// |                 |          | |        |                       ||

			uint64 tmp0 = _depth;
			uint64 tmp1 = uint64(_material) << 0x18;
			uint64 tmp2 = uint64(_trans) << 0x21;
			uint64 tmp3 = uint64(_seq) << 0x23;
			uint64 tmp4 = uint64(_view) << 0x2e;
			uint64 key = tmp0 | tmp1 | tmp2 | tmp3 | tmp4;
			return key;
		}

		void Decode(uint64 key)
		{
			_depth = key & 0xffffffff;
			_material = (key >> 0x18) & (VIDEO_MAX_SURFACE_MATERIALS - 1);
			_trans = (key >> 0x21) & 0x3;
			_seq = (key >> 0x23) & 0x7ff;
			_view = (key >> 0x2e)&(VIDEO_MAX_VIEWS - 1);
		}

		void Reset()
		{
			_depth = 0;
			_material = 0;
			_seq = 0;
			_view = 0;
			_trans = 0;
		}

		int32 _depth;
		uint16 _material;
		uint16 _seq;
		uint8 _view;
		uint8 _trans;
	};

	struct Clear
	{
		uint32 _rgba;
		float _depth;
		uint8 _stencil;
		uint8 _flags;
	};

	struct Rect
	{
		uint16 _x;
		uint16 _y;
		uint16 _width;
		uint16 _height;
	};

	struct MatrixCache
	{
		MatrixCache()
			: _num(1)
		{
			MatrixIdentity(&_cache[0]);
		}

		void Reset()
		{
			_num = 1;
		}

		uint32 Add(const Matrix *matrix, uint16 num)
		{
			if (NULL != matrix)
			{
				Assert(_num + _num < VIDEO_MAX_MATRIX_CACHE);//용량 초과

				uint32 inum = min(VIDEO_MAX_MATRIX_CACHE - _num, num);
				uint32 first = _num;
				memcpy(&_cache[_num], matrix, sizeof(Matrix) * num);
				_num += num;
				return first;
			}

			return 0;
		}

		Matrix _cache[VIDEO_MAX_MATRIX_CACHE];
		uint32 _num;
	};

	struct Sampler
	{
		uint16 _index;
		uint16 _flags;
	};

	struct Constant
	{
		ConstantType::Enum _type;
		uint16 _num;
	};

	#define CONSTANT_OPCODE_MASK(_bits) ( (1<<_bits)-1)

	#define CONSTANT_OPCODE_TYPE_BITS 8
	#define CONSTANT_OPCODE_TYPE_MASK CONSTANT_OPCODE_MASK(CONSTANT_OPCODE_TYPE_BITS)
	#define CONSTANT_OPCODE_LOC_BITS 10
	#define CONSTANT_OPCODE_LOC_MASK CONSTANT_OPCODE_MASK(CONSTANT_OPCODE_LOC_BITS)
	#define CONSTANT_OPCODE_NUM_BITS 10
	#define CONSTANT_OPCODE_NUM_MASK CONSTANT_OPCODE_MASK(CONSTANT_OPCODE_NUM_BITS)
	#define CONSTANT_OPCODE_COPY_BITS 1
	#define CONSTANT_OPCODE_COPY_MASK CONSTANT_OPCODE_MASK(CONSTANT_OPCODE_COPY_BITS)

	#define VIDEO_UNIFORM_FUNCTIONBIT UINT8_C(0x40)
	#define VIDEO_UNIFORM_FRAGMENTBIT UINT8_C(0x80)
	#define VIDEO_UNIFORM_TYPEMASK    UINT8_C(0x3f)

	class ConstantBuffer
	{
	public:
		static ConstantBuffer* Create(uint32 _size)
		{
			uint32 size = BX_ALIGN_16(Uint32Max(_size, sizeof(ConstantBuffer)));
			void* data = gRealloc(NULL, size);
			return ::new(data) ConstantBuffer(_size);
		}

		static void Destroy(ConstantBuffer* constantBuffer)
		{
			constantBuffer->~ConstantBuffer();
			gFree(constantBuffer);
		}

		static uint32 EncodeOpcode(ConstantType::Enum type, uint16 loc, uint16 num, uint16 copy)
		{
			uint32 opcode = 0;

			opcode <<= CONSTANT_OPCODE_TYPE_BITS;
			opcode |= type & CONSTANT_OPCODE_TYPE_MASK;

			opcode <<= CONSTANT_OPCODE_LOC_BITS;
			opcode |= loc & CONSTANT_OPCODE_LOC_MASK;

			opcode <<= CONSTANT_OPCODE_NUM_BITS;
			opcode |= num & CONSTANT_OPCODE_NUM_MASK;

			opcode <<= CONSTANT_OPCODE_COPY_BITS;
			opcode |= copy & CONSTANT_OPCODE_COPY_MASK;

			return opcode;
		}

		static void DecodeOpcode(uint32 opcode, ConstantType::Enum& type, 
			uint16& loc, uint16& num, uint16& copy)
		{
			uint32 icopy;
			uint32 inum;
			uint32 iloc;

			icopy = opcode & CONSTANT_OPCODE_COPY_MASK;
			opcode >>= CONSTANT_OPCODE_COPY_BITS;

			inum = opcode & CONSTANT_OPCODE_NUM_MASK;
			opcode >>= CONSTANT_OPCODE_NUM_BITS;

			iloc = opcode & CONSTANT_OPCODE_LOC_MASK;
			opcode >>= CONSTANT_OPCODE_LOC_BITS;

			type = (ConstantType::Enum)(opcode & CONSTANT_OPCODE_TYPE_MASK);
			opcode >>= CONSTANT_OPCODE_TYPE_BITS;

			copy = (uint16)icopy;
			num = (uint16)inum;
			loc = (uint16)iloc;
		}

		void Write(const void* data, uint32 size)
		{
			Assert(_pos + size < _size);
			if (_pos + size < _size)
			{
				memcpy(&_buffer[_pos], data, size);
				_pos += size;
			}
		}

		void Write(uint32 value)
		{
			Write(&value, sizeof(uint32));
		}

		const char* Read(uint32 size)
		{
			Assert(_pos < _size);
			const char* result = &_buffer[_pos];
			_pos += size;
			return result;
		}

		uint32 Read()
		{
			const char* result = Read(sizeof(uint32));
			return *((uint32*)result);
		}

		bool IsEmpty() const
		{
			return 0 == _pos;
		}

		uint32 GetPos() const
		{
			return _pos;
		}

		void Reset(uint32 pos = 0)
		{
			_pos = pos;
		}

		void Finish()
		{
			Write(ConstantType::eEnd);
			_pos = 0;
		}

		void WriteUniform(ConstantType::Enum type, uint16 loc, const void* value, uint16 num = 1);
		void WriteUniformRef(ConstantType::Enum type, uint16 loc, const void* value, uint16 num = 1);
		void Commit(bool force);

	private:
		ConstantBuffer(uint32 size)
			: _size(size - sizeof(_buffer))
			, _pos(0)
		{
			Finish();
		}

		~ConstantBuffer()
		{
		}

		uint32 _size;
		uint32 _pos;
		char _buffer[8];
	};

	typedef const void *(*UniformFunction)(const void *data);

	struct UniformInfo
	{
		const void *_data;
		UniformFunction _function;
	};

	class UniformRegistry
 	{
	public:
		UniformRegistry()
		{
		}

		~UniformRegistry()
		{
		}
 
 		const UniformInfo* Find(const char *name) const
 		{
			UniformHashMap::const_iterator it = _uniforms.find(name);
			if (it != _uniforms.end() )
			{
				return &it->second;
			}

 			return nullptr;
 		}

		const UniformInfo& Reg(const char *name, const void *data, UniformFunction func = nullptr)
		{
			UniformHashMap::const_iterator it = _uniforms.find(name);
			if (it == _uniforms.end() )
			{
				UniformInfo info;
				info._data = data;
				info._function = func;

				std::pair<UniformHashMap::iterator, bool> result = 
					_uniforms.insert(UniformHashMap::value_type(name, info));
				return result.first->second;	
			}
			return it->second;
		}

 	private:
 		typedef std::unordered_map<std::string, UniformInfo> UniformHashMap;
 		UniformHashMap _uniforms;
 	};

	struct RenderState
	{
		void Reset()
		{
			_constEnd = 0;
			Clear();
		}

		void Clear()
		{
			_constBegin = _constEnd;
			_flags = VIDEO_STATE_DEFAULT;
			_matrix = 0;
			_startIndex = VIDEO_DRAW_WHOLE_INDEX_BUFFER;
			_numIndices = 0;
			_startVertex = 0;
			_numVertices = UINT32_C(0xffffffff);
			_num = 1;
			_vertexBuffer.index = video::invalidHandle;
			_vertexDecl.index = video::invalidHandle;
			_indexBuffer.index = video::invalidHandle;

			for (uint32 i = 0; i < VIDEO_STATE_TEX_COUNT; ++i)
			{
				_sampler[i]._index = video::invalidHandle;
				_sampler[i]._flags = VIDEO_SAMPLER_NONE;
			}
		}

		uint64 _flags;
		uint32 _constBegin;
		uint32 _constEnd;
		uint32 _matrix;
		uint32 _startIndex;
		uint32 _numIndices;
		uint32 _startVertex;
		uint32 _numVertices;
		uint16 _num;

		VertexBufferHandle _vertexBuffer;
		VertexDeclHandle _vertexDecl;
		IndexBufferHandle _indexBuffer;
		Sampler _sampler[VIDEO_STATE_TEX_COUNT];
	};

	struct Resolution
	{
		Resolution()
			: _width(VIDEO_DEFAULT_WIDTH)
			, _height(VIDEO_DEFAULT_HEIGHT)
			, _flags(VIDEO_RESET_NONE)
		{
		}

		uint32 _width;
		uint32 _height;
		uint32 _flags;
	};

	struct Frame
	{
		//VIDEO_CACHE_LINE_ALIGN_MARKER();

		Frame()
		{
			_constantBuffer = ConstantBuffer::Create(VIDEO_MAX_CONSTANT_BUFFER_SIZE);
			Reset();
		}

		~Frame()
		{
			ConstantBuffer::Destroy(_constantBuffer);
		}

		void Reset()
		{
			_state.Reset();
			_matrixCache.Reset();
			_key.Reset();
			_num = 0;
			_numRenderStates = 0;
			_numDropped = 0;
			_iboffset = 0;
			_vboffset = 0;
			_cmdPre.Reset();
			_cmdPost.Reset();
			_constantBuffer->Reset();
			_discard = false;
			ResetFreeHandles();
		}

		void Finish()
		{
			_cmdPre.Finish();
			_cmdPost.Finish();
			_constantBuffer->Finish();

			if (0 < _numDropped)
			{
				Assert(false);
			}
		}

		void SetViewTransform(uint8 id, const Matrix *view, const Matrix *projection, uint8 other)
		{
			if (VIDEO_MAX_VIEWS > other)
			{
				_other[id] = other;
			}
			else
			{
				_other[id] = id;
			}

			if (NULL != view)
			{
				memcpy(_view[id], view, sizeof(Matrix));
			}
			else
			{
				MatrixIdentity(&_view[id]);
			}

			if (NULL != projection)
			{
				memcpy(_projection[id], projection, sizeof(Matrix));
			}
			else
			{
				MatrixIdentity(&_projection[id]);
			}
		}

		void SetViewTransformMask(uint32 viewMask, const Matrix* view, const Matrix* proj, uint8 other)
		{
			for (uint32 id = 0, iviewMask = viewMask, ntz = Uint32BitScan(viewMask); 
				0 != iviewMask; iviewMask >>= 1, id += 1, ntz = Uint32BitScan(iviewMask))
			{
				viewMask >>= ntz;
				id += ntz;

				SetViewTransform(id, view, proj, other);
			}
		}

		void SetState(uint64 state)
		{
			uint8 blend = ((state & VIDEO_STATE_BLEND_MASK) >> VIDEO_STATE_BLEND_SHIFT) & 0xff;
			_key._trans = "\x0\x1\x1\x2\x2\x1\x2\x1\x2\x1\x1\x1\x1\x1\x1\x1\x1"[((blend) & 0xf) + (!!blend)];
			_state._flags = state;
		}

		uint32 SetTransform(Matrix *matrix, uint16 num)
		{
			_state._matrix = _matrixCache.Add(matrix, _num);
			_state._num = _num;

			return _state._matrix;
		}

		void SetTransform(uint32 cache, uint16 num)
		{
			_state._matrix = cache;
			_state._num = _num;
		}

		void SetIndexBuffer(IndexBufferHandle handle, uint32 firstIndex, uint32 numIndices)
		{
			_state._startIndex = firstIndex;
			_state._numIndices = numIndices;
			_state._indexBuffer = handle;
		}

		void SetIndexBuffer(const DynamicIndexBuffer *ib, uint32 numIndices)
		{
			_state._indexBuffer = ib->_handle;
			_state._startIndex = ib->_startIndex;
			_state._numIndices = numIndices;
			_discard = (0 == numIndices);
			gFree(const_cast<DynamicIndexBuffer*>(ib));
		}

		void SetVertexBuffer(VertexBufferHandle handle)
		{
			Assert(handle.index < VIDEO_MAX_VERTEX_BUFFERS);
			_state._startVertex = 0;
			_state._numVertices = UINT32_C(0xffffffff);
			_state._vertexBuffer = handle;
		}

		void SetVertexBuffer(const DynamicVertexBuffer* vb)
		{
			_state._startVertex = vb->_startVertex;
			_state._numVertices = vb->_size / vb->_stride;
			_state._vertexBuffer = vb->_handle;
			_state._vertexDecl = vb->_decl;
			gFree(const_cast<DynamicVertexBuffer*>(vb));
		}

		void SetSurfaceMaterial(SurfaceMaterialHandle handle)
		{
			Assert(invalidHandle == handle.index);
			_key._material = handle.index;
		}

		void SetTexture(uint8 stage, UniformHandle uniformSampler, TextureHandle textureHandle)
		{
			_flags |= VIDEO_STATE_TEX0 << stage;

			Sampler& sampler = _state._sampler[stage];
			sampler._index = textureHandle.index;
			sampler._flags = VIDEO_SAMPLER_NONE;

			if (video::invalidHandle != uniformSampler.index)
			{
				uint32 istage = stage;
				SetUniform(uniformSampler, &istage);
			}
		}

		void SetTexture(uint8 stage, UniformHandle uniformSampler, RenderTargetHandle handle, bool depth)
		{
			_flags |= VIDEO_STATE_TEX0 << stage;
			Sampler& sampler = _state._sampler[stage];
			sampler._index = handle.index;
			sampler._flags = depth ? VIDEO_SAMPLER_RENDERTARGET_DEPTH : VIDEO_SAMPLER_RENDERTARGET_COLOR;

			if (video::invalidHandle != uniformSampler.index)
			{
				uint32 istage = stage;
				SetUniform(uniformSampler, &istage);
			}
		}

		void Submit(uint8 id);
		void SubmitMask(uint32 viewMask);
		void Sort();

		//TODO
		bool CheckAvailDynamicIndexBuffer(uint16 num)
		{
			uint32 offset = _iboffset;
			uint32 iboffset = offset + num * sizeof(uint16);
			iboffset = Uint32Min(iboffset, VIDEO_DYNAMIC_INDEX_BUFFER_SIZE);
			uint32 inum = (iboffset - offset) / sizeof(uint16);
			return inum == num;
		}

		uint32 AllocDynamicIndexBuffer(uint16& num)
		{
			uint32 offset = _iboffset;
			_iboffset = offset + _num * sizeof(uint16);
			_iboffset = Uint32Min(_iboffset, VIDEO_DYNAMIC_INDEX_BUFFER_SIZE);
			_num = uint16((_iboffset - offset) / sizeof(uint16));
			return offset;
		}

		bool CheckAvailDynamicVertexBuffer(uint16 num, uint16 stride)
		{
			uint32 offset = StrideAlign(_vboffset, stride);
			uint32 vboffset = offset + num * stride;
			vboffset = Uint32Min(vboffset, VIDEO_DYNAMIC_VERTEX_BUFFER_SIZE);
			uint32 inum = (vboffset - offset) / stride;
			return inum == num;
		}

		uint32 AllocDynamicVertexBuffer(uint16& num, uint16 stride)
		{
			uint32 offset = StrideAlign(_vboffset, stride);
			_vboffset = offset + _num * stride;
			_vboffset = Uint32Min(_vboffset, VIDEO_DYNAMIC_VERTEX_BUFFER_SIZE);
			_num = uint16((_vboffset - offset) / stride);
			return offset;
		}

		void WriteConstant(ConstantType::Enum type, UniformHandle handle, const void* value, uint16 num)
		{
			_constantBuffer->WriteUniform(type, handle.index, value, num);
		}

		void Free(IndexBufferHandle handle)
		{
			_freeIndexBufferHandle[_numFreeIndexBufferHandles] = handle;
			++_numFreeIndexBufferHandles;
		}

		void Free(VertexDeclHandle handle)
		{
			_freeVertexDeclHandle[_numFreeVertexDeclHandles] = handle;
			++_numFreeVertexDeclHandles;
		}

		void Free(VertexBufferHandle handle)
		{
			_freeVertexBufferHandle[_numFreeVertexBufferHandles] = handle;
			++_numFreeVertexBufferHandles;
		}

		void Free(VertexShaderHandle handle)
		{
			_freeVertexShaderHandle[_numFreeVertexShaderHandles] = handle;
			++_numFreeVertexShaderHandles;
		}

		void Free(FragmentShaderHandle handle)
		{
			_freeFragmentShaderHandle[_numFreeFragmentShaderHandles] = handle;
			++_numFreeFragmentShaderHandles;
		}

		void Free(SurfaceMaterialHandle handle)
		{
			_freeSurfaceMaterialHandle[_numFreeSurfaceMaterialHandles] = handle;
			++_numFreeSurfaceMaterialHandles;
		}

		void Free(TextureHandle _handle)
		{
			_freeTextureHandle[_numFreeTextureHandles] = _handle;
			++_numFreeTextureHandles;
		}

		void Free(RenderTargetHandle handle)
		{
			_freeRenderTargetHandle[_numFreeRenderTargetHandles] = handle;
			++_numFreeRenderTargetHandles;
		}

		void Free(UniformHandle handle)
		{
			_freeUniformHandle[_numFreeUniformHandles] = handle;
			++_numFreeUniformHandles;
		}

		void ResetFreeHandles()
		{
			_numFreeIndexBufferHandles = 0;
			_numFreeVertexDeclHandles = 0;
			_numFreeVertexBufferHandles = 0;

			_numFreeVertexShaderHandles = 0;
			_numFreeFragmentShaderHandles = 0;

			_numFreeSurfaceMaterialHandles = 0;
			_numFreeTextureHandles = 0;
			_numFreeRenderTargetHandles = 0;
			_numFreeUniformHandles = 0;
		}

		SortKey _key;

		RenderTargetHandle _renderTarget[VIDEO_MAX_VIEWS];
		Clear _clear[VIDEO_MAX_VIEWS];
		Rect _rect[VIDEO_MAX_VIEWS];
		Matrix _view[VIDEO_MAX_VIEWS];
		Matrix _projection[VIDEO_MAX_VIEWS];
		uint8 _other[VIDEO_MAX_VIEWS];

		uint64 _sortKeys[VIDEO_MAX_DRAW_CALLS];
		uint16 _sortValues[VIDEO_MAX_DRAW_CALLS];
		RenderState _renderState[VIDEO_MAX_DRAW_CALLS];
		RenderState _state;
		uint64 _flags;

		ConstantBuffer* _constantBuffer;

		uint16 _num;
		uint16 _numRenderStates;
		uint16 _numDropped;

		MatrixCache _matrixCache;

		uint32 _iboffset;
		uint32 _vboffset;
		DynamicIndexBuffer* _dynamicIb;
		DynamicVertexBuffer* _dynamicVb;

		Resolution _resolution;
		uint32 _debug;

		CommandBuffer _cmdPre;
		CommandBuffer _cmdPost;

		uint16 _numFreeIndexBufferHandles;
		uint16 _numFreeVertexDeclHandles;
		uint16 _numFreeVertexBufferHandles;
		uint16 _numFreeVertexShaderHandles;
		uint16 _numFreeFragmentShaderHandles;
		uint16 _numFreeSurfaceMaterialHandles;
		uint16 _numFreeTextureHandles;
		uint16 _numFreeRenderTargetHandles;
		uint16 _numFreeUniformHandles;

		IndexBufferHandle _freeIndexBufferHandle[VIDEO_MAX_INDEX_BUFFERS];
		VertexDeclHandle _freeVertexDeclHandle[VIDEO_MAX_VERTEX_DECLS];
		VertexBufferHandle _freeVertexBufferHandle[VIDEO_MAX_VERTEX_BUFFERS];
		VertexShaderHandle _freeVertexShaderHandle[VIDEO_MAX_VERTEX_SHADERS];
		FragmentShaderHandle _freeFragmentShaderHandle[VIDEO_MAX_FRAGMENT_SHADERS];
		SurfaceMaterialHandle _freeSurfaceMaterialHandle[VIDEO_MAX_SURFACE_MATERIALS];
		TextureHandle _freeTextureHandle[VIDEO_MAX_TEXTURES];
		RenderTargetHandle _freeRenderTargetHandle[VIDEO_MAX_RENDER_TARGETS];
		UniformHandle _freeUniformHandle[VIDEO_MAX_UNIFORMS];

		int64 _waitSubmit;
		int64 _waitRender;

		bool _discard;
	};

	struct SurfaceMaterialRef
	{
		SurfaceMaterialRef()
		{
		}

		SurfaceMaterialHandle Find(uint32 hash)
		{
			SurfaceMaterialMap::const_iterator it = _surfaceMaterialMap.find(hash);
			if (it != _surfaceMaterialMap.end())
			{
				return it->second;
			}

			SurfaceMaterialHandle result;
			result.index = INVALID_HANDLE;
			return result;
		}

		void Add(SurfaceMaterialHandle handle, uint32_t hash)
		{
			_surfaceMaterialMap.insert(std::make_pair(hash, handle));
		}

		typedef std::unordered_map<uint32_t, SurfaceMaterialHandle> SurfaceMaterialMap;
		SurfaceMaterialMap _surfaceMaterialMap;
	};

	struct VertexDeclRef
	{
		VertexDeclRef()
		{
			memset(_vertexDeclRef, 0, sizeof(_vertexDeclRef));
			memset(_vertexBufferRef, 0xff, sizeof(_vertexBufferRef));
		}

		VertexDeclHandle Find(uint32_t hash)
		{
			VertexDeclMap::const_iterator it = _vertexDeclMap.find(hash);
			if (it != _vertexDeclMap.end())
			{
				return it->second;
			}

			VertexDeclHandle result;
			result.index = INVALID_HANDLE;
			return result;
		}

		void Add(VertexBufferHandle handle, VertexDeclHandle declHandle, uint32 hash)
		{
			_vertexBufferRef[handle.index] = declHandle;
			_vertexDeclRef[declHandle.index]++;
			_vertexDeclMap.insert(std::make_pair(hash, declHandle));
		}

		VertexDeclHandle Release(VertexBufferHandle handle)
		{
			VertexDeclHandle declHandle = _vertexBufferRef[handle.index];
			_vertexDeclRef[declHandle.index]--;

			if (0 != _vertexDeclRef[declHandle.index])
			{
				VertexDeclHandle invalid;
				invalid.index = INVALID_HANDLE;
				return invalid;
			}
			return declHandle;
		}

		typedef std::unordered_map<uint32, VertexDeclHandle> VertexDeclMap;
		VertexDeclMap _vertexDeclMap;
		uint16_t _vertexDeclRef[VIDEO_MAX_VERTEX_DECLS];
		VertexDeclHandle _vertexBufferRef[VIDEO_MAX_VERTEX_BUFFERS];
	};

	DWORD WINAPI RenderThread(LPVOID param);

	struct Context
	{
		Context()
			: _render(&_frame[0]),
			_submit(&_frame[1]),
			_indexBufferHandle(VIDEO_MAX_INDEX_BUFFERS),
			_vertexDeclHandle(VIDEO_MAX_VERTEX_DECLS),
			_vertexBufferHandle(VIDEO_MAX_VERTEX_BUFFERS),
			_vertexShaderHandle(VIDEO_MAX_VERTEX_SHADERS),
			_fragmentShaderHandle(VIDEO_MAX_FRAGMENT_SHADERS),
			_surfaceMaterialHandle(VIDEO_MAX_SURFACE_MATERIALS),
			_textureHandle(VIDEO_MAX_TEXTURES),
			_renderTargetHandle(VIDEO_MAX_VIEWS),
			_uniformHandle(VIDEO_MAX_UNIFORMS),
			_frames(0),
			_debug(VIDEO_DEBUG_NONE),
			_initialized(false),
			_rendererInitialized(false)
		{
		}

		~Context()
		{
		}

		// game thread
		void Init(bool createRenderThread);
		void Shutdown();

		void DoFrame()
		{
			// wait for render thread to finish
			RenderSemWait();

			Swap();

			// release render thread
			GameSemPost();

			#if !VIDEO_CONFIG_MULTITHREADED
			RenderFrame();
			#endif // BGFX_CONFIG_MULTITHREADED
		}

		CommandBuffer& GetCommandBuffer(CommandBuffer::Enum cmd)
		{
			CommandBuffer& cmdbuf = cmd < CommandBuffer::eEnd ? _submit->_cmdPre : _submit->_cmdPost;
			uint8 toWrite = (uint8)cmd;
			cmdbuf.Write(toWrite);
			return cmdbuf;
		}

		void Reset(uint32 width, uint32 height, uint32 flags)
		{
			_resolution._width = width;
			_resolution._height = height;
			_resolution._flags = flags & (~VIDEO_RESET_FULLSCREEN_FAKE);

			memset(_renderTarget, 0xff, sizeof(_renderTarget));

			#if BX_PLATFORM_WINDOWS
			uint32 fullscreen = (_flags&BGFX_RESET_FULLSCREEN_MASK) >> BGFX_RESET_FULLSCREEN_SHIFT;
			m_window.adjust(_width, _height, BGFX_RESET_FULLSCREEN_FAKE != fullscreen);
			#endif // BX_PLATFORM_WINDOWS
		}

		void DEBUG_TextClear(uint8 _attr, bool _small)
		{
		}

		void DEBUG_TextPrintfVargs(uint16 _x, uint16 _y, uint8 _attr, const char* _format, va_list _argList)
		{
		}

		IndexBufferHandle CreateIndexBuffer(const Memory* mem)
		{
			IndexBufferHandle handle = { _indexBufferHandle.Alloc() };
			CommandBuffer& cmdbuf = GetCommandBuffer(CommandBuffer::eCreateIndexBuffer);
			cmdbuf.Write(handle);
			cmdbuf.Write(mem);
			return handle;
		}

		void DestroyIndexBuffer(IndexBufferHandle handle)
		{
			CommandBuffer& cmdbuf = GetCommandBuffer(CommandBuffer::eDestroyIndexBuffer);
			cmdbuf.Write(handle);
			_submit->Free(handle);
		}

		DynamicIndexBuffer* CreateDynamicIndexBuffer(uint32 size)
		{
			IndexBufferHandle handle = { _indexBufferHandle.Alloc() };
			CommandBuffer& cmdbuf = GetCommandBuffer(CommandBuffer::eCreateDynamicIndexBuffer);
			cmdbuf.Write(handle);
			cmdbuf.Write(size);

			DynamicIndexBuffer* ib = (DynamicIndexBuffer*)gRealloc(NULL, sizeof(DynamicIndexBuffer) + size);
			ib->_data = (uint8*)&ib[1];
			ib->_size = size;
			ib->_handle = handle;

			return ib;
		}

		void DestroyDynamicIndexBuffer(DynamicIndexBuffer* ib)
		{
			CommandBuffer& cmdbuf = GetCommandBuffer(CommandBuffer::eDestroyDynamicIndexBuffer);
			cmdbuf.Write(ib->_handle);

			_submit->Free(ib->_handle);
			gFree(const_cast<DynamicIndexBuffer*>(ib));
		}

		const DynamicIndexBuffer* AllocDynamicIndexBuffer(uint16 num)
		{
			uint32 offset = _submit->AllocDynamicIndexBuffer(num);

			DynamicIndexBuffer& dib = *_submit->_dynamicIb;

			DynamicIndexBuffer* ib = (DynamicIndexBuffer*)gRealloc(NULL, sizeof(DynamicIndexBuffer));
			ib->_data = &dib._data[offset];
			ib->_size = num * sizeof(uint16);
			ib->_handle = dib._handle;
			ib->_startIndex = offset / sizeof(uint16);

			return ib;
		}

		//TODO Implementt this again
		VertexDeclHandle FindVertexDecl(const VertexDecl& decl)
		{
			VertexDeclHandle declHandle = _declRef.Find(decl._hash);

			if (video::invalidHandle == declHandle.index)
			{
				VertexDeclHandle temp = { _vertexDeclHandle.Alloc() };
				declHandle = temp;
				CommandBuffer& cmdbuf = GetCommandBuffer(CommandBuffer::eCreateVertexDecl);
				cmdbuf.Write(declHandle);
				cmdbuf.Write(decl);
			}

			return declHandle;
		}

		VertexBufferHandle CreateVertexBuffer(const Memory* mem, const VertexDecl& decl)
		{
			VertexBufferHandle handle = { _vertexBufferHandle.Alloc() };

			VertexDeclHandle declHandle = FindVertexDecl(decl);
			_declRef.Add(handle, declHandle, decl._hash);

			CommandBuffer& cmdbuf = GetCommandBuffer(CommandBuffer::eCreateVertexBuffer);
			cmdbuf.Write(handle);
			cmdbuf.Write(mem);
			cmdbuf.Write(declHandle);
			return handle;
		}

		void DestroyVertexBuffer(VertexBufferHandle handle)
		{
			VertexDeclHandle declHandle = _declRef.Release(handle);
			if (video::invalidHandle != declHandle.index)
			{
				CommandBuffer& cmdbuf = GetCommandBuffer(CommandBuffer::eDestroyVertexDecl);
				cmdbuf.Write(declHandle);
			}

			CommandBuffer& cmdbuf = GetCommandBuffer(CommandBuffer::eDestroyVertexBuffer);
			cmdbuf.Write(handle);
			_submit->Free(handle);
		}

		DynamicVertexBuffer* CreateDynamicVertexBuffer(uint32 size, const VertexDecl* decl = nullptr)
		{
			VertexBufferHandle handle = { _vertexBufferHandle.Alloc() };

			uint16 stride = 0;
			VertexDeclHandle declHandle;
			declHandle.index = INVALID_HANDLE;

			if (nullptr != decl)
			{
				declHandle = FindVertexDecl(*decl);
				_declRef.Add(handle, declHandle, decl->_hash);

				stride = decl->_stride;
			}

			CommandBuffer& cmdbuf = GetCommandBuffer(CommandBuffer::eCreateDynamicVertexBuffer);
			cmdbuf.Write(handle);
			cmdbuf.Write(size);

			DynamicVertexBuffer* vb = (DynamicVertexBuffer*)gRealloc(NULL, sizeof(DynamicVertexBuffer) + size);
			vb->_data = (uint8*)&vb[1];
			vb->_size = size;
			vb->_startVertex = 0;
			vb->_stride = stride;
			vb->_handle = handle;
			vb->_decl = declHandle;

			return vb;
		}

		void DestroyDynamicVertexBuffer(DynamicVertexBuffer* vb)
		{
			CommandBuffer& cmdbuf = GetCommandBuffer(CommandBuffer::eDestroyDynamicVertexBuffer);
			cmdbuf.Write(vb->_handle);

			_submit->Free(vb->_handle);
			gFree(const_cast<DynamicVertexBuffer*>(vb));
		}

		const DynamicVertexBuffer* AllocDynamicVertexBuffer(uint16 num, const VertexDecl& decl)
		{
			VertexDeclHandle declHandle = _declRef.Find(decl._hash);

			DynamicVertexBuffer& dvb = *_submit->_dynamicVb;

			if (video::invalidHandle == declHandle.index)
			{
				VertexDeclHandle temp = { _vertexDeclHandle.Alloc() };
				declHandle = temp;
				CommandBuffer& cmdbuf = GetCommandBuffer(CommandBuffer::eCreateVertexDecl);
				cmdbuf.Write(declHandle);
				cmdbuf.Write(decl);
				_declRef.Add(dvb._handle, declHandle, decl._hash);
			}

			uint32 offset = _submit->AllocDynamicVertexBuffer(num, decl._stride);

			DynamicVertexBuffer* vb = (DynamicVertexBuffer*)gRealloc(NULL, sizeof(DynamicVertexBuffer));
			vb->_data = &dvb._data[offset];
			vb->_size = num * decl._stride;
			vb->_startVertex = offset / decl._stride;
			vb->_stride = decl._stride;
			vb->_handle = dvb._handle;
			vb->_decl = declHandle;

			return vb;
		}

		VertexShaderHandle CreateVertexShader(const Memory* mem)
		{
			VertexShaderHandle handle = { _vertexShaderHandle.Alloc() };
			CommandBuffer& cmdbuf = GetCommandBuffer(CommandBuffer::eCreateVertexShader);
			cmdbuf.Write(handle);
			cmdbuf.Write(mem);
			return handle;
		}

		void DestroyVertexShader(VertexShaderHandle handle)
		{
			CommandBuffer& cmdbuf = GetCommandBuffer(CommandBuffer::eDestroyVertexShader);
			cmdbuf.Write(handle);
			_submit->Free(handle);
		}

		FragmentShaderHandle CreateFragmentShader(const Memory* mem)
		{
			FragmentShaderHandle handle = { _fragmentShaderHandle.Alloc() };
			CommandBuffer& cmdbuf = GetCommandBuffer(CommandBuffer::eCreateFragmentShader);
			cmdbuf.Write(handle);
			cmdbuf.Write(mem);
			return handle;
		}

		void DestroyFragmentShader(FragmentShaderHandle handle)
		{
			CommandBuffer& cmdbuf = GetCommandBuffer(CommandBuffer::eDestroyFragmentShader);
			cmdbuf.Write(handle);
			_submit->Free(handle);
		}

		SurfaceMaterialHandle CreateSurfaceMaterial(VertexShaderHandle vsh, FragmentShaderHandle fsh)
		{
			SurfaceMaterialHandle handle;
			// 			uint32 hash = _vsh.index<<16 | _fsh.index;
			// 
			// 			MaterialHandle handle = m_materialRef.find(hash);
			// 
			// 			if (video::invalidHandle != handle.index)
			// 			{
			// 				return handle;
			// 			}
			// 
			handle.index = _surfaceMaterialHandle.Alloc();
			// 			m_materialRef.add(handle, hash);

			CommandBuffer& cmdbuf = GetCommandBuffer(CommandBuffer::eCreateMaterial);
			cmdbuf.Write(handle);
			cmdbuf.Write(vsh);
			cmdbuf.Write(fsh);
			return handle;
		}

		void DestroySurfaceMaterial(SurfaceMaterialHandle handle)
		{
			CommandBuffer& cmdbuf = GetCommandBuffer(CommandBuffer::eDestroyMaterial);
			cmdbuf.Write(handle);
			_submit->Free(handle);
		}

		TextureHandle CreateTexture(const Memory* _mem, uint32 _flags, uint16* _width, uint16* _height)
		{
			if (nullptr != _width || nullptr!= _height)
			{
				int width = 0;
				int height = 0;

				//Dds dds;
				//if (parseDds(dds, _mem))
				//{
				//	width = dds._width;
				//	height = dds._height;
				//}

				//if (NULL != _width)
				//{
				//	*_width = (uint16)width;
				//}

				//if (NULL != _height)
				//{
				//	*_height = (uint16)height;
				//}
			}

			TextureHandle handle = { _textureHandle.Alloc() };
			CommandBuffer& cmdbuf = GetCommandBuffer(CommandBuffer::eCreateTexture);
			cmdbuf.Write(handle);
			cmdbuf.Write(_mem);
			cmdbuf.Write(_flags);
			return handle;
		}

		void DestroyTexture(TextureHandle _handle)
		{
			CommandBuffer& cmdbuf = GetCommandBuffer(CommandBuffer::eDestroyTexture);
			cmdbuf.Write(_handle);
			_submit->Free(_handle);
		}

		RenderTargetHandle CreateRenderTarget(uint16 width, uint16 height, uint32 flags)
		{
			RenderTargetHandle handle = { _renderTargetHandle.Alloc() };

			CommandBuffer& cmdbuf = GetCommandBuffer(CommandBuffer::eCreateRenderTarget);
			cmdbuf.Write(handle);
			cmdbuf.Write(width);
			cmdbuf.Write(height);
			cmdbuf.Write(flags);
			return handle;
		}

		void DestroyRenderTarget(RenderTargetHandle handle)
		{
			CommandBuffer& cmdbuf = GetCommandBuffer(CommandBuffer::eDestroyRenderTarget);
			cmdbuf.Write(handle);
			_submit->Free(handle);
		}

		UniformHandle CreateUniform(const char* name, ConstantType::Enum type, uint16 num)
		{
			Assert(PredefinedUniform::Count == NameToPredefinedUniformEnum(name));

			UniformHandle handle = { _uniformHandle.Alloc() };

			Constant& constant = _constant[handle.index];
			constant._type = type;
			constant._num = num;

			CommandBuffer& cmdbuf = GetCommandBuffer(CommandBuffer::eCreateUniform);
			cmdbuf.Write(handle);
			cmdbuf.Write(type);
			cmdbuf.Write(num);
			uint8 len = (uint8)strlen(name);
			cmdbuf.Write(len);
			cmdbuf.Write(name, len);
			return handle;
		}

		void DestroyUniform(UniformHandle handle)
		{
			CommandBuffer& cmdbuf = GetCommandBuffer(CommandBuffer::eDestroyUniform);
			cmdbuf.Write(handle);
			_submit->Free(handle);
		}

		void SaveScreenShot(const Memory* _mem)
		{
			CommandBuffer& cmdbuf = GetCommandBuffer(CommandBuffer::eSaveScreenShot);
			cmdbuf.Write(_mem);
		}

		void SetUniform(UniformHandle handle, const void* value, uint16 num)
		{
			Constant& constant = _constant[handle.index];
			Assert(constant._num >= num);
			//BX_CHECK(, "Truncated uniform update. %d (max: %d)", _num, constant._num);
			_submit->WriteConstant(constant._type, handle, value, Uint16Min(constant._num, num));
		}

		void SetUniform(SurfaceMaterialHandle material, UniformHandle handle, const void* value)
		{
			Assert(false); //Not implemented
		}

		void SetViewRect(uint8 id, uint16 x, uint16 y, uint16 width, uint16 height)
		{
			Rect& rect = _rect[id];
			rect._x = x;
			rect._y = y;
			rect._width = width;
			rect._height = height;
		}

		void SetViewRectMask(uint32 viewMask, uint16 x, uint16 y, uint16 width, uint16 height)
		{
			for (uint32 id = 0, iviewMask = viewMask, ntz = Uint32BitScan(viewMask); 
				0 != iviewMask; 
				iviewMask >>= 1, id += 1, ntz = Uint32BitScan(iviewMask))
			{
				viewMask >>= ntz;
				id += ntz;

				SetViewRect(id, x, y, width, height);
			}
		}

		void SetViewClear(uint8 id, uint8 flags, uint32 rgba, float depth, uint8 stencil)
		{
			Clear& clear = _clear[id];
			clear._flags = flags;
			clear._rgba = rgba;
			clear._depth = depth;
			clear._stencil = stencil;
		}

		void SetViewClearMask(uint32 viewMask, uint8 flags, uint32 rgba, float depth, uint8 stencil)
		{
			for (uint32 id = 0, iviewMask = viewMask, ntz = Uint32BitScan(viewMask); 
				0 != iviewMask; 
				iviewMask >>= 1, id += 1, ntz = Uint32BitScan(iviewMask))
			{
				iviewMask >>= ntz;
				id += ntz;

				SetViewClear(id, flags, rgba, depth, stencil);
			}
		}

		void SetViewSeq(uint8 id, bool enabled)
		{
			_seqMask[id] = enabled ? 0xffff : 0x0;
		}

		void SetViewSeqMask(uint32 viewMask, bool enabled)
		{
			uint16 mask = enabled ? 0xffff : 0x0;
			for (uint32 id = 0, iviewMask = viewMask, ntz = Uint32BitScan(viewMask); 
				0 != iviewMask; 
				iviewMask >>= 1, id += 1, ntz = Uint32BitScan(iviewMask))
			{
				iviewMask >>= ntz;
				id += ntz;

				_seqMask[id] = mask;
			}
		}

		void SetViewRenderTarget(uint8 id, RenderTargetHandle handle)
		{
			_renderTarget[id] = handle;
		}

		void SetViewRenderTargetMask(uint32 viewMask, RenderTargetHandle handle)
		{
			for (uint32 id = 0, iviewMask = viewMask, ntz = Uint32BitScan(viewMask); 
				0 != iviewMask; 
				iviewMask >>= 1, id += 1, ntz = Uint32BitScan(iviewMask))
			{
				iviewMask >>= ntz;
				id += ntz;

				_renderTarget[id] = handle;
			}
		}

		void DumpViewStats()
		{
			#if 0 // BGFX_DEBUG
			for (uint8 view = 0; view < BGFX_CONFIG_MAX_VIEWS; ++view)
			{
				if (0 < m_seq[view])
				{
					BX_TRACE("%d: %d", view, m_seq[view]);
				}
			}
			#endif // BGFX_DEBUG
		}

		void FreeAllHandles(Frame* _frame)
		{
			for (uint16 i = 0, num = _frame->_numFreeIndexBufferHandles; i < num; ++i)
			{
				_indexBufferHandle.Free(_frame->_freeIndexBufferHandle[i].index);
			}

			for (uint16 i = 0, num = _frame->_numFreeVertexDeclHandles; i < num; ++i)
			{
				_vertexDeclHandle.Free(_frame->_freeVertexDeclHandle[i].index);
			}

			for (uint16 i = 0, num = _frame->_numFreeVertexBufferHandles; i < num; ++i)
			{
				_vertexBufferHandle.Free(_frame->_freeVertexBufferHandle[i].index);
			}

			for (uint16 i = 0, num = _frame->_numFreeVertexShaderHandles; i < num; ++i)
			{
				_vertexShaderHandle.Free(_frame->_freeVertexShaderHandle[i].index);
			}

			for (uint16 i = 0, num = _frame->_numFreeFragmentShaderHandles; i < num; ++i)
			{
				_fragmentShaderHandle.Free(_frame->_freeFragmentShaderHandle[i].index);
			}

			for (uint16 i = 0, num = _frame->_numFreeSurfaceMaterialHandles; i < num; ++i)
			{
				_surfaceMaterialHandle.Free(_frame->_freeSurfaceMaterialHandle[i].index);
			}

			for (uint16 i = 0, num = _frame->_numFreeTextureHandles; i < num; ++i)
			{
				_textureHandle.Free(_frame->_freeTextureHandle[i].index);
			}

			for (uint16 i = 0, num = _frame->_numFreeRenderTargetHandles; i < num; ++i)
			{
				_renderTargetHandle.Free(_frame->_freeRenderTargetHandle[i].index);
			}

			for (uint16 i = 0, num = _frame->_numFreeUniformHandles; i < num; ++i)
			{
				_uniformHandle.Free(_frame->_freeUniformHandle[i].index);
			}
		}

		void Swap()
		{
			_submit->_resolution = _resolution;
			_submit->_debug = _debug;
			memcpy(_submit->_freeRenderTargetHandle, _renderTarget, sizeof(_renderTarget));
			memcpy(_submit->_clear, _clear, sizeof(_clear));
			memcpy(_submit->_rect, _rect, sizeof(_rect));
			_submit->Finish();

			DumpViewStats();

			FreeAllHandles(_render);

			memset(_seq, 0, sizeof(_seq));
			Frame* temp = _render;
			_render = _submit;
			_submit = temp;
			_frames++;
			_submit->Reset();
		}

		void Flip()
		{
			//sContext
		}

		// render thread
		void RenderFrame()
		{
			#if BX_PLATFORM_NACL
			// on NaCl swap buffers generates callback and this is handled inside callback
			#else
			if (_rendererInitialized)
			{
				Flip();
			}
			#endif // BX_PLATFORM_

			GameSemWait();

			RendererExecCommands(_render->_cmdPre);
			if (_rendererInitialized)
			{
				RendererSubmit();
			}
			RendererExecCommands(_render->_cmdPost);

			RenderSemPost();
		}

		void RendererInit();
		void RendererShutdown();
		void RendererCreateIndexBuffer(IndexBufferHandle handle, Memory* mem);
		void RendererDestroyIndexBuffer(IndexBufferHandle handle);
		void RendererCreateDynamicIndexBuffer(IndexBufferHandle handle, uint32 size);
		void RendererDestroyDynamicIndexBuffer(IndexBufferHandle handle);
		void RendererCreateVertexBuffer(VertexBufferHandle handle, Memory* mem, VertexDeclHandle declHandle);
		void RendererDestroyVertexBuffer(VertexBufferHandle handle);
		void RendererCreateDynamicVertexBuffer(VertexBufferHandle handle, uint32 size);
		void RendererDestroyDynamicVertexBuffer(VertexBufferHandle handle);
		void RendererCreateVertexDecl(VertexDeclHandle handle, const VertexDecl& decl);
		void RendererDestroyVertexDecl(VertexDeclHandle handle);
		void RendererCreateVertexShader(VertexShaderHandle handle, Memory* mem);
		void RendererDestroyVertexShader(VertexShaderHandle handle);
		void RendererCreateFragmentShader(FragmentShaderHandle handle, Memory* mem);
		void RendererDestroyFragmentShader(FragmentShaderHandle handle);
		void RendererCreateMaterial(SurfaceMaterialHandle handle, 
				VertexShaderHandle vsh, FragmentShaderHandle fsh);
		void RendererDestroyMaterial(FragmentShaderHandle handle);
		void RendererCreateTexture(TextureHandle handle, Memory* mem, uint32 flags);
		void RendererDestroyTexture(TextureHandle handle);
		void RendererCreateRenderTarget(RenderTargetHandle handle, uint16 width, uint16 height, uint32 flags);
		void RendererDestroyRenderTarget(RenderTargetHandle handle);
		void RendererCreateUniform(UniformHandle handle, ConstantType::Enum type, uint16 num, const char* name);
		void RendererDestroyUniform(UniformHandle handle);
		void RendererSaveScreenShot(Memory* mem);
		void RendererUpdateUniform(uint16 loc, const void* data, uint32 size);
		void RendererSubmit();

		void RendererUpdateUniforms(ConstantBuffer* constantBuffer, uint32 begin, uint32 end)
		{
			constantBuffer->Reset(begin);
			while (constantBuffer->GetPos() < end)
			{
				uint32 opcode = constantBuffer->Read();

				if (ConstantType::eEnd == opcode)
				{
					break;
				}

				ConstantType::Enum type;
				uint16 loc;
				uint16 num;
				uint16 copy;
				ConstantBuffer::DecodeOpcode(opcode, type, loc, num, copy);

				const char* data;
				uint32 size = gConstantTypeSize[type] * num;
				data = constantBuffer->Read(size);
				RendererUpdateUniform(loc, data, size);
			}
		}

		void RendererExecCommands(CommandBuffer& cmdbuf)
		{
			cmdbuf.Reset();

			bool end = false;

			do
			{
				uint8 command;
				cmdbuf.Read(command);

				switch (command)
				{
				case CommandBuffer::eRendererInit:
				{
					RendererInit();
					_rendererInitialized = true;
				}
				break;

				case CommandBuffer::eRendererShutdown:
				{
					RendererShutdown();
					_rendererInitialized = false;
				}
				break;

				case CommandBuffer::eCreateIndexBuffer:
				{
					IndexBufferHandle handle;
					cmdbuf.Read(handle);

					Memory* mem;
					cmdbuf.Read(mem);

					RendererCreateIndexBuffer(handle, mem);

					free(mem);
				}
				break;

				case CommandBuffer::eDestroyIndexBuffer:
				{
					IndexBufferHandle handle;
					cmdbuf.Read(handle);

					RendererDestroyIndexBuffer(handle);
				}
				break;

				case CommandBuffer::eCreateDynamicIndexBuffer:
				{
					IndexBufferHandle handle;
					cmdbuf.Read(handle);

					uint32 size;
					cmdbuf.Read(size);

					RendererCreateDynamicIndexBuffer(handle, size);
				}
				break;

				case CommandBuffer::eDestroyDynamicIndexBuffer:
				{
					IndexBufferHandle handle;
					cmdbuf.Read(handle);

					RendererDestroyDynamicIndexBuffer(handle);
				}
				break;

				case CommandBuffer::eCreateVertexDecl:
				{
					VertexDeclHandle handle;
					cmdbuf.Read(handle);

					VertexDecl decl;
					cmdbuf.Read(decl);

					RendererCreateVertexDecl(handle, decl);
				}
				break;

				case CommandBuffer::eDestroyVertexDecl:
				{
					VertexDeclHandle handle;
					cmdbuf.Read(handle);

					RendererDestroyVertexDecl(handle);
				}
				break;

				case CommandBuffer::eCreateVertexBuffer:
				{
					VertexBufferHandle handle;
					cmdbuf.Read(handle);

					Memory* mem;
					cmdbuf.Read(mem);

					VertexDeclHandle declHandle;
					cmdbuf.Read(declHandle);

					RendererCreateVertexBuffer(handle, mem, declHandle);

					free(mem);
				}
				break;

				case CommandBuffer::eDestroyVertexBuffer:
				{
					VertexBufferHandle handle;
					cmdbuf.Read(handle);

					RendererDestroyVertexBuffer(handle);
				}
				break;

				case CommandBuffer::eCreateDynamicVertexBuffer:
				{
					VertexBufferHandle handle;
					cmdbuf.Read(handle);

					uint32 size;
					cmdbuf.Read(size);

					RendererCreateDynamicVertexBuffer(handle, size);
				}
				break;

				case CommandBuffer::eDestroyDynamicVertexBuffer:
				{
					VertexBufferHandle handle;
					cmdbuf.Read(handle);

					RendererDestroyDynamicVertexBuffer(handle);
				}
				break;

				case CommandBuffer::eCreateVertexShader:
				{
					VertexShaderHandle handle;
					cmdbuf.Read(handle);

					Memory* mem;
					cmdbuf.Read(mem);

					RendererCreateVertexShader(handle, mem);

					free(mem);
				}
				break;

				case CommandBuffer::eDestroyVertexShader:
				{
					VertexShaderHandle handle;
					cmdbuf.Read(handle);

					RendererDestroyVertexShader(handle);
				}
				break;

				case CommandBuffer::eCreateFragmentShader:
				{
					FragmentShaderHandle handle;
					cmdbuf.Read(handle);

					Memory* mem;
					cmdbuf.Read(mem);

					RendererCreateFragmentShader(handle, mem);

					free(mem);
				}
				break;

				case CommandBuffer::eDestroyFragmentShader:
				{
					FragmentShaderHandle handle;
					cmdbuf.Read(handle);

					RendererDestroyFragmentShader(handle);
				}
				break;

				case CommandBuffer::eCreateMaterial:
				{
					SurfaceMaterialHandle handle;
					cmdbuf.Read(handle);

					VertexShaderHandle vsh;
					cmdbuf.Read(vsh);

					FragmentShaderHandle fsh;
					cmdbuf.Read(fsh);

					RendererCreateMaterial(handle, vsh, fsh);
				}
				break;

				case CommandBuffer::eDestroyMaterial:
				{
					FragmentShaderHandle handle;
					cmdbuf.Read(handle);

					RendererDestroyMaterial(handle);
				}
				break;

				case CommandBuffer::eCreateTexture:
				{
					TextureHandle handle;
					cmdbuf.Read(handle);

					Memory* mem;
					cmdbuf.Read(mem);

					uint32 flags;
					cmdbuf.Read(flags);

					RendererCreateTexture(handle, mem, flags);

					free(mem);
				}
				break;

				case CommandBuffer::eDestroyTexture:
				{
					TextureHandle handle;
					cmdbuf.Read(handle);

					RendererDestroyTexture(handle);
				}
				break;

				case CommandBuffer::eCreateRenderTarget:
				{
					RenderTargetHandle handle;
					cmdbuf.Read(handle);

					uint16 width;
					cmdbuf.Read(width);

					uint16 height;
					cmdbuf.Read(height);

					uint32 flags;
					cmdbuf.Read(flags);

					RendererCreateRenderTarget(handle, width, height, flags);
				}
				break;

				case CommandBuffer::eDestroyRenderTarget:
				{
					RenderTargetHandle handle;
					cmdbuf.Read(handle);

					RendererDestroyRenderTarget(handle);
				}
				break;

				case CommandBuffer::eCreateUniform:
				{
					UniformHandle handle;
					cmdbuf.Read(handle);

					ConstantType::Enum type;
					cmdbuf.Read(type);

					uint16 num;
					cmdbuf.Read(num);

					uint8 len;
					cmdbuf.Read(len);

					char name[256];
					cmdbuf.Read(name, len);
					name[len] = '\0';

					RendererCreateUniform(handle, type, num, name);
				}
				break;

				case CommandBuffer::eDestroyUniform:
				{
					UniformHandle handle;
					cmdbuf.Read(handle);

					RendererDestroyUniform(handle);
				}
				break;

				case CommandBuffer::eSaveScreenShot:
				{
					Memory* mem;
					cmdbuf.Read(mem);

					RendererSaveScreenShot(mem);

					free(mem);
				}
				break;

				case CommandBuffer::eEnd:
					end = true;
					break;

				default:
					Assert(false); // ???
					break;
				}
			} while (!end);
		}


		#if VIDEO_CONFIG_MULTITHREADED
		void gameSemPost()
		{
			// 			BX_TRACE("game post");
			m_gameSem.post();
		}

		void gameSemWait()
		{
			// 			BX_TRACE("game wait");
			int64_t start = bx::getHPCounter();
			m_gameSem.wait();
			_render->m_waitSubmit = bx::getHPCounter() - start;
		}

		void renderSemPost()
		{
			// 			BX_TRACE("render post");
			m_renderSem.post();
		}

		void renderSemWait()
		{
			// 			BX_TRACE("render wait");
			int64_t start = bx::getHPCounter();
			m_renderSem.wait();
			_submit->m_waitRender = bx::getHPCounter() - start;
		}

		Semaphore m_renderSem;
		Semaphore m_gameSem;
		#else
		void GameSemPost()
		{
		}

		void GameSemWait()
		{
		}

		void RenderSemPost()
		{
		}

		void RenderSemWait()
		{
		}
		#endif // BGFX_CONFIG_MULTITHREADED

		Frame _frame[2];
		Frame* _render;
		Frame* _submit;

		uint64 _tempKeys[VIDEO_MAX_DRAW_CALLS];
		uint16 _tempValues[VIDEO_MAX_DRAW_CALLS];

		HandleAlloc _indexBufferHandle;
		HandleAlloc _vertexDeclHandle;
		HandleAlloc _vertexBufferHandle;
		HandleAlloc _vertexShaderHandle;
		HandleAlloc _fragmentShaderHandle;
		HandleAlloc _surfaceMaterialHandle;
		HandleAlloc _textureHandle;
		HandleAlloc _renderTargetHandle;
		HandleAlloc _uniformHandle;

		SurfaceMaterialRef _surfaceMaterialRef;
		VertexDeclRef _declRef;

		RenderTargetHandle _renderTarget[VIDEO_MAX_VIEWS];
		Clear _clear[VIDEO_MAX_VIEWS];
		Rect _rect[VIDEO_MAX_VIEWS];
		Constant _constant[VIDEO_MAX_UNIFORMS];
		uint16 _seq[VIDEO_MAX_VIEWS];
		uint16 _seqMask[VIDEO_MAX_VIEWS];

		Resolution _resolution;
		uint32 _frames;
		uint32 _debug;

		#if VIDEO_PLATFORM_WINDOWS
		struct Window
		{
			Window()
				: m_frame(true)
			{
			}

			void init()
			{
				if (NULL == bgfxHwnd)
				{
					bgfxHwnd = CreateWindow("EDIT"
						, NULL
						, WS_OVERLAPPEDWINDOW | WS_VISIBLE
						, 0
						, 0
						, BGFX_DEFAULT_WIDTH
						, BGFX_DEFAULT_HEIGHT
						, 0
						, 0
						, 0
						, 0
					);
				}
			}

			void adjust(uint32 _width, uint32 _height, bool _windowFrame)
			{

				ShowWindow(bgfxHwnd, SW_SHOWNORMAL);
				RECT rect;
				RECT newrect = { 0, 0, (LONG)_width, (LONG)_height };
				DWORD style = WS_POPUP | WS_SYSMENU;

				if (m_frame)
				{
					GetWindowRect(bgfxHwnd, &_rect);
					m_style = GetWindowLong(bgfxHwnd, GWL_STYLE);
				}

				if (_windowFrame)
				{
					rect = _rect;
					style = m_style;
				}
				else
				{
					#if defined(__MINGW32__)
					rect = _rect;
					style = m_style;
					#else
					HMONITOR monitor = MonitorFromWindow(bgfxHwnd, MONITOR_DEFAULTTONEAREST);
					MONITORINFO mi;
					mi.cbSize = sizeof(mi);
					GetMonitorInfo(monitor, &mi);
					newrect = mi.rcMonitor;
					rect = mi.rcMonitor;
					#endif // !defined(__MINGW__)
				}

				SetWindowLong(bgfxHwnd, GWL_STYLE, style);
				AdjustWindowRect(&newrect, style, FALSE);
				UpdateWindow(bgfxHwnd);

				if (rect.left == -32000
					|| rect.top == -32000)
				{
					rect.left = 0;
					rect.top = 0;
				}

				SetWindowPos(bgfxHwnd
					, HWND_TOP
					, rect.left
					, rect.top
					, (newrect.right - newrect.left)
					, (newrect.bottom - newrect.top)
					, SWP_SHOWWINDOW
				);

				ShowWindow(bgfxHwnd, SW_RESTORE);

				m_frame = _windowFrame;
			}

			RECT _rect;
			DWORD m_style;
			bool m_frame;
		};

		Window m_window;
		#endif // BX_PLATFORM_WINDOWS

		bool _initialized;
		bool _rendererInitialized;
	};
}
#endif

