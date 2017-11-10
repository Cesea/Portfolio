#include "stdafx.h"
#include "Video.h"

#include "VideoPlatform.h"



void* ReallocStub(void* ptr, size_t size)
{
	void* ptr = ::realloc(ptr, size);
	Assert(NULL != ptr); //메모리 부족
	return ptr;
}

void FreeStub(void* ptr)
{
	free(ptr);
}

namespace video
{
	#define VIDEO_MAIN_THREAD_MAGIC 0x78666762

	#if VIDEO_MULTITHREADED
	#	define BGFX_MAIN_THREAD() BX_CHECK(BGFX_MAIN_THREAD_MAGIC == s_threadIndex, "Must be called from main thread.")
	#	define BGFX_RENDER_THREAD() BX_CHECK(BGFX_MAIN_THREAD_MAGIC != s_threadIndex, "Must be called from render thread.")
	#else
	#	define VIDEO_MAIN_THREAD()
	#	define VIDEO_RENDER_THREAD()
	#endif // BGFX_CONFIG_MULTITHREADED

	static uint32 sThreadIndex = 0;
	static Context sContext;

	#define RADIX_BITS 11
	#define RADIX_PASSES 6
	#define RADIX_HISTOGRAM_SIZE (1<<RADIX_BITS)
	#define RADIX_BIT_MASK (RADIX_HISTOGRAM_SIZE-1)

	void RadixSort(uint64* keys, uint64* tempKeys, uint16* values, uint16* tempValues, uint32 size)
	{
		uint16 histogram[RADIX_HISTOGRAM_SIZE];
		uint16 shift = 0;
		for (uint32 pass = 0; pass < RADIX_PASSES; ++pass)
		{
			memset(histogram, 0, sizeof(uint16) * RADIX_HISTOGRAM_SIZE);
			for (uint32 i = 0; i < size; ++i)
			{
				uint64 key = keys[i];
				uint16 index = (key >> shift) & RADIX_BIT_MASK;
				++histogram[index];
			}

			uint16 offset = 0;
			for (uint32 i = 0; i < RADIX_HISTOGRAM_SIZE; ++i)
			{
				uint16 count = histogram[i];
				histogram[i] = offset;
				offset += count;
			}

			for (uint32 i = 0; i < size; ++i)
			{
				uint64 key = keys[i];
				uint16 index = (key >> shift) & RADIX_BIT_MASK;
				uint16 dest = histogram[index]++;
				tempKeys[dest] = key;

				uint16_t value = values[i];
				tempValues[dest] = value;
			}

			uint64* swapKeys = tempKeys;
			tempKeys = keys;
			keys = swapKeys;

			uint16* swapValues = tempValues;
			tempValues = values;
			values = swapValues;

			shift += RADIX_BITS;
		}
	}

	static const char *sPredefinedName[PredefinedUniforms::Count] =
	{
		"uViewRect",
		"uViewTexel",
		"uView",
		"uViewProjection",
		"uModel",
		"uModelViewProjection",
		"uAlpha"
	};

	PredefinedUniform::Enum NameToPredefinedUniformEnum(const char* name)
	{
		for (uint32_t i = 0; i < PredefinedUniform::Count; ++i)
		{
			if (0 == strcmp(name, sPredefinedName[i]))
			{
				return PredefinedUniform::Enum(i);
			}
		}

		return PredefinedUniform::Count;
	}

	void Frame::Submit(uint8 id)
	{
		if (_discard)
		{
			_discard = false;
			return;
		}

		if (VIDEO_MAX_DRAW_CALLS - 1 <= _num)
		{
			++_numDropped;
			return;
		}

		_key._view = id;
		_key._seq = sContext._seq[id] & sContext._seqMask[id];
		sContext._seq[id]++;
		uint64_t key = _key.Encode();
		_sortKeys[_num] = key;
		_sortValues[_num] = _numRenderStates;
		++_num;

		_state._constEnd = _constantBuffer->getPos();
		_state._flags |= _flags;
		_renderState[_numRenderStates] = _state;
		++_numRenderStates;
		_state.Clear();
		_flags = VIDEO_STATE_NONE;
	}

	void Frame::SubmitMask(uint32 viewMask)
	{
		if (_discard)
		{
			_discard = false;
			return;
		}

		if (VIDEO_MAX_DRAW_CALLS - 1 <= _num)
		{
			_numDropped += uint32_cntbits(viewMask);
			return;
		}

		for (uint32 id = 0, vm = viewMask, ntz = uint32_cnttz(viewMask); 0 != vm; vm >>= 1, id += 1, ntz = uint32_cnttz(viewMask))
		{
			viewMask >>= ntz;
			id += ntz;

			_key._view = id;
			_key._seq = sContext._seq[id] & sContext._seqMask[id];
			sContext._seq[id]++;
			uint64_t key = _key.Encode();
			_sortKeys[_num] = key;
			_sortValues[_num] = _numRenderStates;
			++_num;
		}

		_state._constEnd = _constantBuffer->getPos();
		_state._flags |= _flags;
		_renderState[_numRenderStates] = _state;
		++_numRenderStates;
		_state.Clear();
		_flags = VIDEO_STATE_NONE;
	}

