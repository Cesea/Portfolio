#include "stdafx.h"
#include "ActionSystem.h"

ActionSystem::ActionSystem()
{
}

ActionSystem::~ActionSystem()
{
}

void ActionSystem::Update(float deltaTime)
{
	static int32 count = 0;

	auto &entities = GetEntities();
	for (int32 i = 0; i < entities.size(); ++i)
	{
		RenderComponent &refRenderComp = entities[i].GetComponent<RenderComponent>();
		if (refRenderComp._type == RenderComponent::Type::eSkinned)
		{
			ActionComponent &refActionComp = entities[i].GetComponent<ActionComponent>();
			refActionComp.UpdateAnimation(deltaTime);
		}
	}
	count++;
}

void ActionSystem::Initialize()
{
}

void ActionSystem::OnEntityAdded(Entity& entity)
{
}

void ActionSystem::OnEntityRemoved(Entity& entity)
{
}