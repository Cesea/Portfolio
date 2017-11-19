#include "stdafx.h"
#include "VideoVertexFormat.h"

namespace video
{
	std::string TerrainVertex::_name = "TerrainVertex";
	std::string StaticTestVertex::_name = "StaticTestVertex";
	std::string StaticMeshVertex::_name = "StaticMeshVertex";
	std::string DebugVertex::_name = "DebugVertex";

	StaticTestVertex::StaticTestVertex()
		:_position(), _normal(), _uv()
	{
	}

	StaticTestVertex::StaticTestVertex(const Vector3 & position, const Vector3 & normal, const Vector2 & uv)
		: _position(position), _normal(normal), _uv(uv)
	{
	}

	StaticMeshVertex::StaticMeshVertex()
		:_position(), _normal(), _binormal(), _tangent(), _uv()
	{

	}

	StaticMeshVertex::StaticMeshVertex(const Vector3 & position, const Vector3 & normal,
		const Vector3 & binormal, const Vector3 & tangent, const Vector2 & uv)
		: _position(position), _normal(normal), _binormal(binormal), _tangent(tangent), _uv(uv)
	{
	}
	
	DebugVertex::DebugVertex()
		:_position(), _color(0xffffffff)
	{
	}

	DebugVertex::DebugVertex(const Vector3 & position, uint32 color)
		: _position(position), _color(color)
	{
	}

	DebugVertex::DebugVertex(const DebugVertex & other)
		: _position(other._position), _color(other._color)
	{
	}

	DebugVertex & DebugVertex::operator=(const DebugVertex & other)
	{
		_position = other._position;
		_color = other._color;
		return *this;
	}
}
