#include "stdafx.h"
#include "CommonUtils.h"

POINT PointMake(int x, int y)
{
	POINT result{};
	result.x = x;
	result.y = y;
	return result;
}

POINT AddPoint(const POINT & p1, const POINT & p2)
{
	return PointMake(p1.x + p2.x, p1.y + p2.y);
}

POINT SubPoint(const POINT & p1, const POINT & p2)
{
	return PointMake(p1.x - p2.x, p1.y - p2.y);
}

POINT operator+(const POINT &p1, const POINT &p2)
{
	return PointMake(p1.x + p2.x, p1.y + p2.y);
}

POINT operator-(const POINT & p1, const POINT & p2)
{
	return PointMake(p1.x - p2.x, p1.y - p2.y);
}

RECT RECTMake(int left, int top, int right, int bottom)
{
	RECT result;
	result.left = left;
	result.top = top;
	result.right = right;
	result.bottom = bottom;
	return result;
}

float RandFloat()
{
	return (float)rand() / (float)RAND_MAX;
}

float RandFloat(float max)
{
	return ((float)rand() / (float)RAND_MAX) * max;
}

float RandFloat(float min, float max)
{
	return ((float)rand() / (float)RAND_MAX) * (max - min) + min;
}

int32 RandInt()
{
	return rand();
}

int32 RandInt(int32 max)
{
	return rand() % max;
}

int32 RandInt(int32 min, int32 max)
{
	return rand() % (max - min); +min;
}

IntRect::IntRect()
	:_left(), _top(), _right(), _bottom()
{
}

IntRect::IntRect(int32 left, int32 top, int32 right, int32 bottom)
	:_left(left), _top(top), _right(right), _bottom(bottom)
{
}

IntRect::IntRect(const IntRect & other)
	:_left(other._left), _top(other._top), _right(other._right), _bottom(other._bottom)
{
}

IntRect & IntRect::operator=(const IntRect & other)
{
	_left = other._left;
	_right = other._right;
	_top = other._top;
	_bottom = other._bottom;
	return *this;
}
bool StopWatch::Tick(float deltaTime)
{
	_currentTime += deltaTime;
	if (_targetTime > 0.0f)
	{
		if (_currentTime > _targetTime)
		{
			_currentTime -= _targetTime;
			return true;
		}
	}
	return false;
}

void StopWatch::Reset(float targetTime)
{
	_currentTime = 0.0f;
	_targetTime = targetTime;
}

void StopWatch::Restart()
{
	_currentTime = 0;
}
