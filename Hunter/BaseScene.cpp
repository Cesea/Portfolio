#include "stdafx.h"
#include "BaseScene.h"

#include "tinyxml2.h"

//struct vec3
//{
//	float x;
//	float y;
//	float z;
//};

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

bool BaseScene::Load()
{
	bool result = true;
	return result;
}

bool BaseScene::Unload()
{
	bool result = true;
	return result;
}

bool BaseScene::Init()
{
	bool result = true;
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

	return result;
}

bool BaseScene::Update(float deltaTime)
{
	bool result = true;
	return result;
}

bool BaseScene::Render()
{
	bool result = true;

	im::BeginFrame(gEngine->GetInput()->mouse, gEngine->GetInput()->keyboard.GetCharInput());

	im::BeginScrollArea("Editor", 100, 100, 400, 600, &_scroll);

	im::Button("Hi", 100, true);
	im::Item("HIIII", 100, true);
	if (im::Check("TODO", _itemCheck))
	{
		_itemCheck = !_itemCheck;
	}
	im::Value("holo");
	if (im::Collapse("TODO", "meto", _collapse))
	{
		_collapse = !_collapse;
	}
	im::Slider("slider", &_sliderValue, -20, 20, 0.1, 200);

	im::EndScrollArea();

	im::Edit(_strings1, 100);
	im::Edit(_strings2, 100);

	im::Button("After", 100);

	im::EndFrame();

	return result;
}

void BaseScene::Release()
{
}

const char * BaseScene::GetSceneName()
{
	return "BaseScene";
}

bool BaseScene::IsActive()
{
	return _active;
}
