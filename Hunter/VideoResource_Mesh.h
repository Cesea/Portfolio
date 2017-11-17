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
	Matrix CombinedTransformationMatrix;			//���������� �θ� ����� ����Ǿ� ��ȯ�� �Ϸ�� ���
};

struct BoneMesh : public D3DXMESHCONTAINER
{
	bool32 _visible{ true };
	ID3DXMesh *WorkingMesh;			
	std::vector<video::MaterialHandle> _materialHandles;

	Matrix** ppBoneMatrixPtrs{};		//������ ��� [ ������ �迭 ]
	Matrix* pBoneOffsetMatices{};		//�ڽ��� �⺻ ��� �迭

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
	};

	//���ϸ��̼��� ���� typedef

	typedef std::vector<LPD3DXANIMATIONSET> AnimationSetVector;
	typedef std::map<std::string, LPD3DXANIMATIONSET> AnimationTable;

	typedef std::map<std::string, BoneMesh *> BoneMeshTable;
	typedef std::map<std::string, Bone *> BoneTable;

	struct SkinnedXMesh
	{
		bool Create(const std::string &fileName, const Matrix* matCorrection = nullptr);
		void Destroy();

		void InitBoneMatrixPointer(Bone *pBone);

		void Update(const Matrix *pWorld);
		void UpdateMatrices(Bone *pBone, Matrix *pParentMatrix) const;

		//NOTE : ���� ������� �ʰ� �ۿ��� ����

		//void FillRenderCommand(RenderView &renderView, AnimationHandle animHandle, video::EffectHandle effect);
		//void RenderBone(RenderView &renderView, Bone *pBone, AnimationHandle animHandle, video::EffectHandle effect) const;

		Matrix _matCorrection;

		Bone *_pRootBone{};
		uint32 _numWorkingPalette;
		ID3DXAnimationController *_pAnimationController{};

		uint32 _numSubset{};

		BoneMeshTable _meshTable;
		BoneTable _boneTable;
	};


	//SkinnedMesh�� �ѹ��� �ҷ�����, SkinnedAnimation�� �������� ���� ����϶�....
	struct SkinnedAnimation
	{
		bool Create(video::SkinnedXMeshHandle handle);
		void Destroy();
		void UpdateAnimation(float deltaTime, const Matrix &world);
		void UpdateMesh();

		void FillRenderCommand(RenderView &renderView, 
			video::EffectHandle skinnedEffect, video::EffectHandle staticEffect);
		void FillRenderCommandInternal(RenderView &renderView, 
			video::EffectHandle skinnedEffect, video::EffectHandle staticEffect, Bone *pBone);

		//void	RenderBoneName(cCamera* pCam, cTransform* pTransform);

		void Play(const std::string &animName, float crossFadeTime = 0.0);
		void Play(int32 animIndex, float crossFadeTime = 0.0);
		void PlayOneShot(const std::string &animName, float inCrossFadeTime = 0.0, float outCrossFadeTime = 0.0f);
		void PlayOneShotAfterHold(const std::string &animName, float crossFadeTime = 0.0);
		void Stop() { _playing = false; }
		void SetPlaySpeed(float speed);

		void SetAnimation(LPD3DXANIMATIONSET animation);

		SkinnedXMesh *_pSkinnedMesh{};

		Matrix _world;

		ID3DXAnimationController *_pAnimationController{};
		uint32 _numAnimation;

		Matrix *_workingPalettes{};
		uint32 _numPalette{};

		AnimationSetVector _animations;
		AnimationTable _animationTable;

		LPD3DXANIMATIONSET _pPlayingAnimationSet{};
		D3DXTRACK_DESC _playingTrackDesc{};

		bool32 _playing{};
		bool32 _looping{};
		LPD3DXANIMATIONSET _pPrevPlayAnimationSet{};

		float _crossFadeTime{};
		float _leftCrossFadeTime{};
		float _outCrossFadeTime{};
		double _animationPlayFactor{};

		float _animDelta{};
	};

}

#endif