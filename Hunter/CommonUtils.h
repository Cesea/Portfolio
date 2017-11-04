#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H

#include <cstdlib>

constexpr float EPSILON = 0.001f;

enum LockFlagBits
{
	LockReadBit = 0,	// lock data for reading
	LockWriteBit,		// lock data for selective writing (not all data is changed)
	LockReplaceBit,	// lock data to completely overwrite all data
	LockExclusiveBit,		// allow no other locks while active
	LockWaitBit,			// wait until data is available
};

enum LockFlags
{
	LockRead = FLAG(LockReadBit),
	LockWriteFlag = FLAG(LockWriteBit),
	LockReplaceFlag = FLAG(LockWriteBit),
	LockExclusive = FLAG(LockExclusiveBit),
	LockWait = FLAG(LockWaitBit),
};

enum LockType
{
	ReadLock = 0,
	WriteLock,
	ReadWriteLock,
};


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
struct Ray {
	D3DXVECTOR3 origin;		//직선의 시작위치
	D3DXVECTOR3 direction;  //직선의 방향(정규화 되어있어야 함);
};

float RandFloat();
float RandFloat(float max);
float RandFloat(float min, float max);

int32 RandInt();
int32 RandInt(int32 max);
int32 RandInt(int32 min, int32 max);



#endif