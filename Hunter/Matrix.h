#ifndef MATRIX_H
#define MATRIX_H

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


#endif