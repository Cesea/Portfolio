#ifndef XML_MANAGER_H
#define XML_MANAGER_H

#include "SingletonBase.h"
#include "tinyxml2.h"

#include <unordered_map>

class XMLManager : public SingletonBase<XMLManager>
{
public :
	XMLManager();
	virtual ~XMLManager();

	tinyxml2::XMLError OpenDocument(const std::string &fileName, tinyxml2::XMLDocument *pOutDocument);
	tinyxml2::XMLError SaveDocument(const std::string &fileName, tinyxml2::XMLDocument &document);

	//tinyxml2::XMLNode *LoadXML(const std::string &fileName);
	//void WriteXML(const std::string &fileName, tinyxml2::XMLDocument &document);

private :

	//typedef std::unordered_map<std::string, > XMLMap;
	//typedef std::unordered_map<std::string, >::iterator XMLMap;


};

#endif