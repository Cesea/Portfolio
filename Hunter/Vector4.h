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

float D3DXVec4Length
    ( const D3DXVECTOR4 *pV );

float D3DXVec4LengthSq
    ( const D3DXVECTOR4 *pV );

float D3DXVec4Dot
    ( const D3DXVECTOR4 *pV1, const D3DXVECTOR4 *pV2 );

D3DXVECTOR4* D3DXVec4Add
    ( D3DXVECTOR4 *pOut, const D3DXVECTOR4 *pV1, const D3DXVECTOR4 *pV2);

D3DXVECTOR4* D3DXVec4Subtract
    ( D3DXVECTOR4 *pOut, const D3DXVECTOR4 *pV1, const D3DXVECTOR4 *pV2);

#endif