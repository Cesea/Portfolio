#include "stdafx.h"
#include "XMLManager.h"

XMLManager::XMLManager()
{
}

XMLManager::~XMLManager()
{
}

tinyxml2::XMLError XMLManager::OpenDocument(const std::string & fileName, tinyxml2::XMLDocument * pOutDocument)
{
	return tinyxml2::XMLError();
}

tinyxml2::XMLError XMLManager::SaveDocument(const std::string & fileName, tinyxml2::XMLDocument & document)
{
	return tinyxml2::XMLError();
}

//tinyxml2::XMLNode *XMLManager::LoadXML(const std::string & fileName)
//{
//	return nullptr;
//}
//
//void XMLManager::WriteXML(const std::string & fileName, tinyxml2::XMLDocument & document)
//{
//}
