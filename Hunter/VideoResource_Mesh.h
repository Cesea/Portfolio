#ifndef VIDEO_RESOURCE_MESH_H
#define VIDEO_RESOURCE_MESH_H

#include "VideoResourceHandles.h"

namespace video
{
	struct SkinnedXMesh;
	struct SkinnedAnimation;
}

struct Bone : public D3DXFRAME
{
	Matrix CombinedTransformationMatrix;			//계층구조의 부모 행렬이 적용되어 변환이 완료된 행렬
};

struct BoneMesh : public D3DXMESHCONTAINER
{
	bool32 _visible{ true };
	ID3DXMesh *WorkingMesh;			
	std::vector<video::MaterialHandle> _materialHandles;

	Matrix** ppBoneMatrixPtrs{};		//본들의 행렬 [ 포인터 배열 ]
	Matrix* pBoneOffsetMatices{};		//자신의 기본 행렬 배열

	DWORD NumPaletteEntries{};		//현본메쉬의 행렬팔래트 수
	DWORD MaxNumFaceInfls{};		//해당 메시에 적용되는 정점하나당 최대 가중치 갯수
	DWORD NumAttributesGroup{};		//메시의 속성 그룹수 ( 해당 메시에 메터리얼정보가 몇개있니? )
	LPD3DXBUFFER BufBoneCombos{};			//본컴비네이션 ( 메시에 적용되는 본 ID 정보와 메터리얼 정보 )
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

	struct MeshVertInfo
	{
		uint32 _numVertices;
		uint32 _numFaces;
		std::vector<Vector3> _positions;
		std::vector<Vector3> _normals;
		std::vector<uint16> _indices;
	};

	//struct MeshContainer
	//{
	//};
	//NOTE : 지금 mesh가 staticMesh, skinnedmesh 두개로 나누어져 있다... 이것을 하나로 합할 수 있을까

	void ResizeMeshAndGetInfos(ID3DXMesh *pMesh, const Matrix &correction, 
		MeshVertInfo *pOutVertInfo, MeshBoundInfo *pOutBoundInfo);

	void CalculateBoundInfo(std::vector<Vector3> &positions, MeshBoundInfo *pOutBoundInfo,
			uint32 startVertex, uint32 endVertex);
	
	struct StaticXMesh
	{
		bool Create(const std::string &fileName, const Matrix* matCorrection = nullptr);
		void Destroy();
		void BuidSubMeshBoundInfo();

		void FillRenderCommand(RenderView &renderView, video::EffectHandle effect);

		bool32 _visible{ true };
		ID3DXMesh *_pMesh{};
		uint32 _numMaterial{};
		std::vector<video::MaterialHandle> _materialHandles;

		MeshBoundInfo _meshBoundInfo{};
		std::vector<MeshBoundInfo> _submeshBoundInfos;

		D3DXATTRIBUTERANGE *_attributeRange{};

		MeshVertInfo _meshVertInfo{};

		VertexBufferHandle _vHandle;
		IndexBufferHandle _iHandle;
	};

	//에니메이션을 위한 typedef


	typedef std::map<std::string, BoneMesh *> BoneMeshTable;
	typedef std::map<std::string, Bone *> BoneTable;

	struct SkinnedXMesh
	{
		bool Create(const std::string &fileName, const Matrix* matCorrection = nullptr);
		void Destroy();

		void InitBoneMatrixPointer(Bone *pBone);

		void Update(const Matrix *pWorld);
		void UpdateMatrices(Bone *pBone, Matrix *pParentMatrix) const;

		//NOTE : 직접 사용하지 않고 밖에서 쓴다
		void RenderBone(const video::Effect &effect, Bone *pBone, animation::AnimationComponent &animation) const;

		Matrix _matCorrection;

		Bone *_pRootBone{};
		uint32 _numWorkingPalette;
		ID3DXAnimationController *_pAnimationController{};

		uint32 _numSubset{};

		BoneMeshTable _meshTable;
		BoneTable _boneTable;
	};

}

#endif