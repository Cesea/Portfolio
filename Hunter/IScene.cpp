#include "stdafx.h"

bool IScene::Render()
{
	return false;
}

bool IScene::RenderToMainCamTexture()
{
	_camera.RenderTextureBegin(0x00101010);
	//ȯ�� ����
	RenderEnvironmentSphere();
	//�����ȴ�.
	Render0();
	Render1();
	Render2();
	
	_camera.RenderTextureEnd();
	return true;
}

LPDIRECT3DTEXTURE9 IScene::GetSceneTexture()
{
	return _camera.GetRenderTexture();
}

bool IScene::RenderEnvironmentSphere()
{
	_pEnvironmentSphere->Render(_camera);
	return true;
}
