#ifndef SCENE_CHANGE_EFFECT_H
#define SCENE_CHANGE_EFFECT_H

#include "IScene.h"

//���ҽ��� video���� ������ ���� �ʴ´�...
class SceneChangeEffect
{
private:
	float		_timeFactor;;

	LPDIRECT3DTEXTURE9 _pPrevRenderTexture{};
	LPDIRECT3DTEXTURE9 _pNextRenderTexture{};

	float _totalTime;		//��ü �ð�
	float _deltaTime;		//��ü ����ð�

	float _startTime;		//��ü�� ���۵� �ð�

	IScene *_pPrevScene;		//������
	IScene *_pNextScene;		//������

	bool _drawReady;		//��ü�� ��ο� ����

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