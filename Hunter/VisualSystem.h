#ifndef VISUAL_SYSTEM_H
#define VISUAL_SYSTEM_H

#include "VisualComponent.h"

constexpr ComponentID BUFFER_VISUAL_COMPONENT_ID = 1;
constexpr ComponentID MESH_VISUAL_COMPONENT_ID = 2;

class VisualSystem
{
public :
	VisualSystem();
	virtual ~VisualSystem();

	void Update(float deltaTime);

	VisualComponent *GetComponent(ComponentID componentID);

private :
	void UpdateComponent(ComponentHandle handle);

};


#endif