#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H

#include <cstdlib>

#include "DataPackage.h"
#include "BitFlags.h"	

#include "TypeTraits.h"

constexpr float EPSILON = 0.001f;
constexpr float ONE_RAD = 0.017453f;

inline bool IsCharacter(WPARAM wParam)
{
	bool result = false;
	if (wParam >= 32 && wParam <= 127 ||
		wParam == 8)
	{
		result = true;
	}
	return result;
}

inline bool IsLetter(WPARAM wParam)
{
	bool result = false;
	if (wParam >= 65 && wParam <= 94 ||
		wParam >= 97 && wParam <= 126)
	{
		result = true;
	}
	return result;
}


inline float Uint32ToFloat(uint32 ms)
{
	return (float)ms * 0.001f;
}

inline float FAbs(float f)
{
	if (f < 0.0f)
	{
		return -f;
	}
	else
	{
		return f;
	}
}

inline bool FloatEqual(float f1, float f2)
{
	if (FAbs(f1 - f2) < EPSILON)
	{
		return true;
	}
	else
	{
		return false;
	}
}

inline bool FloatZero(float f)
{
	if (FAbs(f) < EPSILON)
	{
		return true;
	}
	else
	{
		return false;
	}
}

inline int32 RectWidth(const RECT &rect)
{
	return rect.right - rect.left;
}

inline int32 RectHeight(const RECT &rect)
{
	return rect.bottom - rect.top;
}

inline float ClampFloat(float value, float min, float max)
{
	float result = value;
	if (result < min)
	{
		result = min;
	}
	else if (result > max)
	{
		result = max;
	}
	return result;
}

inline float Clamp01(float value)
{
	float result = value;
	if (result < 0.0f)
	{
		result = 0.0f;
	}
	else if (result > 1.0f)
	{
		result = 1.0f;
	}
	return result;
}

inline float ClampMinusOnePlusOne(float value)
{
	float result = value;
	if (result < -1.0f)
	{
		result = -1.0f;
	}
	else if (result > 1.0f)
	{
		result = 1.0f;
	}
	return result;
}

inline int32 ClampInt(int32 value, int32 min, int32 max)
{
	int32  result = value;
	if (result < min)
	{
		result = min;
	}
	if (result > max)
	{
		result = max;
	}
	return result;
}

POINT PointMake(int x, int y);

POINT AddPoint(const POINT &p1, const POINT &p2);
POINT SubPoint(const POINT &p1, const POINT &p2);

POINT operator+ (const POINT &p1, const POINT &p2);
POINT operator- (const POINT &p1, const POINT &p2);

RECT RECTMake(int left, int top, int right, int bottom);

inline float InterpolateFloat(float start, float end, float t)
{
	return start + ((end - start) * t);
}

inline uint32 uint32_min(uint32 a, uint32 b)
{
	return (a < b) ? a : b;
}

inline uint32 uint32_max(uint32 a, uint32 b)
{
	return (a > b) ? a : b;
}

inline int32 int32_min(int32 a, int32 b)
{
	return (a < b) ? a : b;
}

inline int32 int32_max(int32 a, int32 b)
{
	return (a > b) ? a : b;
}

float RandFloat();
float RandFloat(float max);
float RandFloat(float min, float max);

int32 RandInt();
int32 RandInt(int32 max);
int32 RandInt(int32 min, int32 max);



#endif