#ifndef MESH_H
#define MESH_H

struct SubMesh
{
	uint32 startVertex;
	uint32 numIndices;
};

class Mesh
{
public :
	Mesh();

	void Cull(const Frustum &frustum);
	void Render();

private :

  IDirect3DVertexBuffer9 *_pVertexBuffer;
  IDirect3DIndexBuffer9* _pIndexBuffer;
 
  std::vector<AABB> _boundingBoxes;
  //std::vector<MaterialHandle> _materials;
  std::vector<SubMesh> _subMeshes;
  std::vector<bool32> _visibility;
};





#endif