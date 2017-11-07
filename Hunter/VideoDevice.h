#ifndef VIDEO_SYSTEM_H
#define VIDEO_SYSTEM_H

#include "System.h"

//#include "VisualComponent.h"


#include "imguiRenderer.h"
#include "DebugDraw.h"

#include "Command.h"
#include "CommandBucket.h"

#include "VertexBufferManager.h"
#include "IndexBufferManager.h"

//defines 
#define VIDEO (gEngine->GetVideo())

struct RenderWindow
{
	int32 width;
	int32 height;

	D3DFORMAT backBufferFormat;
	D3DFORMAT depthStencilFormat;
	HWND windowHandle;
};

class VideoDevice : public System
{
public :
	VideoDevice();
	virtual ~VideoDevice();

	virtual bool Init(const std::string &name, const SystemSetting &setting);
	virtual void ShutDown();

	virtual void Update(float deltaTime);
	void Render();

	LPDIRECT3DDEVICE9 GetDevice()
	{
		return _pDevice;
	}
	virtual EventChannel GetChannel() { return _channel; }

	bool32 Draw(VertexBufferHandle vertexHandle,
		uint32 startVertex, uint32 primitiveCount, uint32 matrixIndex);
	bool32 DrawIndexed(VertexBufferHandle vertexHandle, IndexBufferHandle indexHandle, 
		uint32 numVertex, uint32 startIndex, uint32 primitiveCount, uint32 matrixIndex);

	CommandBucket<uint32> &GetCommandBucket() { return _commandBucket; }

	VertexBufferManager *GetVertexBufferManager() { return &_vertexBufferManager; }
	IndexBufferManager *GetIndexBufferManager() { return &_indexBufferManager; }

	MatrixCache &GetMatrixCache() { return _matrixCache; }

private :
	bool InitD3D(HWND windowHandle);
	
	bool InitDefaultRenderState();

	bool PerformRender();

	LPDIRECT3D9 _pD3D;
	LPDIRECT3DDEVICE9 _pDevice;

	D3DPRESENT_PARAMETERS _presentParams;

	RenderWindow _renderWindow;

	//im::GuiRenderer *_pimguiRenderer;

	CommandBucket<uint32> _commandBucket;

	VertexBufferManager _vertexBufferManager;
	IndexBufferManager _indexBufferManager;
	MatrixCache _matrixCache;
	//ResourceManager<IndexBuffer> _indexBufferManager;
	//ResourceManager<Texture> _textureManager;
	//ResourceManager<Effect> _effectManager;
	//ResourceManager<RenderTarget> _renderTargetManager;
};

namespace backend
{
	
}

#endif