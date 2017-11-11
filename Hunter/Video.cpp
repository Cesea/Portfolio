#include "stdafx.h"
#include "Video.h"

#include "VideoPlatform.h"

void* ReallocStub(void* ptr, size_t size)
{
	void* iptr = ::realloc(ptr, size);
	Assert(nullptr != iptr); //메모리 부족
	return iptr;
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

	ReallocFunction gRealloc = ReallocStub;
	FreeFunction gFree = FreeStub;

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

				uint16 value = values[i];
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

	//#include "Charset.h"

	//void CharsetFillTexture(const uint8* charset, uint8* rgba, uint32 height, uint32 pitch, uint32 bpp)
	//{
	//	for (uint32_t i = 0; i < 256; ++i)
	//	{
	//		uint8* pix = &rgba[i * 8 * bpp];
	//		for (uint32 y = 0; y < height; ++y)
	//		{
	//			for (uint32_t x = 0; x < 8; ++x)
	//			{
	//				uint8 bit = 1 << (7 - x);
	//				memset(&pix[x * bpp], charset[i * height + y] & bit ? 255 : 0, bpp);
	//			}

	//			pix += pitch;
	//		}
	//	}
	//}


	//static const uint32 numCharsPerBatch = 1024;
	//static const uint32 numBatchVertices = numCharsPerBatch * 4;
	//static const uint32 numBatchIndices = numCharsPerBatch * 6;

	//void TextVideoMemBlitter::Init()
	//{
	//	_decl.Begin();
	//	_decl.Add(video::Attrib::ePosition, 3, video::AttribType::eFloat);
	//	_decl.Add(video::Attrib::eColor0, 4, video::AttribType::eUint8);
	//	_decl.Add(video::Attrib::eColor1, 4, video::AttribType::eUint8);
	//	_decl.Add(video::Attrib::eTexCoord0, 2, video::AttribType::eFloat);
	//	_decl.End();

	//	uint16 width = 2048;
	//	uint16 height = 24;
	//	uint8 bpp = 1;
	//	uint32 pitch = width*bpp;

	//	const video::Memory* mem;

	//	mem = Alloc(pitch  * height + 16);

	//	StreamWrite stream(mem->_data, mem->_size);
	//	uint32 magic = BGFX_MAGIC;
	//	stream.Write(magic);
	//	stream.Write(width);
	//	stream.Write(height);
	//	stream.Write(bpp);
	//	uint8_t numMips = 1;
	//	stream.Write(numMips);
	//	stream.Align(16);
	//	uint8* rgba = stream.GetDataPtr();

	//	CharsetFillTexture(vga8x8, rgba, 8, pitch, bpp);
	//	CharsetFillTexture(vga8x16, &rgba[8 * pitch], 16, pitch, bpp);
	//	_texture = sContext.CreateTexture(mem, 
	//		VIDEO_TEXTURE_MIN_POINT | VIDEO_TEXTURE_MAG_POINT | VIDEO_TEXTURE_MIP_POINT, NULL, NULL);

	//	mem = video::Alloc(sizeof(vs_debugfont_hlsl) + 1);
	//	memcpy(mem->_data, vs_debugfont_hlsl, mem->_size - 1);

	//	mem = video::Alloc(sizeof(fs_debugfont_hlsl) + 1);
	//	memcpy(mem->_data, fs_debugfont_hlsl, mem->_size - 1);

	//	mem->_data[mem->_size - 1] = '\0';
	//	video::FragmentShaderHandle fsh = video::CreateFragmentShader(mem);

	//	m_material = video::CreateMaterial(vsh, fsh);

	//	_vb = sContext.CreateDynamicVertexBuffer(numBatchVertices * _decl._stride, &_decl);
	//	_ib = sContext.CreateDynamicIndexBuffer(numBatchIndices * 2);
	//}

	//void TextVideoMemBlitter::Blit(const TextVideoMem& _mem)
	//{
	//	struct FontVertex
	//	{
	//		float _x;
	//		float _y;
	//		float _z;
	//		uint32 _fg;
	//		uint32 _bg;
	//		float _u;
	//		float _v;
	//	};

	//	static uint32_t palette[16] =
	//	{
	//		0x0,
	//		0xff0000cc,
	//		0xff069a4e,
	//		0xff00a0c4,
	//		0xffa46534,
	//		0xff7b5075,
	//		0xff9a9806,
	//		0xffcfd7d3,
	//		0xff535755,
	//		0xff2929ef,
	//		0xff34e28a,
	//		0xff4fe9fc,
	//		0xffcf9f72,
	//		0xffa87fad,
	//		0xffe2e234,
	//		0xffeceeee,
	//	};

	//	uint32_t y = 0;
	//	uint32_t x = 0;

	//	const float texelWidth = 1.0f / 2048.0f;
	//	const float texelWidthHalf = texelWidth * 0.5f;
	//	const float texelHeight = 1.0f / 24.0f;
	//	const float texelHeightHalf = texelHeight * 0.5f;
	//	const float utop = (_mem._small ? 0.0f : 8.0f) * texelHeight - texelHeightHalf;
	//	const float ubottom = (_mem._small ? 8.0f : 24.0f) * texelHeight - texelHeightHalf;
	//	const float fontHeight = (_mem._small ? 8.0f : 16.0f);

	//	Setup();

	//	for (; x < _mem._width && y < _mem._height;)
	//	{
	//		FontVertex* vertex = (FontVertex*)_vb->_data;
	//		uint16_t* indices = (uint16_t*)_ib->_data;
	//		uint32_t startVertex = 0;
	//		uint32_t numIndices = 0;

	//		for (; y < _mem._height && numIndices < numBatchIndices; ++y)
	//		{
	//			x = (x < _mem._width) ? x : 0;
	//			const uint8* line = &_mem._mem[(y * _mem._width + x) * 2];

	//			for (; x < _mem._width && numIndices < numBatchIndices; ++x)
	//			{
	//				uint8_t ch = line[0];
	//				uint8_t attr = line[1];

	//				if (0 != (ch | attr)
	//					&& (' ' != ch || 0 != (attr & 0xf0)))
	//				{
	//					uint32_t fg = palette[attr & 0xf];
	//					uint32_t bg = palette[(attr >> 4) & 0xf];

	//					FontVertex vert[4] =
	//					{
	//						{ (x) * 8.0f, (y) * fontHeight, 0.0f, fg, bg, 
	//							(ch) * 8.0f * texelWidth - texelWidthHalf, utop },
	//						{ (x + 1) * 8.0f, (y) * fontHeight, 0.0f, fg, bg,
	//							(ch + 1) * 8.0f*texelWidth - texelWidthHalf, utop },
	//						{ (x + 1) * 8.0f, (y + 1) * fontHeight, 0.0f, fg, bg,
	//							(ch + 1) * 8.0f * texelWidth - texelWidthHalf, ubottom },
	//						{ (x) * 8.0f, (y + 1) * fontHeight, 0.0f, fg, bg, 
	//							(ch) * 8.0f * texelWidth - texelWidthHalf, ubottom },
	//					};

	//					memcpy(vertex, vert, sizeof(vert));
	//					vertex += 4;

	//					indices[0] = startVertex + 0;
	//					indices[1] = startVertex + 1;
	//					indices[2] = startVertex + 2;
	//					indices[3] = startVertex + 2;
	//					indices[4] = startVertex + 3;
	//					indices[5] = startVertex + 0;

	//					startVertex += 4;
	//					indices += 6;

	//					numIndices += 6;
	//				}

	//				line += 2;
	//			}

	//			if (numIndices >= numBatchIndices)
	//			{
	//				break;
	//			}
	//		}

	//		Render(numIndices);
	//	}
	//}

	//TextVideoMemBlitter g_textVideoMemBlitter;


	static const char *sPredefinedName[PredefinedUniform::Count] =
	{
		"uViewRect",
		"uViewTexel",
		"uView",
		"uViewProjection",
		"uViewProjectionX",
		"uModel",
		"uModelViewProjection",
		"uModelViewProjectionX",
		"uAlpha"
	};

	PredefinedUniform::Enum NameToPredefinedUniformEnum(const char* name)
	{
		for (uint32 i = 0; i < PredefinedUniform::Count; ++i)
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

		_state._constEnd = _constantBuffer->GetPos();
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
			_numDropped += Uint32CountBits(viewMask);
			return;
		}

		for (uint32 id = 0, vm = viewMask, ntz = Uint32BitScan(viewMask); 0 != vm; vm >>= 1, id += 1, ntz = Uint32BitScan(viewMask))
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

		_state._constEnd = _constantBuffer->GetPos();
		_state._flags |= _flags;
		_renderState[_numRenderStates] = _state;
		++_numRenderStates;
		_state.Clear();
		_flags = VIDEO_STATE_NONE;
	}

	void Frame::Sort()
	{
		RadixSort(_sortKeys, sContext._tempKeys, _sortValues, sContext._tempValues, _num);
	}


	void Init(bool createRenderThread, ReallocFunction realloc, FreeFunction free)
	{
		if (nullptr != realloc &&  nullptr != free)
		{
			gRealloc = realloc;
			gFree = free;
		}

		sThreadIndex = VIDEO_MAIN_THREAD_MAGIC;
		sContext.Init(createRenderThread);
	}

	void Shutdown()
	{
		VIDEO_MAIN_THREAD();
		sContext.Shutdown();
	}

	void Reset(uint32 width, uint32 height, uint32 flags)
	{
		VIDEO_MAIN_THREAD();
		sContext.Reset(width, height, flags);
	}

	void DoFrame()
	{
		VIDEO_MAIN_THREAD();
		sContext.DoFrame();
	}

	void RenderFrame()
	{
		if (sContext._initialized)
		{
			VIDEO_MAIN_THREAD();
			sContext.RenderFrame();
		}
	}

	static const uint32 sAttribTypeSize[AttribType::Count] =
	{
		1, 2, 4
	};

	void VertexDecl::Begin()
	{
		_hash = 0;
		_stride = 0;
		memset(_attributes, 0xff, sizeof(_attributes));
		memset(_offset, 0, sizeof(_offset));
	}

	void VertexDecl::End()
	{
		_hash = Hash(_attributes, sizeof(_attributes));
	}

	void VertexDecl::Add(Attrib::Enum attrib, uint8 num, AttribType::Enum type, bool normalized)
	{
		const uint8 encoded_norm = (normalized & 1) << 6;
		const uint8 encoded_type = (type & 3) << 3;
		const uint8 encoded_num = (num - 1) & 3;

		_attributes[attrib] = (encoded_norm | encoded_type | encoded_num);
		_offset[attrib] = _stride;
		_stride += sAttribTypeSize[type] * num;
	}

	void VertexDecl::Decode(Attrib::Enum attrib, uint8 & num, AttribType::Enum & type, bool & normalized) const
	{
		uint8 val = _attributes[attrib];
		num = (val & 3) + 1;
		type = AttribType::Enum((val >> 3) & 3);
		normalized = !!(val&(1 << 6));
	}

	const char* GetAttribName(Attrib::Enum attr)
	{
		static const char* attrName[Attrib::Count] = 
		{
			"Attrib::Position",
			"Attrib::Normal",
			"Attrib::Color0",
			"Attrib::Color1",
			"Attrib::Indices",
			"Attrib::Weights",
			"Attrib::TexCoord0",
			"Attrib::TexCoord1",
			"Attrib::TexCoord2",
			"Attrib::TexCoord3",
			"Attrib::TexCoord4",
			"Attrib::TexCoord5",
			"Attrib::TexCoord6",
			"Attrib::TexCoord7",
		};
		return attrName[attr];
	}

	const uint32 gConstantTypeSize[ConstantType::Count] =
	{
		sizeof(int32),
		sizeof(float),
		0,
		1 * sizeof(int32),
		1 * sizeof(float),
		2 * sizeof(float),
		3 * sizeof(float),
		4 * sizeof(float),
		3 * 3 * sizeof(float),
		4 * 4 * sizeof(float),
	};

	void ConstantBuffer::WriteUniform(ConstantType::Enum type, uint16 loc, const void* value, uint16 num)
	{
		uint32 opcode = EncodeOpcode(type, loc, num, true);
		Write(opcode);
		Write(value, gConstantTypeSize[type] * num);
	}

	void ConstantBuffer::WriteUniformRef(ConstantType::Enum type, uint16 loc, const void* value, uint16 num)
	{
		uint32 opcode = EncodeOpcode(type, loc, num, false);
		Write(opcode);
		Write(&value, sizeof(void*));
	}



	void Context::Init(bool createRenderThread)
	{
		Console::Log("init");

		//#if BGFX_CONFIG_MULTITHREADED
		//if (_createRenderThread)
		//{
		//	#	if BX_PLATFORM_WINDOWS|BX_PLATFORM_XBOX360
		//	CreateThread(NULL, 16 << 10, renderThread, NULL, 0, NULL);
		//	#	endif // BX_PLATFORM_WINDOWS|BX_PLATFORM_XBOX360
		//}
		//#endif // BGFX_CONFIG_MULTITHREADED

		memset(_renderTarget, 0xff, sizeof(_renderTarget));
		memset(_clear, 0, sizeof(_clear));
		memset(_rect, 0, sizeof(_rect));
		memset(_seq, 0, sizeof(_seq));
		memset(_seqMask, 0, sizeof(_seqMask));

		GameSemPost();
		_initialized = true;

		GetCommandBuffer(CommandBuffer::eRendererInit);

		//gTextVideoMemBlitter.init();

		_submit->_dynamicVb = CreateDynamicVertexBuffer(VIDEO_DYNAMIC_VERTEX_BUFFER_SIZE);
		_submit->_dynamicIb = CreateDynamicIndexBuffer(VIDEO_DYNAMIC_INDEX_BUFFER_SIZE);
		DoFrame();
		_submit->_dynamicVb = CreateDynamicVertexBuffer(VIDEO_DYNAMIC_VERTEX_BUFFER_SIZE);
		_submit->_dynamicIb = CreateDynamicIndexBuffer(VIDEO_DYNAMIC_INDEX_BUFFER_SIZE);
		DoFrame();
	}

	void Context::Shutdown()
	{
		Console::Log("Context shut down");

		GetCommandBuffer(CommandBuffer::eRendererShutdown);
		DoFrame();
		_initialized = false;
	}


	const Memory * Alloc(uint32 size)
	{
		Memory* mem = (Memory*)gRealloc(NULL, sizeof(Memory) + size);
		mem->_size = size;
		mem->_data = (uint8*)mem + sizeof(Memory);
		return mem;
	}

	const Memory * MakeRef(void * data, uint32 size)
	{
		Memory* mem = (Memory *)gRealloc(NULL, sizeof(Memory));
		mem->_size = size;
		mem->_data = (uint8 *)data;
		return mem;
	}

	void Free(Memory *mem)
	{
		gFree(mem);
	}

	void SetDebug(uint32 debug)
	{
		sContext._debug = debug;
	}

	void DEBUG_TextClear(uint8 attr, bool size)
	{
		sContext.DEBUG_TextClear(attr, size);
	}

	void DEBUG_TextPrintf(uint16 x, uint16 y, uint8 attribute, const char * format, ...)
	{
		va_list argList;
		va_start(argList, format);
		sContext.DEBUG_TextPrintfVargs(x, y, attribute, format, argList);
		va_end(argList);
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

	VertexBufferHandle CreateVertexBuffer(const Memory * mem, const VertexDecl &decl)
	{
		return sContext.CreateVertexBuffer(mem, decl);
	}

	void DestroyVertexBuffer(VertexBufferHandle handle)
	{
		return sContext.DestroyVertexBuffer(handle);
	}

	bool CheckAvailDynamicVertexBuffer(uint16 num, const VertexDecl & decl)
	{
		return sContext._submit->CheckAvailDynamicVertexBuffer(num, decl._stride);
	}

	const DynamicVertexBuffer *AllocDynamicVertexBuffer(uint16 num, const VertexDecl &decl)
	{
		return sContext.AllocDynamicVertexBuffer(num, decl);
	}

	VertexShaderHandle CreateVertexShader(const Memory * mem)
	{
		return sContext.CreateVertexShader(mem);
	}

	void DestroyVertexShader(VertexShaderHandle handle)
	{
		return sContext.DestroyVertexShader(handle);
	}

	FragmentShaderHandle CreateFragmentShader(const Memory * mem)
	{
		return sContext.CreateFragmentShader(mem);
	}

	void DestroyFragmentShader(FragmentShaderHandle handle)
	{
		return sContext.DestroyFragmentShader(handle);
	}

	SurfaceMaterialHandle CreateSurfaceMaterial(VertexShaderHandle vsh, FragmentShaderHandle fsh)
	{
		return sContext.CreateSurfaceMaterial(vsh, fsh);
	}

	void DestroySurfaceMaterial(SurfaceMaterialHandle handle)
	{
		return sContext.DestroySurfaceMaterial(handle);
	}

	TextureHandle CreateTexture(const Memory *mem, uint32 flags, uint16 *width, uint16 *height)
	{
		return sContext.CreateTexture(mem, flags, width, height);
	}

	void DestroyTexture(TextureHandle handle)
	{
		return sContext.DestroyTexture(handle);
	}

	RenderTargetHandle CreateRenderTarget(uint32 width, uint32 height, uint32 flags)
	{
		return sContext.CreateRenderTarget(width, height, flags);
	}

	void DestroyRenderTarget(RenderTargetHandle handle)
	{
		return sContext.DestroyRenderTarget(handle);
	}

	UniformHandle CreateUniform(const char * name, ConstantType::Enum type, uint16 num)
	{
		return sContext.CreateUniform(name, type, num);
	}

	void DestroyUniform(UniformHandle handle)
	{
		return sContext.DestroyUniform(handle);
	}

	void SetViewRect(uint8 id, uint32 x, uint32 y, uint32 width, uint32 height)
	{
		sContext.SetViewRect(id, x, y, width, height);
	}

	void SetViewRectMask(uint32 viewMask, uint32 x, uint32 y, uint32 width, uint32 height)
	{
		sContext.SetViewRectMask(viewMask, x, y, width, height);
	}

	void SetViewClear(uint8 id, uint8 flags, uint32 rgba, float depth, uint8 stencil)
	{
		sContext.SetViewClear(id, flags, rgba, depth, stencil);
	}

	void SetViewClearMask(uint32 viewMask, uint8 flags, uint32 rgba, float depth, uint8 stencil)
	{
		sContext.SetViewClearMask(viewMask, flags, rgba, depth, stencil);
	}

	void SetViewSeq(uint8 id, bool enabled)
	{
		sContext.SetViewSeq(id, enabled);
	}

	void SetViewSeqMask(uint32 viewMask, bool enabled)
	{
		sContext.SetViewSeqMask(viewMask, enabled);
	}

	void SetViewRenderTarget(uint8 id, RenderTargetHandle handle)
	{
		sContext.SetViewRenderTarget(id, handle);
	}

	void SetViewRenderTargetMask(uint32 viewMask, RenderTargetHandle handle)
	{
		sContext.SetViewRenderTargetMask(viewMask, handle);
	}

	void SetViewTransform(uint8 id, const Matrix *view, const Matrix *projection, uint8 other)
	{
		sContext._submit->SetViewTransform(id, view, projection, other);
	}

	void SetViewTransformMask(uint32 viewMask, const Matrix *view, const Matrix *projection, uint8 other)
	{
		sContext._submit->SetViewTransformMask(viewMask, view, projection, other);
	}

	void SetState(uint64 state)
	{
		sContext._submit->SetState(state);
	}

	uint32 SetTransform(Matrix *matrix, uint16 num)
	{
		return sContext._submit->SetTransform(matrix, num);
	}

	void SetUniform(UniformHandle handle, const void * value, uint16 num)
	{
		sContext.SetUniform(handle, value, num);
	}
	void SetUniform(SurfaceMaterialHandle material, UniformHandle handle, const void * value)
	{
		sContext.SetUniform(material, handle, value);
	}

	uint32 SetTransform(const Matrix * matrix, uint16 num)
	{
		return uint32();
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
		uint32 inumIndices = Uint32Min(numIndices, ib->_size / 2);
		sContext._submit->SetIndexBuffer(ib, inumIndices);
	}

	void SetVertexBuffer(VertexBufferHandle handle)
	{
		sContext._submit->SetVertexBuffer(handle);
	}

	void SetVertexBuffer(const DynamicVertexBuffer * vb)
	{
		sContext._submit->SetVertexBuffer(vb);
	}

	void SetSurfaceMaterial(SurfaceMaterialHandle handle)
	{
		sContext._submit->SetSurfaceMaterial(handle);
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
		//TODO Implement
		//sContext.SaveScreenShot(filePath);
	}

}
