#include "stdafx.h"
#include "SoundManager.h"

bool SoundManager::Init(void)
{
	//사운드 시스템 생성
	System_Create(&_system);

	//채널수 설정
	_system->init(TOTALSOUNDBUFFER, FMOD_INIT_NORMAL, 0);
	_system->set3DSettings(1.0f, 1.0f, 1.0f);

	//사운드 채널 생성
	_sound = new Sound*[TOTALSOUNDBUFFER];
	_channel = new Channel*[TOTALSOUNDBUFFER];

	//초기화
	ZeroMemory(_sound, sizeof(Sound*) * TOTALSOUNDBUFFER);
	ZeroMemory(_channel, sizeof(Channel*) * TOTALSOUNDBUFFER);


	_up.x = 0.0f;
	_up.y = 1.0f;
	_up.z = 0.0f;

	return true;
}

void SoundManager::Release(void)
{
	if (_channel != nullptr || _sound != nullptr)
	{
		for (int i = 0; i < TOTALSOUNDBUFFER; i++)
		{
			if (_channel != nullptr)
			{
				if (_channel[i]) _channel[i]->stop();
			}
			if (_sound != nullptr)
			{
				if (_sound[i]) _sound[i]->release();
			}
		}
	}

	//메모리 지우고..
	SAFE_DELETE_ARRAY(_channel);
	SAFE_DELETE_ARRAY(_sound);

	//시스템 닫는다
	if (_system !=  nullptr)
	{
		_system->release();
		_system->close();
	}

	_mTotalSounds.clear();
}

void SoundManager::AddSound(const std::string & keyName, const std::string & soundName, bool background, bool loop)
{
	//반복이냐?
	if (loop)
	{
		//배경음임?
		if (background)
		{
			FMOD_RESULT result = _system->createStream(soundName.c_str(),
				FMOD_LOOP_NORMAL, 0, &_sound[_mTotalSounds.size()]);
			Assert(result == FMOD_OK);
		}
		else
		{
			FMOD_RESULT result = _system->createSound(soundName.c_str(), FMOD_LOOP_NORMAL, 0, &_sound[_mTotalSounds.size()]);
			Assert(result == FMOD_OK);
		}
	}
	else
	{
		//한번만 사운드 재생
		FMOD_RESULT result = _system->createSound(soundName.c_str(),
			FMOD_DEFAULT, 0, &_sound[_mTotalSounds.size()]);
		Assert(result == FMOD_OK);
	}

	//맵에 사운드를 키값과 함께 넣어준다
	_mTotalSounds.insert(make_pair(keyName, &_sound[_mTotalSounds.size()]));
}

void SoundManager::AddSound3D(const std::string & keyName, const std::string & fileName, 
	bool background, bool loop, float minDist, float maxDist)
{
	//배경음임?
	if (loop)
	{
		if (background)
		{
			FMOD_RESULT result = _system->createStream(fileName.c_str(),
				FMOD_3D, 0, &_sound[_mTotalSounds.size()]);
			Assert(result == FMOD_OK);
		}
		else
		{
			FMOD_RESULT result = _system->createSound(fileName.c_str(),
				FMOD_3D, 0, &_sound[_mTotalSounds.size()]);
			_sound[_mTotalSounds.size()]->setMode(FMOD_LOOP_NORMAL);
			Assert(result == FMOD_OK);
		}
	}
	else
	{
		//한번만 사운드 재생
		FMOD_RESULT result = _system->createSound(fileName.c_str(),
			FMOD_3D, 0, &_sound[_mTotalSounds.size()]);
			Assert(result == FMOD_OK);
	}

	//맵에 사운드를 키값과 함께 넣어준다
	_mTotalSounds.insert(make_pair(keyName, &_sound[_mTotalSounds.size()]));

	ArrSoundsIter iter = _mTotalSounds.find(keyName);
	if (iter != _mTotalSounds.end())
	{
		auto aa = iter->second;
		(*aa)->set3DMinMaxDistance(minDist, maxDist);
	}
}

