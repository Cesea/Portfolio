#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H

#include <cstdlib>

constexpr float EPSILON = 0.001f;


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

inline D3DXVECTOR3 InterpolateVector3(const D3DXVECTOR3 &start, const D3DXVECTOR3 &end, float t)
{
	return D3DXVECTOR3(InterpolateFloat(start.x, end.x, t),
		InterpolateFloat(start.y, end.y, t),
		InterpolateFloat(start.z, end.z, t));
}





#endif