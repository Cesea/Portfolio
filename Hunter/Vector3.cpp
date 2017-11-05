#include "stdafx.h"
#include "Vector3.h"

#include "Vector4.h"
#include "Matrix.h"

DEFINE_META(Vector3)
{
	ADD_MEMBER(x);
	ADD_MEMBER(y);
	ADD_MEMBER(z);
}


Vector3::Vector3()
{
	this->x = 0.0f;
	this->y = 0.0f;
	this->z = 0.0f;
}

Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3::~Vector3()
{
}

Vector3::Vector3(const Vector3 & other)
{
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
}

const Vector3 &Vector3::operator=(const Vector3 & other)
{
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	return *this;
}

Vector3 & Vector3::operator+=(const Vector3 & other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

Vector3 & Vector3::operator-=(const Vector3 & other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

Vector3 & Vector3::operator*=(float f)
{
	x *= f;
	y *= f;
	z *= f;
	return *this;
}

Vector3 & Vector3::operator/=(float f)
{
	x /= f;
	y /= f;
	z /= f;
	return *this;
}

//Vector3 Vector3::operator+() const
//{
//	return Vector3(x +);
//}

Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

Vector3 Vector3::operator+(const Vector3 & other) const
{
	return Vector3(x + other.x, y + other.y, z + other.z);
}

Vector3 Vector3::operator-(const Vector3 & other) const
{
	return Vector3(x - other.x, y - other.y, z - other.z);
}

Vector3 Vector3::operator*(float f) const
{
	return Vector3(x * f, y * f, z * f);
}

Vector3 Vector3::operator/(float f) const
{
	return Vector3(x / f, y / f, z / f);
}

bool Vector3::operator==(const Vector3 & other) const
{
	return FloatEqual(x, other.x) &&
		FloatEqual(y, other.y) &&
		FloatEqual(z, other.z);
}

bool Vector3::operator!=(const Vector3 & other) const
{
	return !(*this == other);
}

Vector3 operator*(float f, const Vector3 & other)
{
	return Vector3(other.x * f, other.y * f, other.z * f);
}
