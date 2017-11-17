#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H

#include "Component.h"
#include "AnimationComponent.h"

//COmponent는 model에 대하여 소유권을 가지지 않는다
struct RenderComponent : public Component
{
public:
	RenderComponent() {}
	virtual ~RenderComponent() {}

	//enum MeshType
	//{
	//	eBuffer,
	//	eStatic,
	//	eSkinned
	//};

	//union Handles
	//{
	//	struct 
	//	{
	//		video::VertexBufferHandle _vb;
	//		video::IndexBufferHandle _ib;
	//	};
	//	struct 
	//	{
	//		video::SkinnedXMeshHandle _skinned;
	//		animation::AnimationComponentHandle _animation;
	//	};
	//	video::StaticXMeshHandle _static;
	//	Handles() {}
	//	~Handles() {}
	//};
	//MeshType _type;
	//Handles _handles;

	float radius{};

	uint32 _startVertex{};

	uint32 _startIndex{};
	uint32 _numVertices{};
	uint32 _numPrim{};
};

#endif