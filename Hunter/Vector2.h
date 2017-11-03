#ifndef VECTOR2_H
#define VECTOR2_H

class Vector2 : public D3DXVECTOR2
{
public :

	DECLARE_META(Vector2);

	Vector2();
    Vector2( float x, float y );
	Vector2(const Vector2 &other);

    // casting
    //operator FLOAT* ();
    //operator CONST FLOAT* () const;

    // assignment operators
    Vector2& operator += ( const Vector2& other);
    Vector2& operator -= ( const Vector2& other);
    Vector2& operator *= ( float f);
    Vector2& operator /= ( float f);

    // unary operators
    //Vector2 operator + () const;
    Vector2 operator - () const;

    // binary operators
    Vector2 operator + ( const Vector2& other) const;
    Vector2 operator - ( const Vector2& other) const;
    Vector2 operator * ( float f) const;
    Vector2 operator / ( float f) const;

    friend Vector2 operator * ( float f, const Vector2& other);

    bool operator == ( const Vector2& other) const;
    bool operator != ( const Vector2& other) const;
};

float Vec2Length ( const Vector2 *pV );

float Vec2LengthSq ( const Vector2 *pV );

float Vec2Dot ( const Vector2 *pV1, const Vector2 *pV2 );

Vector2* Vec2Add ( Vector2 *pOut, const Vector2 *pV1, const Vector2 *pV2 );

Vector2* Vec2Subtract ( Vector2 *pOut, const Vector2 *pV1, const Vector2 *pV2 );

Vector2*  Vec2Normalize ( Vector2 *pOut, const Vector2 *pV );


#endif
