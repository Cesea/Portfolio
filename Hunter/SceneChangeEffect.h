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
	float _timeFactor;		//��ü �ð� ������� ( Effect ȿ���� ������� ���� ���� )

	LPDIRECT3DTEXTURE9		_pPrevRenderTexture;		//�������� Texture
	LPDIRECT3DTEXTURE9		_pNextRenderTexture;		//�������� Texture

	float _totalTime;		//��ü �ð�
	float _deltaTime;		//��ü ����ð�

	float _startTime;		//��ü�� ���۵� �ð�

	IScene *_pPrevScene;		//������
	IScene *_pNextScene;		//������

	bool32 _drawReady;		//��ü�� ��ο� ����
};

#endif