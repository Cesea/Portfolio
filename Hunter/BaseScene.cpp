#include "stdafx.h"
#include "BaseScene.h"

#include "XMesh.h"

using namespace video;

struct Vertex
{
	Vector3 position;
	Vector2 uv;

	enum
	{
		FVF = D3DFVF_XYZ | D3DFVF_TEX1
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

	_effect = VIDEO->CreateEffect("../resources/shaders/staticMesh.fx", "staticMesh");

	video::RenderViewHandle renderViewHandle= VIDEO->CreateRenderView();
	_renderView = VIDEO->GetRenderView(renderViewHandle);
	_renderView->_clearColor = 0xff55330;


	VertexDecl vertexDecl;
	vertexDecl.Begin();
	vertexDecl.Add(VertexElement(0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0));
	vertexDecl.Add(VertexElement(0, sizeof(Vector3), D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0));
	vertexDecl.End(sizeof(Vertex));

	VertexDeclHandle declHandle = VIDEO->CreateVertexDecl(&vertexDecl);

	Vertex vertices[3];
	vertices[0].position = Vector3(0.0f, 0.5f, 0.5f);
	vertices[0].uv = Vector2(0.5f, 0.0f);
	vertices[1].position = Vector3(0.5f, -0.5f, 0.5f);
	vertices[1].uv = Vector2(1.0f, 1.0f);
	vertices[2].position = Vector3(-0.5f, -0.5f, 0.5f);
	vertices[2].uv = Vector2(0.0f, 1.0f);

	Memory mem;
	mem._data = vertices;
	mem._size = sizeof(vertices);

	_vertexBuffer0 = VIDEO->CreateVertexBuffer(&mem, declHandle, "testVertex");

	VertexBufferHandle test = VIDEO->GetVertexBuffer("testVertex");

	uint16 indices[3];
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	mem._data = indices;
	mem._size = sizeof(indices);
	_indexBuffer = VIDEO->CreateIndexBuffer(&mem);

	TextureHandle textureHandle0 = VIDEO->CreateTexture("../resources/textures/test.png", "test1");
	TextureHandle textureHandle1 = VIDEO->CreateTexture("../resources/textures/test2.png", "test2");

	_material0 = VIDEO->CreateMaterial("test");
	VIDEO->MaterialAddTexture(_material0, VIDEO_TEXTURE0, textureHandle0);

	_material1 = VIDEO->CreateMaterial("test2");
	VIDEO->MaterialAddTexture(_material1, VIDEO_TEXTURE0, textureHandle1);

	//_model0 = VIDEO->CreateModelFromX("../resources/models/aa.x");
	//VIDEO->ModelSetEffect(_model0, _effect);

	_world.AddSystem<RenderSystem>(_renderSystem);
	_world.AddSystem<TransformSystem>(_transformSystem);

	_world.Refresh();

	_entities.push_back(_world.CreateEntity());
	Entity &entity = _entities.back();

	RenderComponent &renderComp = entity.AddComponent<RenderComponent>();
	//renderComp._effect = _effect;
	//renderComp._vertexBuffer = _vertexBuffer0;
	//renderComp._material = (index % 2) ? _material0 : _material1;
	//renderComp.model = _model0;
	renderComp.radius = 1.0f;

	TransformComponent &transComp = entity.AddComponent<TransformComponent>();
	transComp.MovePositionWorld(0, 0, 0);

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
	
	_camera.GetTransform().MovePositionSelf(0.0f, 0.0f, -5.0f);

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
	bool32 result = true;

	_renderSystem.Render(*_renderView, _camera);

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
