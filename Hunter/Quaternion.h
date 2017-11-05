#ifndef QUATERNION_H
#define QUATERNION_H

class Matrix;

class Quaternion : public D3DXQUATERNION
{
public :
	DECLARE_META(Quaternion);

	Quaternion();
	Quaternion(float x, float y, float z, float w);
	Quaternion(const Quaternion &other);

	// assignment operators
	Quaternion& operator += (const Quaternion& other);
	Quaternion& operator -= (const Quaternion& other);
	Quaternion& operator *= (const Quaternion& other);
	Quaternion& operator *= (float f);
	Quaternion& operator /= (float f);

	// unary operators
	Quaternion  operator + () const;
	Quaternion  operator - () const;

	// binary operators
	Quaternion operator + (const Quaternion &other) const;
	Quaternion operator - (const Quaternion &other) const;
	Quaternion operator * (const Quaternion &other) const;
	Quaternion operator * (float f) const;
	Quaternion operator / (float f) const;

	friend Quaternion operator * (float f, const Quaternion& other);

	bool operator == (const Quaternion& other) const;
	bool operator != (const Quaternion& other) const;

private :
};


inline float QuaternionLength(const Quaternion *pQ) 
{
	return D3DXQuaternionLength(pQ);
}
// Length squared, or "norm"
inline float QuaternionLengthSq(const Quaternion *pQ) 
{
	return D3DXQuaternionLengthSq(pQ);
}
inline float QuaternionDot(const Quaternion *pQ1, const Quaternion *pQ2) 
{
	return D3DXQuaternionDot(pQ1, pQ2);
}

// (0, 0, 0, 1)
inline Quaternion* QuaternionIdentity(Quaternion *pOut) 
{
	return (Quaternion *)D3DXQuaternionIdentity(pOut);
}

inline bool QuaternionIsIdentity(const Quaternion *pQ) 
{
	return D3DXQuaternionIsIdentity(pQ);
}

// (-x, -y, -z, w)
inline Quaternion* QuaternionConjugate(Quaternion *pOut, const Quaternion *pQ) 
{
	return (Quaternion *)D3DXQuaternionConjugate(pOut, pQ);
}



// Compute a quaternin's axis and angle of rotation. Expects unit quaternions.
inline void QuaternionToAxisAngle(const Quaternion *pQ, D3DXVECTOR3 *pAxis, float *pAngle) 
{
	D3DXQuaternionToAxisAngle(pQ, pAxis, pAngle);
}


// Build a quaternion from a rotation matrix.
inline Quaternion* QuaternionRotationMatrix(Quaternion *pOut, const Matrix *pM) 
{
	return (Quaternion *)D3DXQuaternionRotationMatrix(pOut, (D3DXMATRIX *)pM);
}


// Rotation about arbitrary axis.
inline Quaternion* QuaternionRotationAxis(Quaternion *pOut, const Vector3 *pV, float angle) 
{
	return (Quaternion *)D3DXQuaternionRotationAxis(pOut, pV, angle);
}

// Yaw around the Y axis, a pitch around the X axis,
// and a roll around the Z axis.
inline Quaternion* QuaternionRotationYawPitchRoll(Quaternion *pOut, float yaw, float pitch, float roll) 
{
	return (Quaternion *)D3DXQuaternionRotationYawPitchRoll(pOut, yaw, pitch, roll);
}

// Quaternion multiplication.  The result represents the rotation Q2
// followed by the rotation Q1.  (Out = Q2 * Q1)
inline Quaternion* QuaternionMultiply(Quaternion *pOut, const Quaternion *pQ1, const Quaternion *pQ2) 
{
	return (Quaternion *)D3DXQuaternionMultiply(pOut, pQ1, pQ2);
}

inline Quaternion* QuaternionNormalize(Quaternion *pOut, const Quaternion *pQ) 
{
	return (Quaternion *)D3DXQuaternionNormalize(pOut, pQ);
}

// Conjugate and re-norm
inline Quaternion* QuaternionInverse(Quaternion *pOut, const Quaternion *pQ) 
{
	return (Quaternion *)D3DXQuaternionInverse(pOut, pQ);
}

// Expects unit quaternions.
// if q = (cos(theta), sin(theta) * v); ln(q) = (0, theta * v)
inline Quaternion* QuaternionLn(Quaternion *pOut, const Quaternion *pQ) 
{
	return (Quaternion *)D3DXQuaternionLn(pOut, pQ);
}

// Expects pure quaternions. (w == 0)  w is ignored in calculation.
// if q = (0, theta * v); exp(q) = (cos(theta), sin(theta) * v)
inline Quaternion* QuaternionExp(Quaternion *pOut, const Quaternion *pQ) 
{
	return (Quaternion *)QuaternionExp(pOut, pQ);
}

// Spherical linear interpolation between Q1 (t == 0) and Q2 (t == 1).
// Expects unit quaternions.
inline Quaternion* QuaternionSlerp(Quaternion *pOut, const Quaternion *pQ1, const Quaternion *pQ2, float t) 
{
	return (Quaternion *)D3DXQuaternionSlerp(pOut, pQ1, pQ2, t);
}

// Spherical quadrangle interpolation.
// Slerp(Slerp(Q1, C, t), Slerp(A, B, t), 2t(1-t))
inline Quaternion* QuaternionSquad(Quaternion *pOut, const Quaternion *pQ1,
	const Quaternion *pA, const Quaternion *pB, const Quaternion *pC, float t) 
{
	return (Quaternion *)D3DXQuaternionSquad(pOut, pQ1, pA, pB, pC, t);
}

// Setup control points for spherical quadrangle interpolation
// from Q1 to Q2.  The control points are chosen in such a way 
// to ensure the continuity of tangents with adjacent segments.
inline void QuaternionSquadSetup(Quaternion *pAOut, Quaternion *pBOut, Quaternion *pCOut,
	const Quaternion *pQ0, const Quaternion *pQ1, const Quaternion *pQ2, const Quaternion *pQ3) 
{
	return D3DXQuaternionSquadSetup(pAOut, pBOut, pCOut, pQ0, pQ1, pQ2, pQ3);
}

// Barycentric interpolation.
// Slerp(Slerp(Q1, Q2, f+g), Slerp(Q1, Q3, f+g), g/(f+g))
inline Quaternion* QuaternionBaryCentric(Quaternion *pOut, const Quaternion *pQ1,
	const Quaternion *pQ2, const Quaternion *pQ3, float f, float g) 
{
	return (Quaternion *)D3DXQuaternionBaryCentric(pOut, pQ1, pQ2, pQ3, f, g);
}
#endif