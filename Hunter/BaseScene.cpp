#include "stdafx.h"
#include "BaseScene.h"

#include "tinyxml2.h"

//struct vec3
//{
//	float x;
//	float y;
//	float z;
//};

struct Vertex
{
	Vector3 position;
	D3DCOLOR color;

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

	_camera.SetWorldPosition(0.0f, 2.0f, -3.0f);

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


	D3DVERTEXELEMENT9 elements[3] =
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
		D3DDECL_END()
	};

	gpDevice->CreateVertexDeclaration(elements, &_pDecl);

	gpDevice->CreateVertexBuffer(sizeof(Vertex) * 3, D3DUSAGE_WRITEONLY, 
		Vertex::FVF, D3DPOOL_MANAGED, &_pBuffer, nullptr);

	Vertex *pData = nullptr;
	_pBuffer->Lock(0, 0, (void **)&pData, 0);
	pData[0].position = Vector3(0.0f, 0.5f, 0.0f);
	pData[0].color = 0xffff0000;
	pData[1].position = Vector3(0.5f, -0.5f, 0.0f);
	pData[1].color = 0xff00ff00;
	pData[2].position = Vector3(-0.5f, -0.5f, 0.0f);
	pData[2].color = 0xff0000ff;
	_pBuffer->Unlock();

	_active = true;
	return result;
}

bool32 BaseScene::Update(float deltaTime)
{
	bool32 result = true;

	_camera.UpdateMatrix();
	_camera.UpdateCamToDevice(gpDevice);

	return result;
}

bool32 BaseScene::Render()
{
	bool32 result = true;

	//im::BeginFrame(gEngine->GetInput()->mouse, gEngine->GetInput()->keyboard.GetCharInput());

	//im::BeginScrollArea("Editor", 100, 100, 400, 600, &_scroll);

	//if (im::Button("Hi", 100, true))
	//{
	//	EventChannel channel;
	//	channel.Broadcast(SceneSystem::SceneChangeEvent("TestScene"));
	//}
	//im::Item("HIIII", 100, true);
	//if (im::Check("TODO", _itemCheck))
	//{
	//	_itemCheck = !_itemCheck;
	//}
	//im::Value("holo");
	//if (im::Collapse("TODO", "meto", _collapse))
	//{
	//	_collapse = !_collapse;
	//}
	//im::Slider("slider", &_sliderValue, -20, 20, 0.1, 200);

	//im::EndScrollArea();

	//im::Edit(_strings1, 100);
	//im::Edit(_strings2, 100);

	//im::Button("After", 100);

	//im::EndFrame();

	gpDevice->Clear(0, nullptr, D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL | D3DCLEAR_TARGET, 0xff000000, 1.0f, 0.0f);
	gpDevice->BeginScene();

	debugdraw::WorldGrid(0.4, 10);

	gEngine->GetVideo()->Draw(_pBuffer, _pDecl, 0, 3, sizeof(Vertex));

	gpDevice->EndScene();
	gpDevice->Present(nullptr, nullptr, nullptr, nullptr);


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
