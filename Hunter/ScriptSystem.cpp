#include "stdafx.h"
#include "ScriptSystem.h"

ScriptSystem::ScriptSystem()
{
}

ScriptSystem::~ScriptSystem()
{
}

void ScriptSystem::Update(float deltaTime)
{
	auto &entities = GetEntities();
	for (uint32 i = 0; i < entities.size(); ++i)
	{
		ScriptComponent &refScriptComponent = entities[i].GetComponent<ScriptComponent>();

		refScriptComponent._updateFunction.Invoke(deltaTime);
	}
}

void ScriptSystem::Initialize()
{
	Console::Log("Script System Initialized\n");
}

void ScriptSystem::OnEntityAdded(Entity & entity)
{
}

void ScriptSystem::OnEntityRemoved(Entity & entity)
{
}

