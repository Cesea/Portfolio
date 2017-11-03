#include "stdafx.h"
#include "Quaternion.h"

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

float QuaternionLength(const Quaternion * pQ)
{
	return D3DXQuaternionLength(pQ);
}

float QuaternionLengthSq(const Quaternion * pQ)
{
	return D3DXQuaternionLengthSq(pQ);
}

float QuaternionDot(const Quaternion * pQ1, const Quaternion * pQ2)
{
	return D3DXQuaternionDot(pQ1, pQ2);
}

Quaternion * QuaternionIdentity(Quaternion * pOut)
{
	return (Quaternion *)D3DXQuaternionIdentity(pOut);
}

bool QuaternionIsIdentity(const Quaternion * pQ)
{
	return D3DXQuaternionIsIdentity(pQ);
}

Quaternion * QuaternionConjugate(Quaternion * pOut, const Quaternion * pQ)
{
	return (Quaternion *)D3DXQuaternionConjugate(pOut, pQ);
}

void QuaternionToAxisAngle(const Quaternion * pQ, D3DXVECTOR3 * pAxis, float * pAngle)
{
	D3DXQuaternionToAxisAngle(pQ, pAxis, pAngle);
}

Quaternion * QuaternionRotationMatrix(Quaternion * pOut, const Matrix * pM)
{
	return (Quaternion *)D3DXQuaternionRotationMatrix(pOut, pM);
}

Quaternion * QuaternionRotationAxis(Quaternion * pOut, const Vector3 * pV, float angle)
{
	return (Quaternion *)D3DXQuaternionRotationAxis(pOut, pV, angle);
}

Quaternion * QuaternionRotationYawPitchRoll(Quaternion * pOut, float yaw, float pitch, float roll)
{
	return (Quaternion *)D3DXQuaternionRotationYawPitchRoll(pOut, yaw, pitch, roll);
}

Quaternion * QuaternionMultiply(Quaternion * pOut, const Quaternion * pQ1, const Quaternion * pQ2)
{
	return (Quaternion *)D3DXQuaternionMultiply(pOut, pQ1, pQ2);
}

Quaternion * QuaternionNormalize(Quaternion * pOut, const Quaternion * pQ)
{
	return (Quaternion *)D3DXQuaternionNormalize(pOut, pQ);
}

Quaternion * QuaternionInverse(Quaternion * pOut, const Quaternion * pQ)
{
	return (Quaternion *)D3DXQuaternionInverse(pOut, pQ);
}

Quaternion * QuaternionLn(Quaternion * pOut, const Quaternion * pQ)
{
	return (Quaternion *)D3DXQuaternionLn(pOut, pQ);
}

Quaternion * QuaternionExp(Quaternion * pOut, const Quaternion * pQ)
{
	return (Quaternion *)QuaternionExp(pOut, pQ);
}

Quaternion * QuaternionSlerp(Quaternion * pOut, const Quaternion * pQ1, const Quaternion * pQ2, float t)
{
	return (Quaternion *)D3DXQuaternionSlerp(pOut, pQ1, pQ2, t);
}

Quaternion * QuaternionSquad(Quaternion * pOut, const Quaternion * pQ1, const Quaternion * pA, const Quaternion * pB, const Quaternion * pC, float t)
{
	return (Quaternion *)D3DXQuaternionSquad(pOut, pQ1, pA, pB, pC, t);
}

void QuaternionSquadSetup(Quaternion * pAOut, Quaternion * pBOut, Quaternion * pCOut, const Quaternion * pQ0, const Quaternion * pQ1, const Quaternion * pQ2, const Quaternion * pQ3)
{
	return D3DXQuaternionSquadSetup(pAOut, pBOut, pCOut, pQ0, pQ1, pQ2, pQ3);
}

Quaternion * QuaternionBaryCentric(Quaternion * pOut, const Quaternion * pQ1, const Quaternion * pQ2, const Quaternion * pQ3, float f, float g)
{
	return (Quaternion *)D3DXQuaternionBaryCentric(pOut, pQ1, pQ2, pQ3, f, g);
}
