#include "stdafx.h"
#include "ApplicationTimer.h"

ApplicationTimer::ApplicationTimer()
{
}

ApplicationTimer::~ApplicationTimer()
{
}

void ApplicationTimer::Initialize(int32 targetMS)
{
	QueryPerformanceFrequency(&_frequency);

	QueryPerformanceCounter(&_startCounter);
	_prevCounter = _startCounter;

	_targetFramePerMS = targetMS;
	_timeScale = 1.0f / (float)_frequency.QuadPart;

	QueryPerformanceCounter(&_currentCounter);
}

void ApplicationTimer::Tick()
{
	QueryPerformanceCounter(&_currentCounter);

	_currentDeltaSecond = (_currentCounter.QuadPart - _prevCounter.QuadPart) * _timeScale;
	_currentDeltaMS = (int32)(_currentDeltaSecond * 1000);

	int64 timeToSleep = _targetFramePerMS - _currentDeltaMS;

	//Console::Log("%d\n", (int32)timeToSleep);
	if (timeToSleep > 1)
	{
		Sleep(timeToSleep);
	}

	QueryPerformanceCounter(&_currentCounter);
	_prevCounter = _currentCounter;
}

float ApplicationTimer::GetTotalTimeElapsed()
{
	LARGE_INTEGER currentCounter;
	QueryPerformanceCounter(&currentCounter);

	float elapsedTime = (currentCounter.QuadPart - _startCounter.QuadPart) * _timeScale;
	return elapsedTime;
}

