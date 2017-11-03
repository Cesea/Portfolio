#include "stdafx.h"
#include "Vector3.h"

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
	return !(*this == other);
}

Vector3 operator*(float f, const Vector3 & other)
{
	return Vector3(other.x * f, other.y * f, other.z * f);
}

float Vec3Length(const Vector3 * pV)
{
	return sqrtf((pV->x * pV->x) + (pV->y * pV->y) + (pV->z * pV->z));
}

float Vec3LengthSq(const Vector3 * pV)
{
	return (pV->x * pV->x) + (pV->y * pV->y) + (pV->z * pV->z);
}

float Vec3Dot(const Vector3 * pV1, const Vector3 * pV2)
{
	return (pV1->x * pV2->x) + (pV1->y * pV2->y) + (pV1->z * pV2->z);
}

Vector3 * Vec3Cross(Vector3 * pOut, const Vector3 * pV1, const Vector3 * pV2)
{
	return (Vector3 *)D3DXVec3Cross(pOut, pV1, pV2);
}

Vector3 * Vec3Add(Vector3 * pOut, const Vector3 * pV1, const Vector3 * pV2)
{
	return (Vector3 *)D3DXVec3Add(pOut, pV1, pV2);
}

Vector3 * Vec3Subtract(Vector3 * pOut, const Vector3 * pV1, const Vector3 * pV2)
{
	return (Vector3 *)D3DXVec3Subtract(pOut, pV1, pV2);
}

Vector3 * Vec3Normalize(Vector3 * pOut, const Vector3 * pV)
{
	return (Vector3 *)D3DXVec3Normalize(pOut, pV);
}

Vector4 * Vec3Transform(Vector4 * pOut, const Vector3 * pV, const Matrix * pM)
{
	return (Vector4 *)D3DXVec3Transform((D3DXVECTOR4 *)pOut, pV, (D3DXMATRIXA16 *)pM);
}

Vector3 * Vec3TransformCoord(Vector3 * pOut, const Vector3 * pV, const Matrix * pM)
{
	return (Vector3 *)D3DXVec3TransformCoord((D3DXVECTOR3 *)pOut, pV, (D3DXMATRIXA16 *)pM);
}

Vector3 * Vec3TransformNormal(Vector3 * pOut, const Vector3 * pV, const Matrix * pM)
{
	return (Vector3 *)D3DXVec3TransformNormal((D3DXVECTOR3 *)pOut, pV, (D3DXMATRIXA16 *)pM);
}
