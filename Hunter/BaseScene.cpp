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

	//Vertex vertices[3];
	//vertices[0].position = Vector3(0.0f, 0.5f, 0.5f);
	//vertices[0].uv = Vector2(0.5f, 0.0f);
	//vertices[1].position = Vector3(0.5f, -0.5f, 0.5f);
	//vertices[1].uv = Vector2(1.0f, 1.0f);
	//vertices[2].position = Vector3(-0.5f, -0.5f, 0.5f);
	//vertices[2].uv = Vector2(0.0f, 1.0f);

	//Memory mem;
	//mem._data = vertices;
	//mem._size = sizeof(vertices);

	//_vertexBuffer0 = VIDEO->CreateVertexBuffer(&mem, declHandle, "testVertex");

	//VertexBufferHandle test = VIDEO->GetVertexBuffer("testVertex");

	//uint16 indices[3];
	//indices[0] = 0;
	//indices[1] = 1;
	//indices[2] = 2;

	//mem._data = indices;
	//mem._size = sizeof(indices);
	//_indexBuffer = VIDEO->CreateIndexBuffer(&mem);

	//TextureHandle textureHandle0 = VIDEO->CreateTexture("../resources/textures/test.png", "test1");
	//TextureHandle textureHandle1 = VIDEO->CreateTexture("../resources/textures/test2.png", "test2");

	//_material0 = VIDEO->CreateMaterial("test");
	//VIDEO->MaterialAddTexture(_material0, VIDEO_TEXTURE0, textureHandle0);

	//_material1 = VIDEO->CreateMaterial("test2");
	//VIDEO->MaterialAddTexture(_material1, VIDEO_TEXTURE0, textureHandle1);

	//video::VertexDeclHandle vertexHandle = VIDEO->GetVertexDecl(StaticTestVertex::_name);

	_world.AddSystem<RenderSystem>(_renderSystem);
	_world.AddSystem<TransformSystem>(_transformSystem);

	_world.Refresh();

	_entities.push_back(_world.CreateEntity());
	Entity &entity = _entities.back();

	Matrix correctionMat;
	MatrixScaling(&correctionMat, 1.0f, 1.0f, 1.0f);

	RenderComponent &renderComp = entity.AddComponent<RenderComponent>();

	renderComp.model.CreateFromXStatic("../resources/models/sphere.X", &correctionMat);
	//renderComp.model.CreateFromXAnimated("../resources/models/knight/Knight.X", &correctionMat);

	renderComp.model._effect = VIDEO->GetEffect("staticTestMesh.fx");

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

	//video::StaticTestVertex vertices[4];

	//vertices[0] = video::StaticTestVertex(Vector3(-1.0f, 1.0f, 2.0f), Vector3(0.0f, 0.0f, -1.0f), Vector2(0.0f, 0.0f));
	//vertices[1] = video::StaticTestVertex(Vector3(1.0f, 1.0f, 2.0f), Vector3(0.0f, 0.0f, -1.0f), Vector2(1.0f, 0.0f));
	//vertices[2] = video::StaticTestVertex(Vector3(1.0f, -1.0f, 2.0f), Vector3(0.0f, 0.0f, -1.0f), Vector2(1.0f, 1.0f));
	//vertices[3] = video::StaticTestVertex(Vector3(-1.0f, -1.0f, 2.0f), Vector3(0.0f, 0.0f, -1.0f), Vector2(0.0f, 1.0f));

	//PosVertex vertices[4];
	//vertices[0].position = Vector3(-1.0f, 1.0f, -2.0f);
	//vertices[1].position = Vector3(1.0f, 1.0f, -2.0f);
	//vertices[2].position = Vector3(1.0f, -1.0f, -2.0f);
	//vertices[3].position = Vector3(-1.0f, -1.0f, -2.0f);

	//vertices[0].color = 0xff00ff00;
	//vertices[1].color = 0xff00ff00;
	//vertices[2].color = 0xff00ff00;
	//vertices[3].color = 0xff00ff00;

	PosVertex vertices[3];
	vertices[0].position = Vector3(-1.0f, 1.0f, 2.0f);
	vertices[1].position = Vector3(1.0f, 1.0f, 2.0f);
	vertices[2].position = Vector3(1.0f, -1.0f, 2.0f);

	vertices[0].color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	vertices[1].color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
	vertices[2].color = Vector4(0.0f, 0.0f, 1.0f, 1.0f);


	gpDevice->CreateVertexBuffer(sizeof(PosVertex) * 3, D3DUSAGE_WRITEONLY, 0, 
		D3DPOOL_DEFAULT, &_pVertexBuffer, nullptr);

	void *pVertexData = nullptr;
	_pVertexBuffer->Lock(0, 0, (void **)&pVertexData, 0);
	memcpy(pVertexData, vertices, sizeof(PosVertex) * 3);
	_pVertexBuffer->Unlock();

	//gpDevice->CreateIndexBuffer(sizeof(indices), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &_pIndexBuffer, nullptr);

	D3DXCreateEffectFromFile(gpDevice, "../resources/shaders/staticTestMesh.fx", nullptr, nullptr, D3DXSHADER_DEBUG, 
		nullptr, &_pEffect, nullptr);

	D3DVERTEXELEMENT9 elements[] =
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, sizeof(Vector3), D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
		D3DDECL_END()
	};

	gpDevice->CreateVertexDeclaration(elements, &_pDecl);

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
	//_renderSystem.Render(*_renderView, _camera);

	//VIDEO->Render(*_renderView);

	gpDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 0xff406080, 1.0f, 0);

	gpDevice->BeginScene();

	D3DXMATRIX world;
	D3DXMATRIX view;
	D3DXMATRIX projection;

	D3DXMatrixIdentity(&world);
	D3DXMatrixLookAtLH(&view, &Vector3(0.0f, 0.0f, -4.0f), &Vector3(0.0f, 0.0f, 0.0f), &Vector3(0.0f, 1.0f, 0.0f));
	D3DXMatrixPerspectiveFovLH(&projection, D3DX_PI * 0.5f, (float)WINSIZEX / (float)WINSIZEY, 0.1f, 1000.0f);

	gpDevice->SetTransform(D3DTS_WORLD, &world);
	gpDevice->SetTransform(D3DTS_VIEW, &_camera.GetViewMatrix());
	gpDevice->SetTransform(D3DTS_PROJECTION, &_camera.GetProjectionMatrix());

	gpDevice->SetFVF(PosVertex::FVF);

	HRESULT result = S_OK;

	result = _pEffect->SetMatrix("gWorld", &world);
	result = _pEffect->SetMatrix("gView", &view);
	result = _pEffect->SetMatrix("gProjection", &projection);

	result = gpDevice->SetVertexDeclaration(_pDecl);
	result = gpDevice->SetStreamSource(0, _pVertexBuffer, 0, sizeof(PosVertex));

	uint32 pass = 0;

	result = _pEffect->Begin(&pass, 0);

	for (uint32 i = 0; i < pass; ++i)
	{
		result = _pEffect->BeginPass(i);
		result = gpDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
		result = _pEffect->EndPass();
	}
	result = _pEffect->End();

	result = gpDevice->EndScene();
	result = gpDevice->Present(nullptr, nullptr, nullptr, nullptr);

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