	void Frame::Sort()
	{
		radixSort(m_sortKeys, s_ctx.m_tempKeys, m_sortValues, s_ctx.m_tempValues, m_num);
	}


	void Init(bool createRenderThread, ReallocFunction realloc, FreeFunction free)
	{
		memset(m_rt, 0xff, sizeof(m_rt));
		memset(m_clear, 0, sizeof(m_clear));
		memset(m_rect, 0, sizeof(m_rect));
		memset(m_seq, 0, sizeof(m_seq));
		memset(m_seqMask, 0, sizeof(m_seqMask));

		GameSemPost();
		_initialized = true;

		GetCommandLine(CommandBuffer::eRendererInit);

		g_textVideoMemBlitter.init();

		_submit->_dynamicVb = CreateDynamicVertexBuffer(VIDEO_DYNAMIC_VERTEX_BUFFER_SIZE);
		_submit->_dynamicIb = CreateDynamicIndexBuffer(_VIDEODYNAMIC_INDEX_BUFFER_SIZE);
		Frame();
		_submit->_dynamicVb = CreateDynamicVertexBuffer(_VIDEODYNAMIC_VERTEX_BUFFER_SIZE);
		_submit->_dynamicIb = CreateDynamicIndexBuffer(_VIDEODYNAMIC_INDEX_BUFFER_SIZE);
		Frame();
	}

	void Shutdown()
	{
		GetCommandBuffer(CommandBuffer::eRendererShutdown);
		Frame();
		_initialized = false;
	}

	void Reset(uint32 width, uint32 height, uint32 flags)
	{
	}

	void Frame()
	{
	}

	const Memory * Alloc(uint32 size)
	{
		Memory* mem = (Memory*)g_realloc(NULL, sizeof(Memory) + size);
		mem->_size = size;
		mem->_data = (uint8*)mem + sizeof(Memory);
		return mem;
	}

	const Memory * MakeRef(void * data, uint32 size)
	{
		Memory* mem = (Memory *)g_realloc(NULL, sizeof(Memory));
		mem->_size = size;
		mem->_data = (uint8 *)data;
		return mem;
	}

	void SetDebug(uint32 debug)
	{
	}

	void DEBUG_TextClear(uint8 attr, bool size)
	{
	}

	void DEBUG_TextPrintf(uint16 x, uint16 y, uint8 attribute, const char * format, ...)
	{
	}

	IndexBufferHandle CreateIndexBuffer(const Memory * mem)
	{
		return sContext.CreateIndexBuffer(mem);
	}

	void DestroyIndexBuffer(IndexBufferHandle handle)
	{
		sContext.DestroyIndexBuffer(handle);
	}

	bool CheckAvailDynamicIndexBuffer(uint16 num)
	{
		return sContext._submit->CheckAvailDynamicIndexBuffer(num);
	}

	const DynamicIndexBuffer * AllocDynamicIndexBuffer(uint16 num)
	{
		return sContext.AllocDynamicIndexBuffer(num);
	}

	VertexBufferHandle CreateVertexBuffer(const Memory * mem, VertexDeclHandle handle)
	{
		return sContext.CreateVertexBuffer(mem, handle);
	}

	void DestroyVertexBuffer(VertexBufferHandle handle)
	{
		return sContext.DestroyVertexBuffer(handle);
	}

	bool CheckAvailDynamicVertexBuffer(uint16 num, const VertexDecl & decl)
	{
		return sContext._submit->CheckAvailDynamicVertexBuffer(num, );
	}

	const DynamicVertexBuffer *AllocDynamicVertexBuffer(uint16 num, const VertexDecl &decl)
	{
		return sContext.AllocDynamicVertexBuffer(num, decl);
	}

	EffectHandle CreateEffect(const Memory *mem)
	{
		return sContext.CreateEffect(mem);
	}

	SurfaceMaterialHandle CreateMaterial(const Memory *mem)
	{
		return sContext.CreateSurfaceMaterial(handle);
	}

	void DestroyMaterial(SurfaceMaterialHandle handle)
	{
		return sContext.DestroySurfaceMaterial(handle);
	}

	TextureHandle CreateTexture(const Memory *mem, uint32 flags, uint16 *width, uint16 *height)
	{
		return sContext.createTexture(mem, flags, width, height);
	}

	void DestroyTexture(TextureHandle handle)
	{
		return sContext.destroyTexture(handle);
	}

