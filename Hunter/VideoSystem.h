#ifndef VIDEO_SYSTEM_H
#define VIDEO_SYSTEM_H

#include "System.h"

//#include "VisualComponent.h"

#include "imguiRenderer.h"
#include "DebugDraw.h"

#include "Command.h"
#include "CommandBucket.h"

//defines 
#define VIDEO (gEngine->GetVideo())

struct RenderWindow
{
	int32 width;
	int32 height;

	D3DFORMAT backBufferFormat;
	D3DFORMAT depthStencilFormat;
};

class VideoSystem : public System
{
public :
	VideoSystem();
	virtual ~VideoSystem();

	virtual bool Init(const std::string &name, const SystemSetting &setting);
	virtual void ShutDown();

	virtual void Update(float deltaTime);
	void Render();

	LPDIRECT3DDEVICE9 GetDevice()
	{
		return _pDevice;
	}
	virtual EventChannel GetChannel() { return _channel; }

	bool32 Draw(IDirect3DVertexBuffer9 *pVertexBuffer, 
		uint32 startVertex, uint32 primitiveCount, uint32 stride);
	bool32 DrawIndexed(IDirect3DVertexBuffer9 * pVertexBuffer, 
		IDirect3DIndexBuffer9 * pIndexBuffer, 
		IDirect3DVertexDeclaration9 * pVertexDecl, 
		uint32 indexCount, uint32 vertexCount, uint32 startIndex, uint32 baseVerteex, uint32 stride);

	CommandBucket<uint32> &GetCommandBucket() { return _commandBucket; }

private :
	bool InitD3D(HWND windowHandle);
	
	bool InitDefaultRenderState();

	bool PerformRender();

	LPDIRECT3D9 _pD3D;
	LPDIRECT3DDEVICE9 _pDevice;

	D3DPRESENT_PARAMETERS _presentParams;

	RenderWindow _renderWindow;

	im::GuiRenderer *_pimguiRenderer;

	CommandBucket<uint32> _commandBucket;
};

#endif