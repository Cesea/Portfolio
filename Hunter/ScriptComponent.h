#ifndef SCRIPT_COMPONENT_H
#define SCRIPT_COMPONENT_H

#include "Component.h"
#include "Delegate.h"

typedef Delegate<void, float> ScriptUpdateFunction;

#define MAKE_SCRIPT_DELEGATE(Object, Function, Instance) \
	ScriptUpdateFunction::FromFunction<Object, &Object::Function>(&Instance)

struct ScriptComponent : public Component
{
	ScriptComponent() {}
	virtual ~ScriptComponent() {}

	void SetScript(const ScriptUpdateFunction &updateFunction);

	ScriptUpdateFunction _updateFunction;
};

//struct PlayerScript : public ScriptComponent
//{
//	PlayerScript() {};
//	virtual ~PlayerScript() {}
//
//	virtual void Update(float deltaTime)
//	{
//		float b = 0;
//
//	}
//};
//
//struct MonsterScript : public ScriptComponent
//{
//	MonsterScript() {};
//	virtual ~MonsterScript() {}
//	virtual void Update(float deltaTime)
//	{
//		float a = 0;
//	}
//
//};



#endif
