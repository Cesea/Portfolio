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

Matrix* MatrixIdentity ( Matrix *pOut );

bool MatrixIsIdentity ( const Matrix *pM );

float MatrixDeterminant ( const Matrix *pM );

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
    ( Matrix *pOut, float *pDeterminant, const Matrix *pM );

// Build a matrix which scales by (sx, sy, sz)
Matrix* MatrixScaling ( Matrix *pOut, float sx, float sy, float sz );

// Build a matrix which translates by (x, y, z)
Matrix* MatrixTranslation ( Matrix *pOut, float x, float y, float z );

// Build a matrix which rotates around the X axis
Matrix* MatrixRotationX ( Matrix *pOut, float angle );

// Build a matrix which rotates around the Y axis
Matrix* MatrixRotationY ( Matrix *pOut, float angle );

// Build a matrix which rotates around the Z axis
Matrix* MatrixRotationZ ( Matrix *pOut, float angle );

// Build a matrix which rotates around an arbitrary axis
Matrix* MatrixRotationAxis ( Matrix *pOut, const Vector3 *pV, float angle );

// Build a matrix from a quaternion
Matrix* MatrixRotationQuaternion ( Matrix *pOut, const Quaternion *pQ);

// Yaw around the Y axis, a pitch around the X axis,
// and a roll around the Z axis.
Matrix* MatrixRotationYawPitchRoll ( Matrix *pOut, float yaw, float pitch, float roll );

// Build transformation matrix.  NULL arguments are treated as identity.
// Mout = Msc-1 * Msr-1 * Ms * Msr * Msc * Mrc-1 * Mr * Mrc * Mt
Matrix* MatrixTransformation
    ( Matrix *pOut, const Vector3 *pScalingCenter,
      const Quaternion *pScalingRotation, const Vector3 *pScaling,
      const Vector3 *pRotationCenter, const Quaternion *pRotation,
      const Vector3 *pTranslation);

// Build 2D transformation matrix in XY plane.  NULL arguments are treated as identity.
// Mout = Msc-1 * Msr-1 * Ms * Msr * Msc * Mrc-1 * Mr * Mrc * Mt
Matrix* MatrixTransformation2D
    ( Matrix *pOut, const Vector2* pScalingCenter, 
      float ScalingRotation, const Vector2* pScaling, 
      const Vector2* pRotationCenter, float rotation, 
      const Vector2* pTranslation);

// Build affine transformation matrix.  NULL arguments are treated as identity.
// Mout = Ms * Mrc-1 * Mr * Mrc * Mt
Matrix* MatrixAffineTransformation
    ( Matrix *pOut, float Scaling, const Vector3 *pRotationCenter,
      const Quaternion *pRotation, const Vector3 *pTranslation);

// Build 2D affine transformation matrix in XY plane.  NULL arguments are treated as identity.
// Mout = Ms * Mrc-1 * Mr * Mrc * Mt
Matrix* MatrixAffineTransformation2D
    ( Matrix *pOut, float scaling, const Vector2* pRotationCenter, 
      float rotation, const Vector2* pTranslation);

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
    ( Matrix *pOut, float w, float h, float zn, float zf );

// Build a perspective projection matrix. (left-handed)
Matrix* MatrixPerspectiveLH
    ( Matrix *pOut, float w, float h, float zn, float zf );

// Build a perspective projection matrix. (right-handed)
Matrix* MatrixPerspectiveFovRH
    ( Matrix *pOut, float fovy, float aspect, float zn, float zf );

// Build a perspective projection matrix. (left-handed)
Matrix* MatrixPerspectiveFovLH
    ( Matrix *pOut, float fovy, float aspect, float zn, float zf );

// Build a perspective projection matrix. (right-handed)
Matrix* MatrixPerspectiveOffCenterRH
    ( Matrix *pOut, float l, float r, float b, float t, float zn,
      float zf );

// Build a perspective projection matrix. (left-handed)
Matrix* MatrixPerspectiveOffCenterLH
    ( Matrix *pOut, float l, float r, float b, float t, float zn,
      float zf );

// Build an ortho projection matrix. (right-handed)
Matrix* MatrixOrthoRH ( Matrix *pOut, float w, float h, float zn, float zf );

// Build an ortho projection matrix. (left-handed)
Matrix* MatrixOrthoLH ( Matrix *pOut, float w, float h, float zn, float zf );

// Build an ortho projection matrix. (right-handed)
Matrix* MatrixOrthoOffCenterRH ( Matrix *pOut, float l, float r, float b, float t, float zn,
      float zf );

// Build an ortho projection matrix. (left-handed)
Matrix* MatrixOrthoOffCenterLH ( Matrix *pOut, float l, float r, float b, float t, float zn, float zf );

// Build a matrix which flattens geometry into a plane, as if casting
// a shadow from a light.
Matrix* MatrixShadow ( Matrix *pOut, const Vector4 *pLight, const Plane *pPlane );

// Build a matrix which reflects the coordinate system about a plane
Matrix* MatrixReflect ( Matrix *pOut, const Plane *pPlane );

#endif