#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include "SingletonBase.h"

#include <unordered_map>

//fmod.hpp �߰����ش�
#include "inc\fmod.hpp"

//lib ��ũ
#pragma comment (lib, "lib/fmodex_vc.lib")

//ä�� ���� (����~!!!)
#define EXTRACHANNLEBUFFER 50
#define SOUNDBUFFER 100

//�� ���� ����
#define TOTALSOUNDBUFFER (EXTRACHANNLEBUFFER + SOUNDBUFFER)

using namespace FMOD;

class SoundManager : public SingletonBase <SoundManager>
{
private:
	typedef std::unordered_map<std::string, FMOD::Sound**> ArrSounds;
	typedef std::unordered_map<std::string, FMOD::Sound**>::iterator ArrSoundsIter;
	typedef std::unordered_map<std::string, FMOD::Channel**> ArrChannels;
	typedef std::unordered_map<std::string, FMOD::Channel**>::iterator ArrChannelsIter;

	//typedef std::unordered_map<std::string, FMOD::Sound **> ThreeDSounds;
	//typedef std::unordered_map<std::string, FMOD::Sound **>::iterator ThreeDSoundsIter;

private:
	FMOD::System* _system;
	FMOD::Sound** _sound;
	FMOD::Channel** _channel;

	ArrSounds _mTotalSounds;

	FMOD_VECTOR _pos;
	FMOD_VECTOR _vel;
	FMOD_VECTOR _forward;
	FMOD_VECTOR _up;

public:
	bool Init(void);
	void Release(void);

	void AddSound(const std::string &keyName, const std::string & soundName, bool background, bool loop);
	void AddSound3D(const std::string &keyName, const std::string &fileName, bool background, bool loop, 
		float minDist, float maxDist);

	void Play(const std::string & keyName);
	void Play(const std::string & keyName, float volume);

	void Play3D(const std::string &keyName, const Vector3 &pos);
	void Play3D(const std::string &keyName, const Vector3 &pos, float volume);

	void Stop(const std::string & keyName);
	void Pause(const std::string & keyName);
	void Resume(const std::string & keyName);

	bool IsPause(const std::string & keyName);
	bool IsPlay(const std::string & keyName);

	//���� ���̰� ������.
	unsigned int GetLength(const std::string & keyName);

	//���� ��� ��ġ ����
	void SetPosition(const std::string & keyName, unsigned int ms);
	unsigned int GetPosition(const std::string & keyName);

	//fmod �ý��� �����Ѵ�
	void Update(void);

	SoundManager(void);
	virtual ~SoundManager(void);

	void SetListenerInfo(const Vector3 &pos, const Vector3 &vel, const Vector3 &forward);

};

#define SOUNDMANAGER SoundManager::GetInstance()

#endif
