#include "stdafx.h"
#include "Vector3.h"

Vector3::Vector3()
	:x(0.0f), y(0.0f), z(0.0f)
{
}

Vector3::Vector3(float x, float y, float z)
	:x(x), y(y), z(z)
{
}

Vector3::~Vector3()
{
}

Vector3::Vector3(const Vector3 & other)
	:x(other.x), y(other.y), z(other.z)
{
}

Vector3 Vector3::operator=(const Vector3 & other)
{
	return Vector3(other.x, other.y, other.z);
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
	return  !(*this == other);
}

Vector3 operator*(float f, const Vector3 & other)
{
	return Vector3();
}
