#include "stdafx.h"
#include "Plane.h"

Plane::Plane()
{
}

Plane::Plane(float a, float b, float c, float d)
{
	this->a = a;
	this->b = b;
	this->c = c;
	this->d = d;
}

Plane::Plane(const Plane & other)
{
	this->a = other.a;
	this->b = other.b;
	this->c = other.c;
	this->d = other.d;
}

Plane Plane::operator=(const Plane & other)
{
	return Plane(other.a, other.b, other.c, other.d);
}

Plane & Plane::operator*=(float f)
{
	a *= f;
	b *= f;
	c *= f;
	d *= f;
	return *this;
}

Plane & Plane::operator/=(float f)
{
	a /= f;
	b /= f;
	c /= f;
	d /= f;
	return *this;
}

Plane Plane::operator+() const
{
	return Plane(a, b, c, d);
}

Plane Plane::operator-() const
{
	return Plane(-a, -b, -c, -d);
}

Plane Plane::operator*(float f) const
{
	return Plane(a * f, b * f, c * f, d * f);
}

Plane Plane::operator/(float f) const
{
	return Plane(a / f, b / f, c / f, d / f);
}

bool Plane::operator==(const Plane &other) const
{
	return FloatEqual(a, other.a) &&
		FloatEqual(b, other.b) &&
		FloatEqual(c, other.c) &&
		FloatEqual(d, other.d);
}

bool Plane::operator!=(const Plane &other) const
{
	return !(*this == other);
}

void Plane::Create(const Vector3 & a, const Vector3 & b, const Vector3 & c)
{
	//평면의 노말 
	Vector3 normal;
	Vector3 edge1 = b - a;
	Vector3 edge2 = c - a;
	Vec3Cross(&normal, &edge1, &edge2);
	Vec3Normalize(&normal, &normal);

	//평면의 노말 방향으로 원점까지의 최단 거리
	float dist = -Vec3Dot(&normal, &a);

	//평면값 쓰자
	this->a = normal.x;
	this->b = normal.y;
	this->c = normal.z;
	this->d = dist;
}

Plane operator*(float f, const Plane & other)
{
	return Plane(f * other.a, f * other.b, f * other.c, f * other.d);
}

float PlaneDot(const Plane * pP, const D3DXVECTOR4 * pV)
{
	return D3DXPlaneDot(pP, pV);
}

float PlaneDotCoord(const Plane * pP, const D3DXVECTOR3 * pV)
{
	return D3DXPlaneDotCoord(pP, pV);
}

float PlaneDotNormal(const Plane * pP, const D3DXVECTOR3 * pV)
{
	return D3DXPlaneDotNormal(pP, pV);
}

Plane * PlaneScale(Plane * pOut, const Plane * pP, float s)
{
	return (Plane *)D3DXPlaneScale(pOut, pP, s);
}

Plane * PlaneNormalize(Plane * pOut, const Plane * pP)
{
	return  (Plane *)D3DXPlaneNormalize(pOut, pP);
}

Vector3 * PlaneIntersectLine(Vector3 * pOut, const Plane * pP, const D3DXVECTOR3 * pV1, const Vector3 * pV2)
{
	return (Vector3 *)D3DXPlaneIntersectLine(pOut, pP, pV1, pV2);
}

Plane * PlaneFromPointNormal(Plane * pOut, const D3DXVECTOR3 * pPoint, const D3DXVECTOR3 * pNormal)
{
	return (Plane *)D3DXPlaneFromPointNormal(pOut, pPoint, pNormal);
}

Plane * PlaneFromPoints(Plane * pOut, const D3DXVECTOR3 * pV1, const D3DXVECTOR3 * pV2, const D3DXVECTOR3 * pV3)
{
	return (Plane *)D3DXPlaneFromPoints(pOut, pV1, pV2, pV3);
}

Plane * PlaneTransform(Plane * pOut, const Plane * pP, const D3DXMATRIX * pM)
{
	return (Plane *)D3DXPlaneTransform(pOut, pP, pM);
}

Plane * PlaneTransformArray(Plane * pOut, UINT OutStride, const Plane * pP, UINT PStride, const D3DXMATRIX * pM, UINT n)
{
	return (Plane *)D3DXPlaneTransformArray(pOut, OutStride, pP, PStride, pM, n);
}
