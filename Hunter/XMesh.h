#ifndef XMESH_H
#define XMESH_H

struct XMesh 
{
public:
	LPD3DXMESH _pMesh;
	uint32 _materialNum;			//�ε��� �Ž��� ���� ����

	std::vector<std::string> texturePaths;	
	std::vector<D3DMATERIAL9> vecMaterials;	

	std::vector<Vector3> _vertices;				//������
	std::vector<Vector3> _normals;				//���� �븻
	std::vector<uint32>	_indices;				//�ε���
	uint32 _triNum;					//�ﰢ���� ����

	Vector3	_boundMin;					//�ٿ���� �ּ� ��ġ 
	Vector3	_boundMax;					//�ٿ���� �ִ� ��ġ 
	Vector3	_boundCenter;				//�ٿ���� ����	( ���� ���� �Ǻ��� �ƴϴ� )
	Vector3	_boundSize;					//�ٿ���� ũ��
	Vector3	_boundHalfSize;				//�ٿ���� ����ũ��
	float _boundRadius;				//�ٿ���� ������ ������

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

	//������Ĵ�� �޽��� �����Ѵ�.
	void MeshCorrection(const Matrix* pMatCorrection);

	//Utils
	//HRESULT CloneMeshFVF(DWORD options, DWORD fvf, ID3DXMesh *pMeshOut);

public:
};

#endif