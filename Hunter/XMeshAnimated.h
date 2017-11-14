#ifndef XMESH_ANIMATED_H
#define XMESH_ANIMATED_H

struct XMeshAnimated;

struct FrameEX : public D3DXFRAME
{
	Matrix CombinedTransformationMatrix;			//���������� �θ� ����� ����Ǿ� ��ȯ�� �Ϸ�� ���
};

struct ContainerEX : public D3DXMESHCONTAINER
{
	//LPD3DXMESH						OriginalMesh;			//��ȯ�� �޽� ������
	LPD3DXMESH WorkingMesh;			//�������̴��� ����ϱ����� �ִ� �ȷ�Ʈ�� MATRIX_PALETTE_SIZE ��� ��������� ������ �޽� 
	std::vector<std::string> TexturePaths;

	//DWORD								NumAttributesGroup;		//�޽��� �Ӽ� �׷�� ( �ش� �޽ÿ� 
	//D3DXATTRIBUTERANGE*				AttributeTable;			//�޽��� �Ӽ� ���̺� ( �ش� ���� ����� Mesh �� Subset, MaterialID ���� ������ ��� �ִ� )

	Matrix** ppBoneMatrixPtrs;		//������ ��� [ ������ �迭 ]
	Matrix*	pBoneOffsetMatices;		//�ڽ��� �⺻ ��� �迭
														//D3DXMATRIXA16* pCurrentBoneMatrices;	//���� Animation �� ����� ���
	DWORD NumPaletteEntries;		//�����޽��� ����ȷ�Ʈ ��
	DWORD MaxNumFaceInfls;		//�ش� �޽ÿ� ����Ǵ� �����ϳ��� �ִ� ����ġ ����
	DWORD NumAttributesGroup;		//�޽��� �Ӽ� �׷�� ( �ش� �޽ÿ� ���͸��������� ��ִ�? )
	LPD3DXBUFFER BufBoneCombos;			//���ĺ���̼� ( �޽ÿ� ����Ǵ� �� ID ������ ���͸��� ���� )
};

class HierachyEX : public ID3DXAllocateHierarchy
{
public:
	std::string _fileName;		//�ε��Ǵ� ���� ��
	std::string _filePath;		//�ε��Ǵ� ���� ���
	XMeshAnimated *_pAnimatedMesh{};
public:
	//void SetLoadPath( std::string filePath );
	//void SetSkinnedMesh( cXMesh_Skinned* pSkinnedMesh );

public:
	STDMETHOD( CreateFrame )( THIS_ LPCSTR Name, LPD3DXFRAME* ppNewFrame) override;

	STDMETHOD( CreateMeshContainer )( THIS_ LPCSTR Name, CONST D3DXMESHDATA* pMeshData,	
		CONST D3DXMATERIAL* pMaterials,	CONST D3DXEFFECTINSTANCE* pEffectInstance,		
		DWORD NumMaterials,	CONST DWORD* pAdjacency, LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER* ppNewMeshContainer) override; 

	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree) override;
	STDMETHOD( DestroyMeshContainer )( THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree) override; 

};

struct FrameNeighborInfo
{
	FrameEX *pFrame;
	uint16 numChild;
};

struct XMeshAnimated
{
public :
	XMeshAnimated(void);
	~XMeshAnimated(void);
	HRESULT Load(const std::string &filePath, const Matrix* pMatCorrection = nullptr);
	void Release();
	//������Ĵ�� �޽��� �����Ѵ�.
	void MeshCorrection(const Matrix* pMatCorrection);
	void BuidSubMeshBoundInfo();

	//�� ����� �����Ѵ�.
	void InitBoneMatrixPointer(FrameEX *pFrame);

	void GetTotalNumFrame(uint32 *pOutCount, FrameEX *frame = nullptr);
	void ChildFirstTraverse(std::stack<uint32> &indexStack, FrameEX *pFrame = nullptr);

	void GetNumSiblings(uint32 &pOutCount, FrameEX *pFrame = nullptr);

	//Variables
	Matrix _correctionMatrix;

	FrameEX	*_pRootbone; //��Ʈ ��

	DWORD _workingPaletteSize;
	std::vector<FrameEX *> _linearFrames;
	std::vector<uint16> _frameIndex;
};

#endif