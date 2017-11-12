#ifndef XMESH_H
#define XMESH_H

class XMesh 
{
public:
	LPD3DXMESH							pMesh;
	DWORD								dwMaterialsNum;			//로딩된 매쉬의 재질 갯수
	std::vector<LPDIRECT3DTEXTURE9>		vecDiffuseTex;			//로딩된 메쉬에서 사용하는 DiffuseTexture
	std::vector<LPDIRECT3DTEXTURE9>		vecNormalTex;			//로딩된 메쉬에서 사용하는 NormalTexture
	std::vector<LPDIRECT3DTEXTURE9>		vecSpecularTex;			//로딩된 메쉬에서 사용하는 SpecularTexture
	std::vector<LPDIRECT3DTEXTURE9>		vecEmissionTex;			//로딩된 메쉬에서 사용하는 EmissionTexture
	std::vector<D3DMATERIAL9>			vecMaterials;			//로딩된 메쉬에서 사용하는 매터리얼...

	std::vector<D3DXVECTOR3>			Vertices;				//정점들
	std::vector<D3DXVECTOR3>			Normals;				//점정 노말
	std::vector<DWORD>					Indices;				//인덱스
	DWORD								TriNum;					//삼각면의 갯수

	D3DXVECTOR3							Bound_Min;					//바운드의 최소 위치 
	D3DXVECTOR3							Bound_Max;					//바운드의 최대 위치 
	D3DXVECTOR3							Bound_Center;				//바운드의 센터	( 주의 사항 피봇이 아니다 )
	D3DXVECTOR3							Bound_Size;					//바운드의 크기
	D3DXVECTOR3							Bound_HalfSize;				//바운드의 절반크기
	float								Bound_Radius;				//바운드의 외접구 반지름

																	//물리매니져는 내친구.....
	friend class cPhysicManager;

public:
	XMesh(void);
	~XMesh(void);

	virtual HRESULT Init(std::string filePath, const D3DXMATRIXA16* matCorrection = NULL);

	virtual void Release();

	virtual void Render();


private:
	//보정행렬대로 메쉬를 수정한다.
	void MeshCorrection(const D3DXMATRIXA16* pmatCorrection);


public:
};



#endif