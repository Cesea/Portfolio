#ifndef VIDEO_H
#define VIDEO_H

#include "Typedefs.h"

namespace video
{
	#define VIDEO_STATE_DEPTH_WRITE          UINT64_C(0x0000000000000001)

	#define VIDEO_STATE_ALPHA_TEST           UINT64_C(0x0000000000000004)
	#define VIDEO_STATE_ALPHA_WRITE          UINT64_C(0x0000000000000008)
	#define VIDEO_STATE_ALPHA_MASK           UINT64_C(0x000000000000000c)

	#define VIDEO_STATE_DEPTH_TEST_LESS      UINT64_C(0x0000000000000010)
	#define VIDEO_STATE_DEPTH_TEST_LEQUAL    UINT64_C(0x0000000000000020)
	#define VIDEO_STATE_DEPTH_TEST_EQUAL     UINT64_C(0x0000000000000030)
	#define VIDEO_STATE_DEPTH_TEST_GEQUAL    UINT64_C(0x0000000000000040)
	#define VIDEO_STATE_DEPTH_TEST_GREATER   UINT64_C(0x0000000000000050)
	#define VIDEO_STATE_DEPTH_TEST_NOTEQUAL  UINT64_C(0x0000000000000060)
	#define VIDEO_STATE_DEPTH_TEST_NEVER     UINT64_C(0x0000000000000070)
	#define VIDEO_STATE_DEPTH_TEST_ALWAYS    UINT64_C(0x0000000000000080)
	#define VIDEO_STATE_DEPTH_TEST_SHIFT     4
	#define VIDEO_STATE_DEPTH_TEST_MASK      UINT64_C(0x00000000000000f0)

	#define VIDEO_STATE_ALPHA_TEST_LESS      UINT64_C(0x0000000000000100)
	#define VIDEO_STATE_ALPHA_TEST_LEQUAL    UINT64_C(0x0000000000000200)
	#define VIDEO_STATE_ALPHA_TEST_EQUAL     UINT64_C(0x0000000000000300)
	#define VIDEO_STATE_ALPHA_TEST_GEQUAL    UINT64_C(0x0000000000000400)
	#define VIDEO_STATE_ALPHA_TEST_GREATER   UINT64_C(0x0000000000000500)
	#define VIDEO_STATE_ALPHA_TEST_NOTEQUAL  UINT64_C(0x0000000000000600)
	#define VIDEO_STATE_ALPHA_TEST_NEVER     UINT64_C(0x0000000000000700)
	#define VIDEO_STATE_ALPHA_TEST_ALWAYS    UINT64_C(0x0000000000000800)
	#define VIDEO_STATE_ALPHA_TEST_SHIFT     8
	#define VIDEO_STATE_ALPHA_TEST_MASK      UINT64_C(0x0000000000000f00)

	#define VIDEO_STATE_BLEND_ZERO           UINT64_C(0x0000000000001000)
	#define VIDEO_STATE_BLEND_ONE            UINT64_C(0x0000000000002000)
	#define VIDEO_STATE_BLEND_SRC_COLOR      UINT64_C(0x0000000000003000)
	#define VIDEO_STATE_BLEND_INV_SRC_COLOR  UINT64_C(0x0000000000004000)
	#define VIDEO_STATE_BLEND_SRC_ALPHA      UINT64_C(0x0000000000005000)
	#define VIDEO_STATE_BLEND_INV_SRC_ALPHA  UINT64_C(0x0000000000006000)
	#define VIDEO_STATE_BLEND_DST_ALPHA      UINT64_C(0x0000000000007000)
	#define VIDEO_STATE_BLEND_INV_DST_ALPHA  UINT64_C(0x0000000000008000)
	#define VIDEO_STATE_BLEND_DST_COLOR      UINT64_C(0x0000000000009000)
	#define VIDEO_STATE_BLEND_INV_DST_COLOR  UINT64_C(0x000000000000a000)
	#define VIDEO_STATE_BLEND_SRC_ALPHA_SAT  UINT64_C(0x000000000000b000)
	#define VIDEO_STATE_BLEND_SHIFT          12
	#define VIDEO_STATE_BLEND_MASK           UINT64_C(0x000000000ffff000)

