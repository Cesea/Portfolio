#include "stdafx.h"
#include "Mesh.h"

Mesh::Mesh()
{
}


void Mesh::Cull(const Frustum &frustum)
{
	//frustum.Cull(&_boundingBoxes[0], &_visibility[0], _boundingBoxes.size());
}

void Mesh::Render()
{
	//context->BindVertexBuffer();
	//context->BindIndexBuffer();
	//make Draw calls
	//for (uint32 i = 0; i < _numVisibleSubMesh; ++i)
	//{
		//const SubMesh &subMesh = _subMeshes[i];
		//subMesh->Draw
	//}
}
