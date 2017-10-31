#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H


#include <cstdlib>

POINT PointMake(int x, int y);

POINT AddPoint(const POINT &p1, const POINT &p2);
POINT SubPoint(const POINT &p1, const POINT &p2);

POINT operator+ (const POINT &p1, const POINT &p2);
POINT operator- (const POINT &p1, const POINT &p2);

RECT RECTMake(int left, int top, int right, int bottom);


#endif