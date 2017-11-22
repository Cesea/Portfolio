#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H

#include "Component.h"

//COmponent�� model�� ���Ͽ� �������� ������ �ʴ´�
struct RenderComponent : public Component
{
public:
	RenderComponent() {}
	virtual ~RenderComponent() {}

	enum Type
	{
		eBuffer,
		eStatic,
		eSkinned
	};
	Type _type;
	union
	{
		struct
		{
			video::VertexBufferHandle _vHandle;
			video::IndexBufferHandle _iHandle;
		};
		video::StaticXMeshHandle _static;
		video::AnimationInstanceHandle _skinned;
	};
};

#endif