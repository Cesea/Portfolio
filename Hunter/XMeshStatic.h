#ifndef XMESH_STATIC_H
#define XMESH_STATIC_H

//TODO : XMesh에서 하나의 서브셋 별로 BoundInfo를 만들어서 RenderGroup에 전달 할 수 있게끔 만들자

//TODO : TriangleGroup을 계산의 용도로 사용 해야 할 것 같다. XMesh에서 이 정보를 만들고 RenderGroup이나 Model에서 
//사용 할 수 있게금 만들자
struct TriangleGroup
{
	std::vector<Vector3> _vertices;
	std::vector<Vector3> _normals;
	std::vector<uint16> _indices;
};

struct XMeshStatic 
{
	struct BoundInfo
	{
		Vector3 _boundMin;
		Vector3 _boundMax;
		Vector3 _boundCenter;
		Vector3 _boundSize;
		Vector3 _boundHalfSize;
		float _boundRadius;
	};

	XMeshStatic(void);
	~XMeshStatic(void);
	HRESULT Load(const std::string &fileName, const Matrix* matCorrection = nullptr);
	void Release();
	//보정행렬대로 메쉬를 수정한다.
	void MeshCorrection(const Matrix* pMatCorrection);
	void BuidSubMeshBoundInfo();
	//HRESULT CloneMeshFVF(DWORD options, DWORD fvf, ID3DXMesh *pMeshOut);


	LPD3DXMESH _pMesh;
	uint32 _numMaterial;			//로딩된 매쉬의 재질 갯수

	std::vector<std::string> _texturePaths;	
	std::vector<D3DMATERIAL9> _vecMaterials;	

	std::vector<Vector3> _vertices;				//정점들
	std::vector<Vector3> _normals;				//점정 노말
	std::vector<uint32>	_indices;				//인덱스
	uint32 _triNum;					//삼각면의 갯수

	BoundInfo _meshBoundInfo; //메쉬 전체의 바운딩 인포이다.
	BoundInfo *_subMeshBoundInfo{};

	D3DVERTEXELEMENT9 _pVerElement[MAX_FVF_DECL_SIZE];
	D3DXATTRIBUTERANGE *_attributeRange{nullptr};

	LPD3DXBUFFER _pAdjacency{};
	LPD3DXBUFFER _pMaterial{};

	void CalculateBoundingInfo(std::vector<Vector3> &positions, BoundInfo *pOutBoundInfo, 
		uint32 startVertex, uint32 endVertex);
};

#endif