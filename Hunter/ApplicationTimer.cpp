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

	QueryPerformanceCounter(&_currentCounter);
}

void ApplicationTimer::Tick()
{
	QueryPerformanceCounter(&_currentCounter);

	int64 deltaMS = (int64)((_currentCounter.QuadPart - _prevCounter.QuadPart) / _frequency.QuadPart);

	int64 timeToSleep = _targetFramePerMS - deltaMS;

	Sleep(timeToSleep);
	_prevCounter = _currentCounter;
}

float ApplicationTimer::GetDeltaTime()
{
	return (float)_currentDeltaMS / 1000.0f;
}
