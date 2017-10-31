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