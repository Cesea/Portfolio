#ifndef SCENE_CHANGE_EFFECT_H
#define SCENE_CHANGE_EFFECT_H

#include "IScene.h"

class SceneChangeEffect
{
public:
	SceneChangeEffect(void);
	~SceneChangeEffect(void);

	void Init();
	void Relase();

	void ChangeStart(IScene *pPrevScene, IScene *pNextScene, float changeTime);

	void Update(float timeDelta);
	void Render();

	virtual void ChangeEffect(LPDIRECT3DTEXTURE9 pPrev, LPDIRECT3DTEXTURE9 pNext, float factor);

	bool IsDrawReady() 
	{
		return _drawReady;
	}

private :
	float _timeFactor;		//교체 시간 진행비율 ( Effect 효과에 목숨과도 같은 변수 )

	LPDIRECT3DTEXTURE9		_pPrevRenderTexture;		//랜더링될 Texture
	LPDIRECT3DTEXTURE9		_pNextRenderTexture;		//랜더링될 Texture

	float _totalTime;		//교체 시간
	float _deltaTime;		//교체 진행시간

	float _startTime;		//교체가 시작된 시간

	IScene *_pPrevScene;		//이전씬
	IScene *_pNextScene;		//다음씬

	bool32 _drawReady;		//교체씬 드로우 여부
};

#endif