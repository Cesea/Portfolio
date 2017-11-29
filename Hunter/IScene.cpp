#include "stdafx.h"

bool IScene::Render()
{
	return false;
}

bool IScene::RenderToMainCamTexture()
{
	_camera.RenderTextureBegin(0x00101010);
	//환경 랜더
	RenderEnvironmentSphere();
	//랜더된다.
	Scene_Render0();
	Scene_Render1();
	Scene_Render2();
	
	_camera.RenderTextureEnd();}

LPDIRECT3DTEXTURE9 IScene::GetSceneTexture()
{
	return _camera.GetRenderTexture();
}

bool IScene::RenderEnvironmentSphere()
{
	_pEnvironmentSphere->Render();
}
