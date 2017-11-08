#ifndef VIDEO_SYSTEM_H
#define VIDEO_SYSTEM_H

#include "imguiRenderer.h"
#include "DebugDraw.h"

#include "Command.h"
#include "CommandBucket.h"

//defines 
#define VIDEO (gEngine->GetVideo())

constexpr uint32 VERTEXBUFFER_MAX_NUM = 1024;
constexpr uint32 INDEXBUFFER_MAX_NUM = 1024;
constexpr uint32 TEXTURE_MAX_NUM = 256;
constexpr uint32 EFFECT_MAX_NUM = 64;


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
public :
	VideoDevice();
	virtual ~VideoDevice();

	bool Init();
	void ShutDown();

	void Update(float deltaTime);
	void Render();

	LPDIRECT3DDEVICE9 GetDevice()
	{
		return _pDevice;
	}
	EventChannel GetChannel() { return _channel; }

	bool32 Draw(VertexBufferHandle vertexHandle,
		uint32 startVertex, uint32 primitiveCount, uint32 matrixIndex);
	bool32 DrawIndexed(VertexBufferHandle vertexHandle, IndexBufferHandle indexHandle, 
		uint32 numVertex, uint32 startIndex, uint32 primitiveCount, uint32 matrixIndex);

	CommandBucket<uint32> &GetCommandBucket() { return _commandBucket; }

	MatrixCache &GetMatrixCache() { return _matrixCache; }

	CreatevertexBuffer();

private :
	bool InitD3D(HWND windowHandle);
	
	bool InitDefaultRenderState();

	bool PerformRender();

	LPDIRECT3D9 _pD3D;
	LPDIRECT3DDEVICE9 _pDevice;

	D3DPRESENT_PARAMETERS _presentParams;

	IDirect3DSwapChain9 *_additionalSwapChain;

	RenderWindow _renderWindow;

	EventChannel _channel;

	CommandBucket<uint32> _commandBucket;

	VertexBuffer _vertexBuffers[VERTEXBUFFER_MAX_NUM];
	IndexBuffer _indexBuffers[INDEXBUFFER_MAX_NUM];
	Texture _textures[TEXTURE_MAX_NUM];
	Effect _effets[EFFECT_MAX_NUM];

	MatrixCache _matrixCache;
};

namespace backend
{
	
}

#endif