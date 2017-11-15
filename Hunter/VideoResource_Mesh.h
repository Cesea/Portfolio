#ifndef VIDEO_RESOURCE_MESH_H
#define VIDEO_RESOURCE_MESH_H

#include "VideoResourceHandles.h"

namespace video
{
	struct SkinnedXMesh;
}

struct Bone : public D3DXFRAME
{
	Matrix CombinedTransformationMatrix;			//계층구조의 부모 행렬이 적용되어 변환이 완료된 행렬
};

struct BoneMesh : public D3DXMESHCONTAINER
{
	ID3DXMesh *WorkingMesh;			
	std::vector<video::MaterialHandle> _materialHandles;
	//std::vector<D3DMATERIAL9> Materials;				//메시의 재질 데이터
	//std::vector<LPDIRECT3DTEXTURE9> DiffuseTexs;			//로드된 XFile 에서 사용되는 Texture 들
	//std::vector<LPDIRECT3DTEXTURE9> NormalTexs;
	//std::vector<LPDIRECT3DTEXTURE9> SpecularTexs;
	//std::vector<LPDIRECT3DTEXTURE9> EmissionTexs;

	//DWORD								NumAttributesGroup;		//메시의 속성 그룹수 ( 해당 메시에 
	//D3DXATTRIBUTERANGE*				AttributeTable;			//메시의 속성 테이블 ( 해당 본에 적용된 Mesh 의 Subset, MaterialID 같은 정보를 담고 있다 )

	Matrix** ppBoneMatrixPtrs;		//본들의 행렬 [ 포인터 배열 ]
	Matrix* pBoneOffsetMatices;		//자신의 기본 행렬 배열

	DWORD NumPaletteEntries;		//현본메쉬의 행렬팔래트 수
	DWORD MaxNumFaceInfls;		//해당 메시에 적용되는 정점하나당 최대 가중치 갯수
	DWORD NumAttributesGroup;		//메시의 속성 그룹수 ( 해당 메시에 메터리얼정보가 몇개있니? )
	LPD3DXBUFFER BufBoneCombos;			//본컴비네이션 ( 메시에 적용되는 본 ID 정보와 메터리얼 정보 )
};

class BoneHierachy : public ID3DXAllocateHierarchy
{
private:
	video::SkinnedXMesh* _pSkinnedMesh;	//자신이 로딩하는 스킨드 메시
	std::string _filePath;

public:
	void SetSkinnedMesh(video::SkinnedXMesh *pSkinnedMesh);
	void SetFilePath(const std::string &filePath) { _filePath = filePath; }
public:

	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME* ppNewFrame ) override;
	STDMETHOD(CreateMeshContainer)(THIS_ LPCSTR Name, CONST D3DXMESHDATA* pMeshData, CONST D3DXMATERIAL* pMaterials,
		CONST D3DXEFFECTINSTANCE* pEffectInstance, DWORD NumMaterials, CONST DWORD* pAdjacency, LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER* ppNewMeshContainer) override;

	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree) override;
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree) override;
};


namespace video
{
	struct MeshBoundInfo
	{
		Vector3 _min;
		Vector3 _max;
		Vector3 _center;
		Vector3 _size;
		Vector3 _halfSize;
		float _radius;
	};

	//struct MeshContainer
	//{
	//};

	//NOTE : 지금 mesh가 staticMesh, skinnedmesh 두개로 나누어져 있다... 이것을 하나로 합할 수 있을까
	
	struct StaticXMesh
	{
		bool Create(const std::string &fileName, const Matrix* matCorrection = nullptr);
		void Destroy();
		void MeshCorrection(const Matrix* pMatCorrection);
		void BuidSubMeshBoundInfo();
		void CalculateBoundingInfo(std::vector<Vector3> &positions, MeshBoundInfo *pOutBoundInfo,
			uint32 startVertex, uint32 endVertex);

		ID3DXMesh *_pMesh{};
		uint32 _numMaterial{};
		std::vector<video::MaterialHandle> _materialHandles;

		MeshBoundInfo _meshBoundInfo{};
		std::vector<MeshBoundInfo> _submeshBoundInfos;

		std::vector<D3DXATTRIBUTERANGE> _attributeRanges;
		D3DXATTRIBUTERANGE *_attributeRange{};

		uint32 _numVertices{};
		uint32 _numFaces{};
		std::vector<Vector3> _vertices;
		std::vector<Vector3> _normals;
		std::vector<uint16> _indices;
	};

	struct SkinnedXMesh
	{
		bool Create(const std::string &fileName, const Matrix* matCorrection = nullptr);
		void Destroy();

		void InitBoneMatrixPointer(Bone *pBone);
		void UpdateMatrices(Bone *pBone, Matrix *pParentMatrix);

		void Update(const Matrix *pWorld);
		void Render();
		void RenderBone(Bone* pBone);

		//void RenderChildBoneName(cCamera* pCam, Bone* pBone, D3DXVECTOR3* parentPos);
		void SetAnimation(LPD3DXANIMATIONSET animSet);

		Matrix _matCorrection;

		Bone *_pRootBone;
		uint32 _numWorkingPalette;
		Matrix *_workingPalettes;

		ID3DXAnimationController *_pAnimationController;
	};
}

#endif