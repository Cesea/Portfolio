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
	_targetFrameSecond = (double)targetMS / 1000.0;
	_timeScale = 1.0f / (double)_frequency.QuadPart;

	QueryPerformanceCounter(&_currentCounter);
}

void ApplicationTimer::Tick()
{
	QueryPerformanceCounter(&_currentCounter);

	_currentDeltaSecond = (_currentCounter.QuadPart - _prevCounter.QuadPart) * _timeScale;
	_currentDeltaMS = (int32)(_currentDeltaSecond * 1000);

	int32 timeToSleep{};

	while (_currentDeltaSecond < _targetFrameSecond - (_targetFrameSecond * 0.1f))
	{
		QueryPerformanceCounter(&_currentCounter);

		_currentDeltaSecond = (_currentCounter.QuadPart - _prevCounter.QuadPart) * _timeScale;
		if (_currentDeltaSecond > _targetFrameSecond)
		{
			break;
		}
		else
		{
			Sleep(1);
		}
	}
	_prevCounter = _currentCounter;
}

float ApplicationTimer::GetTotalTimeElapsed()
{
	LARGE_INTEGER currentCounter;
	QueryPerformanceCounter(&currentCounter);

	float elapsedTime = (currentCounter.QuadPart - _startCounter.QuadPart) * _timeScale;
	return elapsedTime;
}

