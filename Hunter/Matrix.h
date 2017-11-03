#ifndef MATRIX_H
#define MATRIX_H

class Vector3;

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

Matrix* MatrixIdentity ( Matrix *pOut );

BOOL MatrixIsIdentity ( const Matrix *pM );


FLOAT MatrixDeterminant ( const Matrix *pM );

HRESULT MatrixDecompose ( Vector3 *pOutScale, D3DXQUATERNION *pOutRotation, 
	  Vector3 *pOutTranslation, const Matrix *pM );

Matrix* MatrixTranspose ( Matrix *pOut, const Matrix *pM );

// Matrix multiplication.  The result represents the transformation M2
// followed by the transformation M1.  (Out = M1 * M2)
Matrix* MatrixMultiply ( Matrix *pOut, const Matrix *pM1, const Matrix *pM2 );

// Matrix multiplication, followed by a transpose. (Out = T(M1 * M2))
Matrix* MatrixMultiplyTranspose ( Matrix *pOut, const Matrix *pM1, const Matrix *pM2 );

// Calculate inverse of matrix.  Inversion my fail, in which case NULL will
// be returned.  The determinant of pM is also returned it pfDeterminant
// is non-NULL.
Matrix* MatrixInverse
    ( Matrix *pOut, FLOAT *pDeterminant, const Matrix *pM );

// Build a matrix which scales by (sx, sy, sz)
Matrix* MatrixScaling ( Matrix *pOut, FLOAT sx, FLOAT sy, FLOAT sz );

// Build a matrix which translates by (x, y, z)
Matrix* MatrixTranslation ( Matrix *pOut, FLOAT x, FLOAT y, FLOAT z );

// Build a matrix which rotates around the X axis
Matrix* MatrixRotationX ( Matrix *pOut, FLOAT Angle );

// Build a matrix which rotates around the Y axis
Matrix* MatrixRotationY ( Matrix *pOut, FLOAT Angle );

// Build a matrix which rotates around the Z axis
Matrix* MatrixRotationZ ( Matrix *pOut, FLOAT Angle );

// Build a matrix which rotates around an arbitrary axis
Matrix* MatrixRotationAxis ( Matrix *pOut, const Vector3 *pV, FLOAT Angle );

// Build a matrix from a quaternion
Matrix* MatrixRotationQuaternion ( Matrix *pOut, const D3DXQUATERNION *pQ);

// Yaw around the Y axis, a pitch around the X axis,
// and a roll around the Z axis.
Matrix* MatrixRotationYawPitchRoll ( Matrix *pOut, FLOAT Yaw, FLOAT Pitch, FLOAT Roll );

// Build transformation matrix.  NULL arguments are treated as identity.
// Mout = Msc-1 * Msr-1 * Ms * Msr * Msc * Mrc-1 * Mr * Mrc * Mt
Matrix* MatrixTransformation
    ( Matrix *pOut, const Vector3 *pScalingCenter,
      const D3DXQUATERNION *pScalingRotation, const Vector3 *pScaling,
      const Vector3 *pRotationCenter, const D3DXQUATERNION *pRotation,
      const Vector3 *pTranslation);

// Build 2D transformation matrix in XY plane.  NULL arguments are treated as identity.
// Mout = Msc-1 * Msr-1 * Ms * Msr * Msc * Mrc-1 * Mr * Mrc * Mt
Matrix* MatrixTransformation2D
    ( Matrix *pOut, const D3DXVECTOR2* pScalingCenter, 
      FLOAT ScalingRotation, const D3DXVECTOR2* pScaling, 
      const D3DXVECTOR2* pRotationCenter, FLOAT Rotation, 
      const D3DXVECTOR2* pTranslation);

// Build affine transformation matrix.  NULL arguments are treated as identity.
// Mout = Ms * Mrc-1 * Mr * Mrc * Mt
Matrix* MatrixAffineTransformation
    ( Matrix *pOut, FLOAT Scaling, const Vector3 *pRotationCenter,
      const D3DXQUATERNION *pRotation, const Vector3 *pTranslation);

// Build 2D affine transformation matrix in XY plane.  NULL arguments are treated as identity.
// Mout = Ms * Mrc-1 * Mr * Mrc * Mt
Matrix* MatrixAffineTransformation2D
    ( Matrix *pOut, FLOAT Scaling, const D3DXVECTOR2* pRotationCenter, 
      FLOAT Rotation, const D3DXVECTOR2* pTranslation);

// Build a lookat matrix. (right-handed)
Matrix* MatrixLookAtRH
    ( Matrix *pOut, const Vector3 *pEye, const Vector3 *pAt,
      const Vector3 *pUp );

// Build a lookat matrix. (left-handed)
Matrix* MatrixLookAtLH
    ( Matrix *pOut, const Vector3 *pEye, const Vector3 *pAt,
      const Vector3 *pUp );

// Build a perspective projection matrix. (right-handed)
Matrix* MatrixPerspectiveRH
    ( Matrix *pOut, FLOAT w, FLOAT h, FLOAT zn, FLOAT zf );

// Build a perspective projection matrix. (left-handed)
Matrix* MatrixPerspectiveLH
    ( Matrix *pOut, FLOAT w, FLOAT h, FLOAT zn, FLOAT zf );

// Build a perspective projection matrix. (right-handed)
Matrix* MatrixPerspectiveFovRH
    ( Matrix *pOut, FLOAT fovy, FLOAT Aspect, FLOAT zn, FLOAT zf );

// Build a perspective projection matrix. (left-handed)
Matrix* MatrixPerspectiveFovLH
    ( Matrix *pOut, FLOAT fovy, FLOAT Aspect, FLOAT zn, FLOAT zf );

// Build a perspective projection matrix. (right-handed)
Matrix* MatrixPerspectiveOffCenterRH
    ( Matrix *pOut, FLOAT l, FLOAT r, FLOAT b, FLOAT t, FLOAT zn,
      FLOAT zf );

// Build a perspective projection matrix. (left-handed)
Matrix* MatrixPerspectiveOffCenterLH
    ( Matrix *pOut, FLOAT l, FLOAT r, FLOAT b, FLOAT t, FLOAT zn,
      FLOAT zf );

// Build an ortho projection matrix. (right-handed)
Matrix* MatrixOrthoRH
    ( Matrix *pOut, FLOAT w, FLOAT h, FLOAT zn, FLOAT zf );

// Build an ortho projection matrix. (left-handed)
Matrix* MatrixOrthoLH
    ( Matrix *pOut, FLOAT w, FLOAT h, FLOAT zn, FLOAT zf );

// Build an ortho projection matrix. (right-handed)
Matrix* MatrixOrthoOffCenterRH
    ( Matrix *pOut, FLOAT l, FLOAT r, FLOAT b, FLOAT t, FLOAT zn,
      FLOAT zf );

// Build an ortho projection matrix. (left-handed)
Matrix* MatrixOrthoOffCenterLH
    ( Matrix *pOut, FLOAT l, FLOAT r, FLOAT b, FLOAT t, FLOAT zn,
      FLOAT zf );

// Build a matrix which flattens geometry into a plane, as if casting
// a shadow from a light.
Matrix* MatrixShadow
    ( Matrix *pOut, const D3DXVECTOR4 *pLight,
      const D3DXPLANE *pPlane );

// Build a matrix which reflects the coordinate system about a plane
Matrix* MatrixReflect
    ( Matrix *pOut, const D3DXPLANE *pPlane );

#ifdef __cplusplus
}
#endif




#endif