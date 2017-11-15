#ifndef VIDEO_SYSTEM_H
#define VIDEO_SYSTEM_H


#include "VideoResource.h"
#include "VideoResource_Mesh.h"

#include "imguiRenderer.h"
#include "DebugDraw.h"


//TODOs
//1. DynamicVertexBuffer, DynamicIndexBuffer를 만들자... Update
//2. Texture Update를 만들자 Lock, Unlock, UpdateContents
//3. Texture CreateFromMemory를 만들자
//4. PredefinedUniforms를 만들자

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

		RenderView *GetRenderView(RenderViewHandle handle);

		//버텍스 버퍼 생성
		VertexBufferHandle CreateVertexBuffer(Memory *memory, VertexDeclHandle declHandle, const std::string &name = "");
		VertexBufferHandle GetVertexBuffer(const std::string &name);
		const VertexBuffer *GetVertexBuffer(VertexBufferHandle handle);
		void DestroyVertexBuffer(VertexBufferHandle handle);

		//인덱스 버퍼 생성
		IndexBufferHandle CreateIndexBuffer(Memory *memory, const std::string &name = "");
		IndexBufferHandle GetIndexBuffer(const std::string &name);
		const IndexBuffer *GetIndexBuffer(IndexBufferHandle handle);
		void DestroyIndexBuffer(IndexBufferHandle handle);

		//버텍스 데클 생성
		VertexDeclHandle CreateVertexDecl(const VertexDecl *decl, const std::string &name = "");
		VertexDeclHandle GetVertexDecl(const std::string &name);
		const VertexDecl &GetVertexDecl(VertexDeclHandle handle);
		void DestroyVertexDecl(VertexDeclHandle handle);

		//텍스쳐 생성
		TextureHandle CreateTexture(const std::string &fileName, const std::string &name = "");
		TextureHandle GetTexture(const std::string &name);
		const Texture *GetTexture(TextureHandle handle);
		void DestroyTexture(TextureHandle handle);

		//이펙트 생성
		EffectHandle CreateEffect(const std::string &fileName, const std::string &name = "");
		EffectHandle GetEffect(const std::string &name);
		const Effect *GetEffect(EffectHandle handle);
		void DestroyEffect(EffectHandle handle);

		//렌더 뷰 생성
		RenderViewHandle CreateRenderView(const std::string &name = "");
		RenderViewHandle GetRenderView(const std::string &name);
		void DestroyRenderView(RenderViewHandle handle);
		
		MaterialHandle CreateMaterial(const std::string &name = "");
		MaterialHandle GetMaterial(const std::string &name);
		const Material *GetMaterial(MaterialHandle handle);
		void DestroyMaterial(MaterialHandle handle);
		void MaterialAddTexture(MaterialHandle material, uint32 textureSlot, TextureHandle texture);

		void SetCurrentRenderView(RenderViewHandle handle);
		void SetRenderViewProjectionMatrix(RenderViewHandle handle, const Matrix &view, const Matrix &projection);
		void SetCurrentRenderViewProjectionMatrix(const Matrix &view, const Matrix &projection);

		//CreateModelFromX 함수는 불러온 파일 경로에서 이름을 추출하여 HandlePool에 자동으로 이름을 등록한다
		RenderGroupHandle CreateRenderGroup(video::VertexBufferHandle vHandle, video::IndexBufferHandle iHandle, 
			const RenderGroup::MaterialRange &materialRange, const std::string &name = "");
		RenderGroupHandle GetRenderGroup(const std::string &name);
		void DestroyRenderGroup(RenderGroupHandle handle);
		void RenderGroupSetEffect(RenderGroupHandle group, EffectHandle effect);

		StaticXMeshHandle CreateStaticXMesh(const std::string fileName, const Matrix *pCorrection, const std::string &name);
		StaticXMeshHandle GetStaticXMesh(const std::string &name);
		const StaticXMesh *GetStaticXMesh(StaticXMeshHandle handle);
		void  DestroyStaticXMesh(StaticXMeshHandle handle);

		SkinnedXMeshHandle CreateSkinnedXMesh(const std::string fileName, const Matrix *pCorrection, const std::string &name);
		SkinnedXMeshHandle GetSkinnedXMesh(const std::string &name);
		const SkinnedXMesh *GetSkinnedXMesh(SkinnedXMeshHandle handle);
		void DestroySkinnedMesh(SkinnedXMeshHandle handle);


	private:
		//Private Functions
		bool InitD3D(HWND windowHandle);

		bool InitDefaultRenderState();

		void DrawWithEffect(const video::RenderState &renderState, Matrix *matrices);
		void DrawWithoutEffect(const video::RenderState &renderState, Matrix *matrices);

		//void DrawPrimitive(const video::RenderState &renderState, Matrix *matrices);
		//void DrawIndexPrimitive(const video::RenderState &renderState, Matrix *matrices);

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

		RenderState _activeState;

		VertexBuffer _vertexBuffers[VIDEO_CONFIG_VERTEXBUFFER_MAX_NUM];
		IndexBuffer _indexBuffers[VIDEO_CONFIG_INDEXBUFFER_MAX_NUM];
		Texture _textures[VIDEO_CONFIG_TEXTURE_MAX_NUM];
		Effect _effects[VIDEO_CONFIG_EFFECT_MAX_NUM];
		VertexDecl _vertexDecls[VIDEO_CONFIG_DECL_MAX_NUM];

		RenderView _renderViews[VIDEO_CONFIG_RENDER_VIEW_MAX_NUM];
		RenderView *_pCurrentView{};

		Material _materials[VIDEO_CONFIG_MATERIAL_MAX_NUM];
		RenderGroup _renderGroups[VIDEO_CONFIG_RENDER_GROUP_MAX_NUM];

		StaticXMesh _staticMeshes[VIDEO_CONFIG_STATIC_XMESH_MAX_NUM];
		SkinnedXMesh _skinnedMeshes[VIDEO_CONFIG_SKINNED_XMESH_MAX_NUM];


		ResourceHandlePool<VertexBufferHandle> _vertexBufferPool;
		ResourceHandlePool<IndexBufferHandle> _indexBufferPool;
		ResourceHandlePool<TextureHandle> _textureHandlePool;
		ResourceHandlePool<EffectHandle> _effectHandlePool;
		ResourceHandlePool<VertexDeclHandle> _vertexDeclHandlePool;

		ResourceHandlePool<RenderViewHandle> _renderViewHandlePool;

		ResourceHandlePool<MaterialHandle> _materialHandlePool;
		ResourceHandlePool<RenderGroupHandle> _renderGroupHandlePool;

		ResourceHandlePool<video::StaticXMeshHandle> _staticXMeshHandlePool;
		ResourceHandlePool<video::SkinnedXMeshHandle> _skinnedXMeshHandlePool;
	};
}
#endif