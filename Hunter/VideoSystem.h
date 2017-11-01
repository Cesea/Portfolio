#ifndef VIDEO_SYSTEM_H
#define VIDEO_SYSTEM_H

#include "System.h"

//#include "RenderComponent.h"

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

	LPDIRECT3DDEVICE9 GetDevice()
	{
		return _pDevice;
	}
	virtual EventChannel GetChannel() { return _channel; }

private :
	bool InitD3D(HWND windowHandle);

	bool PerformRender();

	LPDIRECT3D9 _pD3D;
	LPDIRECT3DDEVICE9 _pDevice;

	D3DPRESENT_PARAMETERS _presentParams;

	RenderWindow _renderWindow;
};

#endif