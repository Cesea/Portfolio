#ifndef VIDEO_SYSTEM_H
#define VIDEO_SYSTEM_H


#include "VideoResource.h"

#include "imguiRenderer.h"
#include "DebugDraw.h"

#include "DXUtils.h"


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
		void DestroyVertexBuffer(VertexBufferHandle handle);

		//인덱스 버퍼 생성
		IndexBufferHandle CreateIndexBuffer(Memory *memory, const std::string &name = "");
		IndexBufferHandle GetIndexBuffer(const std::string &name);
		void DestroyIndexBuffer(IndexBufferHandle handle);

		//버텍스 데클 생성
		VertexDeclHandle CreateVertexDecl(const VertexDecl *decl);
		VertexDeclHandle GetVertexDecl(const std::string &name);
		void DestroyVertexDecl(VertexDeclHandle handle);

		//텍스쳐 생성
		TextureHandle CreateTexture(const std::string &fileName, const std::string &name = "");
		TextureHandle GetTexture(const std::string &name);
		void DestroyTexture(TextureHandle handle);

		//이펙트 생성
		EffectHandle CreateEffect(const std::string &fileName, const std::string &name = "");
		EffectHandle GetEffect(const std::string &name);
		void DestroyEffect(EffectHandle handle);

		//렌더 뷰 생성
		RenderViewHandle CreateRenderView(const std::string &name = "");
		RenderViewHandle GetRenderView(const std::string &name);
		void DestroyRenderView(RenderViewHandle handle);
		
		MaterialHandle CreateMaterial(const std::string &name = "");
		MaterialHandle GetMaterial(const std::string &name);
		void DestroyMaterial(MaterialHandle handle);

		void AddTextureToMaterial(MaterialHandle material, uint32 textureSlot, TextureHandle texture);

		void SetCurrentRenderView(RenderViewHandle handle);
		void SetRenderViewProjectionMatrix(RenderViewHandle handle, const Matrix &view, const Matrix &projection);
		void SetCurrentRenderViewProjectionMatrix(const Matrix &view, const Matrix &projection);

		ModelHandle CreateModelFromX(const std::string &fileName, const std::string &name = "");
		ModelHandle GetModel(const std::string &name);
		void DestroyModel(ModelHandle handle);


	private:
		bool InitD3D(HWND windowHandle);

		bool InitDefaultRenderState();

		void DrawPrimitive(const video::RenderState &renderState, Matrix *matrices);
		void DrawIndexPrimitive(const video::RenderState &renderState, Matrix *matrices);

	private :

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
		Model _models[VIDEO_CONFIG_MODEL_MAX_NUM];

		ResourceHandlePool<VertexBufferHandle> _vertexBufferPool;
		ResourceHandlePool<IndexBufferHandle> _indexBufferPool;
		ResourceHandlePool<TextureHandle> _textureHandlePool;
		ResourceHandlePool<EffectHandle> _effectHandlePool;
		ResourceHandlePool<VertexDeclHandle> _vertexDeclHandlePool;

		ResourceHandlePool<RenderViewHandle> _renderViewHandlePool;

		ResourceHandlePool<MaterialHandle> _materialHandlePool;
		ResourceHandlePool<ModelHandle> _modelHandlePool;

	};
}
#endif