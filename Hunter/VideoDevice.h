#ifndef VIDEO_SYSTEM_H
#define VIDEO_SYSTEM_H


#include "VideoResource.h"
#include "VideoResource_Mesh.h"

#include "imguiRenderer.h"

#ifdef CreateFont
	#undef CreateFont
#endif

//TODOs
//1. Texture Update를 만들자 Lock, Unlock, UpdateContents
//2. Texture CreateFromMemory를 만들자
//3. PredefinedUniforms를 만들자

//defines 
#define VIDEO (gEngine->GetVideo())

constexpr uint16 INVALID_BUFFER = 0;

namespace video
{
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
		~VideoDevice();

		//VideoDevice(const VideoDevice &other) = delete;
		bool Init();
		void ShutDown();

		void Update(float deltaTime);

		void Render(RenderView &renderView);

		LPDIRECT3DDEVICE9 GetDevice() { return _pDevice; }
		EventChannel GetChannel() { return _channel; }


		//버텍스 버퍼 생성
		VertexBufferHandle CreateVertexBuffer(Memory *memory, VertexDeclHandle declHandle, const std::string &name = "");
		VertexBufferHandle GetVertexBufferFromXMesh(ID3DXMesh *pMesh, const std::string &name = "");
		VertexBufferHandle GetVertexBuffer(const std::string &name);
		VertexBuffer *GetVertexBuffer(VertexBufferHandle handle);
		void DestroyVertexBuffer(VertexBufferHandle handle);
		//uint8 *UpdateVertexBuffer(VertexBufferHandle handle, Memory *memory);

		//인덱스 버퍼 생성
		IndexBufferHandle CreateIndexBuffer(Memory *memory, uint32 stride, const std::string &name = "");
		IndexBufferHandle GetIndexBufferFromXMesh(ID3DXMesh *pMesh, const std::string &name = "");
		IndexBufferHandle GetIndexBuffer(const std::string &name);
		IndexBuffer *GetIndexBuffer(IndexBufferHandle handle);
		void DestroyIndexBuffer(IndexBufferHandle handle);
		//uint8 *UpdateIndexBuffer(IndexBufferHandle handle, Memory *memory);

		//버텍스 데클 생성
		VertexDeclHandle CreateVertexDecl(const VertexDecl *decl, const std::string &name = "");
		VertexDeclHandle GetVertexDecl(const std::string &name);
		VertexDecl *GetVertexDecl(VertexDeclHandle handle);
		void DestroyVertexDecl(VertexDeclHandle handle);

		//텍스쳐 생성
		//파일로부터 텍스쳐를 생성한다.
		TextureHandle CreateTexture(const std::string &fileName, const std::string &name = "");
		//주어진 인자로부터 빈 텍스쳐를 생성한다.
		TextureHandle CreateTexture(int32 width, int32 height, D3DFORMAT format, D3DPOOL pool, 
			const std::string &name = "");

		TextureHandle GetTexture(const std::string &name);
		Texture *GetTexture(TextureHandle handle);
		void DestroyTexture(TextureHandle handle);

		//이펙트 생성
		EffectHandle CreateEffect(const std::string &fileName, const std::string &name = "");
		EffectHandle GetEffect(const std::string &name);
		Effect *GetEffect(EffectHandle handle);
		void DestroyEffect(EffectHandle handle);

		//렌더 뷰 생성
		RenderViewHandle CreateRenderView(const std::string &name = "");
		RenderViewHandle GetRenderView(const std::string &name);
		RenderView *GetRenderView(RenderViewHandle handle);
		void DestroyRenderView(RenderViewHandle handle);
		
		MaterialHandle CreateMaterial(const std::string &name = "");
		MaterialHandle GetMaterial(const std::string &name);
		Material *GetMaterial(MaterialHandle handle);
		void DestroyMaterial(MaterialHandle handle);
		void MaterialAddTexture(MaterialHandle material, uint32 textureSlot, TextureHandle texture);

		StaticXMeshHandle CreateStaticXMesh(const std::string fileName, const Matrix *pCorrection, const std::string &name);
		StaticXMeshHandle GetStaticXMesh(const std::string &name);
		StaticXMesh *GetStaticXMesh(StaticXMeshHandle handle);
		void  DestroyStaticXMesh(StaticXMeshHandle handle);

