#ifndef I_MESH_H
#define I_MESH_H

class IMesh
{
public:
	IMesh();
	~IMesh();

	virtual HRESULT Init(const std::string &filePath, const D3DXMATRIXA16* matCorrection = NULL) = 0;
	virtual void Release() = 0;
	//virtual void Render(const Transform& Trans) = 0;
};

#endif