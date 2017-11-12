#include "stdafx.h"
#include "BaseScene.h"

#include "tinyxml2.h"

using namespace video;

struct Vertex
{
	Vector3 position;
	D3DXCOLOR color;

	enum
	{
		FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE
	};
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

	_effect = VIDEO->CreateEffect("../resources/shaders/basic.fx", "BasicShader");

	video::RenderViewHandle renderViewHandle= VIDEO->CreateRenderView();
	_renderView = VIDEO->GetRenderView(renderViewHandle);
	_renderView->_clearColor = 0xff55330;

	Matrix view;
	MatrixLookAtLH(&view, &Vector3(0.0f, -0.0f, -2.0f), &Vector3(0.0f, 0.0f, 0.0f), &Vector3(0.0f, 1.0f, 0.0f));
	Matrix projection;
	MatrixPerspectiveFovLH(&projection, D3DX_PI / 2.0f, (float)WINSIZEX / (float)WINSIZEY, 0.1f, 1000.0f);
	_renderView->SetViewProjection(view, projection);

	VertexDecl vertexDecl;
	vertexDecl.Begin();
	vertexDecl.Add(VertexElement(0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0));
	vertexDecl.Add(VertexElement(0, sizeof(Vector3), D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0));
	vertexDecl.End(sizeof(Vertex));

	VertexDeclHandle declHandle = VIDEO->CreateVertexDecl(&vertexDecl);

	Vertex vertices[3];
	vertices[0].position = Vector3(0.0f, 0.5f, 0.5f);
	vertices[0].color = 0xffff0000;
	vertices[1].position = Vector3(0.5f, -0.5f, 0.5f);
	vertices[1].color = 0xff00ff00;
	vertices[2].position = Vector3(-0.5f, -0.5f, 0.5f);
	vertices[2].color = 0xff0000ff;

	Memory mem;
	mem._data = vertices;
	mem._size = sizeof(vertices);

	_vertexBuffer0 = VIDEO->CreateVertexBuffer(&mem, declHandle);

	uint16 indices[3];

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	mem._data = indices;
	mem._size = sizeof(indices);
	_indexBuffer = VIDEO->CreateIndexBuffer(&mem);

	_world.AddSystem<RenderSystem>(_renderSystem);
	_world.AddSystem<TransformSystem>(_transformSystem);

	_world.Refresh();


	for (int32 y = 0; y < 20; ++y)
	{
		for (uint32 x = 0; x < 20; ++x)
		{
			_entities.push_back(_world.CreateEntity());
			Entity &entity = _entities.back();
			entity.Activate();

			RenderComponent &triangleRender = entity.AddComponent<RenderComponent>();
			triangleRender._effect = _effect;
			triangleRender._vertexBuffer = _vertexBuffer0;

			TransformComponent &triangleTransform = entity.AddComponent<TransformComponent>();
			triangleTransform.MovePositionWorld(x, y, 0);
		}
	}

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
	_renderView->SetViewProjection(_camera.GetViewMatrix(), _camera.GetProjectionMatrix());

	return result;
}

bool32 BaseScene::Render()
{
	bool32 result = true;

	_renderSystem.Render(*_renderView);

	//_renderView->Begin();
	//_renderView->SetEffect(_effect);
	//_renderView->Submit(_vertexBuffer0);
	//_renderView->Submit(_indexBuffer);
	//_renderView->Draw();
	//Matrix world;
	//MatrixTranslation(&world, 1.0f, 0.0f, 0.0f);
	//_renderView->SetTransform(world);
	//_renderView->Submit(_vertexBuffer1);
	//_renderView->Submit(_indexBuffer);
	//_renderView->Draw();
	//MatrixTranslation(&world, -1.0f, 0.0f, 0.0f);
	//_renderView->SetTransform(world);
	//_renderView->Submit(_vertexBuffer2);
	//_renderView->Draw();
	//_renderView->End();

	VIDEO->Render(*_renderView);

	return result;
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