	#define VIDEO_STATE_CULL_CW              UINT64_C(0x0000000010000000)
	#define VIDEO_STATE_CULL_CCW             UINT64_C(0x0000000020000000)
	#define VIDEO_STATE_CULL_SHIFT           28
	#define VIDEO_STATE_CULL_MASK            UINT64_C(0x0000000030000000)

	#define VIDEO_STATE_RGB_WRITE            UINT64_C(0x0000000040000000)

	#define VIDEO_STATE_ALPHA_REF_SHIFT      32
	#define VIDEO_STATE_ALPHA_REF_MASK       UINT64_C(0x000000ff00000000)

	#define VIDEO_STATE_PT_LINES             UINT64_C(0x0000010000000000)
	#define VIDEO_STATE_PT_SHIFT             40
	#define VIDEO_STATE_PT_MASK              UINT64_C(0x0000010000000000)

	#define VIDEO_STATE_NONE                 UINT64_C(0x0000000000000000)
	#define VIDEO_STATE_MASK                 UINT64_C(0xffffffffffffffff)
	#define VIDEO_STATE_DEFAULT (0 \
			| VIDEO_STATE_RGB_WRITE \
			| VIDEO_STATE_ALPHA_WRITE \
			| VIDEO_STATE_DEPTH_TEST_LESS \
			| VIDEO_STATE_DEPTH_WRITE \
			| VIDEO_STATE_CULL_CW \
			)

	#define VIDEO_CLEAR_NONE                 UINT8_C(0x00)
	#define VIDEO_CLEAR_COLOR_BIT            UINT8_C(0x01)
	#define VIDEO_CLEAR_DEPTH_BIT            UINT8_C(0x02)
	#define VIDEO_CLEAR_STENCIL_BIT          UINT8_C(0x04)

	#define VIDEO_DEBUG_NONE                 UINT32_C(0x00000000)
	#define VIDEO_DEBUG_WIREFRAME            UINT32_C(0x00000001)
	#define VIDEO_DEBUG_IFH                  UINT32_C(0x00000002)
	#define VIDEO_DEBUG_STATS                UINT32_C(0x00000004)
	#define VIDEO_DEBUG_TEXT                 UINT32_C(0x00000008)

	#define VIDEO_TEXTURE_NONE               UINT32_C(0x00000000)
	#define VIDEO_TEXTURE_U_REPEAT           UINT32_C(0x00000000)
	#define VIDEO_TEXTURE_U_MIRROR           UINT32_C(0x00000001)
	#define VIDEO_TEXTURE_U_CLAMP            UINT32_C(0x00000002)
	#define VIDEO_TEXTURE_U_SHIFT            0
	#define VIDEO_TEXTURE_U_MASK             UINT32_C(0x00000003)
	#define VIDEO_TEXTURE_V_REPEAT           UINT32_C(0x00000000)
	#define VIDEO_TEXTURE_V_MIRROR           UINT32_C(0x00000010)
	#define VIDEO_TEXTURE_V_CLAMP            UINT32_C(0x00000020)
	#define VIDEO_TEXTURE_V_SHIFT            4
	#define VIDEO_TEXTURE_V_MASK             UINT32_C(0x00000030)
	#define VIDEO_TEXTURE_MIN_POINT          UINT32_C(0x00000100)
	#define VIDEO_TEXTURE_MIN_SHIFT          8
	#define VIDEO_TEXTURE_MIN_MASK           UINT32_C(0x00000100)
	#define VIDEO_TEXTURE_MAG_POINT          UINT32_C(0x00001000)
	#define VIDEO_TEXTURE_MAG_SHIFT          12
	#define VIDEO_TEXTURE_MAG_MASK           UINT32_C(0x00001000)
	#define VIDEO_TEXTURE_MIP_POINT          UINT32_C(0x00010000)
	#define VIDEO_TEXTURE_MIP_SHIFT          16
	#define VIDEO_TEXTURE_MIP_MASK           UINT32_C(0x00010000)

	#define VIDEO_RENDER_TARGET_NONE         UINT32_C(0x00000000)
	#define VIDEO_RENDER_TARGET_COLOR_RGBA   UINT32_C(0x00000001)
	#define VIDEO_RENDER_TARGET_COLOR_R32F   UINT32_C(0x00000002)
	#define VIDEO_RENDER_TARGET_COLOR_SHIFT  0
	#define VIDEO_RENDER_TARGET_COLOR_MASK   UINT32_C(0x000000ff)
	#define VIDEO_RENDER_TARGET_DEPTH        UINT32_C(0x00000100)
	#define VIDEO_RENDER_TARGET_DEPTH_SHIFT  8
	#define VIDEO_RENDER_TARGET_DEPTH_MASK   UINT32_C(0x0000ff00)

	#define VIDEO_RESET_NONE                 UINT32_C(0x00000000)
	#define VIDEO_RESET_FULLSCREEN           UINT32_C(0x00000001)
	#define VIDEO_RESET_FULLSCREEN_FAKE      UINT32_C(0x00000002)
	#define VIDEO_RESET_FULLSCREEN_SHIFT     0
	#define VIDEO_RESET_FULLSCREEN_MASK      UINT32_C(0x00000003)
	#define VIDEO_RESET_MSAA_X2              UINT32_C(0x00000010)
	#define VIDEO_RESET_MSAA_X4              UINT32_C(0x00000020)
	#define VIDEO_RESET_MSAA_X8              UINT32_C(0x00000030)
	#define VIDEO_RESET_MSAA_X16             UINT32_C(0x00000040)
	#define VIDEO_RESET_MSAA_SHIFT           4
	#define VIDEO_RESET_MSAA_MASK            UINT32_C(0x00000070)
	#define VIDEO_RESET_VSYNC                UINT32_C(0x00000080)


	static const uint16 invalidHandle = 0xffff;

	#define INVALID_HANDLE (video::invalidHandle)

	struct Attrib
	{
		enum Enum
		{
			ePosition = 0,
			eNormal,
			eColor0,
			eColor1,
			eIndices,
			eWeight,
			eTexCoord0,
			eTexCoord1,
			eTexCoord2,
			eTexCoord3,
			eTexCoord4,
			eTexCoord5,
			eTexCoord6,
			eTexCoord7,
			Count,
		};
	};

	struct AttribType
	{
		enum Enum
		{
			eUint8,
			eUint16,
			eFloat,
			Count,
		};
	};

	struct VertexBufferHandle { uint16 index; };
	struct VertexDeclHandle { uint16 index; };
	struct IndexBufferHandle { uint16 index; };
	struct SurfaceMaterialHandle { uint16 index; };
	struct UniformHandle { uint16 index; };
	struct TextureHandle { uint16 index; };
	struct VertexShaderHandle { uint16 index; };
	struct FragmentShaderHandle { uint16 index; };
	struct RenderTargetHandle { uint16 index; };

	struct ConstantType
	{
		enum Enum
		{
			eUniform1i,
			eUniform1f,
			eEnd,

			eUniform1iv,
			eUniform1fv,
			eUniform2fv,
			eUniform3fv,
			eUniform4fv,
			eUniform3x3fv,
			eUniform4x4fv,
			Count
		};
	};


	struct VertexDecl
	{
		void Begin();
		void End();
		void Add(Attrib::Enum attrib, uint8 num, AttribType::Enum type, bool normalized = false);
		void Decode(Attrib::Enum attrib, uint8 &num, AttribType::Enum &type, bool &normalized) const;

		uint32 _hash;
		uint16 _stride;
		uint16 _offset[Attrib::Count];
		uint8 _attributes[Attrib::Count];
	};

	struct Memory
	{
		uint8 *_data;
		uint32 _size;
	};

	struct DynamicIndexBuffer
	{
		uint8 *_data;
		uint32 _size;
		IndexBufferHandle _handle;
		uint32 _startIndex;
	};

	struct DynamicVertexBuffer
	{
		uint8* _data;
		uint32 _size;
		uint32 _startVertex;
		uint16 _stride;
		VertexBufferHandle _handle;
		VertexDeclHandle _decl;
	};

	typedef void* (*ReallocFunction)(void* ptr, size_t size);
	typedef void(*FreeFunction)(void* ptr);

	void Init(bool _createRenderThread = true, ReallocFunction _realloc = nullptr, FreeFunction free = nullptr);
	void Shutdown();
	void Reset(uint32 width, uint32 height, uint32 flags = VIDEO_RESET_NONE);
	void DoFrame();
	const Memory* Alloc(uint32 size);
	const Memory* MakeRef(void* data, uint32 size);

	void SetDebug(uint32 debug);
	void DEBUG_TextClear(uint8 attr = 0, bool size = false);
	void DEBUG_TextPrintf(uint16 x, uint16 y, uint8 attribute, const char* format, ...);

	IndexBufferHandle CreateIndexBuffer(const Memory* mem);
	void DestroyIndexBuffer(IndexBufferHandle handle);
	bool CheckAvailDynamicIndexBuffer(uint16 num);
	const DynamicIndexBuffer* AllocDynamicIndexBuffer(uint16 num);
	void DestroyVertexBuffer(VertexBufferHandle handle);
	bool CheckAvailDynamicVertexBuffer(uint16 num, const VertexDecl& decl);
	const DynamicVertexBuffer* AllocDynamicVertexBuffer(uint16 num, const VertexDecl& decl);

	VertexShaderHandle CreateVertexShader(const Memory* mem);
	void DestroyVertexShader(VertexShaderHandle handle);
	FragmentShaderHandle CreateFragmentShader(const Memory* mem);
	void DestroyFragmentShader(FragmentShaderHandle handle);

	SurfaceMaterialHandle CreateSurfaceMaterial(VertexShaderHandle vsh, FragmentShaderHandle fsh);
	void DestroySurfaceMaterial(SurfaceMaterialHandle handle);

	TextureHandle CreateTexture(const Memory* mem, uint32 flags = VIDEO_TEXTURE_NONE,
		uint16* width = nullptr, uint16* height = nullptr);
	void DestroyTexture(TextureHandle handle);

	RenderTargetHandle CreateRenderTarget(uint32 width, uint32 height, uint32 flags = VIDEO_RENDER_TARGET_COLOR_RGBA);
	void DestroyRenderTarget(RenderTargetHandle handle);

	UniformHandle CreateUniform(const char *name, ConstantType::Enum type, uint16 num = 1);
	void DestroyUniform(UniformHandle handle);

	void SetViewRect(uint8 id, uint32 x, uint32 y, uint32 width, uint32 height);
	void SetViewRectMask(uint32 viewMask, uint32 x, uint32 y, uint32 width, uint32 height);
	void SetViewClear(uint8 id, uint8 flags, uint32 rgba = 0x000000ff, float depth = 1.0f, uint8 stencil = 0);
	void SetViewClearMask(uint32 viewMask, uint8 flags, uint32 rgba = 0x000000ff,
		float depth = 1.0f, uint8 stencil = 0);

	void SetViewSeq(uint8 id, bool enabled);
	void SetViewSeqMask(uint32 viewMask, bool enabled);

	void SetViewRenderTarget(uint8 id, RenderTargetHandle handle);
	void SetViewRenderTargetMask(uint32 viewMask, RenderTargetHandle handle);

	void SetViewTransform(uint8 id, const Matrix *view, const Matrix *projection, uint8 other = 0xff);
	void SetViewTransformMask(uint32 viewMask, const Matrix *view, const Matrix *projection, uint8 other = 0xff);

	void SetState(uint64 state);

	uint32 SetTransform(const Matrix* matrix, uint16 num = 1);
	void SetTransform(uint32 cache = 0, uint32 num = 1);

	void SetUniform(UniformHandle handle, const void *value, uint16 num = 1);

	void SetIndexBuffer(IndexBufferHandle handle, uint32 firstIndex, uint32 numIndices);
	void SetIndexBuffer(IndexBufferHandle handle);
	void SetIndexBuffer(const DynamicIndexBuffer* ib, uint32 numIndices = 0xffffffff);
	void SetVertexBuffer(VertexBufferHandle handle);
	void SetVertexBuffer(const DynamicVertexBuffer* vb);
	void SetSurfaceMaterial(SurfaceMaterialHandle handle);
	void SetTexture(uint8 stage, UniformHandle sampler, TextureHandle handle);
	void SetTexture(uint8 stage, UniformHandle sampler, RenderTargetHandle handle, bool depth = false);
	void Submit(uint8 id);
	void SubmitMask(uint32 viewMask);
	void SaveScreenShot(const char* filePath);
	void RenderFrame();
}//video

#endif