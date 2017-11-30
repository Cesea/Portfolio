#ifndef SCENE_CHANGE_EFFECT_H
#define SCENE_CHANGE_EFFECT_H

#include "IScene.h"

//리소스를 video에서 가지고 있지 않는다...
class SceneChangeEffect
{
private:
	float		_timeFactor;;

	LPDIRECT3DTEXTURE9 _pPrevRenderTexture{};
	LPDIRECT3DTEXTURE9 _pNextRenderTexture{};

	float _totalTime;		//교체 시간
	float _deltaTime;		//교체 진행시간

	float _startTime;		//교체가 시작된 시간

	IScene *_pPrevScene;		//이전씬
	IScene *_pNextScene;		//다음씬

	bool _drawReady;		//교체씬 드로우 여부

public:
	SceneChangeEffect(void);
	~SceneChangeEffect(void);

	void Init();
	void Relase();
	
	void ChangeStart( IScene* pPrevScene, IScene* pNextScene, float changeTime );
	
	void Update( float deltaTime );
	void Render();

	virtual void ChangeEffect( LPDIRECT3DTEXTURE9 pPrev, LPDIRECT3DTEXTURE9 pNext, float factor );

	bool IsDrawReady(){ return _drawReady; }

};


#endif