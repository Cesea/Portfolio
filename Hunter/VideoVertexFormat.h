#ifndef VIDEO_VERTEX_FORMAT_H
#define VIDEO_VERTEX_FORMAT_H

#include "Typedefs.h"

namespace video
{
	struct TerrainVertex
	{
		Vector3 _pos;
		Vector3 _normal;
		Vector3 _binormal;
		Vector3 _tangent;
		Vector2 _baseUV;
		Vector2 _tileUV;

		static std::string _name;
	};

	struct StaticTestVertex
	{
		StaticTestVertex();
		StaticTestVertex(const Vector3 &position, const Vector3 &normal, const Vector2 &uv);

		Vector3 _position;
		Vector3 _normal;
		Vector2 _uv;

		static std::string _name;
	};

	struct StaticMeshVertex
	{
		StaticMeshVertex();
		StaticMeshVertex(const Vector3 &position, const Vector3 &normal, const Vector3 &binormal, const Vector3 &tangent, const Vector2 &uv);

		Vector3 _position;
		Vector3 _normal;
		Vector3 _binormal;
		Vector3 _tangent;
		Vector2 _uv;

		static std::string _name;
	};

	struct DebugVertex
	{
		DebugVertex();
		DebugVertex(const Vector3 &position, const uint32 color);

		Vector3 _position;
		uint32 _color;

		static std::string _name;
	};

}

#endif
