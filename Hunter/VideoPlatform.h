#ifndef VIDEO_PLATFORM_H
#define VIDEO_PLATFORM_H

#include "stdafx.h"

#include "Video.h"
#include <inttypes.h>
#include <stdarg.h>

#include <unordered_map>


	#define VIDEO_RENDERER_DIRECT3D (BX_PLATFORM_WINDOWS|BX_PLATFORM_XBOX360)
	#define VIDEO_MULTITHREADED  \
		( (BX_PLATFORM_WINDOWS|BX_PLATFORM_XBOX360|BX_PLATFORM_NACL|BX_PLATFORM_LINUX)&(!BGFX_CONFIG_RENDERER_NULL) )
	#define VIDEO_MAX_DRAW_CALLS (8<<10)
	#define VIDEO_MAX_MATRIX_CACHE (16<<10)
	#define VIDEO_MAX_VIEWS 32
	#define VIDEO_MAX_INDEX_BUFFERS (4<<10)
	#define VIDEO_MAX_VERTEX_DECLS 64
	#define VIDEO_MAX_VERTEX_BUFFERS (4<<10)
	#define VIDEO_MAX_EFFECT 256
	#define VIDEO_MAX_MATERIALS 512
	#define VIDEO_MAX_TEXTURES (4<<10)
	#define VIDEO_MAX_RENDER_TARGETS 64
	#define VIDEO_MAX_UNIFORMS 512
	#define VIDEO_MAX_COMMAND_BUFFER_SIZE (64<<10)
	#define VIDEO_DYNAMIC_VERTEX_BUFFER_SIZE (6<<20)
	#define VIDEO_DYNAMIC_INDEX_BUFFER_SIZE (2<<20)
	#define MAX_CONSTANT_BUFFER_SIZE (512<<10)


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

	extern const uint32_t gConstantTypeSize[ConstantType::Count];
	extern ReallocFunction gRealloc;
	extern FreeFunction gFree;
	extern void Free(Memory* _mem);
	//extern void saveTga(const char* _filePath, uint32_t _width, uint32_t _height, uint32_t _pitch, const void* _data);
	extern const char* GetAttribName(Attrib::Enum _attr);


	inline uint32_t uint16_min(uint16_t _a, uint16_t _b)
	{
		return _a > _b ? _b : _a;
	}

	inline uint32_t hash(const void* _data, uint32_t _size)
	{
		HashMurmur2A murmur;
		murmur.begin();
		murmur.add(_data, (int)_size);
		return murmur.end();
	}

	inline uint32_t gcd(uint32_t _a, uint32_t _b)
	{
		do
		{
			uint32_t tmp = _a % _b;
			_a = _b;
			_b = tmp;
		} while (_b);

		return _a;
	}

	inline uint32_t lcm(uint32_t _a, uint32_t _b)
	{
		return _a * (_b / gcd(_a, _b));
	}

	inline uint32_t strideAlign(uint32_t _offset, uint32_t _stride)
	{
		const uint32_t mod = uint32_mod(_offset, _stride);
		const uint32_t add = uint32_sub(_stride, mod);
		const uint32_t mask = uint32_cmpeq(mod, 0);
		const uint32_t tmp = uint32_selb(mask, 0, add);
		const uint32_t result = uint32_add(_offset, tmp);

		return result;
	}

	inline uint32_t strideAlign16(uint32_t _offset, uint32_t _stride)
	{
		uint32_t align = lcm(16, _stride);
		return _offset + align - (_offset%align);
	}

	inline uint32_t strideAlign256(uint32_t _offset, uint32_t _stride)
	{
		uint32_t align = lcm(256, _stride);
		return _offset + align - (_offset%align);
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

		void Resize(bool sm = false, uint16_t width = VIDEO_DEFAULT_WIDTH, uint16 _height = BGFX_DEFAULT_HEIGHT)
		{
			_small = sm;
			_width = uint32_max(1, _width / 8);
			_height = uint32_max(1, _height / (_small ? 8 : 16));
			_size = _width * _height * 2;

			_mem = (uint8_t*)g_realloc(_mem, _size);

			Clear();
		}

		void Clear()
		{
			memset(_mem, 0, _size);
		}

		void PrintfVargs(uint16_t x, uint16_t y, uint8_t attr, const char* format, va_list argList)
		{
			if (x < _width && y < _height)
			{
				char* temp = (char*)alloca(_width);

				uint32_t num = vsnprintf(temp, _width, format, argList);

				uint8_t* mem = &_mem[(y * _width + x) * 2];
				for (uint32_t i = 0, xx = x; i < num && xx < _width; ++i, ++xx)
				{
					mem[0] = temp[i];
					mem[1] = attr;
					mem += 2;
				}
			}
		}

		void Printf(uint16_t x, uint16_t y, uint8_t attr, const char* format, ...)
		{
			va_list argList;
			va_start(argList, format);
			PrintfVargs(x, y, attr, format, argList);
			va_end(argList);
		}

		uint8_t* _mem;
		uint32_t _size;
		uint16_t _width;
		uint16_t _height;
		bool _small;
	};

	struct TextVideoMemBlitter
	{
		void Init();
		void Blit(const TextVideoMem& _mem);
		void Setup();
		void Render(uint32_t _numIndices);

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
			eCount
		};

		uint8 _type;
		uint16 _loc;
	};
	PredefinedUniform::Enum NameToPredefinedUniformEnum(const char* name);

	class StreamRead
	{
	public:
		StreamRead(const void* _data, uint32_t _size)
			: m_data((uint8_t*)_data)
			, m_size(_size)
			, m_pos(0)
		{
		}

		~StreamRead()
		{
		}

		void skip(uint32_t _size)
		{
			BX_CHECK(m_size - m_pos >= _size, "Available %d, requested %d.", m_size - m_pos, _size);
			m_pos += _size;
		}

		void read(void* _data, uint32_t _size)
		{
			BX_CHECK(m_size - m_pos >= _size, "Available %d, requested %d.", m_size - m_pos, _size);
			memcpy(_data, &m_data[m_pos], _size);
			m_pos += _size;
		}

		template<typename Ty>
		void read(Ty& _value)
		{
			read(&_value, sizeof(Ty));
		}

		const uint8_t* getDataPtr() const
		{
			return &m_data[m_pos];
		}

		uint32_t getPos() const
		{
			return m_pos;
		}

		void align(uint16_t _align)
		{
			m_pos = strideAlign(m_pos, _align);
		}

	private:
		const uint8_t* m_data;
		uint32_t m_size;
		uint32_t m_pos;
	};

	class StreamWrite
	{
	public:
		StreamWrite(void* _data, uint32_t _size)
			: m_data((uint8_t*)_data)
			, m_size(_size)
			, m_pos(0)
		{
		}

		~StreamWrite()
		{
		}

		void write(void* _data, uint32_t _size)
		{
			BX_CHECK(m_size - m_pos >= _size, "Write out of bounds. Available %d, requested %d.", m_size - m_pos, _size);
			memcpy(&m_data[m_pos], _data, _size);
			m_pos += _size;
		}

		template<typename Ty>
		void write(Ty& _value)
		{
			write(&_value, sizeof(Ty));
		}

		uint8_t* getDataPtr() const
		{
			return &m_data[m_pos];
		}

		uint32_t getPos() const
		{
			return m_pos;
		}

		void align(uint16_t _align)
		{
			m_pos = strideAlign(m_pos, _align);
		}

	private:
		uint8_t* m_data;
		uint32_t m_size;
		uint32_t m_pos;
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
			_pos += _size;
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

		void Eecode(uint64 key)
		{
			_depth = key & 0xffffffff;
			_material = (key >> 0x18)&(VIDEO_MAX_MATERIALS - 1);
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

				uint32 num = min(VIDEO_MAX_MATRIX_CACHE - _num, num);
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
		uint16 _idx;
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

	#define BGFX_UNIFORM_FUNCTIONBIT UINT8_C(0x40)
	#define BGFX_UNIFORM_FRAGMENTBIT UINT8_C(0x80)
	#define BGFX_UNIFORM_TYPEMASK    UINT8_C(0x3f)

	class ConstantBuffer
	{
	public:
		static ConstantBuffer* Create(uint32_t _size)
		{
			uint32_t size = BX_ALIGN_16(uint32_max(_size, sizeof(ConstantBuffer)));
			void* data = g_realloc(NULL, size);
			return ::new(data) ConstantBuffer(_size);
		}

		static void Destroy(ConstantBuffer* _constantBuffer)
		{
			_constantBuffer->~ConstantBuffer();
			g_free(_constantBuffer);
		}

		static uint32_t EncodeOpcode(ConstantType::Enum _type, uint16_t _loc, uint16_t _num, uint16_t _copy)
		{
			uint32_t opcode = 0;

			opcode <<= CONSTANT_OPCODE_TYPE_BITS;
			opcode |= _type&CONSTANT_OPCODE_TYPE_MASK;

			opcode <<= CONSTANT_OPCODE_LOC_BITS;
			opcode |= _loc&CONSTANT_OPCODE_LOC_MASK;

			opcode <<= CONSTANT_OPCODE_NUM_BITS;
			opcode |= _num&CONSTANT_OPCODE_NUM_MASK;

			opcode <<= CONSTANT_OPCODE_COPY_BITS;
			opcode |= _copy&CONSTANT_OPCODE_COPY_MASK;

			return opcode;
		}

		static void DecodeOpcode(uint32_t _opcode, ConstantType::Enum& _type, uint16_t& _loc, uint16_t& _num, uint16_t& _copy)
		{
			uint32_t copy;
			uint32_t num;
			uint32_t loc;

			copy = _opcode&CONSTANT_OPCODE_COPY_MASK;
			_opcode >>= CONSTANT_OPCODE_COPY_BITS;

			num = _opcode&CONSTANT_OPCODE_NUM_MASK;
			_opcode >>= CONSTANT_OPCODE_NUM_BITS;

			loc = _opcode&CONSTANT_OPCODE_LOC_MASK;
			_opcode >>= CONSTANT_OPCODE_LOC_BITS;

			_type = (ConstantType::Enum)(_opcode&CONSTANT_OPCODE_TYPE_MASK);
			_opcode >>= CONSTANT_OPCODE_TYPE_BITS;

			_copy = (uint16_t)copy;
			_num = (uint16_t)num;
			_loc = (uint16_t)loc;
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

		void WriteUniform(ConstantType::Enum _type, uint16_t _loc, const void* _value, uint16_t _num = 1);
		void WriteUniformRef(ConstantType::Enum _type, uint16_t _loc, const void* _value, uint16_t _num = 1);
		void Commit(bool _force);

	private:
		ConstantBuffer(uint32 _size)
			: _size(_size - sizeof(_buffer))
			, _pos(0)
		{
			BX_TRACE("ConstantBuffer %d, %d", _size, m_size);
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

				std::pair<UniformHashMap::iterator, bool> result = _uniforms.insert(UniformHashMap::value_type(_name, info) );
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
				_sampler[i]._idx = video::invalidHandle;
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
		VIDEO_CACHE_LINE_ALIGN_MARKER();

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
			for (uint32 id = 0, viewMask = viewMask, ntz = uint32_cnttz(_viewMask); 0 != viewMask; viewMask >>= 1, id += 1, ntz = uint32_cnttz(viewMask))
			{
				viewMask >>= ntz;
				id += ntz;

				SetViewTransform(id, _view, proj, other);
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

		void SetIndexBuffer(const DynamicIndexBuffer* ib, uint32 numIndices)
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

		void SetMaterial(SurfaceMaterialHandle handle)
		{
			Assert(invalidHandle = handle.index);
			_key._material = handle.index;
		}

		void SetTexture(uint8 stage, UniformHandle uniformSampler, TextureHandle textureHandle)
		{
			_flags |= VIDEO_STATE_TEX0 << _stage;
			Sampler& sampler = _state._sampler[_stage];
			sampler.index = handle.index;
			sampler._flags = VIDEO_SAMPLER_NONE;

			if (video::invalidHandle != sampler.idx)
			{
				uint32 stage = _stage;
				SetUniform(_sampler, &stage);
			}
		}

		void SetTexture(uint8 stage, UniformHandle uniformSampler, RenderTargetHandle handle, bool depth)
		{
			_flags |= VIDEO_STATE_TEX0 << stage;
			Sampler& sampler = _state._sampler[_stage];
			sampler._idx = handle.idx;
			sampler._flags = _depth ? BGFX_SAMPLER_RENDERTARGET_DEPTH : BGFX_SAMPLER_RENDERTARGET_COLOR;

			if (video::invalidHandle != _sampler.idx)
			{
				uint32 stage = _stage;
				SetUniform(_sampler, &stage);
			}
		}

		void Submit(uint8 id);
		void SubmitMask(uint32 viewMask);
		void Sort();

		bool CheckAvailDynamicIndexBuffer(uint16 num)
		{
			uint32 offset = _iboffset;
			uint32 iboffset = offset + _num * sizeof(uint16);
			iboffset = uint32_min(iboffset, VIDEO_DYNAMIC_INDEX_BUFFER_SIZE);
			uint32 num = (iboffset - offset) / sizeof(uint16);
			return num == _num;
		}

		uint32 AllocDynamicIndexBuffer(uint16& _num)
		{
			uint32 offset = _iboffset;
			_iboffset = offset + _num * sizeof(uint16);
			_iboffset = uint32_min(_iboffset, VIDEO_DYNAMIC_INDEX_BUFFER_SIZE);
			_num = uint16((_iboffset - offset) / sizeof(uint16));
			return offset;
		}

		bool CheckAvailDynamicVertexBuffer(uint16 num, uint16 stride)
		{
			uint32 offset = strideAlign(_vboffset, stride);
			uint32 vboffset = offset + num * stride;
			vboffset = uint32_min(vboffset, VIDEO_DYNAMIC_VERTEX_BUFFER_SIZE);
			uint32 num = (vboffset - offset) / stride;
			return num == num;
		}

		uint32 AllocDynamicVertexBuffer(uint16& num, uint16 stride)
		{
			uint32 offset = strideAlign(_vboffset, stride);
			_vboffset = offset + _num * stride;
			_vboffset = uint32_min(_vboffset, VIDEO_DYNAMIC_VERTEX_BUFFER_SIZE);
			_num = uint16((_vboffset - offset) / stride);
			return offset;
		}

		void WriteConstant(ConstantType::Enum _type, UniformHandle _handle, const void* _value, uint16 _num)
		{
			_constantBuffer->WriteUniform(_type, _handle.index, _value, _num);
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
			_freeFragmentShaderHandle[m_numFreeFragmentShaderHandles] = handle;
			++_numFreeFragmentShaderHandles;
		}

		void Free(SurfaceMaterialHandle handle)
		{
			_freeMaterialHandle[_numFreeMaterialHandles] = handle;
			++_numFreeMaterialHandles;
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
			_numFreeEffectHandles = 0;
			_numFreeSurfaceMaterialHandles = 0;
			_numFreeTextureHandles = 0;
			_numFreeRenderTargetHandles = 0;
			_numFreeUniformHandles = 0;
		}

		SortKey _key;

		RenderTargetHandle _renderTargets[VIDEO_MAX_VIEWS];
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
		uint16 _numFreeEffectHandles;
		uint16 _numFreeSurfaceMaterialHandles;
		uint16 _numFreeTextureHandles;
		uint16 _numFreeRenderTargetHandles;
		uint16 _numFreeUniformHandles;

		IndexBufferHandle _freeIndexBufferHandle[VIDEO_MAX_INDEX_BUFFERS];
		VertexDeclHandle _freeVertexDeclHandle[VIDEO_MAX_VERTEX_DECLS];
		VertexBufferHandle _freeVertexBufferHandle[VIDEO_MAX_VERTEX_BUFFERS];
		VertexShaderHandle _freeVertexShaderHandle[VIDEO_MAX_VERTEXSHADERS]
		FragmentShaderHandle _freeFragmentShaderHandle[VIDEO_MAX_FRAGMENTSHADERS]
		SurfaceMaterialHandle _freeMaterialHandle[VIDEO_MAX_MATERIALS];
		TextureHandle _freeTextureHandle[VIDEO_MAX_TEXTURES];
		RenderTargetHandle _freeRenderTargetHandle[VIDEO_MAX_RENDER_TARGETS];
		UniformHandle _freeUniformHandle[VIDEO_MAX_UNIFORMS];

		int64 _waitSubmit;
		int64 _waitRender;

		bool _discard;
	};

	struct Context
	{
		Context()
			: _render(&_frame[0]),
			_submit(&_frame[1]),
			_indexBufferHandle(VIDEO_MAX_INDEX_BUFFERS),
			_vertexDeclHandle(VIDEO_MAX_VERTEX_DECLS),
			_vertexShaderHandle(),
			_fragmentShaderHandle,
			_effectHandle(VIDEO_MAX_EFFECTS),
			_materialHandle(VIDEO_MAX_MATERIALS),
			_textureHandle(VIDEO_MAX_TEXTURES),
			_renderTargetHandle(VIDEO_MAX_RENDER_TARGETS),
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

		void Frame()
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
			_resolution._flags = flags&(~VIDEO_RESET_FULLSCREEN_FAKE);

			memset(_rt, 0xff, sizeof(m_rt));

			#if BX_PLATFORM_WINDOWS
			uint32 fullscreen = (_flags&BGFX_RESET_FULLSCREEN_MASK) >> BGFX_RESET_FULLSCREEN_SHIFT;
			m_window.adjust(_width, _height, BGFX_RESET_FULLSCREEN_FAKE != fullscreen);
			#endif // BX_PLATFORM_WINDOWS
		}

		void DEBUG_TextClear(uint8_t _attr, bool _small)
		{
		}

		void DEBUG_TextPrintfVargs(uint16 _x, uint16 _y, uint8_t _attr, const char* _format, va_list _argList)
		{
		}

		IndexBufferHandle CreateIndexBuffer(const Memory* mem)
		{
			IndexBufferHandle handle = { _indexBufferHandle.alloc() };
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
			IndexBufferHandle handle = { _indexBufferHandle.alloc() };
			CommandBuffer& cmdbuf = GetCommandBuffer(CommandBuffer::eCreateDynamicIndexBuffer);
			cmdbuf.Write(handle);
			cmdbuf.Write(size);

			DynamicIndexBuffer* ib = (DynamicIndexBuffer*)g_realloc(NULL, sizeof(DynamicIndexBuffer) + size);
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
			g_free(const_cast<DynamicIndexBuffer*>(ib));
		}

		const DynamicIndexBuffer* AllocDynamicIndexBuffer(uint16 num)
		{
			uint32 offset = _submit->AllocDynamicIndexBuffer(num);

			DynamicIndexBuffer& dib = *_submit->_dynamicIb;

			DynamicIndexBuffer* ib = (DynamicIndexBuffer*)g_realloc(NULL, sizeof(DynamicIndexBuffer));
			ib->_data = &dib._data[offset];
			ib->_size = num * sizeof(uint16);
			ib->_handle = dib._handle;
			ib->_startIndex = offset / sizeof(uint16);

			return ib;
		}

		//TODO Implementt this again
		VertexDeclHandle FindVertexDecl(const VertexDecl& _decl)
		{
			VertexDeclHandle declHandle = _declRef.find(_decl._hash);

			if (video::invalidHandle == declHandle.index)
			{
				VertexDeclHandle temp = { _vertexDeclHandle.alloc() };
				declHandle = temp;
				CommandBuffer& cmdbuf = GetCommandBuffer(CommandBuffer::eCreateVertexDecl);
				cmdbuf.Write(declHandle);
				cmdbuf.Write(_decl);
			}

			return declHandle;
		}

		VertexBufferHandle CreateVertexBuffer(const Memory* mem, const VertexDecl& decl)
		{
			VertexBufferHandle handle = { _vertexBufferHandle.alloc() };

			VertexDeclHandle declHandle = FindVertexDecl(decl);
			_declRef.add(handle, declHandle, _decl.m_hash);

			CommandBuffer& cmdbuf = GetCommandBuffer(CommandBuffer::eCreateVertexBuffer);
			cmdbuf.Write(handle);
			cmdbuf.Write(mem);
			cmdbuf.Write(declHandle);
			return handle;
		}

		void DestroyVertexBuffer(VertexBufferHandle handle)
		{
			VertexDeclHandle declHandle = m_declRef.release(handle);
			if (video::invalidHandle != declHandle.index)
			{
				CommandBuffer& cmdbuf = GetCommandBuffer(CommandBuffer::eDestroyVertexDecl);
				cmdbuf.Write(declHandle);
			}

			CommandBuffer& cmdbuf = GetCommandBuffer(CommandBuffer::eDestroyVertexBuffer);
			cmdbuf.Write(handle);
			_submit->free(handle);
		}

		DynamicVertexBuffer* CreateDynamicVertexBuffer(uint32 _size, const VertexDecl* _decl = NULL)
		{
			VertexBufferHandle handle = { _vertexBufferHandle.alloc() };

			uint16 stride = 0;
			VertexDeclHandle declHandle = INVALID_HANDLE;

			if (NULL != _decl)
			{
				declHandle = findVertexDecl(*_decl);
				_declRef.add(handle, declHandle, _decl->m_hash);

				stride = _decl->m_stride;
			}

			CommandBuffer& cmdbuf = GetCommandBuffer(CommandBuffer::eCreateDynamicVertexBuffer);
			cmdbuf.Write(handle);
			cmdbuf.Write(_size);

			DynamicVertexBuffer* vb = (DynamicVertexBuffer*)g_realloc(NULL, sizeof(DynamicVertexBuffer) + _size);
			vb->_data = (uint8_t*)&vb[1];
			vb->_size = _size;
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

			_submit->free(vb->_handle);
			g_free(const_cast<DynamicVertexBuffer*>(vb));
		}

		const DynamicVertexBuffer* AllocDynamicVertexBuffer(uint16 num, const VertexDecl& decl)
		{
			VertexDeclHandle declHandle = _declRef.find(decl._hash);

			DynamicVertexBuffer& dvb = *_submit->_dynamicVb;

			if (video::invalidHandle == declHandle.index)
			{
				VertexDeclHandle temp = { _vertexDeclHandle.alloc() };
				declHandle = temp;
				CommandBuffer& cmdbuf = GetCommandBuffer(CommandBuffer::eCreateVertexDecl);
				cmdbuf.Write(declHandle);
				cmdbuf.Write(decl);
				m_declRef.add(dvb.handle, declHandle, _decl.m_hash);
			}

			uint32 offset = m_submit->allocDynamicVertexBuffer(num, _decl.m_stride);

			DynamicVertexBuffer* vb = (DynamicVertexBuffer*)g_realloc(NULL, sizeof(DynamicVertexBuffer));
			vb->_data = &dvb.data[offset];
			vb->_size = _num * _decl.m_stride;
			vb->_startVertex = offset / decl._stride;
			vb->_stride = _decl.m_stride;
			vb->_handle = dvb.handle;
			vb->_decl = declHandle;

			return vb;
		}

		//VertexShaderHandle createVertexShader(const Memory* _mem)
		//{
		//	VertexShaderHandle handle = { m_vertexShaderHandle.alloc() };
		//	CommandBuffer& cmdbuf = getCommandBuffer(CommandBuffer::CreateVertexShader);
		//	cmdbuf.write(handle);
		//	cmdbuf.write(_mem);
		//	return handle;
		//}

		//void destroyVertexShader(VertexShaderHandle _handle)
		//{
		//	CommandBuffer& cmdbuf = getCommandBuffer(CommandBuffer::DestroyVertexShader);
		//	cmdbuf.write(_handle);
		//	m_submit->free(_handle);
		//}

		//FragmentShaderHandle createFragmentShader(const Memory* _mem)
		//{
		//	FragmentShaderHandle handle = { m_fragmentShaderHandle.alloc() };
		//	CommandBuffer& cmdbuf = getCommandBuffer(CommandBuffer::CreateFragmentShader);
		//	cmdbuf.write(handle);
		//	cmdbuf.write(_mem);
		//	return handle;
		//}

		//void destroyFragmentShader(FragmentShaderHandle _handle)
		//{
		//	CommandBuffer& cmdbuf = getCommandBuffer(CommandBuffer::DestroyFragmentShader);
		//	cmdbuf.write(_handle);
		//	m_submit->free(_handle);
		//}

		EffectHandle CreateEffect(const Memory *mem)
		{

		}

		void destroyEffect(EffectHandle handle)
		{

		}

		MaterialHandle createMaterial(VertexShaderHandle _vsh, FragmentShaderHandle _fsh)
		{
			MaterialHandle handle;
			// 			uint32 hash = _vsh.idx<<16 | _fsh.idx;
			// 
			// 			MaterialHandle handle = m_materialRef.find(hash);
			// 
			// 			if (bgfx::invalidHandle != handle.idx)
			// 			{
			// 				return handle;
			// 			}
			// 
			handle.idx = m_materialHandle.alloc();
			// 			m_materialRef.add(handle, hash);

			CommandBuffer& cmdbuf = getCommandBuffer(CommandBuffer::CreateMaterial);
			cmdbuf.write(handle);
			cmdbuf.write(_vsh);
			cmdbuf.write(_fsh);
			return handle;
		}

		void destroyMaterial(MaterialHandle _handle)
		{
			CommandBuffer& cmdbuf = getCommandBuffer(CommandBuffer::DestroyMaterial);
			cmdbuf.write(_handle);
			m_submit->free(_handle);
		}

		TextureHandle createTexture(const Memory* _mem, uint32 _flags, uint16* _width, uint16* _height)
		{
			if (NULL != _width
				|| NULL != _height)
			{
				int width = 0;
				int height = 0;

				Dds dds;
				if (parseDds(dds, _mem))
				{
					width = dds.m_width;
					height = dds.m_height;
				}

				if (NULL != _width)
				{
					*_width = (uint16)width;
				}

				if (NULL != _height)
				{
					*_height = (uint16)height;
				}
			}

			TextureHandle handle = { m_textureHandle.alloc() };
			CommandBuffer& cmdbuf = getCommandBuffer(CommandBuffer::CreateTexture);
			cmdbuf.write(handle);
			cmdbuf.write(_mem);
			cmdbuf.write(_flags);
			return handle;
		}

		void destroyTexture(TextureHandle _handle)
		{
			CommandBuffer& cmdbuf = getCommandBuffer(CommandBuffer::DestroyTexture);
			cmdbuf.write(_handle);
			m_submit->free(_handle);
		}

		RenderTargetHandle createRenderTarget(uint16 _width, uint16 _height, uint32 _flags)
		{
			RenderTargetHandle handle = { m_renderTargetHandle.alloc() };

			CommandBuffer& cmdbuf = getCommandBuffer(CommandBuffer::CreateRenderTarget);
			cmdbuf.write(handle);
			cmdbuf.write(_width);
			cmdbuf.write(_height);
			cmdbuf.write(_flags);
			return handle;
		}

		void destroyRenderTarget(RenderTargetHandle _handle)
		{
			CommandBuffer& cmdbuf = getCommandBuffer(CommandBuffer::DestroyRenderTarget);
			cmdbuf.write(_handle);
			m_submit->free(_handle);
		}

		UniformHandle createUniform(const char* _name, ConstantType::Enum _type, uint16 _num)
		{
			BX_CHECK(PredefinedUniform::Count == nameToPredefinedUniformEnum(_name), "%s is predefined uniform name.", _name);

			UniformHandle handle = { m_uniformHandle.alloc() };

			Constant& constant = m_constant[handle.idx];
			constant.m_type = _type;
			constant.m_num = _num;

			CommandBuffer& cmdbuf = getCommandBuffer(CommandBuffer::CreateUniform);
			cmdbuf.write(handle);
			cmdbuf.write(_type);
			cmdbuf.write(_num);
			uint8_t len = (uint8_t)strlen(_name);
			cmdbuf.write(len);
			cmdbuf.write(_name, len);
			return handle;
		}

		void destroyUniform(UniformHandle _handle)
		{
			CommandBuffer& cmdbuf = getCommandBuffer(CommandBuffer::DestroyUniform);
			cmdbuf.write(_handle);
			m_submit->free(_handle);
		}

		void saveScreenShot(const Memory* _mem)
		{
			CommandBuffer& cmdbuf = getCommandBuffer(CommandBuffer::SaveScreenShot);
			cmdbuf.write(_mem);
		}

		void setUniform(UniformHandle _handle, const void* _value, uint16 _num)
		{
			Constant& constant = m_constant[_handle.idx];
			BX_CHECK(constant.m_num >= _num, "Truncated uniform update. %d (max: %d)", _num, constant.m_num);
			m_submit->writeConstant(constant.m_type, _handle, _value, uint16_min(constant.m_num, _num));
		}

		void setUniform(MaterialHandle _material, UniformHandle _handle, const void* _value)
		{
			BX_CHECK(false, "NOT IMPLEMENTED!");
		}

		void setViewRect(uint8_t _id, uint16 _x, uint16 _y, uint16 _width, uint16 _height)
		{
			Rect& rect = m_rect[_id];
			rect.m_x = _x;
			rect.m_y = _y;
			rect.m_width = _width;
			rect.m_height = _height;
		}

		void setViewRectMask(uint32 _viewMask, uint16 _x, uint16 _y, uint16 _width, uint16 _height)
		{
			for (uint32 id = 0, viewMask = _viewMask, ntz = uint32_cnttz(_viewMask); 0 != viewMask; viewMask >>= 1, id += 1, ntz = uint32_cnttz(viewMask))
			{
				viewMask >>= ntz;
				id += ntz;

				setViewRect(id, _x, _y, _width, _height);
			}
		}

		void setViewClear(uint8_t _id, uint8_t _flags, uint32 _rgba, float _depth, uint8_t _stencil)
		{
			Clear& clear = m_clear[_id];
			clear.m_flags = _flags;
			clear.m_rgba = _rgba;
			clear.m_depth = _depth;
			clear.m_stencil = _stencil;
		}

		void setViewClearMask(uint32 _viewMask, uint8_t _flags, uint32 _rgba, float _depth, uint8_t _stencil)
		{
			for (uint32 id = 0, viewMask = _viewMask, ntz = uint32_cnttz(_viewMask); 0 != viewMask; viewMask >>= 1, id += 1, ntz = uint32_cnttz(viewMask))
			{
				viewMask >>= ntz;
				id += ntz;

				setViewClear(id, _flags, _rgba, _depth, _stencil);
			}
		}

		void setViewSeq(uint8_t _id, bool _enabled)
		{
			m_seqMask[_id] = _enabled ? 0xffff : 0x0;
		}

		void setViewSeqMask(uint32 _viewMask, bool _enabled)
		{
			uint16 mask = _enabled ? 0xffff : 0x0;
			for (uint32 id = 0, viewMask = _viewMask, ntz = uint32_cnttz(_viewMask); 0 != viewMask; viewMask >>= 1, id += 1, ntz = uint32_cnttz(viewMask))
			{
				viewMask >>= ntz;
				id += ntz;

				m_seqMask[id] = mask;
			}
		}

		void setViewRenderTarget(uint8_t _id, RenderTargetHandle _handle)
		{
			m_rt[_id] = _handle;
		}

		void setViewRenderTargetMask(uint32 _viewMask, RenderTargetHandle _handle)
		{
			for (uint32 id = 0, viewMask = _viewMask, ntz = uint32_cnttz(_viewMask); 0 != viewMask; viewMask >>= 1, id += 1, ntz = uint32_cnttz(viewMask))
			{
				viewMask >>= ntz;
				id += ntz;

				m_rt[id] = _handle;
			}
		}

		void dumpViewStats()
		{
			#if 0 // BGFX_DEBUG
			for (uint8_t view = 0; view < BGFX_CONFIG_MAX_VIEWS; ++view)
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
				_indexBufferHandle.free(_frame->_freeIndexBufferHandle[i].idx);
			}

			for (uint16 i = 0, num = _frame->_numFreeVertexDeclHandles; i < num; ++i)
			{
				_vertexDeclHandle.free(_frame->_freeVertexDeclHandle[i].idx);
			}

			for (uint16 i = 0, num = _frame->_numFreeVertexBufferHandles; i < num; ++i)
			{
				_vertexBufferHandle.free(_frame->_freeVertexBufferHandle[i].idx);
			}

			for (uint16 i = 0, num = _frame->_numFreeVertexShaderHandles; i < num; ++i)
			{
				_vertexShaderHandle.free(_frame->_freeVertexShaderHandle[i].idx);
			}

			for (uint16 i = 0, num = _frame->_numFreeFragmentShaderHandles; i < num; ++i)
			{
				_fragmentShaderHandle.free(_frame->_freeFragmentShaderHandle[i].idx);
			}

			for (uint16 i = 0, num = _frame->_numFreeMaterialHandles; i < num; ++i)
			{
				_materialHandle.free(_frame->_freeMaterialHandle[i].idx);
			}

			for (uint16 i = 0, num = _frame->_numFreeTextureHandles; i < num; ++i)
			{
				_textureHandle.free(_frame->_freeTextureHandle[ii].idx);
			}

			for (uint16 i = 0, num = _frame->_numFreeRenderTargetHandles; i < num; ++i)
			{
				_renderTargetHandle.free(_frame->_freeRenderTargetHandle[i].idx);
			}

			for (uint16 i = 0, num = _frame->_numFreeUniformHandles; i < num; ++i)
			{
				_uniformHandle.free(_frame->_freeUniformHandle[i].idx);
			}
		}

		void Swap()
		{
			_submit->_resolution = _resolution;
			_submit->_debug = _debug;
			memcpy(_submit->_rt, _rt, sizeof(_rt));
			memcpy(_submit->_clear, _clear, sizeof(_clear));
			memcpy(_submit->_rect, _rect, sizeof(_rect));
			_submit->finish();

			DumpViewStats();

			FreeAllHandles(_render);

			memset(_seq, 0, sizeof(_seq));
			Frame* temp = _render;
			_render = _submit;
			_submit = temp;
			_frames++;
			_submit->Reset();
		}

		void Flip();

		// render thread
		void enderFrame()
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
		void RendererCreateDynamicIndexBuffer(IndexBufferHandle handle, uint32 _size);
		void RendererDestroyDynamicIndexBuffer(IndexBufferHandle handle);
		void RendererCreateVertexBuffer(VertexBufferHandle handle, Memory* _mem, VertexDeclHandle _declHandle);
		void RendererDestroyVertexBuffer(VertexBufferHandle handle);
		void RendererCreateDynamicVertexBuffer(VertexBufferHandle handle, uint32 _size);
		void RendererDestroyDynamicVertexBuffer(VertexBufferHandle handle);
		void RendererCreateVertexDecl(VertexDeclHandle handle, const VertexDecl& _decl);
		void RendererDestroyVertexDecl(VertexDeclHandle _handle);
		//void RendererCreateVertexShader(VertexShaderHandle handle, Memory* _mem);
		//void RendererDestroyVertexShader(VertexShaderHandle _handle);
		//void RendererCreateFragmentShader(FragmentShaderHandle _handle, Memory* _mem);
		//void RendererDestroyFragmentShader(FragmentShaderHandle _handle);
		void RendererCreateMaterial(SurfaceMaterialHandle _handle, 
				VertexShaderHandle _vsh, FragmentShaderHandle _fsh);
		void RendererDestroyMaterial(FragmentShaderHandle _handle);
		void RendererCreateTexture(TextureHandle _handle, Memory* _mem, uint32 _flags);
		void RendererDestroyTexture(TextureHandle _handle);
		void RendererCreateRenderTarget(RenderTargetHandle _handle, uint16 _width, uint16 _height, uint32 _flags);
		void RendererDestroyRenderTarget(RenderTargetHandle _handle);
		void RendererCreateUniform(UniformHandle _handle, ConstantType::Enum _type, uint16 _num, const char* _name);
		void RendererDestroyUniform(UniformHandle _handle);
		void RendererSaveScreenShot(Memory* _mem);
		void RendererUpdateUniform(uint16 _loc, const void* _data, uint32 _size);

		void RendererUpdateUniforms(ConstantBuffer* constantBuffer, uint32 begin, uint32 end)
		{
			_constantBuffer->reset(_begin);
			while (_constantBuffer->getPos() < _end)
			{
				uint32 opcode = _constantBuffer->read();

				if (ConstantType::End == opcode)
				{
					break;
				}

				ConstantType::Enum type;
				uint16 loc;
				uint16 num;
				uint16 copy;
				ConstantBuffer::decodeOpcode(opcode, type, loc, num, copy);

				const char* data;
				uint32 size = g_constantTypeSize[type] * num;
				data = _constantBuffer->read(size);
				rendererUpdateUniform(loc, data, size);
			}
		}

		void RendererExecCommands(CommandBuffer& cmdbuf)
		{
			cmdbuf.Reset();

			bool end = false;

			do
			{
				uint8_t command;
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

					rendererDestroyVertexDecl(handle);
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

				case CommandBuffer::CreateVertexShader:
				{
					VertexShaderHandle handle;
					cmdbuf.read(handle);

					Memory* mem;
					cmdbuf.read(mem);

					rendererCreateVertexShader(handle, mem);

					free(mem);
				}
				break;

				case CommandBuffer::DestroyVertexShader:
				{
					VertexShaderHandle handle;
					cmdbuf.read(handle);

					rendererDestroyVertexShader(handle);
				}
				break;

				case CommandBuffer::CreateFragmentShader:
				{
					FragmentShaderHandle handle;
					cmdbuf.read(handle);

					Memory* mem;
					cmdbuf.read(mem);

					rendererCreateFragmentShader(handle, mem);

					free(mem);
				}
				break;

				case CommandBuffer::DestroyFragmentShader:
				{
					FragmentShaderHandle handle;
					cmdbuf.read(handle);

					rendererDestroyFragmentShader(handle);
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

					uint8_t len;
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

		void RendererSubmit();

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
			m_render->m_waitSubmit = bx::getHPCounter() - start;
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
			m_submit->m_waitRender = bx::getHPCounter() - start;
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

		void EenderSemPost()
		{
		}

		void EenderSemWait()
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
		HandleAlloc _materialHandle;
		HandleAlloc _textureHandle;
		HandleAlloc _renderTargetHandle;
		HandleAlloc _uniformHandle;

		MaterialRef _materialRef;
		VertexDeclRef _declRef;

		RenderTargetHandle _renderTargetHandle[VIDEO_MAX_VIEWS];
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
					GetWindowRect(bgfxHwnd, &m_rect);
					m_style = GetWindowLong(bgfxHwnd, GWL_STYLE);
				}

				if (_windowFrame)
				{
					rect = m_rect;
					style = m_style;
				}
				else
				{
					#if defined(__MINGW32__)
					rect = m_rect;
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

			RECT m_rect;
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