		SkinnedXMeshHandle CreateSkinnedXMesh(const std::string fileName, const Matrix *pCorrection, const std::string &name);
		SkinnedXMeshHandle GetSkinnedXMesh(const std::string &name);
		SkinnedXMesh *GetSkinnedXMesh(SkinnedXMeshHandle handle);
		void DestroySkinnedMesh(SkinnedXMeshHandle handle);

		AnimationInstanceHandle CreateAnimationInstance(SkinnedXMeshHandle xMesh, const std::string &name);
		AnimationInstanceHandle GetAnimationInstance(const std::string &name);
		AnimationInstance *GetAnimationInstance(AnimationInstanceHandle handle);
		void DestroyAnimationInstance(AnimationInstanceHandle handle);


		//Font Functions
		FontHandle CreateFont(const D3DXFONT_DESC &fontDesc, const std::string &name);
		FontHandle GetFont(const std::string &name);
		Font *GetFont(FontHandle handle);
		void DestroyFont(FontHandle handle);
		void DrawFont(FontHandle handle, const std::string &str, int32 x, int32 y, uint32 color);
		void DrawFontShadow(FontHandle handle, const std::string &str, int32 x, int32 y, uint32 color, uint32 shadow = 0xff000000);
		void GetBoundingRect(FontHandle handle, const std::string &str, int32 x, int32 y, RECT *pOutRect);

	private:
		//Private Functions
		bool InitD3D(HWND windowHandle);

		bool InitDefaultRenderState();

		//void DrawWithEffect(const video::RenderState &renderState, Matrix *matrices);
		//void DrawWithoutEffect(const video::RenderState &renderState, Matrix *matrices);

		void LoadDefaultTextures();
		void MakeDefaultVertexDecls();
		void LoadDefaultEffects();

	private :
		//Private Variables

		LPDIRECT3D9 _pD3D;
		LPDIRECT3DDEVICE9 _pDevice;

		D3DPRESENT_PARAMETERS _presentParams;

		IDirect3DSwapChain9 *_additionalSwapChain{};

		RenderWindow _renderWindow;

		EventChannel _channel;

		VertexBuffer _vertexBuffers[VIDEO_CONFIG_VERTEXBUFFER_MAX_NUM];
		IndexBuffer _indexBuffers[VIDEO_CONFIG_INDEXBUFFER_MAX_NUM];
		Texture _textures[VIDEO_CONFIG_TEXTURE_MAX_NUM];
		Effect _effects[VIDEO_CONFIG_EFFECT_MAX_NUM];
		VertexDecl _vertexDecls[VIDEO_CONFIG_DECL_MAX_NUM];

		RenderView _renderViews[VIDEO_CONFIG_RENDER_VIEW_MAX_NUM];
		RenderView *_pCurrentView{};

		Material _materials[VIDEO_CONFIG_MATERIAL_MAX_NUM];

		StaticXMesh _staticMeshes[VIDEO_CONFIG_STATIC_XMESH_MAX_NUM];
		SkinnedXMesh _skinnedMeshes[VIDEO_CONFIG_SKINNED_XMESH_MAX_NUM];
		AnimationInstance _animationInstances[VIDEO_CONFIG_ANIMATION_MAX_NUM];

		Font _fonts[VIDEO_CONFIG_FONT_MAX_NUM];

		ResourceHandlePool<VertexBufferHandle> _vertexBufferPool;
		ResourceHandlePool<IndexBufferHandle> _indexBufferPool;
		ResourceHandlePool<TextureHandle> _textureHandlePool;
		ResourceHandlePool<EffectHandle> _effectHandlePool;
		ResourceHandlePool<VertexDeclHandle> _vertexDeclHandlePool;

		ResourceHandlePool<RenderViewHandle> _renderViewHandlePool;

		ResourceHandlePool<MaterialHandle> _materialHandlePool;

		ResourceHandlePool<StaticXMeshHandle> _staticXMeshHandlePool;
		ResourceHandlePool<SkinnedXMeshHandle> _skinnedXMeshHandlePool;

		ResourceHandlePool<AnimationInstanceHandle> _animationInstanceHandlePool;
		ResourceHandlePool<FontHandle> _fontHandlePool;
	};
}
#endif