	RenderTargetHandle CreateRenderTarget(uint32 width, uint32 height, uint32 flags)
	{
		return sContext.createRenderTarget(width, height, flags);
	}

	void DestroyRenderTarget(RenderTargetHandle handle)
	{
		sContext.destroyRenderTarget(handle);
	}

	void SetViewRect(uint8 id, uint32 x, uint32 y, uint32 width, uint32 height)
	{
		sContext.setViewRect(id, x, y, width, height);
	}

	void SetViewRectMask(uint32 viewMask, uint32 x, uint32 y, uint32 width, uint32 height)
	{
		sContext.setViewRectMask(viewMask, x, y, width, height);
	}

	void SetViewClear(uint8 id, uint8 flags, uint32 rgba, float depth, uint8 stencil)
	{
		sContext.setViewClear(id, flags, rgba, depth, stencil);
	}

	void SetViewClearMask(uint32 viewMask, uint8 flags, uint32 rgba, float depth, uint8 stencil)
	{
		sContext.setViewClearMask(viewMask, flags, rgba, depth, stencil);
	}

	void SetViewSeq(uint8 id, bool enabled)
	{
		sContext.setViewSeq(id, enabled);
	}

	void SetViewSeqMask(uint32 viewMask, bool enabled)
	{
		sContext.setViewSeqMask(viewMask, enabled);
	}

	void SetViewRenderTarget(uint8 id, RenderTargetHandle handle)
	{
		sContext.setViewRenderTarget(id, handle);
	}

	void SetViewRenderTargetMask(uint32 viewMask, RenderTargetHandle handle)
	{
		sContext.setViewRenderTargetMask(viewMask, handle);
	}

	void SetViewTransform(uint8 id, const Matrix *view, const Matrix *projection, uint8 other)
	{
		sContext.setViewTransform(id, view, projection, other);
	}

	void SetViewTransformMask(uint32 viewMask, const Matrix *view, const Matrix *projection, uint8 other)
	{
		sContext.setViewRenderTarget(viewMask, view, projection, other);
	}

	void SetState(uint64 state)
	{
		sContext.SetState(state);
	}

	uint32 SetTransform(const void * matrix, uint16 num)
	{
		sContext.setViewRenderTarget(viewMask, handle);
	}

	void SetUniform(UniformHandle handle, const void * value, uint16 num)
	{
		sContext.SetUniform(handle, value, num);
	}
	void SetUniform(SurfaceMaterialHandle material, UniformHandle handle, const void * value)
	{
		sContext.SetUniform(material, handle, value);
	}

	void SetTransform(uint32 cache, uint32 num)
	{
		sContext._submit->SetTransform(cache, num);
	}

	void SetIndexBuffer(IndexBufferHandle handle, uint32 firstIndex, uint32 numIndices)
	{
		sContext._submit->SetIndexBuffer(handle, firstIndex, numIndices);
	}

	void SetIndexBuffer(IndexBufferHandle handle)
	{
		sContext._submit->SetIndexBuffer(handle, VIDEO_DRAW_WHOLE_INDEX_BUFFER, 0);
	}

	void SetIndexBuffer(const DynamicIndexBuffer * ib, uint32 numIndices)
	{
		uint32 numIndices = uint32_min(numIndices, ib->_size / 2);
		sContext._submit->SetIndexBuffer(ib, numIndices);
	}

	void SetVertexBuffer(VertexBufferHandle handle)
	{
		sContext._submit->SetVertexBuffer(handle);
	}

	void SetVertexBuffer(const DynamicVertexBuffer * vb)
	{
		sContext._submit->SetVertexBuffer(vb);
	}

	void SetMaterial(SurfaceMaterialHandle handle)
	{
		sContext._submit->SetMaterial(handle);
	}

	void SetTexture(uint8 stage, UniformHandle sampler, TextureHandle handle)
	{
		sContext._submit->SetTexture(stage, sampler, handle);
	}

	void SetTexture(uint8 stage, UniformHandle sampler, RenderTargetHandle handle, bool depth)
	{
		sContext._submit->SetTexture(stage, sampler, handle, depth);
	}

	void Submit(uint8 id)
	{
		sContext._submit->Submit(id);
	}

	void SubmitMask(uint32 viewMask)
	{
		sContext._submit->SubmitMask(viewMask);
	}

	void SaveScreenShot(const char * filePath)
	{
		sContext.setViewRenderTarget(viewMask, handle);
	}

	void RenderFrame()
	{
		sContext.setViewRenderTarget(viewMask, handle);
	}
	void VertexDecl::Begin()
	{
	}
	void VertexDecl::End()
	{
	}
	void VertexDecl::Add(Attrib::Enum attrib, uint8 num, AttribType::Enum type, bool normalized)
	{
	}
	void VertexDecl::Decode(Attrib::Enum attrib, uint8 & num, AttribType::Enum & type, bool & normalized) const
	{
	}
}
