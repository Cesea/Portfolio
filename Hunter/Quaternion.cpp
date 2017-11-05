#include "stdafx.h"
#include "Quaternion.h"

DEFINE_META(Quaternion)
{
	ADD_MEMBER(x);
	ADD_MEMBER(y);
	ADD_MEMBER(z);
	ADD_MEMBER(w);
}

Quaternion::Quaternion()
{
}

Quaternion::Quaternion(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Quaternion::Quaternion(const Quaternion & other)
{
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	this->w = other.w;
}

Quaternion & Quaternion::operator+=(const Quaternion & other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	w += other.w;
	return *this;
}

Quaternion & Quaternion::operator-=(const Quaternion & other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	w -= other.w;
	return *this;
}

Quaternion & Quaternion::operator*=(const Quaternion & other)
{
	return Quaternion(
		w * other.w - x * other.x - y * other.y - z * other.z,
		w * other.x + x * other.w + y * other.z - z * other.y,
		w * other.y + y * other.w + z * other.x - x * other.z,
		w * other.z + z * other.w + x * other.y - y * other.x);
}

Quaternion & Quaternion::operator*=(float f)
{
	x *= f;
	y *= f;
	z *= f;
	w *= f;
	return *this;
}

Quaternion & Quaternion::operator/=(float f)
{
	x /= f;
	y /= f;
	z /= f;
	w /= f;
	return *this;
}

Quaternion Quaternion::operator+() const
{
	return Quaternion(x, y, z, w);
}

Quaternion Quaternion::operator-() const
{
	return Quaternion(-x, -y, -z, -w);
}

Quaternion Quaternion::operator+(const Quaternion & other) const
{
	return Quaternion(x + other.x, y + other.y, z + other.z, w + other.w);
}

Quaternion Quaternion::operator-(const Quaternion & other) const
{
	return Quaternion(x - other.x, y - other.y, z - other.z, w - other.w);
}

Quaternion Quaternion::operator*(const Quaternion & other) const
{
	return Quaternion(w * other.w - x * other.x - y * other.y - z * other.z,
		w * other.x + x * other.w + y * other.z - z * other.y,
		w * other.y + y * other.w + z * other.x - x * other.z,
		w * other.z + z * other.w + x * other.y - y * other.x);
}

Quaternion Quaternion::operator*(float f) const
{
	return Quaternion(x * f, y * f, z * f, w * f);
}

Quaternion Quaternion::operator/(float f) const
{
	return Quaternion(x / f, y / f, z / f, w / f);
}

bool Quaternion::operator==(const Quaternion & other) const
{
	return FloatEqual(x, other.x) &&
		FloatEqual(y, other.y) &&
		FloatEqual(z, other.z) &&
		FloatEqual(w, other.w);
}

bool Quaternion::operator!=(const Quaternion & other) const
{
	return !(*this == other);
}

Quaternion operator*(float f, const Quaternion & other)
{
	return Quaternion(other.x * f, other.y * f, other.z * f, other.w * f);
}