#ifndef PLANE_H
#define PLANE_H

class Plane : public D3DXPLANE
{
public :
	Plane();
	Plane(float a, float b, float c, float d);
	Plane(const Plane &other);

	Plane operator= (const Plane &other);

	// assignment operators
	Plane& operator *= (float f);
	Plane& operator /= (float f);

	// unary operators
	Plane operator + () const;
	Plane operator - () const;

	// binary operators
	Plane operator * (float f) const;
	Plane operator / (float f) const;

	friend Plane operator * (float f, const Plane& p);

	bool operator == (const Plane&) const;
	bool operator != (const Plane&) const;

};


// ax + by + cz + dw
float PlaneDot (const Plane *pP, const D3DXVECTOR4 *pV);

// ax + by + cz + d
float PlaneDotCoord (const Plane *pP, const D3DXVECTOR3 *pV);

// ax + by + cz
float PlaneDotNormal (const Plane *pP, const D3DXVECTOR3 *pV);

Plane* PlaneScale (Plane *pOut, const Plane *pP, float s);

// Normalize plane (so that |a,b,c| == 1)
Plane* PlaneNormalize(Plane *pOut, const Plane *pP);

// Find the intersection between a plane and a line.  If the line is
// parallel to the plane, NULL is returned.
Vector3* PlaneIntersectLine(Vector3 *pOut, const Plane *pP, const D3DXVECTOR3 *pV1, const Vector3 *pV2);

// Construct a plane from a point and a normal
Plane* PlaneFromPointNormal(Plane *pOut, const D3DXVECTOR3 *pPoint, const D3DXVECTOR3 *pNormal);

// Construct a plane from 3 points
Plane* PlaneFromPoints(Plane *pOut, const D3DXVECTOR3 *pV1, const D3DXVECTOR3 *pV2, const D3DXVECTOR3 *pV3);

// TransformComponent a plane by a matrix.  The vector (a,b,c) must be normal.
// M should be the inverse transpose of the transformation desired.
Plane* PlaneTransform(Plane *pOut, const Plane *pP, const D3DXMATRIX *pM);

// TransformComponent an array of planes by a matrix.  The vectors (a,b,c) must be normal.
// M should be the inverse transpose of the transformation desired.
Plane* PlaneTransformArray(Plane *pOut, UINT OutStride, const Plane *pP, UINT PStride, const D3DXMATRIX *pM, UINT n);


#endif