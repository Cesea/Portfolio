#ifndef VIDEO_VERTEX_FORMAT_H
#define VIDEO_VERTEX_FORMAT_H

#include "Typedefs.h"

namespace video
{
	struct StaticMeshVertex
	{
		StaticMeshVertex();
		StaticMeshVertex(const Vector3 &position, const Vector3 &normal, const Vector3 &binormal, const Vector3 &tangent, const Vector2 &uv);


		Vector3 _position;
		Vector3 _normal;
		Vector3 _binormal;
		Vector3 _tangent;
		Vector2 _uv;
	};

}

#endif
