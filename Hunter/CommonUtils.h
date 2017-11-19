#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H

#include <cstdlib>

#include "BitFlags.h"	

#include "TypeTraits.h"

#include "Hash.h"

#include "Delegate.h"

//TODO :
#define BX_ALIGN_MASK(_value, _mask) ( ( (_value)+(_mask) ) & ( (~0)&(~(_mask) ) ) )
#define BX_ALIGN_16(_value) BX_ALIGN_MASK(_value, 0xf)
#define BX_ALIGN_256(_value) BX_ALIGN_MASK(_value, 0xff)
#define BX_ALIGN_4096(_value) BX_ALIGN_MASK(_value, 0xfff)

constexpr float EPSILON = 0.001f;
constexpr float ONE_RAD = 0.017453f;

struct IntRect
{
	IntRect();
	IntRect(int32 left, int32 top, int32 right, int32 bottom);
	IntRect(const IntRect &other);

	IntRect &operator= (const IntRect &other);
	int32 _left;
	int32 _top;
	int32 _right;
	int32 _bottom;
};

inline int32 Index2D(int32 x, int32 y, int32 xMax)
{
	return x + (xMax * y);
}

//TODO : Implement this
inline int32 ClosestPowerOfTow(int32 val)
{
	return 0;
}

//NOTE : 이게 맞나??
//inline int32 index3D(int32 x, int32 y, int32 z, int32 xMax, int32 yMax)
//{
//	return x + (x * y) + (x * y * z);
//}

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

inline uint32 Uint32Min(uint32 a, uint32 b)
{
	return (a < b) ? a : b;
}

inline uint32 Uint32Max(uint32 a, uint32 b)
{
	return (a > b) ? a : b;
}

inline int32 Int32Min(int32 a, int32 b)
{
	return (a < b) ? a : b;
}

inline int32 Int32Max(int32 a, int32 b)
{
	return (a > b) ? a : b;
}

inline uint32 Uint32CountBits(uint32 val)
{
}

inline uint32 Uint32BitScan(uint32 val)
{
	unsigned long index;
	_BitScanForward(&index, val);
	return index;
}

inline uint32 Uint32And(uint32 a, uint32 b)
{
	return a & b;
}
inline uint32 Uint32AndC(uint32 a, uint32 b)
{
	return a & (~b);
}
inline uint32 Uint32Or(uint32 a, uint32 b)
{
	return a | b;
}
inline uint32 Uint32Selb(uint32 mask, uint32 a, uint32 b)
{
	const uint32 sel_a = Uint32And(a, mask);
	const uint32 sel_b = Uint32AndC(b, mask);
	const uint32 result = Uint32Or(sel_a, sel_b);
	return result;
}

inline std::string GetFilePath(std::string &str)
{
	std::string path;
	int lastPathIndex = 0;		
	lastPathIndex = str.find_last_of('/');		
	if (lastPathIndex == -1) 
	{			
		lastPathIndex = str.find_last_of('\\');
	}
	if (lastPathIndex != -1) 
	{
		path = str.substr(0, lastPathIndex + 1);
	}
	return path;
}

inline std::string GetFileExtension(std::string &str)
{
	std::string extension;
	int lastPathIndex = 0;
	lastPathIndex = str.find_last_of('.');
	if (lastPathIndex == -1)
	{
		Assert(false); //주어진 str에 .문자가 없다
	}

	if (lastPathIndex != -1)
	{
		extension = str.substr(lastPathIndex + 1, str.length() - lastPathIndex);
	}
	return extension;
}

inline std::string GetFileName(std::string &str, bool removeExtension)
{
	std::string name;
	int lastPathIndex = 0;		
	lastPathIndex = str.find_last_of('/');		
	if (lastPathIndex == -1) 
	{			
		lastPathIndex = str.find_last_of('\\');
	}

	if (lastPathIndex != -1) 
	{
		name = str.substr(lastPathIndex + 1, str.length() - lastPathIndex);
	}

	if (removeExtension)
	{
		lastPathIndex = name.find_last_of('.');
		if (lastPathIndex == -1)
		{
			Assert(false); //주어진 str에 .문자가 없다
		}
		if (lastPathIndex != -1)
		{
			name = name.substr(0, lastPathIndex);
		}
	}
	return name;
}

inline void SplitFilePathToNamePathExtension(std::string &original,
	std::string &name, std::string &path, std::string &extension)
{
	name = GetFileName(original, true);
	path = GetFilePath(original);
	extension = GetFileExtension(original);
}

inline void SplitNameToNameExtension(std::string &original,
	std::string &name, std::string &extension)
{
	extension = GetFileExtension(original);
	name = original;
	for (uint32 i = 0; i < extension.size() + 1; ++i)
	{
		name.pop_back();
	}
}


float RandFloat();
float RandFloat(float max);
float RandFloat(float min, float max);

int32 RandInt();
int32 RandInt(int32 max);
int32 RandInt(int32 min, int32 max);

inline float absFloat(float num)
{
	if (num < 0)
	{
		return num*-1;
	}
	else
	{
		return num;
	}
}
inline int absInt(int num)
{
	if (num < 0)
	{
		return num*-1;
	}
	else
	{
		return num;
	}
}

inline float FMax(float f1, float f2)
{
	if (f1 > f2)
	{
		return f1;
	}
	else
	{
		return f2;
	}
}

inline float FMin(float f1, float f2)
{
	if (f1 < f2)
	{
		return f1;
	}
	else
	{
		return f2;
	}
}

//bottom값이 더 작다...
inline bool IsInRect(const RECT *pRect, POINT p)
{
	return (p.x >= pRect->left) && (p.x <= pRect->right) && (p.y <= pRect->top) && (p.y >= pRect->bottom);
}



#endif