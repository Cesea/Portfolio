#ifndef XMESH_H
#define XMESH_H

struct XMesh 
{
public:
	LPD3DXMESH _pMesh;
	uint32 _materialNum;			//로딩된 매쉬의 재질 갯수

	std::vector<std::string> texturePaths;	
	std::vector<D3DMATERIAL9> vecMaterials;	

	std::vector<Vector3> _vertices;				//정점들
	std::vector<Vector3> _normals;				//점정 노말
	std::vector<uint32>	_indices;				//인덱스
	uint32 _triNum;					//삼각면의 갯수

	Vector3	_boundMin;					//바운드의 최소 위치 
	Vector3	_boundMax;					//바운드의 최대 위치 
	Vector3	_boundCenter;				//바운드의 센터	( 주의 사항 피봇이 아니다 )
	Vector3	_boundSize;					//바운드의 크기
	Vector3	_boundHalfSize;				//바운드의 절반크기
	float _boundRadius;				//바운드의 외접구 반지름

	D3DVERTEXELEMENT9 _pVerElement[MAX_FVF_DECL_SIZE];
	D3DXATTRIBUTERANGE *_attributeRange{nullptr};

	LPD3DXBUFFER _pAdjacency{};
	LPD3DXBUFFER _pMaterial{};

public:
	XMesh(void);
	~XMesh(void);

	virtual HRESULT Load(const std::string &fileName, const Matrix* matCorrection = nullptr);

	virtual void Release();

	virtual void Render();

	//보정행렬대로 메쉬를 수정한다.
	void MeshCorrection(const Matrix* pMatCorrection);

	//Utils
	//HRESULT CloneMeshFVF(DWORD options, DWORD fvf, ID3DXMesh *pMeshOut);

public:
};

#endif