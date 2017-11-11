#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H

#include "Component.h"

struct RenderComponent : public Component
{
public:
	RenderComponent();
	virtual ~RenderComponent() {}

public :
	//VertexBufferHandle _vertexBufferHandle;
	//IndexBufferHandle _indexBufferHandle;

	//EffectHandle _effectHandle;
	//SurfaceMaterialHandle _materialHandle;

};

#endif