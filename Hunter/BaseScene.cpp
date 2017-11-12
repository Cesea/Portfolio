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

void DumpToStdOut(const tinyxml2::XMLAttribute *pAttribute, uint32 indent = 0)
{
	const tinyxml2::XMLAttribute *attri;
	for (attri = (tinyxml2::XMLAttribute *)pAttribute; 
		attri != 0; 
		(const tinyxml2::XMLAttribute *)attri = attri->Next())
	{
		for (int32 i = 0; i < indent; ++i)
		{
			printf("%s\n", attri->Value());
		}
	}
}

void DumpToStdOut(const tinyxml2::XMLNode *pParent, uint32 indent = 0)
{
	if (pParent == nullptr)
	{
		return;
	}

	const tinyxml2::XMLNode *child;

	const tinyxml2::XMLElement *pElement;
	const tinyxml2::XMLAttribute *pAttribute;
	const tinyxml2::XMLText *pText;
	for (child = pParent->FirstChild(); child != 0; child = (tinyxml2::XMLNode *)child->NextSibling())
	{
		for (int i = 0; i < indent; ++i)
		{
			printf("	");
		}
		if (pElement = child->ToElement())
		{
			printf("<Element>");
		}
		printf("%s\n", child->Value());
		if (pElement)
		{
			pAttribute = (tinyxml2::XMLAttribute *)pElement->FirstAttribute();
			if (pAttribute)
			{
				DumpToStdOut(pAttribute, indent + 1);
			}
		}
		DumpToStdOut(child, indent + 1);
	}
}

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

	//_camera.SetWorldPosition(0.0f, 2.0f, -3.0f);

	//std::vector<int> arrays;
	//for (int i = 0; i < 10; ++i)
	//{
	//	arrays.push_back(RandInt(10));
	//}
	//std::vector<float> floatArray;
	//for (int i = 0; i < 10; ++i)
	//{
	//	floatArray.push_back(RandFloat(10));
	//}
	//tinyxml2::XMLDocument document;
	//tinyxml2::XMLElement *pRootNode = document.NewElement("IntList");
	//document.InsertEndChild(pRootNode);
	//for (auto element : arrays)
	//{
	//	tinyxml2::XMLElement *newNode = document.NewElement("element");
	//	newNode->SetText(element);
	//	pRootNode->InsertEndChild(newNode);
	//}
	//tinyxml2::XMLElement *nextnode = document.NewElement("FloatList");
	//document.InsertEndChild(nextnode);
	//for (auto element : floatArray)
	//{
	//	tinyxml2::XMLElement *newNode = document.NewElement("element");
	//	newNode->SetText(element);
	//	nextnode->InsertEndChild(newNode);
	//}
	//vec3 position;
	//position.x = RandFloat(10.0f);
	//position.y = RandFloat(10.0f);
	//position.z = RandFloat(10.0f);
	//nextnode = document.NewElement("position");
	//nextnode->SetAttribute("x", position.x);
	//nextnode->SetAttribute("y", position.y);
	//nextnode->SetAttribute("z", position.z);
	//document.InsertEndChild(nextnode);
	//document.SaveFile("res/data.xml");
	//tinyxml2::XMLDocument document;
	//tinyxml2::XMLError error =  document.LoadFile("res/data.xml");
	//if (error != tinyxml2::XML_SUCCESS)
	//{
	//	return false;
	//}
	//tinyxml2::XMLElement *pRootElement = document.RootElement();
	//tinyxml2::XMLElement *pPositionElement = pRootElement->NextSiblingElement("position");
	//std::string str;
	//const tinyxml2::XMLElement *pChild = pPositionElement->;
	//while (pPositionElement)
	//{
	//	str = pPositionElement->Name();
	//	std::cout << str << std::endl;
	//	const tinyxml2::XMLAttribute *pAttribute = pPositionElement->FirstAttribute();
	//	while (pAttribute != nullptr)
	//	{
	//		str = pAttribute->Name();
	//		std::cout << str << std::endl;
	//		str = pAttribute->Value();
	//		std::cout << str << std::endl;
	//		pAttribute = pAttribute->Next();
	//	}
	//	pPositionElement = pPositionElement->NextSiblingElement();
	//}
	//const tinyxml2::XMLElement *pElement = pRootElement;
	//for (; pElement != nullptr; pElement->NextSibling())
	//{
	//	str = pElement->Name();
	//	std::cout << str << std::endl;
	//	const tinyxml2::XMLAttribute *pAttribute = nullptr;
	//	for (pAttribute = pElement->FirstAttribute(); pAttribute != nullptr; pAttribute->Next())
	//	{
	//		str = pAttribute->Name();
	//		std::cout << str << std::endl;
	//		str = pAttribute->Value();
	//		std::cout << str << std::endl;
	//	}
	//}

	//_world.AddSystem(_moveSystem);
	//_world.AddSystem(_renderSystem);

	//for (int32 i = 0; i < 2; ++i)
	//{
	//	Entity entity = _world.CreateEntity();
	//	entity.AddComponent<TransformComponent>();
	//	entity.AddComponent<MoveComponent>();
	//	entity.AddComponent<RenderComponent>();
	//	entity.Activate();
	//}

	_effect = VIDEO->CreateEffect("../resources/shaders/basic.fx", "BasicShader");

	video::RenderViewHandle renderViewHandle= VIDEO->CreateRenderView();
	_renderView = VIDEO->GetRenderView(renderViewHandle);
	_renderView->_clearColor = 0xff50ff50;

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

	Vertex vertices1[3];
	vertices1[0].position = Vector3(0.0f, -0.5f, 0.5f);
	vertices1[0].color = 0xffff0050;
	vertices1[1].position = Vector3(-0.5f, 0.5f, 0.5f);
	vertices1[1].color = 0xff002f00;
	vertices1[2].position = Vector3(0.5f, 0.5f, 0.5f);
	vertices1[2].color = 0xff0090ff;

	mem._data = vertices1;
	mem._size = sizeof(vertices1);

	_vertexBuffer1 = VIDEO->CreateVertexBuffer(&mem, declHandle);
	_vertexBuffer2 = _vertexBuffer0;

	uint16 indices[3];

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	mem._data = indices;
	mem._size = sizeof(indices);
	_indexBuffer = VIDEO->CreateIndexBuffer(&mem);

	_firstWindowPos.x = 100;
	_firstWindowPos.y = 100;

	_secondWindowPos.x = 200;
	_secondWindowPos.y = 200;

	_active = true;
	return result;
}

bool32 BaseScene::Update(float deltaTime)
{
	bool32 result = true;
	_renderView->Begin();

	_renderView->SetEffect(_effect);

	_renderView->Submit(_vertexBuffer0);
	_renderView->Submit(_indexBuffer);
	_renderView->Draw();

	Matrix world;
	MatrixTranslation(&world, 1.0f, 0.0f, 0.0f);

	_renderView->SetTransform(world);
	_renderView->Submit(_vertexBuffer1);
	_renderView->Submit(_indexBuffer);
	_renderView->Draw();

	MatrixTranslation(&world, -1.0f, 0.0f, 0.0f);
	_renderView->SetTransform(world);
	_renderView->Submit(_vertexBuffer2);
	_renderView->Submit(_indexBuffer);
	_renderView->Draw();

	_renderView->End();

	VIDEO->Render(*_renderView);
	return result;
}


bool32 BaseScene::Render()
{
	bool32 result = true;


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
