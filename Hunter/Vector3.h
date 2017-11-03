#ifndef VECTOR3_H
#define VECTOR3_H

class Vector4;
class Matrix;


class Vector3 : public D3DXVECTOR3
{
public :
	DECLARE_META(Vector3);

	Vector3();
	Vector3(float x, float y, float z);
	~Vector3();
	Vector3(const Vector3 &other);
	Vector3 operator= (const Vector3 &other);

	// assignment operators
	Vector3& operator += (const Vector3& other);
	Vector3& operator -= (const Vector3& other);
	Vector3& operator *= (float f);
	Vector3& operator /= (float f);

	// unary operators
	//Vector3 operator + () const;
	Vector3 operator - () const;

	// binary operators
	Vector3 operator + (const Vector3& other) const;
	Vector3 operator - (const Vector3& other) const;
	Vector3 operator * (float f) const;
	Vector3 operator / (float f) const;

	friend Vector3 operator * (float f, const Vector3 &other);

	bool operator == (const Vector3& other) const;
	bool operator != (const Vector3& other) const;

private :
};

Vector3 operator * (float f, const Vector3 &other);

inline float Vec3Length ( const Vector3 *pV );

inline float Vec3LengthSq ( const Vector3 *pV );

inline float Vec3Dot ( const Vector3 *pV1, const Vector3 *pV2 );

inline Vector3* Vec3Cross ( Vector3 *pOut, const Vector3 *pV1, const Vector3 *pV2 );

inline Vector3* Vec3Add ( Vector3 *pOut, const Vector3 *pV1, const Vector3 *pV2 );

inline Vector3* Vec3Subtract ( Vector3 *pOut, const Vector3 *pV1, const Vector3 *pV2 );

inline Vector3* Vec3Normalize (Vector3 *pOut, const Vector3 *pV);

// Transform (x, y, z, 1) by matrix.
inline Vector4* Vec3Transform (Vector4 *pOut, const Vector3 *pV, const Matrix *pM);

// Transform (x, y, z, 1) by matrix, project result back into w=1.
inline Vector3* Vec3TransformCoord (Vector3 *pOut, const Vector3 *pV, const Matrix *pM);

// Transform (x, y, z, 0) by matrix.  If you transforming a normal by a 
// non-affine matrix, the matrix you pass to this function should be the 
// transpose of the inverse of the matrix you would use to transform a coord.
inline Vector3* Vec3TransformNormal (Vector3 *pOut, const Vector3 *pV, const Matrix *pM);

#endif