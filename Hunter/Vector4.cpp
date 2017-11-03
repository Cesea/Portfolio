#include "stdafx.h"
#include "Vector4.h"

#include "Vector3.h"

DEFINE_META(Vector4)
{
	ADD_MEMBER(x);
	ADD_MEMBER(y);
	ADD_MEMBER(z);
	ADD_MEMBER(w);
}

Vector4::Vector4()
{
	this->x = 0.0f;
	this->y = 0.0f;
	this->z = 0.0f;
	this->w =0.0f;
}

Vector4::Vector4(const Vector3 &v, float w)
{
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
	this->w = w;
}

Vector4::Vector4(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Vector4::Vector4(const Vector4 & other)
{
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	this->w = other.w;
}

Vector4 & Vector4::operator+=(const Vector4 & other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	w += other.w;
	return *this;
}

Vector4 & Vector4::operator-=(const Vector4 & other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	w -= other.w;
	return *this;
}

Vector4 & Vector4::operator*=(float f)
{
	x *= f;
	y *= f;
	z *= f;
	w *= f;
	return *this;
}

Vector4 & Vector4::operator/=(float f)
{
	x /= f;
	y /= f;
	z /= f;
	w /= f;
	return *this;
}

Vector4 Vector4::operator-() const
{
	return Vector4(-x, -y, -z, -w);
}

Vector4 Vector4::operator+(const Vector4 & other) const
{
	return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
}

Vector4 Vector4::operator-(const Vector4 & other) const
{
	return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
}

Vector4 Vector4::operator*(float f) const
{
	return Vector4(x * f, y * f, z * f, w * f);
}

Vector4 Vector4::operator/(float f) const
{
	return Vector4(x / f, y / f, z / f, w / f);
}

bool Vector4::operator==(const Vector4 & other) const
{
	return FloatEqual(x, other.x) &&
		FloatEqual(y, other.y) &&
		FloatEqual(z, other.z) &&
		FloatEqual(w, other.w);
}

bool Vector4::operator!=(const Vector4 & other) const
{
	return !(*this == other);
}

Vector4 operator*(float f, const Vector4 & other)
{
	return Vector4(f * other.x, f * other.y, f * other.z, f * other.w);
}

float Vec4Length(const Vector4 * pV)
{
	return D3DXVec4Length(pV);
}

float Vec4LengthSq(const Vector4 * pV)
{
	return D3DXVec4LengthSq(pV);
}

float Vec4Dot(const Vector4 * pV1, const Vector4 * pV2)
{
	return D3DXVec4Dot(pV1, pV2);
}

Vector4 * Vec4Add(Vector4 * pOut, const Vector4 * pV1, const Vector4 * pV2)
{
	return (Vector4 *)D3DXVec4Add(pOut, pV1, pV2);
}

Vector4 * Vec4Subtract(Vector4 * pOut, const Vector4 * pV1, const Vector4 * pV2)
{
	return (Vector4 *)D3DXVec4Subtract(pOut, pV1, pV2);
}
