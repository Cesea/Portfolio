#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H

#include "Component.h"

//COmponent�� model�� ���Ͽ� �������� ������ �ʴ´�
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