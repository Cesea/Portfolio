#ifndef XMESH_ANIMATED_H
#define XMESH_ANIMATED_H

struct XMeshAnimated;

struct FrameEX : public D3DXFRAME
{
	Matrix CombinedTransformationMatrix;			//계층구조의 부모 행렬이 적용되어 변환이 완료된 행렬
};

struct ContainerEX : public D3DXMESHCONTAINER
{
	//LPD3DXMESH						OriginalMesh;			//변환전 메시 데이터
	LPD3DXMESH WorkingMesh;			//정점셰이더를 사용하기위해 최대 팔래트수 MATRIX_PALETTE_SIZE 대로 서브셋으로 나눠진 메쉬 
	std::vector<std::string> TexturePaths;

	//DWORD								NumAttributesGroup;		//메시의 속성 그룹수 ( 해당 메시에 
	//D3DXATTRIBUTERANGE*				AttributeTable;			//메시의 속성 테이블 ( 해당 본에 적용된 Mesh 의 Subset, MaterialID 같은 정보를 담고 있다 )

	Matrix** ppBoneMatrixPtrs;		//본들의 행렬 [ 포인터 배열 ]
	Matrix*	pBoneOffsetMatices;		//자신의 기본 행렬 배열
														//D3DXMATRIXA16* pCurrentBoneMatrices;	//현제 Animation 이 적용된 행렬
	DWORD NumPaletteEntries;		//현본메쉬의 행렬팔래트 수
	DWORD MaxNumFaceInfls;		//해당 메시에 적용되는 정점하나당 최대 가중치 갯수
	DWORD NumAttributesGroup;		//메시의 속성 그룹수 ( 해당 메시에 메터리얼정보가 몇개있니? )
	LPD3DXBUFFER BufBoneCombos;			//본컴비네이션 ( 메시에 적용되는 본 ID 정보와 메터리얼 정보 )
};

class HierachyEX : public ID3DXAllocateHierarchy
{
public:
	std::string _fileName;		//로딩되는 파일 명
	std::string _filePath;		//로딩되는 파일 경로
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
	//보정행렬대로 메쉬를 수정한다.
	void MeshCorrection(const Matrix* pMatCorrection);
	void BuidSubMeshBoundInfo();

	//본 행렬을 셋팅한다.
	void InitBoneMatrixPointer(FrameEX *pFrame);

	void GetTotalNumFrame(uint32 *pOutCount, FrameEX *frame = nullptr);
	void ChildFirstTraverse(std::stack<uint32> &indexStack, FrameEX *pFrame = nullptr);

	void GetNumSiblings(uint32 &pOutCount, FrameEX *pFrame = nullptr);

	//Variables
	Matrix _correctionMatrix;

	FrameEX	*_pRootbone; //루트 본

	DWORD _workingPaletteSize;
	std::vector<FrameEX *> _linearFrames;
	std::vector<uint16> _frameIndex;
};

#endif