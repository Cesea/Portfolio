#ifndef ANIMATION_COMPONENT_H
#define ANIMATION_COMPONENT_H

namespace animation
{
	struct AnimationComponentHandle : public ResourceHandle
	{
		AnimationComponentHandle()
		{
			this->index = 0;
			this->count = 0;
		}
		AnimationComponentHandle(uint16 index, uint16 count)
		{
			this->index = index;
			this->count = count;
		}
	};


	typedef std::vector<LPD3DXANIMATIONSET> AnimationSetVector;
	typedef std::map<std::string, LPD3DXANIMATIONSET> AnimationTable;

	struct AnimationComponent : public Component
	{
		AnimationComponent();
		virtual ~AnimationComponent();

		bool Create(video::SkinnedXMeshHandle handle);
		void Destroy();
		//void UpdateAnimation(float deltaTime, const Matrix &world);
		//void UpdateMatrixPalettesInternal(Bone *pBone);

		//void	RenderBoneName(cCamera* pCam, cTransform* pTransform);

		void Play(const std::string &animName, float crossFadeTime = 0.0);
		void Play(int32 animIndex, float crossFadeTime = 0.0);
		void PlayOneShot(const std::string &animName, float inCrossFadeTime = 0.0, float outCrossFadeTime = 0.0f);
		void PlayOneShotAfterHold(const std::string &animName, float crossFadeTime = 0.0);
		void Stop() { _playing = false; }
		void SetPlaySpeed(float speed);

		void SetAnimation(LPD3DXANIMATIONSET animation);

		video::SkinnedXMesh *_pSkinnedMesh{};

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

		//이 함수는 직접적으로 사용하지 마라...
		void UpdateMesh();
	};
}
#endif