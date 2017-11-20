#ifndef ACTION_COMPONENT_H
#define ACTION_COMPONENT_H

#include "Component.h"

#define ACTION_MAX_NUM 10

//에니메이션을 위한 typedef
typedef std::vector<LPD3DXANIMATIONSET> AnimationSetVector;
typedef std::map<std::string, LPD3DXANIMATIONSET> AnimationTable;

struct Action
{
	int32 _animationIndex{};
	//만약 뒤에 에니메이션이 온다면 이 에니메이션이 끝나고 실행되게 할거냐 아니면 바로 블랜딩이 되게끔 할 거냐??
	bool32 _blocking{};
	float _playSpeed{};

	float _crossFadeTime{};
	float _outCrossFadeTime{};

	char extraInfo[16];
};

struct ActionQueue
{
	Action _actions[ACTION_MAX_NUM];
	uint32 _count{};

};

struct ActionComponent : public Component
{
public :
	ActionComponent() {}
	virtual ~ActionComponent() {}

	bool CreateFrom(video::SkinnedAnimationHandle skinnedAnimation);
	void Destroy();

	void UpdateAnimation(float deltaTime);
	//인덱스로 찾기
	void Play(int32 animIndex, float crossFadeTime = 0.0);
	void PlayOneShot(int32 animIndex, float inCrossFadeTime = 0.0, float outCrossFadeTime = 0.0f);
	void PlayOneShotAfterHold(int32 animIndex, float crossFadeTime = 0.0);

	//문자열로 찾기
	void Play(const std::string &animName, float crossFadeTime = 0.0);
	void PlayOneShot(const std::string &animName, float inCrossFadeTime = 0.0, float outCrossFadeTime = 0.0f);
	void PlayOneShotAfterHold(const std::string &animName, float crossFadeTime = 0.0);

	void Stop() { _playing = false; }
	void SetPlaySpeed(float speed);

	void SetAnimation(ID3DXAnimationSet *animationSet);

	void UpdateMesh(const Matrix &world);

	ActionQueue _actionQueue;

	ID3DXAnimationController *_pAnimationController{};
	uint32 _numAnimation{};

	ID3DXAnimationSet *_pPlayingAnimationSet{};
	D3DXTRACK_DESC _playingTrackDesc{};

	bool32 _playing{};
	bool32 _looping{};
	ID3DXAnimationSet *_pPrevPlayingAnimationSet{};

	float _crossFadeTime{};
	float _leftCrossFadeTime{};
	float _outCrossFadeTime{};
	double _animationPlayFactor{};

	float _animDelta{};

	AnimationSetVector _animations;
	AnimationTable _animationTable;
};


#endif