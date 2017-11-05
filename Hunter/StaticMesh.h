#ifndef STATIC_MESH_H
#define STATIC_MESH_H

#include "IMesh.h"

class StaticMesh : public IMesh
{
public :
	StaticMesh();
	~StaticMesh();

	virtual HRESULT Init(const std::string &filePath, const D3DXMATRIXA16* matCorrection = NULL);
	virtual void Release();

private :
};

#endif