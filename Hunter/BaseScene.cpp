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

	_camera.SetMoveSpeed(30.0f);

	_world.AddSystem<RenderSystem>(_renderSystem);
	_world.AddSystem<TransformSystem>(_transformSystem);

	_world.Refresh();

	_entities.push_back(_world.CreateEntity());
	Entity &entity = _entities.back();

	Matrix correctionMat;
	MatrixScaling(&correctionMat, 0.2f, 0.2f, 0.2f);

	RenderComponent &renderComp = entity.AddComponent<RenderComponent>();

	//renderComp.model.CreateFromXStatic("../resources/models/sphere.X", &correctionMat);

	MatrixScaling(&correctionMat, 1.0f, 1.0f, 1.0f);
	renderComp.model.CreateFromXAnimated("../resources/models/Snake/Snake.X", &correctionMat);
	renderComp.model._effect = VIDEO->GetEffect("staticMesh.fx");
	//renderComp.model.CreateFromXAnimated("../resources/models/Knight/Knight.X", &correctionMat);
	//renderComp.model._effect = VIDEO->GetEffect("staticTestMesh.fx");

	//Radius 설정 해주자...
	renderComp.radius = 1.0f;

	TransformComponent &transComp = entity.AddComponent<TransformComponent>();
	transComp.MovePositionWorld(0, 0, 0);

	entity.Activate();

	//for (int32 y = 0; y < 40; ++y)
	//{
	//	for (uint32 x = 0; x < 40; ++x)
	//	{
	//		uint32 index = x + y * 40;
	//		_entities.push_back(_world.CreateEntity());
	//		Entity &entity = _entities.back();
	//		entity.Activate();
	//		RenderComponent &triangleRender = entity.AddComponent<RenderComponent>();
	//		triangleRender._effect = _effect;
	//		triangleRender._vertexBuffer = _vertexBuffer0;
	//		triangleRender._material = (index % 2) ? _material0 : _material1;
	//		triangleRender.radius = 1.0f;
	//		TransformComponent &triangleTransform = entity.AddComponent<TransformComponent>();
	//		triangleTransform.MovePositionWorld(x, y, 0);
	//	}
	//}
	_camera.GetTransform().MovePositionSelf(0.0f, 0.0f, -15.0f);

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
	return result;
}

bool32 BaseScene::Render()
{
	_renderSystem.Render(*_renderView, _camera);

	VIDEO->Render(*_renderView);

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
