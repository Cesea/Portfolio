#include "stdafx.h"
#include "BaseScene.h"

#include "tinyxml2.h"

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

	for (int32 i = 0; i < 2; ++i)
	{
		Entity entity = _world.CreateEntity();
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<MoveComponent>();
		entity.AddComponent<RenderComponent>();
		entity.Activate();
	}

	Vertex vertices[3];
	vertices[0].position = Vector3(0.0f, 0.5f, 0.5f);
	vertices[0].color = 0xffff0000;
	vertices[1].position = Vector3(0.5f, -0.5f, 0.5f);
	vertices[1].color = 0xff00ff00;
	vertices[2].position = Vector3(-0.5f, -0.5f, 0.5f);
	vertices[2].color = 0xff0000ff;
	//_vertexHandle =  VIDEO->GetVertexBufferManager()->CreateStatic(3, sizeof(Vertex), vertices);

	uint16 indices[3];
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	//_indexHandle = VIDEO->GetIndexBufferManager()->CreateStatic(3, IndexBuffer::IndexFormat::Uint16, indices);


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

	_world.Refresh();

	//_moveSystem.Update(deltaTime);

	//_camera.UpdateMatrix();
	//_camera.UpdateCamToDevice(gpDevice);

	return result;
}


bool32 BaseScene::Render()
{
	bool32 result = true;

	//im::BeginFrame(gEngine->GetInput()->mouse, 
	//	gEngine->GetInput()->keyboard.GetCharInput(), 
	//	gEngine->GetInput()->keyboard.GetShiftDown());

	//im::BeginScrollArea("Editor", _firstWindowPos, 400, 600, &_scroll);

	//im::Button("X");
	//im::Button("Y");
	//im::Button("Z");

	//if (im::Collapse("col", _collapse))
	//{
	//	_collapse = !_collapse;
	//}
	//if (!_collapse)
	//{
	//	im::Indent();

	//	im::Button("yaya");
	//	im::Button("tototo");

	//	im::Unindent();
	//}
	//
	//im::Slider("Slider", &_sliderValue, -10, 10, 0.1);

	//static int test = 0;
	//if (test % 20 == 0)
	//{
	//	std::cout << _sliderValue << std::endl;
	//}
	//test++;

	//if (im::Check("toto", _checked))
	//{
	//	_checked = !_checked;
	//}

	//im::EndScrollArea();


	//im::BeginScrollArea("Editor", _secondWindowPos, 400, 600, &_scroll);

	//im::Button("X");
	//im::Button("Y");
	//im::Button("Z");

	//if (im::Collapse("col", _collapse))
	//{
	//	_collapse = !_collapse;
	//}
	//if (!_collapse)
	//{
	//	im::Indent();

	//	im::Button("yaya");
	//	im::Button("tototo");

	//	im::Unindent();
	//}

	//im::Slider("Slider", &_sliderValue, -10, 10, 0.1);


	//if (im::Check("toto", _checked))
	//{
	//	_checked = !_checked;
	//}

	//if (im::Edit(_strings1))
	//{
	//	std::cout << _strings1 << std::endl;
	//}
	//if (im::Edit(_strings2))
	//{
	//	std::cout << _strings2 << std::endl;
	//}
	//im::EndScrollArea();
	//im::EndFrame();

	//_renderSystem.Update(0.0f);
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
