#ifndef VIDEO_RESOURCE_MESH_H
#define VIDEO_RESOURCE_MESH_H

#include "VideoResourceHandles.h"

namespace video
{
	struct SkinnedXMesh;
	struct AnimationInstance;
}

struct Bone : public D3DXFRAME
{
	Matrix CombinedTransformationMatrix;			//���������� �θ� ����� ����Ǿ� ��ȯ�� �Ϸ�� ���
};

struct BoneMesh : public D3DXMESHCONTAINER
{
	bool32 _visible{ true };
	ID3DXMesh *WorkingMesh;			
	std::vector<video::MaterialHandle> _materialHandles;

	Matrix** ppBoneMatrixPtrs{};		//������ ��� [ ������ �迭 ]
	Matrix* pBoneOffsetMatices{};		//�ڽ��� �⺻ ��� �迭

	D3DXATTRIBUTERANGE *_attributeRange{};

	DWORD NumPaletteEntries{};		//�����޽��� ����ȷ�Ʈ ��
	DWORD MaxNumFaceInfls{};		//�ش� �޽ÿ� ����Ǵ� �����ϳ��� �ִ� ����ġ ����
	DWORD NumAttributesGroup{};		//�޽��� �Ӽ� �׷�� ( �ش� �޽ÿ� ���͸��������� ��ִ�? )
	LPD3DXBUFFER BufBoneCombos{};			//���ĺ���̼� ( �޽ÿ� ����Ǵ� �� ID ������ ���͸��� ���� )

	video::VertexBufferHandle _vHandle;
	video::IndexBufferHandle _iHandle;
};

class BoneHierachy : public ID3DXAllocateHierarchy
{
private:
	video::SkinnedXMesh* _pSkinnedMesh;	//�ڽ��� �ε��ϴ� ��Ų�� �޽�
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
	//NOTE : ���� mesh�� staticMesh, skinnedmesh �ΰ��� �������� �ִ�... �̰��� �ϳ��� ���� �� ������

	void ResizeMeshAndGetInfos(ID3DXMesh *pMesh, const Matrix &correction, 
		MeshVertInfo *pOutVertInfo, MeshBoundInfo *pOutBoundInfo);

	void CalculateBoundInfo(std::vector<Vector3> &positions, MeshBoundInfo *pOutBoundInfo,
			uint32 startVertex, uint32 endVertex);
	
	struct StaticXMesh
	{
		bool Create(const std::string &fileName, const Matrix* matCorrection = nullptr);
		void Destroy();
		void BuidSubMeshBoundInfo();

		void FillRenderCommand(RenderView &renderView, video::EffectHandle effect, const Matrix *pMatrix = nullptr);

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

		static video::EffectHandle sDefaultEffectHandle;
	};


	//typedef std::map<std::string, BoneMesh *> BoneMeshTable;
	//typedef std::map<std::string, Bone *> BoneTable;

	struct SkinnedXMesh
	{
		bool Create(const std::string &fileName, const Matrix* matCorrection = nullptr);
		void Destroy();

		void InitBoneMatrixPointer(Bone *pBone);

		void Update(const Matrix *pMatrix);
		void UpdateMatrices(Bone *pBone, Matrix *pParentMatrix) const;

		//NOTE : ���� ������� �ʰ� �ۿ��� ����
		//void FillRenderCommand(RenderView &renderView, AnimationHandle animHandle, video::EffectHandle effect);
		//void RenderBone(RenderView &renderView, Bone *pBone, AnimationHandle animHandle, video::EffectHandle effect) const;

		Matrix _matCorrection;

		Bone *_pRootBone{};
		uint32 _numWorkingPalette;
		ID3DXAnimationController *_pAnimationController{};

		uint32 _numSubset{};
		MeshBoundInfo _boundInfo{};

		//BoneMeshTable _meshTable;
		//BoneTable _boneTable;
	};


	//SkinnedMesh�� �ѹ��� �ҷ�����, SkinnedAnimation�� �������� ���� ����϶�....
	struct AnimationInstance
	{
		bool Create(video::SkinnedXMeshHandle handle);
		void Destroy();

		void FillRenderCommand(RenderView &renderView, 
			video::EffectHandle skinnedEffect, video::EffectHandle staticEffect);
		void FillRenderCommandInternal(RenderView &renderView, 
			video::EffectHandle skinnedEffect, video::EffectHandle staticEffect, Bone *pBone);

		//void	RenderBoneName(cCamera* pCam, cTransform* pTransform);
		SkinnedXMesh *_pSkinnedMesh{};

		ID3DXAnimationController *_pAnimationController{};
		int32 _numPalette{};
		Matrix *_workingPalettes{};

		static video::EffectHandle sDefaultEffectHandle;
	};
}

#endif