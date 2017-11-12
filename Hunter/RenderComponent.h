#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H

#include "Component.h"




struct RenderComponent : public Component
{
public:
	RenderComponent();
	virtual ~RenderComponent() {}

public :
	video::VertexBufferHandle _vertexBuffer{};
	video::IndexBufferHandle _indexBuffer{};
	video::MaterialHandle _material{};

	video::EffectHandle _effect{};

	float radius{};

};

#endif