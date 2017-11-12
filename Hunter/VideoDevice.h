#ifndef VIDEO_SYSTEM_H
#define VIDEO_SYSTEM_H


#include "VideoResource.h"

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

		//CommandBucket<uint32> &GetCommandBucket() { return _commandBucket; }

		//이 모양으로 가자....
		//IndexBufferHandle CreateIndexBuffer(DataPackage* mem, uint16 flags)
		//{
		//	IndexBufferHandle result = _indexPool.GetNextFreeHandle();
		//	_indexBuffers[result.index].Create(mem->Size(), mem->Data(), flags);
		//	return result;
		//}
		//void DestroyIndexBuffer(IndexBufferHandle handle)
		//{
		//	_indexBuffers[handle.index].Destroy();
		//}
		//void CreateVertexDecl(VertexDeclHandle handle, const VertexDecl& decl)
		//{
		//	VertexDecl& refDecl = _vertexDecls[handle.index];
		//	memcpy(&refDecl, &decl, sizeof(VertexDecl));
		//}
		//void DestroyVertexDecl(VertexDeclHandle /*handle*/)
		//{
		//}
		//void CreateVertexBuffer(VertexBufferHandle handle, DataPackage* mem,
		//	VertexDeclHandle declHandle, uint16 /*_flags*/)
		//{
		//	_vertexBuffers[handle.index].Create(mem->Size(), mem->Data(), declHandle);
		//}
		//void DestroyVertexBuffer(VertexBufferHandle handle)
		//{
		//	_vertexBuffers[handle.index].Destroy();
		//}
		//void CreateDynamicIndexBuffer(DynamicIndexBufferHandle handle, uint32_t size, uint16 flags)
		//{
		//	_indexBuffers[handle.index].Create(size, NULL, flags);
		//}
		////void UpdateDynamicIndexBuffer(IndexBufferHandle handle, uint32_t _offset,
		////	uint32_t _size, DataPackage* _mem)
		////{
		////	_indexBuffers[handle.index].Update(_offset, bx::uint32_min(_size, _mem->size), _mem->data);
		////}
		//void DestroyDynamicIndexBuffer(DynamicIndexBufferHandle handle)
		//{
		//	_indexBuffers[handle.index].Destroy();
		//}
		//void CreateDynamicVertexBuffer(DynamicVertexBufferHandle handle, uint32_t _size, uint16 /*_flags*/)
		//{
		//	VertexDeclHandle decl = INVALID_HANDLE;
		//	_vertexBuffers[handle.index].Create(_size, NULL, decl);
		//}
		////void UpdateDynamicVertexBuffer(VertexBufferHandle handle, uint32_t _offset,
		////	uint32_t _size, DataPackage* _mem)
		////{
		////	_vertexBuffers[handle.index].Update(_offset, bx::uint32_min(_size, _mem->size), _mem->data);
		////}
		//void DestroyDynamicVertexBuffer(DynamicVertexBufferHandle handle)
		//{
		//	_vertexBuffers[handle.index].Destroy();
		//}
		//void CreateEffect(EffectHandle handle, const std::string &fileName)
		//{
		//	_effects[handle.index].Create(fileName);
		//}
		//void DestroyEffect(EffectHandle handle)
		//{
		//	_effects[handle.index].Destroy();
		//}
		////void CreateTexture(TextureHandle handle, DataPackage* mem, uint32_t flags, uint8_t skip)
		////{
		////	_textures[handle.index].Create(mem, flags, skip);
		////}
		//void CreateTextureFromFile(TextureHandle handle, const std::string &fileName)
		//{
		//	_textures[handle.index].CreateFromFile(fileName);
		//}

		RenderView *GetRenderView(RenderViewHandle handle);

		VertexBufferHandle CreateVertexBuffer(Memory *memory, VertexDeclHandle declHandle, const std::string &name = "");
		void DestroyVertexBuffer(VertexBufferHandle handle);

		IndexBufferHandle CreateIndexBuffer(Memory *memory, const std::string &name = "");
		void DestroyIndexBuffer(IndexBufferHandle handle);

		VertexDeclHandle CreateVertexDecl(const VertexDecl *decl);
		void DestroyVertexDecl(VertexDeclHandle handle);

		TextureHandle CreateTexture(const std::string &fileName, const std::string &name = "");
		void DestroyTexture(TextureHandle handle);

		EffectHandle CreateEffect(const std::string &fileName, const std::string &name = "");
		void DestroyEffect(EffectHandle handle);

		RenderViewHandle CreateRenderView(const std::string &name = "");
		void DestroyRenderView(RenderViewHandle handle);

		void SetCurrentRenderView(RenderViewHandle handle);
		void SetRenderViewProjectionMatrix(RenderViewHandle handle, const Matrix &view, const Matrix &projection);
		void SetCurrentRenderViewProjectionMatrix(const Matrix &view, const Matrix &projection);


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

		ResourceHandlePool<VertexBufferHandle> _vertexBufferPool;
		ResourceHandlePool<IndexBufferHandle> _indexBufferPool;
		ResourceHandlePool<TextureHandle> _textureHandlePool;
		ResourceHandlePool<EffectHandle> _effectHandlePool;
		ResourceHandlePool<VertexDeclHandle> _vertexDeclHandlePool;

		ResourceHandlePool<RenderViewHandle> _renderViewHandlePool;
	};
}
#endif