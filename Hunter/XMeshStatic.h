#ifndef XMESH_STATIC_H
#define XMESH_STATIC_H

//TODO : XMesh���� �ϳ��� ����� ���� BoundInfo�� ���� RenderGroup�� ���� �� �� �ְԲ� ������

//TODO : TriangleGroup�� ����� �뵵�� ��� �ؾ� �� �� ����. XMesh���� �� ������ ����� RenderGroup�̳� Model���� 
//��� �� �� �ְԱ� ������
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
	//������Ĵ�� �޽��� �����Ѵ�.
	void MeshCorrection(const Matrix* pMatCorrection);
	void BuidSubMeshBoundInfo();
	//HRESULT CloneMeshFVF(DWORD options, DWORD fvf, ID3DXMesh *pMeshOut);


	LPD3DXMESH _pMesh;
	uint32 _numMaterial;			//�ε��� �Ž��� ���� ����

	std::vector<std::string> _texturePaths;	
	std::vector<D3DMATERIAL9> _vecMaterials;	

	std::vector<Vector3> _vertices;				//������
	std::vector<Vector3> _normals;				//���� �븻
	std::vector<uint32>	_indices;				//�ε���
	uint32 _triNum;					//�ﰢ���� ����

	BoundInfo _meshBoundInfo; //�޽� ��ü�� �ٿ�� �����̴�.
	BoundInfo *_subMeshBoundInfo{};

	D3DVERTEXELEMENT9 _pVerElement[MAX_FVF_DECL_SIZE];
	D3DXATTRIBUTERANGE *_attributeRange{nullptr};

	LPD3DXBUFFER _pAdjacency{};
	LPD3DXBUFFER _pMaterial{};

	void CalculateBoundingInfo(std::vector<Vector3> &positions, BoundInfo *pOutBoundInfo, 
		uint32 startVertex, uint32 endVertex);
};

#endif