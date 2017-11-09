#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H

#include "Component.h"

struct RenderComponent : public Component
{
public:
	RenderComponent();
	virtual ~RenderComponent() {}

};

#endif