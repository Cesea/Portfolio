#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H

#include "Component.h"

//COmponent는 model에 대하여 소유권을 가지지 않는다
struct RenderComponent : public Component
{
public:
	RenderComponent() {}
	virtual ~RenderComponent() {}


	float radius{};

	uint32 _startVertex{};

	uint32 _startIndex{};
	uint32 _numVertices{};
	uint32 _numPrim{};
};

#endif