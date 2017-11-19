#include "stdafx.h"
#include "ScriptComponent.h"	

void ScriptComponent::SetScript(const ScriptUpdateFunction &function)
{
	_updateFunction = function;
}
