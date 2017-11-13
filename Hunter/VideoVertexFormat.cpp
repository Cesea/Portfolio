#include "stdafx.h"
#include "VideoVertexFormat.h"

namespace video
{
	StaticMeshVertex::StaticMeshVertex()
		:_position(), _normal(), _binormal(), _tangent(), _uv()
	{

	}

	StaticMeshVertex::StaticMeshVertex(const Vector3 & position, const Vector3 & normal,
		const Vector3 & binormal, const Vector3 & tangent, const Vector2 & uv)
		: _position(position), _normal(normal), _binormal(binormal), _tangent(tangent), _uv(uv)
	{
	}
	
}
