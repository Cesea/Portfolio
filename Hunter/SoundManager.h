#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include "SingletonBase.h"

#include <unordered_map>

//fmod.hpp 추가해준다
#include "inc\fmod.hpp"

//lib 링크
#pragma comment (lib, "lib/fmodex_vc.lib")

//채널 갯수 (버퍼~!!!)
#define EXTRACHANNLEBUFFER 50
#define SOUNDBUFFER 100

//총 사운드 갯수
#define TOTALSOUNDBUFFER (EXTRACHANNLEBUFFER + SOUNDBUFFER)

using namespace FMOD;

class SoundManager : public SingletonBase <SoundManager>
{
private:
	typedef std::unordered_map<std::string, FMOD::Sound**> ArrSounds;
	typedef std::unordered_map<std::string, FMOD::Sound**>::iterator ArrSoundsIter;
	typedef std::unordered_map<std::string, FMOD::Channel**> ArrChannels;
	typedef std::unordered_map<std::string, FMOD::Channel**>::iterator ArrChannelsIter;

private:
	FMOD::System* _system;
	FMOD::Sound** _sound;
	FMOD::Channel** _channel;

	ArrSounds _mTotalSounds;

public:
	bool Init(void);
	void Release(void);

	void AddSound(const std::string &keyName, const std::string & soundName, bool background, bool loop);
	void Play(const std::string & keyName);
	void Play(const std::string & keyName, float volume);
	void Stop(const std::string & keyName);
	void Pause(const std::string & keyName);
	void Resume(const std::string & keyName);

	bool IsPause(const std::string & keyName);
	bool IsPlay(const std::string & keyName);

	//음악 길이값 가져옴.
	unsigned int GetLength(const std::string & keyName);

	//음악 재생 위치 설정
	void SetPosition(const std::string & keyName, unsigned int ms);
	unsigned int GetPosition(const std::string & keyName);

	//fmod 시스템 갱신한다
	void Update(void);

	SoundManager(void);
	virtual ~SoundManager(void);
};

#define SOUNDMANAGER SoundManager::GetInstance()

#endif
