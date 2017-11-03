#ifndef VECTOR3_H
#define VECTOR3_H

class Vector3 : public D3DXVECTOR3
{
public :

	//DECLARE_META(Vector3);

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

	friend Vector3 operator * (float f, const struct Vector3 &other);

	bool operator == (const Vector3& other) const;
	bool operator != (const Vector3& other) const;

private :
};

#endif