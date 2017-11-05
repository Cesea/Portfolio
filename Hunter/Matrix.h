#ifndef MATRIX_H
#define MATRIX_H

class Vector3;
class Plane;

class Matrix : public D3DXMATRIXA16
{
public :
	DECLARE_META(Matrix);

    Matrix();
    Matrix( const float * );
    Matrix( const Matrix& other);
    Matrix( float _11, float _12, float _13, float _14,
                float _21, float _22, float _23, float _24,
                float _31, float _32, float _33, float _34,
                float _41, float _42, float _43, float _44 );


    // access grants
    float& operator () ( uint32 row, uint32 col );
    float  operator () ( uint32 row, uint32 col ) const;


    // assignment operators
    Matrix& operator *= ( const Matrix& other);
    Matrix& operator += ( const Matrix& other);
    Matrix& operator -= ( const Matrix& other);
    Matrix& operator *= ( float f);
    Matrix& operator /= ( float f);

    Matrix operator - () const;

    // binary operators
    Matrix operator * ( const Matrix& other) const;
    Matrix operator + ( const Matrix& other) const;
    Matrix operator - ( const Matrix& other) const;
    Matrix operator * ( float f) const;
    Matrix operator / ( float f) const;

    friend Matrix operator * ( float f, const Matrix& other);

    bool operator == ( const Matrix& other) const;
    bool operator != ( const Matrix& other) const;

private :
};

Matrix operator * (float f, const Matrix& other);

inline Matrix* MatrixIdentity ( Matrix *pOut )
{
	return (Matrix *)D3DXMatrixIdentity(pOut);
}

inline bool MatrixIsIdentity ( const Matrix *pM )
{
	return D3DXMatrixIsIdentity(pM);
}
inline float MatrixDeterminant ( const Matrix *pM )
{
	return D3DXMatrixDeterminant(pM);
}

inline Matrix* MatrixTranspose ( Matrix *pOut, const Matrix *pM )
{
	return (Matrix *)D3DXMatrixTranspose(pOut, pM);
}

// Matrix multiplication.  The result represents the transformation M2
// followed by the transformation M1.  (Out = M1 * M2)
inline Matrix* MatrixMultiply ( Matrix *pOut, const Matrix *pM1, const Matrix *pM2 )
{
	return (Matrix *)D3DXMatrixMultiply(pOut, pM1, pM2);
}

// Matrix multiplication, followed by a transpose. (Out = T(M1 * M2))
inline Matrix* MatrixMultiplyTranspose ( Matrix *pOut, const Matrix *pM1, const Matrix *pM2 )
{
	return (Matrix *)D3DXMatrixMultiplyTranspose(pOut, pM1, pM2);
}

// Calculate inverse of matrix.  Inversion my fail, in which case NULL will
// be returned.  The determinant of pM is also returned it pfDeterminant
// is non-NULL.
inline Matrix* MatrixInverse
    ( Matrix *pOut, float *pDeterminant, const Matrix *pM )
{
	return (Matrix *)D3DXMatrixInverse(pOut, pDeterminant, pM);
}

// Build a matrix which scales by (sx, sy, sz)
inline Matrix* MatrixScaling ( Matrix *pOut, float sx, float sy, float sz )
{
	return (Matrix *)D3DXMatrixScaling(pOut, sx, sy, sz);
}

// Build a matrix which translates by (x, y, z)
inline Matrix* MatrixTranslation (Matrix *pOut, float x, float y, float z )
{
	return (Matrix *)D3DXMatrixTranslation(pOut, x, y, z);
}

// Build a matrix which rotates around the X axis
inline Matrix* MatrixRotationX ( Matrix *pOut, float angle )
{
	return (Matrix *)D3DXMatrixRotationX(pOut, angle);
}

// Build a matrix which rotates around the Y axis
inline Matrix* MatrixRotationY ( Matrix *pOut, float angle ) 
{
	return (Matrix *)D3DXMatrixRotationY(pOut, angle);
}
// Build a matrix which rotates around the Z axis
inline Matrix* MatrixRotationZ ( Matrix *pOut, float angle )
{
	return (Matrix *)D3DXMatrixRotationZ(pOut, angle);
}

// Build a matrix which rotates around an arbitrary axis
inline Matrix* MatrixRotationAxis ( Matrix *pOut, const Vector3 *pV, float angle )
{
	return (Matrix *)D3DXMatrixRotationAxis(pOut, pV, angle);
}

// Build a matrix from a quaternion
inline Matrix* MatrixRotationQuaternion ( Matrix *pOut, const Quaternion *pQ)
{
	return (Matrix *)D3DXMatrixRotationQuaternion(pOut, pQ);
}

// Yaw around the Y axis, a pitch around the X axis,
// and a roll around the Z axis.
inline Matrix* MatrixRotationYawPitchRoll ( Matrix *pOut, float yaw, float pitch, float roll )
{
	return (Matrix *)D3DXMatrixRotationYawPitchRoll(pOut, yaw, pitch, roll);
}

// Build transformation matrix.  NULL arguments are treated as identity.
// Mout = Msc-1 * Msr-1 * Ms * Msr * Msc * Mrc-1 * Mr * Mrc * Mt
inline Matrix* MatrixTransformation
    ( Matrix *pOut, const Vector3 *pScalingCenter,
      const Quaternion *pScalingRotation, const Vector3 *pScaling,
      const Vector3 *pRotationCenter, const Quaternion *pRotation,
      const Vector3 *pTranslation)
{
	return (Matrix *)D3DXMatrixTransformation(pOut, pScalingCenter, pScalingRotation, pScaling, pRotationCenter, pRotation, pTranslation);
}

