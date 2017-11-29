#ifndef SCENE_CHANGE_EFFECT_UP_DOWN_H
#define SCENE_CHANGE_EFFECT_UP_DOWN_H

#include "SceneChangeEffect.h"

class SceneChangeEffectUpDown : public SceneChangeEffect
{
public:
	SceneChangeEffectUpDown(void);
	~SceneChangeEffectUpDown(void);

	virtual void ChangeEffect( LPDIRECT3DTEXTURE9 pPrev, LPDIRECT3DTEXTURE9 pNext, float factor );

};

#endif