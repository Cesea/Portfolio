#include "stdafx.h"
#include "BaseScene.h"

using namespace video;

struct PosVertex
{
	Vector3 position;
	Vector4 color;
	enum {FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE};
};

bool32 BaseScene::Load()
{
	bool32 result = true;
	return result;
}

bool32 BaseScene::Unload()
{
	bool32 result = true;
	return result;
}

bool32 BaseScene::Init()
{
	bool32 result = true;

	video::RenderViewHandle renderViewHandle= VIDEO->CreateRenderView();
	_renderView = VIDEO->GetRenderView(renderViewHandle);
	_renderView->_clearColor = 0xff55330;

	_camera.SetRotationSpeed(0.1f);
	_camera.SetMoveSpeed(1.0f);

	_world.AddSystem<RenderSystem>(_renderSystem);
	_world.AddSystem<TransformSystem>(_transformSystem);

	_world.Refresh();

	_entities.push_back(_world.CreateEntity());
	Entity &entity = _entities.back();

	Matrix correctionMat;
	MatrixScaling(&correctionMat, 0.1f, 0.1f, 0.1f);
	//_meshHandle = VIDEO->CreateStaticXMesh("../resources/models/Knight/Knight.x", &correctionMat, "Knight");


	_staticMeshHandle = VIDEO->CreateStaticXMesh("../resources/models/Snake/Snake.X", &correctionMat, "Snake");
	_skinnedMeshHandle = VIDEO->CreateSkinnedXMesh("../resources/models/Snake/Snake.X", &correctionMat, "Snake");

	_staticEffect = VIDEO->GetEffect("staticMesh.fx");
	_skinnedEffect = VIDEO->GetEffect("skinnedMesh.fx");


	//renderComp.model.CreateFromXAnimated("../resources/models/Snake/Snake.X", &correctionMat);
	//renderComp.model._effect = VIDEO->GetEffect("staticMesh.fx");
	//renderComp.model.CreateFromXAnimated("../resources/models/Knight/Knight.X", &correctionMat);
	//renderComp.model._effect = VIDEO->GetEffect("staticTestMesh.fx");

	_camera.GetTransform().MovePositionSelf(0.0f, 0.0f, -1.0f);

	_active = true;
	return result;
}

bool32 BaseScene::Update(float deltaTime)
{
	bool32 result = true;

	_world.Refresh();

	_transformSystem.PreUpdate(deltaTime);

	//Update Camera
	_transformSystem.UpdateTransform(_camera.GetTransform());
	_camera.UpdateMatrix();

	gpDevice->SetTransform(D3DTS_VIEW, &_camera.GetViewMatrix());
	gpDevice->SetTransform(D3DTS_PROJECTION, &_camera.GetProjectionMatrix());

	return result;
}

bool32 BaseScene::Render()
{
	//_renderSystem.Render(*_renderView, _camera);

	//VIDEO->Render(*_renderView);

	gpDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 0xff208020, 1.0f, 0);
	gpDevice->BeginScene();

	//const video::SkinnedXMesh *mesh = VIDEO->GetSkinnedXMesh(_skinnedMeshHandle);
	const video::StaticXMesh *mesh = VIDEO->GetStaticXMesh(_staticMeshHandle);
	const video::Effect *effect = VIDEO->GetEffect(_staticEffect);

	Matrix world;
	MatrixIdentity(&world);
	effect->SetMatrix("matWorld", world);
	effect->SetMatrix("matViewProjection", _camera.GetViewMatrix() * _camera.GetProjectionMatrix());
	effect->DrawStaticMesh(*mesh);

	//for (uint32 i = 0; i < mesh->_numMaterial; ++i)
	//{
	//	const Material *pMat = VIDEO->GetMaterial(mesh->_materialHandles[i]);
	//	gpDevice->SetTexture(0, VIDEO->GetTexture(pMat->_textureHandles[0])->_ptr);
	//	mesh->_pMesh->DrawSubset(i);
	//}

	gpDevice->EndScene();
	gpDevice->Present(nullptr, nullptr, NULL, nullptr);

	return true;
}

void BaseScene::Release()
{
}

const char * BaseScene::GetSceneName()
{
	return "BaseScene";
}

bool32 BaseScene::IsActive()
{
	return _active;
}
