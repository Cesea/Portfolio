#include "stdafx.h"
#include "CodeTimer.h"

CodeTimer::CodeTimer(const char * name)
{
}

void CodeTimer::BeginSession()
{
}

void CodeTimer::EndSession()
{
}

void CodeTimer::Reset()
{
}

void CodeTimer::Output(u32Flags messageFlags)
{
}

void CodeTimer::OutputAllTimers(u32Flags messageFlags)
{
}

void CodeTimer::ResetAllTimers()
{
}

float CodeTimer::AverageTime() const
{
	if (_totalCalls)
	{
		return _totalTime / (float)_totalCalls;
	}
	return 0.0f;
}

float CodeTimer::TotalTime() const
{
	return _totalTime;
}

uint32 CodeTimer::TotalCalls() const
{
	return _totalCalls;
}

float CodeTimer::MaximumTimeSample() const
{
	return _maximumTimeSample;
}

float CodeTimer::MinimumTimeSample() const
{
	return _minimumTimeSample;
}

const char * CodeTimer::Name() const
{
	_name;
}
