#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H

#include "Component.h"

//COmponent�� model�� ���Ͽ� �������� ������ �ʴ´�
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