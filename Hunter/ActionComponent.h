#ifndef ACTION_COMPONENT_H
#define ACTION_COMPONENT_H

#include "Component.h"

#define ACTION_MAX_NUM 16
#define ACTION_MAX_NAME 36

//���ϸ��̼��� ���� typedef
typedef std::vector<LPD3DXANIMATIONSET> AnimationSetVector;
typedef std::map<std::string, int32> AnimationSetTable;

class GameObjectAnimationCallbackHandler;

//������ ���ϸ��̼� ���� ����, �ݺ������͸� �־��ٸ� ���� ���� Controller�� �߰���Ų��
bool AddCallbackKeysAndCompress(LPD3DXANIMATIONCONTROLLER pAnimationController,
	LPD3DXKEYFRAMEDANIMATIONSET pAnimationSet,
	DWORD numCallbackKeys,
	D3DXKEY_CALLBACK *pKeys,
	DWORD compressionFlags,
	float compression);

struct Action
{
	Action();
	Action(const Action &other);
	Action &operator= (const Action &other);

	char _name[ACTION_MAX_NAME];
	//���� �ڿ� ���ϸ��̼��� �´ٸ� �� ���ϸ��̼��� ������ ����ǰ� �Ұų� �ƴϸ� �ٷ� ������ �ǰԲ� �� �ų�??
	bool32 _blocking;
	bool32 _playOnce{true};
	bool32 _stop{ false };
	float _playSpeed;

	float _crossFadeTime;
	float _outCrossFadeTime;
};

class ActionQueue
{
public :
	void PushAction(const Action &action);
	const Action &Front();
	void PopAction();

	bool HasAction() { return (_head != _tail); }
	void ClearQueue() { _tail = _head; }

private :
	Action _actions[ACTION_MAX_NUM]{};
	uint32 _head{};
	uint32 _tail{};

};

struct ActionComponent : public Component
{
public :
	ActionComponent() {}
	virtual ~ActionComponent() {}

	bool CreateFrom(video::AnimationInstanceHandle skinnedAnimation);
	void MakeAnimationList();
	void SetFirstAction(const Action &action);
	void Destroy();

	void UpdateAnimation(float deltaTime);
	//�ε����� ã��
	bool PlayActionImmediate(const Action &action);
	//void PlayOneShot(const Action &action);
	//void PlayOneShotAfterHold(const Action &action);

	////���ڿ��� ã��
	//void Play(const std::string &animName, float crossFadeTime = 0.0);
	//void PlayOneShot(const std::string &animName, float inCrossFadeTime = 0.0, float outCrossFadeTime = 0.0f);
	//void PlayOneShotAfterHold(const std::string &animName, float crossFadeTime = 0.0);


	void Play( const std::string &animName, float crossFadeTime = 0.0, bool32 blocking = false);
	void Play( int32 animIndex, float crossFadeTime = 0.0, bool32 blocking = false);
	void Play( LPD3DXANIMATIONSET animSet, float crossFadeTime = 0.0 , bool32 blocking = false);
	void PlayOneShot( const std::string &animName, 
		float inCrossFadeTime = 0.0, float outCrossFadeTime = 0.0f , bool32 blocking = false);
	void PlayOneShotAfterHold( const std::string &animName, float crossFadeTime = 0.0 , bool32 blocking = false);

	void Stop() { _playing = false; }
	void SetPlaySpeed(float speed);

	inline void SetAnimation(int32 index) { SetAnimation(_animations[index]); }
	void SetAnimation(ID3DXAnimationSet *animationSet);

	ActionQueue _actionQueue;

	ID3DXAnimationController *_pAnimationController{};
	uint32 _numAnimation{};

	ID3DXAnimationSet *_pPlayingAnimationSet{};
	D3DXTRACK_DESC _playingTrackDesc{};

	bool32 _playing{};
	bool32 _looping{};
	bool32 _blocking{};
	ID3DXAnimationSet *_pPrevPlayingAnimationSet{};

	float _crossFadeTime{};
	float _leftCrossFadeTime{};
	float _outCrossFadeTime{};
	double _animationPlayFactor{};

	float _animDelta{};

	GameObjectAnimationCallbackHandler *_pCallbackHandler{};

	AnimationSetVector _animations;
	AnimationSetTable _animationTable;
};


#endif