#include "stdafx.h"
#include "ComponentFactory.h"

#include "CommandComponent.h"
#include "HealthComponent.h"
#include "PhysicsComponent.h"
#include "RenderComponent.h"

ComponentFactory::ComponentFactory()
{
}

ComponentFactory::~ComponentFactory()
{
}

bool ComponentFactory::Init()
{
	//RegisterBuilder("PlayerControlComponent", new TComponentBuilder<PlayerControlComponent>());
	//RegisterBuilder("AIControlComponent", new TComponentBuilder<AIControlComponent>());
	//RegisterBuilder("RenderComponent", new TComponentBuilder<RenderComponent>());
	return S_OK;
}

void ComponentFactory::Release()
{
}

Component * ComponentFactory::Build(const std::string & key, tinyxml2::XMLElement * element)
{
	return nullptr;
}

void ComponentFactory::RegisterBuilder(const std::string & key, ComponentBuilder * builder)
{
}

void ComponentFactory::UnRegisterBuilder(const std::string & key)
{
}
