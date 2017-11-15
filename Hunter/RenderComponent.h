#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H

#include "Component.h"

//COmponent는 model에 대하여 소유권을 가지지 않는다
struct RenderComponent : public Component
{
public:
	RenderComponent();
	virtual ~RenderComponent() {}

public :
	enum MeshType
	{
		eBuffer,
		eStatic,
		eSkinned
	};

	union
	{
		struct
		{
			video::VertexBufferHandle _vb;
			video::IndexBufferHandle _ib;
		};
		video::SkinnedXMeshHandle _skinned;
		video::StaticXMeshHandle _static;
	};
	float radius{};
};

#endif