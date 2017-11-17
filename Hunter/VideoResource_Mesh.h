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

	video::VertexBufferHandle _vHandle;
	video::IndexBufferHandle _iHandle;
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

	//에니메이션을 위한 typedef

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

		//NOTE : 직접 사용하지 않고 밖에서 쓴다

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


	//SkinnedMesh는 한번만 불러오고, SkinnedAnimation은 여러개를 만들어서 사용하라....
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