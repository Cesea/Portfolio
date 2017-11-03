#include "stdafx.h"
#include "Vector2.h"

DEFINE_META(Vector2)
{
	ADD_MEMBER(x);
	ADD_MEMBER(y);
}

Vector2::Vector2()
{
	this->x = 0.0f;
	this->y = 0.0f;
}

Vector2::Vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

Vector2::Vector2(const Vector2 & other)
{
	this->x = other.x;
	this->y = other.y;
}

Vector2 & Vector2::operator+=(const Vector2 & other)
{
	x += other.x;
	y += other.y;
	return *this;
}


Vector2 & Vector2::operator-=(const Vector2 & other)
{
	x -= other.x;
	y -= other.y;
	return *this;
}

Vector2 & Vector2::operator*=(float f)
{
	x *= f;
	y *= f;
	return *this;
}

Vector2 & Vector2::operator/=(float f)
{
	x /= f;
	y /= f;
	return *this;
}

Vector2 Vector2::operator-() const
{
	return Vector2(-x, -y);
}

Vector2 Vector2::operator+(const Vector2 & other) const
{
	return Vector2(x + other.x, y + other.y);
}

Vector2 Vector2::operator-(const Vector2 & other) const
{
	return Vector2(x - other.x, y - other.y);
}

Vector2 Vector2::operator*(float f) const
{
	return Vector2(x * f, y * f);
}

Vector2 Vector2::operator/(float f) const
{
	return Vector2(x * f, y / f);
}

bool Vector2::operator==(const Vector2 & other) const
{
	return FloatEqual(x, other.x) && FloatEqual(y, other.y);
}

bool Vector2::operator!=(const Vector2 & other) const
{
	return !(*this == other);
}

Vector2 operator*(float f, const Vector2 & other)
{
	return Vector2(other.x * f, other.y * f);
}

float Vec2Length(const Vector2 * pV)
{
	return sqrtf((pV->x * pV->x) + (pV->y * pV->y));
}

float Vec2LengthSq(const Vector2 * pV)
{
	return ((pV->x * pV->x) + (pV->y * pV->y));
}

float Vec2Dot(const Vector2 * pV1, const Vector2 * pV2)
{
	return (pV1->x * pV2->x) + (pV1->y * pV2->y);
}

Vector2 * Vec2Add(Vector2 * pOut, const Vector2 * pV1, const Vector2 * pV2)
{
	pOut->x = pV1->x + pV2->x;
	pOut->y = pV1->y + pV2->y;
	return  pOut;
}

Vector2 * Vec2Subtract(Vector2 * pOut, const Vector2 * pV1, const Vector2 * pV2)
{
	pOut->x = pV1->x - pV2->x;
	pOut->y = pV1->y - pV2->y;
	return  pOut;
}

Vector2 * Vec2Normalize(Vector2 * pOut, const Vector2 * pV)
{
	float lengthInv = 1.0f / Vec2Length(pV);
	*pOut = (*pV) * lengthInv;
	return pOut;
}
