#ifndef VIDEO_RESOURCE_MESH_H
#define VIDEO_RESOURCE_MESH_H

#include "VideoResourceHandles.h"

namespace video
{
	struct SkinnedXMesh;
}

struct Bone : public D3DXFRAME
{
	Matrix CombinedTransformationMatrix;			//���������� �θ� ����� ����Ǿ� ��ȯ�� �Ϸ�� ���
};

struct BoneMesh : public D3DXMESHCONTAINER
{
	ID3DXMesh *WorkingMesh;			
	std::vector<video::MaterialHandle> _materialHandles;
	//std::vector<D3DMATERIAL9> Materials;				//�޽��� ���� ������
	//std::vector<LPDIRECT3DTEXTURE9> DiffuseTexs;			//�ε�� XFile ���� ���Ǵ� Texture ��
	//std::vector<LPDIRECT3DTEXTURE9> NormalTexs;
	//std::vector<LPDIRECT3DTEXTURE9> SpecularTexs;
	//std::vector<LPDIRECT3DTEXTURE9> EmissionTexs;

	//DWORD								NumAttributesGroup;		//�޽��� �Ӽ� �׷�� ( �ش� �޽ÿ� 
	//D3DXATTRIBUTERANGE*				AttributeTable;			//�޽��� �Ӽ� ���̺� ( �ش� ���� ����� Mesh �� Subset, MaterialID ���� ������ ��� �ִ� )

	Matrix** ppBoneMatrixPtrs;		//������ ��� [ ������ �迭 ]
	Matrix* pBoneOffsetMatices;		//�ڽ��� �⺻ ��� �迭

	DWORD NumPaletteEntries;		//�����޽��� ����ȷ�Ʈ ��
	DWORD MaxNumFaceInfls;		//�ش� �޽ÿ� ����Ǵ� �����ϳ��� �ִ� ����ġ ����
	DWORD NumAttributesGroup;		//�޽��� �Ӽ� �׷�� ( �ش� �޽ÿ� ���͸��������� ��ִ�? )
	LPD3DXBUFFER BufBoneCombos;			//���ĺ���̼� ( �޽ÿ� ����Ǵ� �� ID ������ ���͸��� ���� )
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

	//struct MeshContainer
	//{
	//};

	//NOTE : ���� mesh�� staticMesh, skinnedmesh �ΰ��� �������� �ִ�... �̰��� �ϳ��� ���� �� ������
	
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

	typedef std::vector<LPD3DXANIMATIONSET> AnimationSetVector;
	typedef std::map<std::string, LPD3DXANIMATIONSET> AnimationTable;

	struct SkinnedXMesh
	{
		bool Create(const std::string &fileName, const Matrix* matCorrection = nullptr);
		void Destroy();

		void InitBoneMatrixPointer(Bone *pBone);

		void Update(float deltaTime, const Matrix *pWorld);
		void UpdateMatrices(Bone *pBone, Matrix *pParentMatrix) const;

		//NOTE : ���� ������� �ʰ� �ۿ��� ����
		void RenderBone(const video::Effect &effect, Bone *pBone) const;

		void Play(const std::string &animName, float crossFadeTime = 0.0);
		void Play(int32 animIndex, float crossFadeTime = 0.0);
		void PlayOneShot(const std::string &animName, float inCrossFadeTime = 0.0, float outCrossFadeTime = 0.0f);
		void PlayOneShotAfterHold(const std::string &animName, float crossFadeTime = 0.0);
		void Stop() { _playing = false; }
		void SetPlaySpeed(float speed);

		void SetAnimation(LPD3DXANIMATIONSET animation);

		Matrix _matCorrection;

		Bone *_pRootBone;
		uint32 _numWorkingPalette;
		Matrix *_workingPalettes;

		ID3DXAnimationController *_pAnimationController;

		AnimationSetVector _animations;
		AnimationTable _animationTable;

		LPD3DXANIMATIONSET _pPlayingAnimationSet{};
		D3DXTRACK_DESC _playingTrackDesc{};

		LPD3DXANIMATIONSET _pPrevPlayAnimationSet{};//OneShot �÷��̽� �ѹ� Animation �÷��̵ǰ� �ٽ� �ǵ��ư� Animaiton

		float _crossFadeTime{};
		float _leftCrossFadeTime{};
		float _outCrossFadeTime{};
		double _animationPlayFactor{};

		uint32 _numAnimations{};

		bool32 _playing{};
		bool32 _looping{};
	};

	//SkinnedMesh�� �ѹ��� �ҷ�����, SkinnedAnimation�� �������� ���� ����϶�....
	struct SkinnedAnimation
	{
		SkinnedXMesh *_pSkinnedMesh{};

		ID3DXAnimationController *_pAnimationController;
		uint32 _animNum;
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

		HRESULT	Create(video::SkinnedXMeshHandle handle);
		void	Destroy();
		void	Update(float timeDelta, const Matrix *pMatrix);
		void	Render();
		//void	RenderBoneName(cCamera* pCam, cTransform* pTransform);

		void Play(const std::string &animName, float crossFadeTime = 0.0);
		void Play(int32 animIndex, float crossFadeTime = 0.0);
		void PlayOneShot(const std::string &animName, float inCrossFadeTime = 0.0, float outCrossFadeTime = 0.0f);
		void PlayOneShotAfterHold(const std::string &animName, float crossFadeTime = 0.0);
		void Stop() { _playing = false; }
		void SetPlaySpeed(float speed);

		void SetAnimation(LPD3DXANIMATIONSET animation);

	};
}

#endif