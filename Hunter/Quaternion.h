#ifndef QUATERNION_H
#define QUATERNION_H

class Matrix;

class Quaternion : public D3DXQUATERNION
{
public :
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



inline float QuaternionLength(const Quaternion *pQ);

// Length squared, or "norm"
inline float QuaternionLengthSq(const Quaternion *pQ);

inline float QuaternionDot(const Quaternion *pQ1, const Quaternion *pQ2);

// (0, 0, 0, 1)
inline Quaternion* QuaternionIdentity(Quaternion *pOut);

inline bool QuaternionIsIdentity(const Quaternion *pQ);

// (-x, -y, -z, w)
inline Quaternion* QuaternionConjugate(Quaternion *pOut, const Quaternion *pQ);


// Compute a quaternin's axis and angle of rotation. Expects unit quaternions.
inline void QuaternionToAxisAngle(const Quaternion *pQ, D3DXVECTOR3 *pAxis, float *pAngle);

// Build a quaternion from a rotation matrix.
inline Quaternion* QuaternionRotationMatrix(Quaternion *pOut, const Matrix *pM);

// Rotation about arbitrary axis.
inline Quaternion* QuaternionRotationAxis(Quaternion *pOut, const Vector3 *pV, float Angle);

// Yaw around the Y axis, a pitch around the X axis,
// and a roll around the Z axis.
inline Quaternion* QuaternionRotationYawPitchRoll(Quaternion *pOut, float Yaw, float Pitch, float Roll);

// Quaternion multiplication.  The result represents the rotation Q2
// followed by the rotation Q1.  (Out = Q2 * Q1)
inline Quaternion* QuaternionMultiply(Quaternion *pOut, const Quaternion *pQ1, const Quaternion *pQ2);

inline Quaternion* QuaternionNormalize(Quaternion *pOut, const Quaternion *pQ);

// Conjugate and re-norm
inline Quaternion* QuaternionInverse(Quaternion *pOut, const Quaternion *pQ);

// Expects unit quaternions.
// if q = (cos(theta), sin(theta) * v); ln(q) = (0, theta * v)
inline Quaternion* QuaternionLn(Quaternion *pOut, const Quaternion *pQ);

// Expects pure quaternions. (w == 0)  w is ignored in calculation.
// if q = (0, theta * v); exp(q) = (cos(theta), sin(theta) * v)
inline Quaternion* QuaternionExp(Quaternion *pOut, const Quaternion *pQ);

// Spherical linear interpolation between Q1 (t == 0) and Q2 (t == 1).
// Expects unit quaternions.
inline Quaternion* QuaternionSlerp(Quaternion *pOut, const Quaternion *pQ1, const Quaternion *pQ2, float t);

// Spherical quadrangle interpolation.
// Slerp(Slerp(Q1, C, t), Slerp(A, B, t), 2t(1-t))
inline Quaternion* QuaternionSquad(Quaternion *pOut, const Quaternion *pQ1,
	const Quaternion *pA, const Quaternion *pB, const Quaternion *pC, float t);

// Setup control points for spherical quadrangle interpolation
// from Q1 to Q2.  The control points are chosen in such a way 
// to ensure the continuity of tangents with adjacent segments.
inline void QuaternionSquadSetup(Quaternion *pAOut, Quaternion *pBOut, Quaternion *pCOut,
	const Quaternion *pQ0, const Quaternion *pQ1, const Quaternion *pQ2, const Quaternion *pQ3);

// Barycentric interpolation.
// Slerp(Slerp(Q1, Q2, f+g), Slerp(Q1, Q3, f+g), g/(f+g))
inline Quaternion* QuaternionBaryCentric(Quaternion *pOut, const Quaternion *pQ1,
	const Quaternion *pQ2, const Quaternion *pQ3, float f, float g);

#endif