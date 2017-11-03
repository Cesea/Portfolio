#ifndef COMPONENT_FACTORY_H
#define COMPONENT_FACTORY_H

#include "Component.h"
#include "tinyxml2.h"

#include <map>

class ComponentBuilder
{
public:
	virtual ~ComponentBuilder() {}
	virtual Component *Build(tinyxml2::XMLElement *element) = 0;
};

template <typename T>
class TComponentBuilder : public ComponentBuilder
{
public:
	virtual ~TComponentBuilder() {}
	T *Build(tinyxml2::XMLElement *element) override
	{
		T *result = new T;
		result->Init(element);
		return result;
	}
};

class ComponentFactory : public SingletonBase<ComponentFactory>
{
	typedef std::map<std::string, ComponentBuilder *> ComponentBuilderMap;
	typedef std::map<std::string, ComponentBuilder *>::iterator ComponentBuilderMapIter;

public:
	ComponentFactory();
	~ComponentFactory();

	bool Init();
	void Release();

	Component *Build(const std::string &key, tinyxml2::XMLElement *element);

	void RegisterBuilder(const std::string &key, ComponentBuilder *builder);
	void UnRegisterBuilder(const std::string &key);


private:
	ComponentBuilderMap _builders;
};

#endif