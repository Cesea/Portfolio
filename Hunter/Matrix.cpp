#include "stdafx.h"
#include "Matrix.h"

DEFINE_META(Matrix)
{
	ADD_MEMBER(_11);
	ADD_MEMBER(_12);
	ADD_MEMBER(_13);
	ADD_MEMBER(_14);

	ADD_MEMBER(_21);
	ADD_MEMBER(_22);
	ADD_MEMBER(_23);
	ADD_MEMBER(_24);

	ADD_MEMBER(_31);
	ADD_MEMBER(_32);
	ADD_MEMBER(_33);
	ADD_MEMBER(_34);

	ADD_MEMBER(_41);
	ADD_MEMBER(_42);
	ADD_MEMBER(_43);
	ADD_MEMBER(_44);
}

Matrix::Matrix()
{

}

Matrix::Matrix(const float *pArray)
{
	for (int32 row = 0; row < 4; ++row)
	{
		for (int32 col = 0; col < 4; ++col)
		{
			m[row][col] = pArray[col + row * 4];
		}
	}
}

Matrix::Matrix(const Matrix & other)
{
	for (int32 row = 0; row < 4; ++row)
	{
		for (int32 col = 0; col < 4; ++col)
		{
			m[row][col] = other.m[row][col];
		}
	}
}

Matrix::Matrix(float _11, float _12, float _13, float _14, 
	float _21, float _22, float _23, float _24, 
	float _31, float _32, float _33, float _34, 
	float _41, float _42, float _43, float _44)
{
	this->_11 = _11; this->_12 = _12; this->_13 = _13; this->_14 = _14; 

	this->_21 = _21; this->_22 = _22; this->_23 = _23; this->_24 = _24;

	this->_31 = _31; this->_32 = _32; this->_33 = _33; this->_34 = _34;

	this->_41 = _41; this->_42 = _42; this->_43 = _43; this->_44 = _44;
}

float & Matrix::operator()(uint32 row, uint32 col)
{
	return m[row][col];
}

float Matrix::operator()(uint32 row, uint32 col) const
{
	return m[row][col];
}

Matrix & Matrix::operator*=(const Matrix & other)
{
	Matrix result = *this;

	for ( int32 i = 0; i < 4; ++i )
	{
		for ( int32 j = 0; j < 4; ++j )
		{
			*this[i][j] = 0.0f;
			for ( int32 k = 0; k < 4; ++k )
			{
				*this[i][j] += ( result )[i][k] * result[k][j];
			}
		}
	}
	return *this;
}

Matrix & Matrix::operator+=(const Matrix & other)
{
	for (int32 row = 0; row < 4; ++row)
	{
		for (int32 col = 0; col < 4; ++col)
		{
			this->m[row][col] += other.m[row][col];
		}
	}
	return *this;
}

Matrix & Matrix::operator-=(const Matrix & other)
{
	for (int32 row = 0; row < 4; ++row)
	{
		for (int32 col = 0; col < 4; ++col)
		{
			this->m[row][col] -= other.m[row][col];
		}
	}
	return *this;
}

Matrix & Matrix::operator*=(float f)
{
	for (int32 row = 0; row < 4; ++row)
	{
		for (int32 col = 0; col < 4; ++col)
		{
			this->m[row][col] *= f;
		}
	}
	return *this;
}

Matrix & Matrix::operator/=(float f)
{
	for (int32 row = 0; row < 4; ++row)
	{
		for (int32 col = 0; col < 4; ++col)
		{
			this->m[row][col] /= f;
		}
	}
	return *this;
}

Matrix Matrix::operator-() const
{
	return Matrix(-_11, -_12, -_13, -_14,
		-_21, -_22, -_23, -_24,
		-_31, -_32, -_33, -_34,
		-_41, -_42, -_43, -_44);
}

Matrix Matrix::operator*(const Matrix & other) const
{
	Matrix result;

	for ( int32 i = 0; i < 4; ++i )
	{
		for ( int32 j = 0; j < 4; ++j )
		{
			ret[i][j] = 00.f;
			for ( int k = 0; k < 4; ++k )
			{
				ret[i][j] += ( *this )[i][k] * result[k][j];
			}
		}
	}

	return ret;
}

Matrix Matrix::operator+(const Matrix & other) const
{
	return Matrix(_11 + other._11, _12 + other._12, _13 + other._13, _14 + other._14,
		_21 + other._21, _22 + other._22, _23 + other._23, _24 + other._24,
		_31 + other._31, _32 + other._32, _33 + other._33, _34 + other._34,
		_41 + other._41, _42 + other._42, _43 + other._43, _44 + other._44);
}

Matrix Matrix::operator-(const Matrix & other) const
{
	return Matrix(_11 - other._11, _12 - other._12, _13 - other._13, _14 - other._14,
		_21 - other._21, _22 - other._22, _23 - other._23, _24 - other._24,
		_31 - other._31, _32 - other._32, _33 - other._33, _34 - other._34,
		_41 - other._41, _42 - other._42, _43 - other._43, _44 - other._44);
}

Matrix Matrix::operator*(float f) const
{
	return Matrix(_11 * f, _12 * f, _13 * f, _14 * f,
		_21 * f, _22 * f, _23 * f, _24 * f,
		_31 * f, _32 * f, _33 * f, _34 * f,
		_41 * f, _42 * f, _43 * f, _44 * f);
}

Matrix Matrix::operator/(float f) const
{
	return Matrix(_11 / f, _12 / f, _13 / f, _14 / f,
		_21 / f, _22 / f, _23 / f, _24 / f,
		_31 / f, _32 / f, _33 / f, _34 / f,
		_41 / f, _42 / f, _43 / f, _44 / f);
}

bool Matrix::operator==(const Matrix & other) const
{
	for (int32 row = 0; row < 4; ++row)
	{
		for (int32 col = 0; col < 4; ++col)
		{
			if (!FloatEqual(m[row][col], other.m[row][col]))
			{
				return false;
			}
		}
	}
	return true;
}

bool Matrix::operator!=(const Matrix & other) const
{
	return !(*this == other);
}

Matrix operator*(float f, const Matrix & other)
{
	return Matrix(other._11 * f, other._12 * f, other._13 * f, other._14 * f,
		other._21 * f, other._22 * f, other._23 * f, other._24 * f,
		other._31 * f, other._32 * f, other._33 * f, other._34 * f,
		other._41 * f, other._42 * f, other._43 * f, other._44 * f);
}
