#ifndef BASE_SCENE_H
#define BASE_SCENE_H

#include "IScene.h"

#include "DataPool.h"

class BaseScene : public IScene
{
public :
	BaseScene() {};
	virtual ~BaseScene() {}

	virtual bool32 Load();
	virtual bool32 Unload();

	virtual bool32 Init();
	virtual bool32 Update(float deltaTime);
	virtual bool32 Render();

	virtual void Release();

	virtual const char *GetSceneName();
	virtual bool32 IsActive();

protected :
	bool32 _active;

	bool32 _itemCheck{ false };
	bool32 _collapse{ false };

	float _sliderValue{ 0 };

	char _strings1[MAX_PATH] = {0, };
	char _strings2[MAX_PATH] = {0, };

	int32 _scroll{ 0 };

	Camera _camera;

	IDirect3DVertexBuffer9 *_pBuffer;
	IDirect3DVertexDeclaration9 *_pDecl;

};


#endif