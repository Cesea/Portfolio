#ifndef VECTOR4_H
#define VECTOR4_H

class Vector3;

class Vector4 : public D3DXVECTOR4
{
public :
	DECLARE_META(Vector4);

	Vector4();
	Vector4(const Vector3& xyz, float w);
	Vector4(float x, float y, float z, float w);
	Vector4(const Vector4 &other);



	// assignment operators
	Vector4& operator += (const Vector4 &other);
	Vector4& operator -= (const Vector4 &other);
	Vector4& operator *= (float f);
	Vector4& operator /= (float f);

	// unary operators
	//Vector4 operator + () const;
	Vector4 operator - () const;

	// binary operators
	Vector4 operator + (const Vector4 &other) const;
	Vector4 operator - (const Vector4 &other) const;
	Vector4 operator * (float f) const;
	Vector4 operator / (float f) const;

	friend Vector4 operator * (float f, const Vector4& other);

	bool operator == (const Vector4 &other) const;
	bool operator != (const Vector4 &other) const;
};

Vector4 operator * (float f, const Vector4& other);

inline float Vec4Length ( const Vector4 *pV )
{
	return D3DXVec4Length(pV);
}
inline float Vec4LengthSq ( const Vector4 *pV )
{
	return D3DXVec4LengthSq(pV);
}

inline float Vec4Dot ( const Vector4 *pV1, const Vector4 *pV2 )
{
	return D3DXVec4Dot(pV1, pV2);
}

inline Vector4* Vec4Add ( Vector4 *pOut, const Vector4 *pV1, const Vector4 *pV2)
{
	return (Vector4 *)D3DXVec4Add(pOut, pV1, pV2);
}

inline Vector4* Vec4Subtract ( Vector4 *pOut, const Vector4 *pV1, const Vector4 *pV2)
{
	return (Vector4 *)D3DXVec4Subtract(pOut, pV1, pV2);
}


#endif