void SoundManager::Play(const std::string & keyName)
{
	ArrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//사운드 플레이~~~
			_system->playSound(FMOD_CHANNEL_FREE, 
				*iter->second, false, &_channel[count]);
		}
	}
}

void SoundManager::Play(const std::string & keyName, float volume)
{
	ArrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//사운드 플레이~~~
			_system->playSound(FMOD_CHANNEL_FREE, 
				*iter->second, false, &_channel[count]);
			_channel[count]->setVolume(volume);
		}
	}
}

void SoundManager::Play3D(const std::string & keyName, const Vector3 & pos)
{
	FMOD_VECTOR p;
	p.x = pos.x;
	p.y = pos.y;
	p.z = pos.z;
	ArrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//사운드 플레이~~~
			_system->playSound(FMOD_CHANNEL_FREE, 
				*iter->second, false, &_channel[count]);

			_channel[count]->set3DAttributes(&p, &_vel);
		}
	}
}

void SoundManager::Play3D(const std::string & keyName, const Vector3 & pos, float volume)
{
	FMOD_VECTOR p;
	p.x = pos.x;
	p.y = pos.y;
	p.z = pos.z;
	ArrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//사운드 플레이~~~
			_system->playSound(FMOD_CHANNEL_FREE, 
				*iter->second, false, &_channel[count]);
			_channel[count]->set3DAttributes(&p, &_vel);
			_channel[count]->setVolume(volume);
		}
	}
}

void SoundManager::Stop(const std::string & keyName)
{
	ArrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->stop();
			break;
		}
	}
}

void SoundManager::Pause(const std::string & keyName)
{
	ArrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->setPaused(true);
			break;
		}
	}
}

void SoundManager::Resume(const std::string & keyName)
{
	ArrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->setPaused(false);
			break;
		}
	}
}

bool SoundManager::IsPause(const std::string & keyName)
{
	bool pause;
	ArrSoundsIter iter = _mTotalSounds.begin();
	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->getPaused(&pause);
			break;
		}
	}

	return pause;
}

bool SoundManager::IsPlay(const std::string & keyName)
{
	bool pause;
	ArrSoundsIter iter = _mTotalSounds.begin();
	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->isPlaying(&pause);
			break;
		}
	}

	return pause;
}

unsigned int SoundManager::GetLength(const std::string & keyName)
{
	ArrSoundsIter iter = _mTotalSounds.begin();
	
	for (iter; iter != _mTotalSounds.end(); ++iter)
	{
		if (keyName == iter->first)
		{
			unsigned int length = 0;
			(*iter->second)->getLength(&length, FMOD_TIMEUNIT_MS);

			return length;
		}
	}

	return NULL;
}

void SoundManager::SetPosition(const std::string & keyName, unsigned int ms)
{
	ArrSoundsIter iter = _mTotalSounds.begin();
	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->setPosition(ms, FMOD_TIMEUNIT_MS);
			break;
		}
	}
}

unsigned int SoundManager::GetPosition(const std::string & keyName)
{
	ArrSoundsIter iter = _mTotalSounds.begin();
	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			unsigned int length = 0;
			_channel[count]->getPosition(&length, FMOD_TIMEUNIT_MS);
			return length;
		}
	}

	return NULL;

}

void SoundManager::Update(void)
{
	_system->set3DListenerAttributes(0, &_pos, &_vel, &_forward, &_up);
	_system->update();
}

SoundManager::SoundManager(void)
{
}

SoundManager::~SoundManager(void)
{
}

void SoundManager::SetListenerInfo(const Vector3 & pos, const Vector3 & vel, const Vector3 & forward)
{
	_pos.x = pos.x;
	_pos.y = pos.y;
	_pos.z = pos.z;

	_vel.x = vel.x;
	_vel.y = vel.y;
	_vel.z = vel.z;

	_forward.x = forward.x;
	_forward.y = forward.y;
	_forward.z = forward.z;
}