// Build 2D transformation matrix in XY plane.  NULL arguments are treated as identity.
// Mout = Msc-1 * Msr-1 * Ms * Msr * Msc * Mrc-1 * Mr * Mrc * Mt
inline Matrix* MatrixTransformation2D
    ( Matrix *pOut, const Vector2* pScalingCenter, 
      float scalingRotation, const Vector2* pScaling, 
      const Vector2* pRotationCenter, float rotation, 
      const Vector2* pTranslation)
{
	return (Matrix *)D3DXMatrixTransformation2D(pOut, pScalingCenter, scalingRotation, pScaling, pRotationCenter, rotation, pTranslation);
}

// Build affine transformation matrix.  NULL arguments are treated as identity.
// Mout = Ms * Mrc-1 * Mr * Mrc * Mt
inline Matrix* MatrixAffineTransformation
    ( Matrix *pOut, float scaling, const Vector3 *pRotationCenter,
      const Quaternion *pRotation, const Vector3 *pTranslation)
{
	return (Matrix *)D3DXMatrixAffineTransformation(pOut, scaling, pRotationCenter, pRotation, pTranslation);
}

// Build 2D affine transformation matrix in XY plane.  NULL arguments are treated as identity.
// Mout = Ms * Mrc-1 * Mr * Mrc * Mt
inline Matrix* MatrixAffineTransformation2D
    ( Matrix *pOut, float scaling, const Vector2* pRotationCenter, 
      float rotation, const Vector2* pTranslation)
{
	return (Matrix *)D3DXMatrixAffineTransformation2D(pOut, scaling, pRotationCenter, rotation, pTranslation);
}

// Build a lookat matrix. (right-handed)
inline Matrix* MatrixLookAtRH
    ( Matrix *pOut, const Vector3 *pEye, const Vector3 *pAt,
      const Vector3 *pUp )
{
	return (Matrix *)D3DXMatrixLookAtRH(pOut, pEye, pAt, pUp);
}

// Build a lookat matrix. (left-handed)
inline Matrix* MatrixLookAtLH
    ( Matrix *pOut, const Vector3 *pEye, const Vector3 *pAt,
      const Vector3 *pUp )
{
	return (Matrix *)D3DXMatrixLookAtLH(pOut, pEye, pAt, pUp);
}

// Build a perspective projection matrix. (right-handed)
inline Matrix* MatrixPerspectiveRH
    ( Matrix *pOut, float w, float h, float zn, float zf )
{
	return (Matrix *)D3DXMatrixPerspectiveRH(pOut, w, h, zn, zf);
}

// Build a perspective projection matrix. (left-handed)
inline Matrix* MatrixPerspectiveLH
    ( Matrix *pOut, float w, float h, float zn, float zf )
{
	return (Matrix *)D3DXMatrixPerspectiveLH(pOut, w, h, zn, zf);
}

// Build a perspective projection matrix. (right-handed)
inline Matrix* MatrixPerspectiveFovRH
    ( Matrix *pOut, float fovy, float aspect, float zn, float zf )
{
	return (Matrix *)D3DXMatrixPerspectiveFovRH(pOut, fovy, aspect, zn, zf);
}

// Build a perspective projection matrix. (left-handed)
inline Matrix* MatrixPerspectiveFovLH
    ( Matrix *pOut, float fovy, float aspect, float zn, float zf )
{
	return (Matrix *)D3DXMatrixPerspectiveFovLH(pOut, fovy, aspect, zn, zf);
}

// Build a perspective projection matrix. (right-handed)
inline Matrix* MatrixPerspectiveOffCenterRH
    ( Matrix *pOut, float l, float r, float b, float t, float zn,
      float zf )
{
	return (Matrix *)D3DXMatrixPerspectiveOffCenterRH(pOut, l, r, b, t, zn, zf);
}

// Build a perspective projection matrix. (left-handed)
inline Matrix* MatrixPerspectiveOffCenterLH
    ( Matrix *pOut, float l, float r, float b, float t, float zn,
      float zf )
{
	return (Matrix *)D3DXMatrixPerspectiveOffCenterLH(pOut, l, r, b, t, zn, zf);
}

// Build an ortho projection matrix. (right-handed)
inline Matrix* MatrixOrthoRH ( Matrix *pOut, float w, float h, float zn, float zf )
{
	return (Matrix *)D3DXMatrixOrthoRH(pOut, w, h, zn, zf);
}

// Build an ortho projection matrix. (left-handed)
inline Matrix* MatrixOrthoLH ( Matrix *pOut, float w, float h, float zn, float zf )
{
	return (Matrix *)D3DXMatrixOrthoLH(pOut, w, h, zn, zf);
}


// Build an ortho projection matrix. (right-handed)
inline Matrix* MatrixOrthoOffCenterRH ( Matrix *pOut, float l, float r, float b, float t, float zn, float zf )
{
	return (Matrix *)D3DXMatrixOrthoOffCenterRH(pOut, l, r, b, t, zn, zf);
}

// Build an ortho projection matrix. (left-handed)
inline Matrix* MatrixOrthoOffCenterLH ( Matrix *pOut, float l, float r, float b, float t, float zn, float zf )
{
	return (Matrix *)D3DXMatrixOrthoOffCenterLH(pOut, l, r, b, t, zn, zf);
}

// Build a matrix which flattens geometry into a plane, as if casting
// a shadow from a light.
inline Matrix* MatrixShadow ( Matrix *pOut, const Vector4 *pLight, const Plane *pPlane )
{
	return (Matrix *)D3DXMatrixShadow(pOut, pLight, pPlane);
}

// Build a matrix which reflects the coordinate system about a plane
inline Matrix* MatrixReflect ( Matrix *pOut, const Plane *pPlane )
{
	return (Matrix *)D3DXMatrixReflect(pOut, pPlane);
}


#endif